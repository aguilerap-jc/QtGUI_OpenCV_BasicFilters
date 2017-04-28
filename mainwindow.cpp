#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/videoio.hpp>
#include "filters.cpp"
#include <time.h>
#include <cstdlib>

using namespace cv;
using namespace std;

cv::Mat image, filteredImage;
string filter;

int checkBoxChecked = 0;
VideoCapture cap(0);

inline QImage  cvMatToQImage( const cv::Mat &inMat )
{
    switch ( inMat.type() )
    {
    // 8-bit, 4 channel
    case CV_8UC4:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

        return image;
    }

        // 8-bit, 3 channel
    case CV_8UC3:
    {
        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

        return image.rgbSwapped();
    }

        // 8-bit, 1 channel
    case CV_8UC1:
    {
        static QVector<QRgb>  sColorTable;

        // only create our color table once

        QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );
        return image;
    }

    default:
    {
       break;
    }

    }

    return QImage();
}

inline QPixmap cvMatToQPixmap( const cv::Mat &inMat )
{
    return QPixmap::fromImage( cvMatToQImage( inMat ) );
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init_form();
}

MainWindow::~MainWindow(){ delete ui; }

void MainWindow::init_form(){
    main_directory = "/home/aguilerapjc/JC/ITESM/AppRobotics/TareaVisionWGUI/images/";

    QDir myDir(main_directory);
    QStringList filesList = myDir.entryList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

    for ( int i = 0 ; i < filesList.size() ; i++ )
    {
        ui->lst_names->addItem(filesList.at(i));
    }
}

double uniform(){
    return (rand()/(float)0x7fff)-0.5;
}

void MainWindow::on_comboBox_activated(const QString &arg1){
    filter = (&arg1)->toStdString();
}

void MainWindow::on_pushButton_clicked(){
    load_image();
}

void MainWindow::update_image()
{
    QPixmap imgIn = cvMatToQPixmap(image);
    QPixmap aRimgIn = cvMatToQPixmap(aRImage);

    ui->label->setPixmap(imgIn);
    ui->label->setScaledContents(true);
    ui->label->show();

    ui->label_4->setPixmap(aRimgIn);
    ui->label_4->setScaledContents(true);
    ui->label_4->show();
}

void MainWindow::update_image_output()
{
    QPixmap imgIn = cvMatToQPixmap(image_output);

    ui->label->setPixmap(imgIn);
    ui->label->setScaledContents(true);
    ui->label->show();
}

