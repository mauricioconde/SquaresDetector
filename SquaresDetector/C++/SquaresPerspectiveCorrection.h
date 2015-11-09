//
//  SqauresPerspectiveCorrection.h
//  SquaresDetector
//
//  Created by Mauricio Conde on 20/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#ifndef __SquaresDetector__SqauresPerspectiveCorrection__
#define __SquaresDetector__SqauresPerspectiveCorrection__

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <stdio.h>

class SquaresPerspectiveCorrection
{
public: static cv::Mat applyPerspectiveCorrection (cv::Mat image, std::vector<cv::Point> square);
    public: static cv::Mat applyPerspectiveCorrection (cv::Mat image);
    
    
};

#endif /* defined(__SquaresDetector__SqauresPerspectiveCorrection__) */
