#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <iostream>
#include <string>

#include <QMainWindow>
#include <QtCore>
#include <QTimer>

#include "image.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void displayVideo();
    void on_verticalSlider_valueChanged(int value);

    void on_verticalSlider_2_valueChanged(int value);

    void on_BeginPaintButt_clicked();

    void on_saveDataButton_clicked();

    void on_blackbutton_clicked();

    void on_whitebutton_clicked();

    void on_redbutton_clicked();

    void on_bluebutton_clicked();

    void on_greenbutton_clicked();

    void on_yellowbutton_clicked();

    void on_invertButton_clicked();

    void on_blackWhitebutton_clicked();




public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Image *testImg;
    Image *backgroundMask;
    Ui::MainWindow *ui;
    QTimer *timer;
    Image *img,*hsvImg, *result;
    double satVal,hueVal;
    bool beginPaint;
    int paintColorIndex;

};

#endif // MAINWINDOW_H
