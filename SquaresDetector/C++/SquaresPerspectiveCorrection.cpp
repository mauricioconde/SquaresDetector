//
//  SqauresPerspectiveCorrection.cpp
//  SquaresDetector
//
//  Created by Mauricio Conde on 20/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#include "SquaresPerspectiveCorrection.h"


cv::Point2f center(0,0);
static cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b);
static void sortCorners(std::vector<cv::Point2f>& corners,cv::Point2f center);
static void drawLines(std::vector<cv::Vec4i> lines, cv::Mat &image);
static void drawCornerPoints(cv::Mat &image, std::vector<cv::Point2f> corners);
static void drawMassCenter(cv::Mat &image, cv::Point2f center);

cv::Mat SquaresPerspectiveCorrection::applyPerspectiveCorrection (cv::Mat image, std::vector<cv::Point> square) {
    std::vector<cv::Point2f> corners;
    cv::Mat(square).copyTo(corners);
    
    //Check if the approximate polygonal curve has 4 vertices
    std::vector<cv::Point2f> approx;
    cv::approxPolyDP(cv::Mat(corners), approx, cv::arcLength(cv::Mat(corners), true) * 0.02, true);
    if (approx.size() != 4)
    {
        std::cout << "The object is not quadrilateral!\n" << std::endl;
        //return -1;
    }else std::cout << "The object is a quadrilateral!\n";
    
    //Determine top-left, bottom-left, top-right, and bottom-right corner
    
    // Get mass center
    for (int i = 0; i < corners.size(); i++)
        center += corners[i];
    center *= (1. / corners.size());
    sortCorners(corners, center);
    if (corners.size() == 0){
        std::cout << "The corners were not sorted correctly!\n" << std::endl;
        //return -1;
    }
    
    //Apply the perspective transformation
    // Define the destination image
    cv::Mat quad = cv::Mat::zeros(300, 220, CV_8UC3);
    
    // Corners of the destination image
    std::vector<cv::Point2f> quad_pts;
    quad_pts.push_back(cv::Point2f(0, 0));
    quad_pts.push_back(cv::Point2f(quad.cols, 0));
    quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
    quad_pts.push_back(cv::Point2f(0, quad.rows));
    
    // Get transformation matrix
    cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
    
    // Apply perspective transformation
    cv::warpPerspective(image, quad, transmtx, quad.size());
    std::cout << "perspective applied\n";
    
    return quad;
    
}
cv::Mat SquaresPerspectiveCorrection::applyPerspectiveCorrection (cv::Mat image) {
    std::cout << "everything starts\n";
    //Get the edge map using Canny operator
    
    cv::Mat bw; //grayscaled source image
    cv::cvtColor(image, bw, CV_BGR2GRAY);
    cv::blur(bw, bw, cv::Size(3, 3));
    cv::Canny(bw, bw, 100, 100, 3);
    std::cout << "image changed to grayscale\n";
    
    //Detect lines with Hough transform
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(bw, lines, 1, CV_PI/180, 70, 30, 10);
    std::cout << "lines detected in image\n";
    
    
    // Needed for visualization only.  Expand the lines
    for (int i = 0; i < lines.size(); i++)
    {
        cv::Vec4i v = lines[i];
        lines[i][0] = 0;
        lines[i][1] = ((float)v[1] - v[3]) / (v[0] - v[2]) * -v[0] + v[1];
        lines[i][2] = image.cols;
        lines[i][3] = ((float)v[1] - v[3]) / (v[0] - v[2]) * (image.cols - v[2]) + v[3];
    }
    
    //Get the corners by finding intersections between lines
    std::vector<cv::Point2f> corners;
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = i+1; j < lines.size(); j++)
        {
            cv::Point2f pt = computeIntersect(lines[i], lines[j]); //-> static calling
            if (pt.x >= 0 && pt.y >= 0)
                corners.push_back(pt);
        }
    }
    std::cout << "corners found\n";
    
    
    //Check if the approximate polygonal curve has 4 vertices
    std::vector<cv::Point2f> approx;
    cv::approxPolyDP(cv::Mat(corners), approx, cv::arcLength(cv::Mat(corners), true) * 0.02, true);
    if (approx.size() != 4)
    {
        std::cout << "The object is not quadrilateral!\n" << std::endl;
        //return -1;
    }else std::cout << "The object is a quadrilateral!\n";

    //Determine top-left, bottom-left, top-right, and bottom-right corner
    
    // Get mass center
    for (int i = 0; i < corners.size(); i++)
        center += corners[i];
    center *= (1. / corners.size());
    std::cout << "mass center calculated\n";
    sortCorners(corners, center);
    if (corners.size() == 0){
        std::cout << "The corners were not sorted correctly!\n" << std::endl;
        //return -1;
    }
    cv::Mat dst = image.clone();
    
    // Draw lines
    drawLines(lines, dst);
    
    // Draw corner points
    drawCornerPoints(dst, corners);
    
    // Draw mass center
    drawMassCenter(dst, center);
    
    //Apply the perspective transformation
    // Define the destination image
    cv::Mat quad = cv::Mat::zeros(300, 220, CV_8UC3);
    
    // Corners of the destination image
    std::vector<cv::Point2f> quad_pts;
    quad_pts.push_back(cv::Point2f(0, 0));
    quad_pts.push_back(cv::Point2f(quad.cols, 0));
    quad_pts.push_back(cv::Point2f(quad.cols, quad.rows));
    quad_pts.push_back(cv::Point2f(0, quad.rows));
    
    // Get transformation matrix
    cv::Mat transmtx = cv::getPerspectiveTransform(corners, quad_pts);
    
    // Apply perspective transformation
    cv::warpPerspective(image, quad, transmtx, quad.size());
    std::cout << "perspective applied\n";
    //cv::imshow("image", dst);
    //cv::imshow("quadrilateral", quad);
    
    return quad;
    
}

