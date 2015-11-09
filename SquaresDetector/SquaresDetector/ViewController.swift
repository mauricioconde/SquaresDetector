//
//  ViewController.swift
//  SquaresDetector
//
//  Created by Mauricio Conde on 14/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

import UIKit

class ViewController: UIViewController {
    
    var imgView: UIImageView!;
    var img: UIImage!;
    
    var detectColor = false;
    var applyPerspectiveCorrection = false;
    var findSquares = true;
    
    var colorDetector: ColorDetector!;
    var squaresDetector: SquaresDetector!;
    
    //Constants for squares detection
    let TOLERANCE: CGFloat = 0.3;
    let THRESHOLD: Int = 50;
    let LEVELS: Int = 9;
    let ACCURACY: Int = 0;

    override func viewDidLoad() {
        super.viewDidLoad()
        
        //load image
        var imgName: String = "";
        if(detectColor){ imgName = "TwoS.png";}
        else if(applyPerspectiveCorrection){imgName = "card.jpg";}
        else if(findSquares){imgName = "squares.jpg";}
        
        img = UIImage(named: imgName)!;
        imgView = (img != nil) ? UIImageView(frame: view.frame) : nil;
        print("image width: \(img.size.width), height: \(img.size.height)")
        
        //set image to main view
        if(imgView != nil) {
            //imgView.frame = view.frame
            imgView.layer.borderColor = UIColor.redColor().CGColor
            imgView.layer.borderWidth = 3
            self.view.addSubview(imgView)
        }
        else { print("image is nil") }
        
        
        if(img != nil) {
            if(findSquares){
                squaresDetector = SquaresDetector(tolerance: TOLERANCE, threshold: THRESHOLD, levels: LEVELS, accuracy: ACCURACY);
                let image: UIImage = squaresDetector.detectedSquaresInImage(img);
                imgView.image = image;
                /*
                var frames: NSArray = CVWrapper.getSquaresInImage(img, tolerance: TOLERANCE, threshold: THRESHOLD, levels: LEVELS, accuracy: ACCURACY);
                
                for(var i=0; i<frames.count; i++){
                    
                    let r: CGRect = (frames.objectAtIndex(i) as NSValue).CGRectValue();
                    let boxView: UIView = UIView(frame: r)
                    boxView.layer.borderColor = UIColor.redColor().CGColor;
                    boxView.layer.borderWidth = 2.0;
                    self.view.addSubview(boxView);
                }*/
                print("boxview added");
                locateTouch()
                
            }
            
            if(detectColor) {
                colorDetector = ColorDetector();
                
                let image: UIImage = CVWrapper.detectColorInImage(img,minHSV: colorDetector.minHSV, maxHSV: colorDetector.maxHSV);
                self.imgView.image = image;
                
            }
            
            if(applyPerspectiveCorrection) {
                print("about to do perspective correction");
                let image: UIImage = CVWrapper.applyPerspectiveCorrection(img);
                self.imgView.image = image;
                print("perspective correction done");
            }
        }
        
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    func locateTouch(){
        func createTempPoint(point: CGPoint, color: UIColor){
            let myButton = UIButton(frame: CGRect(x: 0, y: 0, width: 10, height: 10))
            myButton.backgroundColor = color
            myButton.layer.masksToBounds = true
            myButton.layer.cornerRadius = 10.0
            myButton.layer.position = point
            
            self.imgView.addSubview(myButton)
            
        }
        
        //get the point
        let Point: CGPoint = CGPoint(x: 50, y: 50)
        print("Point: \(Point.x) , \(Point.y)")
        
        let ptX: CGFloat = (Point.x * view.bounds.size.width) / img.size.width
        let ptY: CGFloat = (Point.y * view.bounds.size.height) / img.size.height
        let pt: CGPoint = CGPoint(x: ptX, y: ptY);
        print("pt: \(Point.x) , \(Point.y)")
        
        createTempPoint(Point, color: UIColor.orangeColor())
        createTempPoint(pt, color: UIColor.yellowColor())
    }


}

