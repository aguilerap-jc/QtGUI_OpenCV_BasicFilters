#ifndef FILTERS_H
#define FILTERS_H

#endif // FILTERS_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"

using namespace cv;
using namespace std;

class filters{

    public:
        Mat oCVErodeFilter(Mat originalImage, int erosion_type);
        Mat oCVdilationFilter(Mat originalImage, int dilation_type);
        Mat oCVbinaryFilter(Mat originalImage, int thrs);
        Mat oCVenhancedFilter(Mat originalImage);
        Mat oCVedgesFilter(Mat originalImage);
        Mat oCVinverseFilter(Mat originalImage, int thrs);
        Mat oCVmirrorFilter(Mat originalImage);
        Mat oCVgrayscale(Mat originalImage);

        Mat rAgrayscale(Mat originalImage);
        Mat rAerodeFilter(Mat originalImage);
        Mat rAdilationFilter(Mat originalImage);
        Mat rAbinaryFilter(Mat originalImage, int thrs);
        Mat rAenhancedFilter(Mat originalImage);
        Mat rAedgesFilter(Mat originalImage, int SobelG);
        Mat rAinverseFilter(Mat originalImage, int thrs);
        Mat rAedgesFilterV01(Mat originalImage);
    private:
};