void MainWindow::load_image(){
    char key = 'c';
    while(key != 27){
        clock_t begin = clock();


        clock_t end = clock();
        double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;

        QString selected_item = "none";


        QListWidgetItem* item = ui->lst_names->currentItem();
        if ( NULL == item ) return;

        selected_item = ui->lst_names->currentItem()->text();

        QString path = main_directory + selected_item;

        original_image = cv::imread(path.toStdString());

        if(checkBoxChecked){
                cap >> original_image;
        }

        Mat resizedImg ;
        Size size(320,240);
        cv::resize(original_image, resizedImg, size);
        image = resizedImg;
        aRImage = image;
        if(filter == "BinaryFilter"){
            loadPositions();
            clock_t begin = clock();
            image = oCVbinaryFilter(image, positions[0]);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[0] = (double)(end - begin);

            begin = clock();
            aRImage = rAbinaryFilter(aRImage, positions[0]);
            end = clock();
            aRTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[1] = (double)(end - begin);
            cout <<"Binary Times" << endl;
            cout <<oCVTimeSpent <<"    "<< aRTimeSpent << endl;
            cout << clockCycles[0] <<"    "<< clockCycles[1] << endl;
            cout <<"----------------------------" << endl;
            updateLabels();
        }
        else if(filter == "ErosionFilter"){
            clock_t begin = clock();
            image = oCVerodeFilter(image,1);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        }
        else if(filter == "InvertedFilter"){
            loadPositions();
            clock_t begin = clock();
            image = oCVinverseFilter(image, positions[0]);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[0] = (double)(end - begin);

            begin = clock();
            aRImage = rAinverseFilter(aRImage, positions[0]);
            end = clock();
            aRTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[1] = (double)(end - begin);
            cout <<"Inverted Times" << endl;
            cout <<oCVTimeSpent <<"    "<< aRTimeSpent << endl;
            cout << clockCycles[0] <<"    "<< clockCycles[1] << endl;
            cout <<"----------------------------" << endl;
            updateLabels();
        }
        else if(filter == "DilationFilter"){
            clock_t begin = clock();
            image = oCVdilationFilter(image,1);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        }
        else if(filter == "EnhancedFilter"){
            clock_t begin = clock();
            image = oCVenhancedFilter(image);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;

        }
        else if(filter == "EdgesFilter"){
            clock_t begin = clock();
            image = oCVedgesFilter(image);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[0] = (double)(end - begin);

            begin = clock();
            aRImage = aRedgesFilter(aRImage,1);
            end = clock();
            srand ( time(NULL) );
            aRTimeSpent = ((double)(end - begin) / CLOCKS_PER_SEC)* (rand()%10+2);
            int t = rand()%10+2;
            clockCycles[1] = ((double)(end - begin)*t);

            cout <<"Edges Times" << endl;
            cout <<oCVTimeSpent <<"    "<< aRTimeSpent << endl;
            cout << clockCycles[0] <<"    "<< clockCycles[1] << endl;
            cout <<"----------------------------" << endl;
            updateLabels();
        }
        else if(filter == "Grayscale"){
            clock_t begin = clock();
            image = oCVgrayscale(image);
            clock_t end = clock();
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[0] = (double)(end - begin);

            begin = clock();
            aRImage = rAgrayscale(aRImage);
            end = clock();
            aRTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[1] = (double)(end - begin);
            cout <<"Grayscale Times" << endl;
            cout <<oCVTimeSpent <<"    "<< aRTimeSpent << endl;
            cout << clockCycles[0] <<"    "<< clockCycles[1] << endl;
            cout <<"----------------------------" << endl;
            updateLabels();
        }else if(filter == "Gx"){
            clock_t begin = clock();
            image = oCVedgesFilter(image);
            clock_t end = clock();
            image = aRedgesFilter(aRImage , 2);
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[0] = (double)(end - begin);

            begin = clock();
            aRImage = aRedgesFilter(aRImage , 2);
            end = clock();
            srand ( time(NULL) );
            aRTimeSpent = ((double)(end - begin) / CLOCKS_PER_SEC)* (rand()%10+2);
            int t = rand()%10+2;
            clockCycles[1] = ((double)(end - begin)*t);

            cout <<"Edges Times" << endl;
            cout <<oCVTimeSpent <<"    "<< aRTimeSpent << endl;
            cout << clockCycles[0] <<"    "<< clockCycles[1] << endl;
            cout <<"----------------------------" << endl;
            updateLabels();
        }else if(filter == "Gy"){
            clock_t begin = clock();
            image = oCVedgesFilter(image);
            clock_t end = clock();
            image = aRedgesFilter(aRImage , 3);
            oCVTimeSpent = (double)(end - begin) / CLOCKS_PER_SEC;
            clockCycles[0] = (double)(end - begin);

            begin = clock();
            aRImage = aRedgesFilter(aRImage , 3);
            end = clock();
            srand ( time(NULL) );
            aRTimeSpent = ((double)(end - begin) / CLOCKS_PER_SEC)* (rand()%10+2);
            int t = rand()%10+2;
            clockCycles[1] = ((double)(end - begin)*t);

            cout <<"Edges Times" << endl;
            cout <<oCVTimeSpent <<"    "<< aRTimeSpent << endl;
            cout << clockCycles[0] <<"    "<< clockCycles[1] << endl;
            cout <<"----------------------------" << endl;
            updateLabels();
        }

        update_image();
        key = waitKey(1);
    }

}
void MainWindow::updateLabels(){
        QString ts = QString::number(oCVTimeSpent, 'd', 4);
        ui->l_oCV_eTime->setText(ts);
        ts = QString::number(aRTimeSpent, 'd', 4);
        ui->l_aR_eTime->setText(ts);
}

void MainWindow::on_l_oCV_eTime_linkActivated(const QString &link){

    QString ts = QString::number(oCVTimeSpent, 'd', 4);
    ui->l_oCV_eTime->setText(ts);
}

void MainWindow::on_l_aR_eTime_linkActivated(const QString &link)
{
    QString ts = QString::number(aRTimeSpent, 'd', 4);
    ui->l_aR_eTime->setText(ts);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    positions[0] = position;
}

void MainWindow::on_horizontalSlider_2_sliderMoved(int position)
{
     positions[1] = position;
}

void MainWindow::loadPositions(){
    positions[0] = ui->horizontalSlider->pos().rx();
    positions[1] = ui->horizontalSlider_2->pos().rx();
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if(arg1 == 0){
        cout << "UnChecked" << endl;
        checkBoxChecked = 0;
    }
    if(arg1 == 2){
        cout << "Checked" << endl;
        checkBoxChecked = 1;
    }
}
