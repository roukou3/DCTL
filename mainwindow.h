#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSlider>
#include <QCheckBox>
#include <QImage>
#include <QPushButton>

#include "dctl.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_As_triggered();
    void on_actionAbout_triggered();
    void on_comboBoxZoom_currentTextChanged(const QString &arg1);
    void on_checkBoxCenterLines_stateChanged(int);
    void on_actionE_xit_triggered();

private:
    Ui::MainWindow *ui;

    void addParamWidget(int& row, QPushButton* resetAll, DCTL_CheckBox&);
    void addParamWidget(int& row, QPushButton* resetAll, DCTL_Slider&);

    void adjustDefaults();
    void updateZoom(const QString& zoom);

    QString m_filename;
    QImage m_image;
    int m_zoom;

    Texture txr;
    Texture txg;
    Texture txb;

    void openImage(QString newImageFilename);
};

#endif // MAINWINDOW_H
