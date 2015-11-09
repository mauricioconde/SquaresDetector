//
//  ColorDetector.cpp
//  SquaresDetector
//
//  Created by Mauricio Conde on 18/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#include "ColorDetector.h"

using namespace cv;
using namespace std;

static int SCALAR_SIZE = 3;
int iLowH = 75;
int iHighH = 130;

int iLowS = 90;
int iHighS = 255;

int iLowV = 80;
int iHighV = 255;


cv::Mat ColorDetector::detectColor(cv::Mat imgOriginal, std::vector<int> minHSV, std::vector<int> maxHSV){
    Mat imgHSV;
    Mat imgThresholded;
    
    if (minHSV.size() != SCALAR_SIZE || maxHSV.size() != SCALAR_SIZE) {
        cout << "[Color detector]: inapropiate size either minHSV or maxHSV vector " << minHSV.size() << " - " << maxHSV.size();
        return imgOriginal;
        
    }
    
    //cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    cvtColor(imgOriginal, imgHSV, CV_BGR2HSV    ); //Convert the captured frame from BGR to HSV
    inRange(imgHSV, Scalar(minHSV[0], minHSV[1], minHSV[2]), Scalar(maxHSV[0], maxHSV[1], maxHSV[2]), imgThresholded); //Threshold the image
    
    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    //morphological closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    
    return imgThresholded;
    
}