//Get the corners by finding intersections between lines
static cv::Point2f computeIntersect(cv::Vec4i a, cv::Vec4i b)
{
    int x1 = a[0], y1 = a[1], x2 = a[2], y2 = a[3], x3 = b[0], y3 = b[1], x4 = b[2], y4 = b[3];
    float denom;
    if (float d = ((float)(x1 - x2) * (y3 - y4)) - ((y1 - y2) * (x3 - x4)))
    {
        cv::Point2f pt;
        pt.x = ((x1 * y2 - y1 * x2) * (x3 - x4) - (x1 - x2) * (x3 * y4 - y3 * x4)) / d;
        pt.y = ((x1 * y2 - y1 * x2) * (y3 - y4) - (y1 - y2) * (x3 * y4 - y3 * x4)) / d;
        return pt;
    }
    else
        return cv::Point2f(-1, -1);
}

/*
 To determine the top-left, bottom-left, top-right, and bottom right corner, we’ll use the simplest method:
 
 Get the mass center.
 Points that have lower y-axis than mass center are the top points, otherwise they are bottom points.
 Given two top points, the one with lower x-axis is the top-left. The other is the top-right.
 Given two bottom points, the one with lower x-axis is the bottom-left. The other is the bottom-right.
 */
static void sortCorners(std::vector<cv::Point2f>& corners,cv::Point2f center)
{
    std::vector<cv::Point2f> top, bot;
    for (int i = 0; i < corners.size(); i++)
    {
        if (corners[i].y < center.y)
            top.push_back(corners[i]);
        else
            bot.push_back(corners[i]);
    }
    corners.clear();
    if (top.size() == 2 && bot.size() == 2){
        cv::Point2f tl = top[0].x > top[1].x ? top[1] : top[0];
        cv::Point2f tr = top[0].x > top[1].x ? top[0] : top[1];
        cv::Point2f bl = bot[0].x > bot[1].x ? bot[1] : bot[0];
        cv::Point2f br = bot[0].x > bot[1].x ? bot[0] : bot[1];
        corners.push_back(tl);
        corners.push_back(tr);
        corners.push_back(br);
        corners.push_back(bl);
    }
}

static void drawLines(std::vector<cv::Vec4i> lines, cv::Mat &image){
    
    for (int i = 0; i < lines.size(); i++)
    {
        cv::Vec4i v = lines[i];
        cv::line(image, cv::Point(v[0], v[1]), cv::Point(v[2], v[3]), CV_RGB(0,255,0));
    }
    std::cout << "lines drawn\n";
    
}

static void drawCornerPoints(cv::Mat &image, std::vector<cv::Point2f> corners){
    for (int i=0; i<corners.size(); i++) {
        //Image,center,radius,color,
        cv::circle(image, corners[i], 3, CV_RGB(255,0,0), 2);
    }
    /*cv::circle(image, corners[0], 3, CV_RGB(255,0,0), 2);
    cv::circle(image, corners[1], 3, CV_RGB(0,255,0), 2);
    cv::circle(image, corners[2], 3, CV_RGB(0,0,255), 2);
    cv::circle(image, corners[3], 3, CV_RGB(255,255,255), 2);*/
    std::cout << "corner points drawn\n";
}

static void drawMassCenter(cv::Mat &image, cv::Point2f center){
    cv::circle(image, center, 3, CV_RGB(255,255,0), 2);
    std::cout << "mass center drawn\n";
}