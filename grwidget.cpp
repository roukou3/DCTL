#include "grwidget.h"
#include "ui_grwidget.h"

#include <QPainter>
#include <QPaintEvent>

#include <thread>

GrWidget::GrWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GrWidget)
{
    ui->setupUi(this);
}

GrWidget::~GrWidget()
{
    delete ui;
}

void GrWidget::updateResultImage()
{
    int threads = m_spinBoxThreads->value();
    processImage(m_resultImage, *m_zoom, threads);
    update();
}

void GrWidget::processImage(QImage& result, int z, int threads)
{
    QSize sz(m_txr->w * z / 100, m_txr->h * z / 100);
    pixbuf.resize(sz.width() * sz.height() * 4);
    auto processImageSectionFunc = [this, sz](int y0, int y1) -> void {
        for (int y = y0; y < y1; ++y)
        {
            for (int x = 0; x != sz.width(); ++x)
            {
                int xb = x * m_txr->w / sz.width();
                int yb = y * m_txr->h / sz.height();
                float3 color = ::transform(m_txr->w, m_txr->h, xb, yb, *m_txr, *m_txg, *m_txb);

                int rc = color.z * 255;
                int gc = color.y * 255;
                int bc = color.x * 255;

                pixbuf[(x + y * sz.width()) * 4 + 0] = std::clamp(rc, 0, 255);
                pixbuf[(x + y * sz.width()) * 4 + 1] = std::clamp(gc, 0, 255);
                pixbuf[(x + y * sz.width()) * 4 + 2] = std::clamp(bc, 0, 255);
                pixbuf[(x + y * sz.width()) * 4 + 3] = 255;
            }
        }
    };
    if (threads < 2)
    {
        processImageSectionFunc(0, sz.height());
    }
    else
    {
        int sectionSize = sz.height() / threads;
        std::vector<std::thread> tv;
        for (int i = 0; i < threads; ++i)
        {
            int y0 = i * sectionSize;
            int y1 = (i + 1) * sectionSize;
            if (i + 1 == threads)
            {
                y1 = sz.height();
            }
            if (y0 != y1)
            {
                tv.push_back(std::thread(processImageSectionFunc, y0, y1));
            }
        }
        for (auto& t : tv)
        {
            t.join();
        }
    }
    if (EQUIRECT.value)
    {
        int height = sz.width() / 2;
        int yoff = (sz.height() - height) / 2;
        if (yoff >= 0)
        {
            QImage tmpImg(pixbuf.data() + yoff * sz.width() * 4, sz.width(), height, QImage::Format_ARGB32);
            result.swap(tmpImg);
        }
        else
        {
        }
    }
    else
    {
        QImage tmpImg(pixbuf.data(), sz.width(), sz.height(), QImage::Format_ARGB32);
        result.swap(tmpImg);
    }
}

void GrWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    QSize sz = size();
    QPoint imgPos((sz.width() - m_resultImage.width())/2, (sz.height() - m_resultImage.height())/2);
    painter.fillRect(event->rect(), QWidget::palette().color(QWidget::backgroundRole()));
    painter.drawImage(imgPos, m_resultImage);
    if (m_checkBoxCenterLines->isChecked())
    {
        painter.fillRect(QRect(imgPos.x(), imgPos.y() + m_resultImage.height() / 2, m_resultImage.width(), 2), Qt::white);
        painter.fillRect(QRect(imgPos.x() + m_resultImage.width() / 2, imgPos.y(), 2, m_resultImage.height()), Qt::white);
    }
    QWidget::paintEvent(event);
    painter.end();
}
