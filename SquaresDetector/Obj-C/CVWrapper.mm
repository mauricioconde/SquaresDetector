//
//  CVWrapper.m
//  FacialRecognitionSwift
//
//  Created by Mauricio Conde on 10/6/14.
//  Copyright (c) 2014 gbmobile. All rights reserved.
//

#import "CVWrapper.h"
#import "UIImage+OpenCV.h"
#import "Squares.h"
#import "SquaresPerspectiveCorrection.h"
#import "ColorDetector.h"

using namespace std;
using namespace cv;




//remove 'magic numbers' from original C++ source so we can manipulate them from obj-C
#define TOLERANCE 0.3
#define THRESHOLD 50
#define LEVELS 9
#define ACCURACY 0




@implementation CVWrapper

+ (UIImage*) detectedSquaresInImage:(UIImage*) image
{
    //if we call this method with no parameters,
    //we use the defaults from the original c++ project
    return [[self class] detectedSquaresInImage:image
                                      tolerance:TOLERANCE
                                      threshold:THRESHOLD
                                         levels:LEVELS
                                       accuracy:ACCURACY];
}

+ (UIImage*) detectedSquaresInImage:(UIImage*) image
                          tolerance:(CGFloat) tolerance
                          threshold:(NSInteger)threshold
                             levels:(NSInteger)levels
                           accuracy:(NSInteger)accuracy
{
    UIImage* uiImage = nil;
    cv::Mat matImage = [image CVMat];
    
    matImage = Squares::detectedSquaresInImage (matImage, tolerance, threshold, levels, accuracy);
    uiImage = [UIImage UIImageFromCVMat:matImage];
    
    return uiImage;
}

+ (NSArray*) getSquaresInImage: (UIImage*) image
                     tolerance:(CGFloat)tolerance
                     threshold:(NSInteger)threshold
                        levels:(NSInteger)levels
                      accuracy:(NSInteger)accuracy
{
    cv::Mat matImage = [image CVMat];
    matImage = Squares::detectedSquaresInImage (matImage, tolerance, threshold, levels, accuracy);
    
    std::vector<cv::Rect> rects = Squares::squaresAsRects();
    int total = rects.size();
    
    NSMutableArray* frames = [[NSMutableArray alloc] init];
    
    for (NSInteger i=0; i<total; i++) {
        CGRect r = CGRectMake(rects[i].tl().x, rects[i].tl().y, rects[i].size().width, rects[i].size().height);
        
        [frames addObject:[NSValue valueWithCGRect:r]];
    }
    return frames;
}

+(UIImage*) detectColorInImage: (UIImage*) image
                        minHSV:(NSArray*)minHSV
                        maxHSV:(NSArray*)maxHSV;
{
    UIImage* result = nil;
    NSNumber *n;
    
    cv::Mat matImage = [image CVMat];
    int count = [minHSV count];
    std::vector<int>pMinHSV;
    std::vector<int> pMaxHSV;
    
    
    for (int i=0; i<count; i++) {
        n = [minHSV objectAtIndex:i];
        pMinHSV.push_back([n intValue]);
    }
    
    for (int i=0; i<count; i++) {
        n = [maxHSV objectAtIndex:i];
        pMaxHSV.push_back([n intValue]);
    }
    
    matImage = ColorDetector::detectColor(matImage, pMinHSV, pMaxHSV);
    result = [UIImage UIImageFromCVMat:matImage];
    return result;
}

+(UIImage*) applyPerspectiveCorrection: (UIImage*) image{
    UIImage* result = nil;
    
    cv::Mat matImage = [image CVMat];
    
    matImage = SquaresPerspectiveCorrection::applyPerspectiveCorrection(matImage);
    result = [UIImage UIImageFromCVMat:matImage];
    return result;
}

@end
