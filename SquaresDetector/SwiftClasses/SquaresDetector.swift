//
//  SquaresDetector.swift
//  SquaresDetector
//
//  Created by Mauricio Conde on 22/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

import Foundation

class SquaresDetector {
    
    let TOLERANCE: CGFloat;
    let THRESHOLD: Int;
    let LEVELS: Int;
    let ACCURACY: Int;
    
    var pointsArr: [Int]?;
    
    
    
    init(){
        TOLERANCE = 0.3;
        THRESHOLD = 50;
        LEVELS = 9; // 11
        ACCURACY = 0;
    }
    
    init(tolerance: CGFloat, threshold: Int, levels: Int, accuracy: Int){
        self.TOLERANCE = tolerance;
        self.THRESHOLD = threshold;
        self.LEVELS = levels;
        self.ACCURACY = accuracy;
    }
    
    
    
    
    func detectedSquaresInImage(image: UIImage)-> UIImage {
        return CVWrapper.detectedSquaresInImage(image, tolerance: TOLERANCE, threshold: THRESHOLD, levels: LEVELS, accuracy: ACCURACY);
    }
    
    
    
}