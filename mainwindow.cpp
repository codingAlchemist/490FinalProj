#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    img = new Image();
    hsvImg = new Image();
    result = new Image();
    testImg = new Image();
    backgroundMask = new Image();

    beginPaint = false;
    img->setCameraNum(0);
    img->takePicture();

    hueVal = ui->verticalSlider->value();
    satVal = ui->verticalSlider_2->value();

    ui->Display_Window->setScaledContents(true);
    ui->Display_Window_2->setScaledContents(true);
    ui->Display_Window_5->setScaledContents(true);
    ui->BackgroundMask->setScaledContents(true);

    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(displayVideo()));
    timer->start(10);

}

void MainWindow::displayVideo(){
    img->takePicture();
    img->handDetect3(hsvImg,result,hueVal,satVal);
    backgroundMask->applyBackgroundSub(img);
    if(beginPaint){
        result->handDetect(true,img,paintColorIndex);
    }else{
        result->handDetect(false,img,paintColorIndex);
    }


    img->displayImage(*ui->Display_Window,false);
    hsvImg->displayImage(*ui->Display_Window_2,false);
    result->displayImage(*ui->Display_Window_5,false);
    backgroundMask->displayImage(*ui->BackgroundMask,true);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::on_verticalSlider_valueChanged(int value)
{
    hueVal = value;
}

void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    satVal = value;
}

void MainWindow::on_BeginPaintButt_clicked()
{
    if(!beginPaint){
        beginPaint = true;
    }else{
        beginPaint = false;
    }
}


void MainWindow::on_saveDataButton_clicked()
{
    img->saveImage("bw_contours");
    hsvImg->saveImage("hsv");
    result->saveImage("paint_img");
}
void MainWindow::on_blackbutton_clicked(){
    paintColorIndex = 4;
    ui->blackSelectedLabel->setVisible(true);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_whitebutton_clicked(){
    paintColorIndex = 3;
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(true);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_redbutton_clicked(){
    paintColorIndex = 0;
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(true);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_bluebutton_clicked(){
    paintColorIndex = 1;
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(true);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_greenbutton_clicked(){
    paintColorIndex = 2;
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(true);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_yellowbutton_clicked(){
    paintColorIndex = 5;
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(true);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_invertButton_clicked(){
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(true);
    ui->blackWhiteSelectedLabel->setVisible(false);
}

void MainWindow::on_blackWhitebutton_clicked(){
    paintColorIndex = 6;
    ui->blackSelectedLabel->setVisible(false);
    ui->redSelectedLabel->setVisible(false);
    ui->whiteSelectedLabel->setVisible(false);
    ui->blueSelectedLabel->setVisible(false);
    ui->greenSelectedLabel->setVisible(false);
    ui->yellowSelectedLabel->setVisible(false);
    ui->invertSelectedLabel->setVisible(false);
    ui->blackWhiteSelectedLabel->setVisible(true);
}
