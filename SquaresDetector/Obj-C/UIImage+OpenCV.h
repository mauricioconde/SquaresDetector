//
//  UIImage+OpenCV.h
//  SquaresDetector
//
//  Created by Mauricio Conde on 15/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//



#import <UIKit/UIKit.h>
#include <opencv2/opencv.hpp>




@interface UIImage (OpenCV)

//cv::Mat to UIImage
+(UIImage *)UIImageFromCVMat:(cv::Mat)cvMat;

//UIImage to cv::Mat
-(cv::Mat)CVMat;
+ (cv::Mat) CVMatFromUIImage:(UIImage *)image;

@end
