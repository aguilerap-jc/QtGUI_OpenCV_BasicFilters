#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <time.h>
#include <mainwindow.h>

using namespace cv;
using namespace std;

int threshold_value = 100;

Mat getWebCamImage(){
    //VideoCapture cap(0);
    //Mat frame;
    //cap >> frame;
    //return frame;
}

Mat oCVmirrorFilter(Mat originalImage){
    Mat ret;
    flip(originalImage, ret, 1);
    return ret;
}

Mat oCVerodeFilter(Mat originalImage, int erosion_type){
    int erosion_elem = 0;
    int erosion_size = 3;
    int threshold_value = 200;
    int threshold_type = 0;
    Mat erosionImg = originalImage.clone();

    Mat threasholdImg ;
    cvtColor( originalImage, threasholdImg, CV_BGR2GRAY );
    //imshow("B/W", threasholdImg);
    threshold( threasholdImg, threasholdImg, threshold_value, 255 ,threshold_type );

    if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
    else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
    else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement(erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ));

    //int erosion_type;


    erode(threasholdImg,erosionImg,element);
    //imshow("Erotion Demo", erosionImg);
    return erosionImg;
}

Mat oCVdilationFilter(Mat originalImage, int dilation_type){
    int dilation_elem = 0;
    int dilation_size = 3;
    int threshold_value = 200;
    int threshold_type = 0;

    Mat dilationImg = originalImage.clone();
    //Image Binarization
    Mat threasholdImg ;
    cvtColor( originalImage, threasholdImg, CV_BGR2GRAY );
    //threasholdImg = originalImage.clone();
    //imshow("B/W", threasholdImg);
    threshold( threasholdImg, threasholdImg, threshold_value, 255 ,threshold_type );
    //imshow("Threahold Img", threasholdImg);

    //int dilation_type;
    if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
    else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
    else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }

    Mat element = getStructuringElement(dilation_type,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ));

    dilate(threasholdImg,dilationImg,element);
    //imshow("Dilation Demo", dilationImg);
    return dilationImg;

}

Mat oCVbinaryFilter(Mat originalImage, int thrs){
    int threshold_type = 0;
    threshold_value = 100;
    Mat thresholdImg;
    cvtColor( originalImage, thresholdImg, CV_BGR2GRAY );
    threshold( thresholdImg, thresholdImg, threshold_value, 255 ,threshold_type );
    return thresholdImg;
}

Mat oCVenhancedFilter(Mat originalImage){
    Mat enhancementImg = originalImage.clone();
    /*
    Mat lapl = originalImage.clone();
    lapl = HPFLaplacian(enhancementImg);
    substract(lapl,enhancementImg,enhancementImg);
    */
    GaussianBlur(enhancementImg, enhancementImg, cv::Size(0, 0), 0.1);
    addWeighted(enhancementImg, 1.5, enhancementImg, -0.5, 0, enhancementImg);

    return enhancementImg;
}

Mat oCVedgesFilter(Mat originalImage){
    int KENREL_SIZE = 3;
    Mat src_gray, detected_edges;
    int LOWTHRESHOLD = 75;
    int RATIO = 2;
    /// Convert the image to grayscale
    Mat edgeDetectorImg = originalImage.clone();
    cvtColor( originalImage, src_gray, CV_BGR2GRAY );

    // Create a Trackbar for user to enter threshold
    //createTrackbar( "Min Threshold:", "Edge Detector Image" , &LOWTHRESHOLD, MAX_LOWTHRESHOLD );

    /// Reduce noise with a kernel 3x3
    blur( src_gray, detected_edges, Size(3,3) );

    /// Canny detector
    Canny( detected_edges, detected_edges, LOWTHRESHOLD, LOWTHRESHOLD*RATIO, KENREL_SIZE );

    /// Using Canny's output as a mask, we display our result
    edgeDetectorImg = Scalar::all(0);

    originalImage.copyTo( edgeDetectorImg, detected_edges);
    //imshow( "Edge Detector Image", edgeDetectorImg );
    //imshow( "detected_edges Image", detected_edges );
    return detected_edges;
}



Mat oCVinverseFilter(Mat originalImage, int thrs){
    thrs = 100;
    Mat o = oCVbinaryFilter(originalImage, thrs);
    Mat inv;
    subtract(Scalar::all(255),o,inv);
    return inv;
}

Mat oCVgrayscale(Mat originalImage){
    Mat thresholdImg;
    cvtColor( originalImage, thresholdImg, CV_BGR2GRAY );
    return thresholdImg;
}

