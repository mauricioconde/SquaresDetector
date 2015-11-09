//
//  Utility.cpp
//  SquaresDetector
//
//  Created by Mauricio Conde on 06/02/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#include "Utility.h"
using namespace std;
using namespace cv;

bool Utility::isPointInsideAnySquare(cv::Point point, std::vector<std::vector<cv::Point>> squares)
{
    bool resp = false;
    cv::Point TL,TR,BR,BL;
    float xLeft, xRight;
    
    //we assume that each vector of squares is ordered like this tl,tr,br,bl
    for (int i=0; i<squares.size(); i++) {
        //Retrieve the 4 point of each rect
        TL = squares[i][0];
        TR = squares[i][1];
        BR = squares[i][2];
        BL = squares[i][3];
        
        std::cout << "TL: " << TL << ", TR: " << TR << ", BR: " << BR << ", BL: " << BL;
        
        //Create a valid range in x-axis
        xLeft = (TL.x < BL.x)? TL.x : BL.x;
        xRight = (TR.x > BR.x)? TR.x : BR.x;
        
        //check if the Point.x is inside of the interval we have created
        if(xLeft <= point.x && point.x <= xRight) {
            //create a valid range in y-axis
        }
    }
    
    return resp;
}