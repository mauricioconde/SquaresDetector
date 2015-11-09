//
//  CVWrapper.h
//  FacialRecognitionSwift
//
//  Created by Mauricio Conde on 10/6/14.
//  Copyright (c) 2014 gbmobile. All rights reserved.
//


#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface CVWrapper : NSObject

+ (UIImage*) detectedSquaresInImage:(UIImage*) image;

+ (UIImage*) detectedSquaresInImage:(UIImage*) image
                          tolerance:(CGFloat)tolerance
                          threshold:(NSInteger)threshold
                             levels:(NSInteger)levels
                           accuracy:(NSInteger)accuracy;

+ (NSArray*) getSquaresInImage: (UIImage*) image
                        tolerance:(CGFloat)tolerance
                        threshold:(NSInteger)threshold
                           levels:(NSInteger)levels
                         accuracy:(NSInteger)accuracy;

+(UIImage*) detectColorInImage: (UIImage*) image
                        minHSV:(NSArray*)minHSV
                        maxHSV:(NSArray*)maxHSV;

+(UIImage*) applyPerspectiveCorrection: (UIImage*) image;

@end