Mat rAgrayscale(Mat originalImage){
    Mat grayscale(originalImage.size(),CV_8U );

    for(int i = 0; i < originalImage.rows; i++){
        for(int j = 0; j < originalImage.cols; j++){
            Vec3b intensity = originalImage.at<Vec3b>(i,j);
            grayscale.at<uchar>(i,j) = (0.07*intensity.val[0] + 0.72*intensity.val[1] + 0.21*intensity.val[2] );
        }
    }
    return grayscale;
}

Mat rAerodeFilter(Mat originalImage){

}

Mat rAdilationFilter(Mat originalImage){

}

Mat rAbinaryFilter(Mat originalImage, int thrs){
    Mat grayscale(originalImage.size(),CV_8U );
    thrs = 100;
    for(int i = 0; i < originalImage.rows; i++){
        for(int j = 0; j < originalImage.cols; j++){
            Vec3b intensity = originalImage.at<Vec3b>(i,j);
            int value = (0.07*intensity.val[0] + 0.72*intensity.val[1] + 0.21*intensity.val[2] );
            if(value < thrs)
                grayscale.at<uchar>(i,j) = 0;
            else
                grayscale.at<uchar>(i,j) = 255;
        }
    }
    return grayscale;
}

Mat rAenhancedFilter(Mat originalImage){

}

Mat rAedgesFilterV01(Mat originalImage){
    Mat grayscale(originalImage.size(),CV_8U );
    Mat tempImg(originalImage);
    Mat Gx = (Mat_<double>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
    Mat Gy = (Mat_<double>(3,3) << -1, -2, -1, 0, 0, 0, 1, 2, 1);
    for(int i = 1; i < originalImage.rows -1; i++){
        for(int j = 1; j < originalImage.cols -1; j++){
            Vec3b intensity = originalImage.at<Vec3b>(i,j);

            //grayscale.at<uchar>(i,j) = (0.07*intensity.val[0] + 0.72*intensity.val[1] + 0.21*intensity.val[2] );
        }
    }
    return grayscale;
}

Mat rAinverseFilter(Mat originalImage, int thrs){
    Mat grayscale(originalImage.size(),CV_8U );
    thrs = 100;
    for(int i = 0; i < originalImage.rows; i++){
        for(int j = 0; j < originalImage.cols; j++){
            Vec3b intensity = originalImage.at<Vec3b>(i,j);
            int value = (0.07*intensity.val[0] + 0.72*intensity.val[1] + 0.21*intensity.val[2] );
            if(value < thrs)
                grayscale.at<uchar>(i,j) = 255;
            else
                grayscale.at<uchar>(i,j) = 0;
        }
    }
    return grayscale;
}


Mat aRedgesFilter(Mat originalImage, int SobelG){
    /// Convert it to gray
        //cvtColor( image, image, CV_RGB2GRAY );
        //resize(image,image,Size(0,0),0.5,0.5,INTER_LINEAR);
        /// Generate grad_x and grad_y
        Mat grad_x, grad_y;
        Mat abs_grad_x, abs_grad_y;
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;
        Mat grad;
        Mat image(originalImage);

        /// Gradient X
        //Scharr( image, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
        Sobel( image, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_x, abs_grad_x );

        /// Gradient Y
        // Scharr( image, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
        Sobel( image, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
        convertScaleAbs( grad_y, abs_grad_y );
        /// Total Gradient (approximate)

        addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

         Mat orientation = Mat(abs_grad_x.rows, abs_grad_y.cols, CV_32F); //to store the gradients
          Mat img=Mat(abs_grad_x.rows, abs_grad_y.cols, CV_32F);//to draw out the map
          img = cv::Scalar(255,255,255);//all white


         // Calculate orientations of gradients --> in degrees
        // Loop over all matrix values and calculate the accompanied orientation

         for(int i = 0; i < abs_grad_x.rows; i++){
            for(int j = 0; j < abs_grad_x.cols; j++){
                // Retrieve a single value

                float valueX = abs_grad_x.at<float>(i,j);
                float valueY = abs_grad_x.at<float>(i,j);
                // Calculate the corresponding single direction, done by applying the arctangens function
                float result = fastAtan2(valueX,valueY);
                // Store in orientation matrix element
                orientation.at<float>(i,j) = result;

            }
         }
         cvtColor( grad, grad, CV_RGB2GRAY );
         threshold( grad, grad, 100, 255 ,0);
         cvtColor( abs_grad_x, abs_grad_x, CV_RGB2GRAY );
         threshold( abs_grad_x, abs_grad_x, 100, 255 ,0);
         cvtColor( abs_grad_y, abs_grad_y, CV_RGB2GRAY );
         threshold( abs_grad_y, abs_grad_y, 100, 255 ,0);

         if( SobelG == 1)
            return grad;
         else if(SobelG == 2)
            return abs_grad_x;
         else
             return abs_grad_y;

}
