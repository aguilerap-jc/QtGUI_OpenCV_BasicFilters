#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDir>
#include <QString>
#include <math.h>

//--------------------------------------------- OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <time.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    cv::Mat image_output;
    void update_image_output();

    ~MainWindow();

private slots:
    void on_pushButton_clicked(bool checked);

    void on_comboBox_activated(const QString &arg1);

    void on_pushButton_clicked();

    void on_l_oCV_eTime_linkActivated(const QString &link);

    void on_l_aR_eTime_linkActivated(const QString &link);


    void on_horizontalSlider_sliderMoved(int position);

    void on_horizontalSlider_2_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    cv::Mat original_image; //loaded image
    cv::Mat image; //current image
    cv::Mat aRImage;

    void init_form();
    void preprocess_image();
    void load_image();
    void update_image();
    void updateLabels();

    double aRTimeSpent = -1;
    double oCVTimeSpent = -1;
    double clockCycles[2];
    int positions[2];
    void loadPositions();
    QString main_directory;

};

#endif // MAINWINDOW_H
