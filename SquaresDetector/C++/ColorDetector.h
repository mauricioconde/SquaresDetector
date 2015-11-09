//
//  ColorDetector.h
//  SquaresDetector
//
//  Created by Mauricio Conde on 18/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#ifndef __SquaresDetector__ColorDetector__
#define __SquaresDetector__ColorDetector__

#include <stdio.h>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class ColorDetector
{
    
public:
    static cv::Mat detectColor(cv::Mat imgOriginal, std::vector<int> minHSV, std::vector<int> maxHSV);
};

#endif /* defined(__SquaresDetector__ColorDetector__) */
