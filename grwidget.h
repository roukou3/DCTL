#ifndef GRWIDGET_H
#define GRWIDGET_H

#include <QWidget>
#include <QSpinBox>

#include "dctl.h"

namespace Ui {
class GrWidget;
}

class GrWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GrWidget(QWidget *parent = nullptr);
    ~GrWidget();

    void paintEvent(QPaintEvent *event);
    void setParams(Texture* r, Texture* g, Texture* b, int* zoom, QSpinBox* thr, QCheckBox* checkBoxCenterLines)
    {
        m_txr = r;
        m_txg = g;
        m_txb = b;
        m_zoom = zoom;
        m_spinBoxThreads = thr;
        m_checkBoxCenterLines = checkBoxCenterLines;
    }

    void processImage(QImage& result, int z, int thr);

public slots:
    void updateResultImage();

private:
    Ui::GrWidget *ui;

    Texture* m_txr;
    Texture* m_txg;
    Texture* m_txb;
    int* m_zoom;
    QSpinBox* m_spinBoxThreads;
    QCheckBox* m_checkBoxCenterLines;

    QImage m_resultImage;

    std::vector<uint8_t> pixbuf;
};

#endif // GRWIDGET_H
