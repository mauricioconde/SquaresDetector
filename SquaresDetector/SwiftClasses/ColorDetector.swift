//
//  ColorDetector.swift
//  SquaresDetector
//
//  Created by gbmobile on 19/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

import Foundation

class ColorDetector {
    
    var lowH = 75;
    var highH = 130;
    
    var lowS = 90;
    var highS = 255;
    
    var lowV = 80;
    var highV = 255;
    
    var minHSV: [Int];
    var maxHSV: [Int];
    
    
    
    
    init(){
        
        self.lowH = 75;
        self.lowS = 90;
        self.lowV = 80;
        
        self.highH = 130;
        self.highS = 255;
        self.highV = 255;
        
        minHSV = [lowH,lowS,lowV];
        maxHSV = [highH,highS,highV];
        
    }
    
    init(minHSV: [Int], maxHSV: [Int]){
        self.minHSV = minHSV;
        self.maxHSV = maxHSV;
        
        self.lowH = minHSV[0];
        self.lowS = minHSV[1];
        self.lowV = minHSV[2];
        
        self.highH = maxHSV[0];
        self.highS = maxHSV[1];
        self.highV = maxHSV[2];
    }
}