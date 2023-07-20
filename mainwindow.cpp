#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QSpacerItem>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->widget->setParams(&txr, &txg, &txb, &m_zoom, ui->spinBoxThreads, ui->checkBoxCenterLines);

    updateZoom(ui->comboBoxZoom->currentText());

    auto resetAll = new QPushButton("⟲", this);
    resetAll->setToolTip("Reset all parameters");
    QGridLayout* gl = dynamic_cast<QGridLayout*>(ui->scrollArea->widget()->layout());
    gl->addWidget(resetAll, 0, 3);

    ui->comboBoxZoom->setFocus();

    adjustDefaults();

    int row = 1;
    addParamWidget(row, resetAll, CIRCLE_ADJ);
    addParamWidget(row, resetAll, CIRCLE_CENTX);
    addParamWidget(row, resetAll, CIRCLE_CENTY);
    addParamWidget(row, resetAll, CIRCLE_RAD);
    addParamWidget(row, resetAll, PIX_ASPECT);
    addParamWidget(row, resetAll, FOCAL_LENGTH);
    addParamWidget(row, resetAll, INTERP);
    addParamWidget(row, resetAll, EQUIRECT);
    addParamWidget(row, resetAll, HIDE_OUTSIDE);
    addParamWidget(row, resetAll, ANGLE_PAN);
    addParamWidget(row, resetAll, ANGLE_TILT);
    addParamWidget(row, resetAll, ANGLE_ROTATE);
    addParamWidget(row, resetAll, DEFISH_RATE);
    addParamWidget(row, resetAll, ANGLE_PAN2);
    addParamWidget(row, resetAll, ANGLE_TILT2);
    addParamWidget(row, resetAll, ANGLE_ROTATE2);
    addParamWidget(row, resetAll, HIDE_OUTSIDE_DEGREE);

    gl->addItem(new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding), row, 0);
}

void MainWindow::adjustDefaults()
{
    CIRCLE_CENTX.def = -0.94;
    CIRCLE_CENTY.def = -0.91;
    CIRCLE_RAD.def = 33.43;
}

void MainWindow::addParamWidget(int& row, QPushButton* resetAll, DCTL_CheckBox& cbData)
{
    cbData.value = cbData.def;

    QGridLayout* gl = dynamic_cast<QGridLayout*>(ui->scrollArea->widget()->layout());

    auto w = new QCheckBox(cbData.name);
    cbData.cb = w;

    w->setChecked(cbData.def);

    connect(w, &QCheckBox::stateChanged, &cbData, &DCTL_CheckBox::stateChanged);
    connect(w, &QCheckBox::stateChanged, ui->widget, qOverload<>(&GrWidget::updateResultImage));
    cbData.stateChanged(w->checkState());

    gl->addWidget(w, row, 1);

    auto rb = new QPushButton("⟲");
    gl->addWidget(rb, row, 3);
    connect(rb, &QPushButton::clicked, &cbData, &DCTL_CheckBox::reset);
    connect(resetAll, &QPushButton::clicked, &cbData, &DCTL_CheckBox::reset);

    ++row;
}

void MainWindow::addParamWidget(int& row, QPushButton* resetAll, DCTL_Slider& sliderData)
{
    sliderData.value = sliderData.def;

    QGridLayout* gl = dynamic_cast<QGridLayout*>(ui->scrollArea->widget()->layout());

    auto l = new QLabel(sliderData.name);
    auto w = new QSlider(Qt::Horizontal);
    sliderData.sl = w;

    w->setMinimum(0);
    w->setMaximum((sliderData.hi - sliderData.lo)/sliderData.step);
    w->setValue(std::floor((sliderData.value - sliderData.lo)/sliderData.step));
    w->setPageStep(1);

    connect(w, &QSlider::valueChanged, &sliderData, &DCTL_Slider::valueChanged);
    connect(w, &QSlider::valueChanged, ui->widget, qOverload<>(&GrWidget::updateResultImage));

    auto v = new QLabel();
    sliderData.labelValue = v;
    sliderData.valueChanged(w->value());

    gl->addWidget(l, row, 0);
    gl->addWidget(w, row, 1);
    gl->addWidget(v, row, 2);

    auto rb = new QPushButton("⟲");
    gl->addWidget(rb, row, 3);
    connect(rb, &QPushButton::clicked, &sliderData, &DCTL_Slider::reset);
    connect(resetAll, &QPushButton::clicked, &sliderData, &DCTL_Slider::reset);

    ++row;
}

void MainWindow::updateZoom(const QString& zoom)
{
    QString z = zoom;
    z.replace('%', ' ');
    m_zoom = z.trimmed().toInt();
    m_zoom = std::clamp(m_zoom, 1, 100);

    ui->widget->updateResultImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openImage(QString newImageFilename)
{
    m_filename = newImageFilename;
    ui->statusbar->showMessage("Opening...");
    QApplication::processEvents();
    if (m_image.load(m_filename))
    {
        ui->statusbar->showMessage("Image opened, " + QString::number(m_image.width()) + "x" + QString::number(m_image.height()));
        txr.resize(m_image.width(), m_image.height());
        txg.resize(m_image.width(), m_image.height());
        txb.resize(m_image.width(), m_image.height());

        for (int y = 0; y != m_image.height(); ++y)
        {
            for (int x = 0; x != m_image.width(); ++x)
            {
                QRgb rgb = m_image.pixel(x, y);

                txr.p[x + y * txr.w] = qRed(rgb)/255.f;
                txg.p[x + y * txg.w] = qGreen(rgb)/255.f;
                txb.p[x + y * txb.w] = qBlue(rgb)/255.f;

            }
        }
        ui->widget->updateResultImage();
    }
    else
    {
        ui->statusbar->showMessage("Error loading " + m_filename);
    }
}

void MainWindow::on_actionOpen_triggered()
{
    QFileDialog dialog(this);
    dialog.setNameFilter("Images (*.png *.jpg);;All files (*)");
    if (dialog.exec())
    {
        if (dialog.selectedFiles().size() == 1)
        {
            openImage(dialog.selectedFiles()[0]);
        }
    }
}

void MainWindow::on_actionSave_As_triggered()
{
    QFileDialog dialog(this);
    QString saveFileName = QFileDialog::getSaveFileName(this, QString(),
                                                        m_filename + "_dfe.png",
                                                        tr("PNG image (*.png)"));
    if (!saveFileName.isEmpty())
    {
        ui->statusbar->showMessage("Saving...");
        QApplication::processEvents();

        QImage proc;
        ui->widget->processImage(proc, 100, 1);

        proc.save(saveFileName);

        ui->statusbar->showMessage("Saved " + saveFileName);
    }
}

void MainWindow::on_comboBoxZoom_currentTextChanged(const QString &arg1)
{
    updateZoom(arg1);
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About DeFisheye",
                             "© 2023 Franticware\n"
                             "github.com/Franticware/DCTL-Qt\n\n"
                             "DCTL plugin by roukou3\n"
                             "github.com/roukou3/DCTL\n\n"
                             "MIT License"
                             ,
                             QMessageBox::Ok);

}


void MainWindow::on_checkBoxCenterLines_stateChanged(int)
{
    ui->widget->update();
}


void MainWindow::on_actionE_xit_triggered()
{
    close();
}

