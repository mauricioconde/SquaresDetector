//
//  Utility.h
//  SquaresDetector
//
//  Created by Mauricio Conde on 06/02/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#ifndef __SquaresDetector__Utility__
#define __SquaresDetector__Utility__

#include <stdio.h>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>

class Utility
{
    public:
        static bool isPointInsideAnySquare(cv::Point point, std::vector<std::vector<cv::Point>> squares);
    
};

#endif /* defined(__SquaresDetector__Utility__) */
