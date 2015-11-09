//
//  Squares.h
//  SquaresDetector
//
//  Created by Mauricio Conde on 15/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#ifndef __SquaresDetector__Squares__
#define __SquaresDetector__Squares__

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <opencv2/opencv.hpp>

//class definition
//We do not need a class
//as we have no instance variables and just a few static functions.
//We could instead just declare the functions but this form seems clearer

class Squares
{
    public:
        static cv::Mat detectedSquaresInImage (cv::Mat image, float tol, int threshold, int levels, int accuracy);
        static cv::Mat cannyDetect(const cv::Mat &image);
        static void drawSquares(cv:: Mat& image, const std::vector<std::vector<cv::Point> >& squares );
        static std::vector<cv::Rect> squaresAsRects();
        static std::vector<std::vector<cv::Point> > getSquares();
};

#endif /* defined(__SquaresDetector__Squares__) */
