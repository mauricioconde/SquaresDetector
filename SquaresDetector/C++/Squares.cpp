//
//  Squares.cpp
//  SquaresDetector
//
//  Created by Mauricio Conde on 15/01/15.
//  Copyright (c) 2015 Mauricio Conde. All rights reserved.
//

#include "Squares.h"
#include "SquaresPerspectiveCorrection.h"
using namespace std;
using namespace cv;




static float tolerance = 0.01; // (0.01 or 0.3)
static int thresh = 50;
static int N = 11; //threshold levels (11 or 9)
static int accuracy = 0;
static int MAX_AREA = 500;
static int LINE_AA = 16; //from: docs.opencv.org/java/2.4.2/constant-values.html#org.opencv.core.Core.LINE_AA
static int RADIUS = 50; // remove edges that reference the same square
static vector<vector<cv::Point> > squares;

//static const string BASE_DIR = "/Users/mauricioconde/Documents/SwiftProjects/SquaresDetector/Images/test/";
static const string BASE_DIR = "/Users/gbmobile/Documents/SwiftProjects/SquaresDetector/Images/test/";


//adding declarations at top of file to allow public function (was main{}) at top
static double angle( Point pt1, Point pt2, Point pt0 );
static void findSquaresOfficial( const Mat& image, vector<vector<Point> >& squares );
static void findSquaresCustom( const Mat& image, vector<vector<Point> >& squares );
static void printSquares(const vector<vector<Point>> & squares);
static vector<Point> retrieveBiggestSquare(vector<vector<Point>> & squares);
static cv::Rect transformPointsToRect(vector<cv::Point> points);
static void drawCornerPoints(cv::Mat &image,  std::vector<std::vector<cv::Point>> squares);




//This public function performs the role of main{} in the original file
//static cv::Mat detectSquaresInImage (cv::Mat image, float tol, int threshold, int levels, int acc)
cv::Mat Squares::detectedSquaresInImage (cv::Mat image, float tol, int threshold, int levels, int acc)
{
    thresh      = threshold;
    N           = levels;
    tolerance   = tol;
    accuracy    = acc;
    
    if( image.empty() ) cout << "Squares.cpp: Couldn't load image" << endl;
    
    // delete
    cv:: Mat img, gray, mask, kernel1, close, div, res, res2;
    cv::GaussianBlur(image, img, cv::Size(5,5), 0); //img = cv2.GaussianBlur(img,(5,5),0)
    cv::cvtColor( img, gray, CV_BGR2GRAY ); //gray = cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
    //mask = np.zeros((gray.shape),np.uint8)
    kernel1 = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(11,11));//kernel1 = cv2.getStructuringElement(cv2.MORPH_ELLIPSE,(11,11))
    
    cv::morphologyEx(gray, close, cv::MORPH_CLOSE, kernel1);//close = cv2.morphologyEx(gray,cv2.MORPH_CLOSE,kernel1)
    //div = np.float32(gray)/(close)
    //res = np.uint8(cv2.normalize(div,div,0,255,cv2.NORM_MINMAX))
    //res2 = cv2.cvtColor(res,cv2.COLOR_GRAY2BGR)
    cv::imwrite(BASE_DIR + "imgGaussian.jpg", img);
    cv::imwrite(BASE_DIR + "imgGray.jpg", gray);
    cv::imwrite(BASE_DIR + "kernel1.jpg", kernel1);
    cv::imwrite(BASE_DIR + "close.jpg", close);
    //
    
    //OPTIMIZATION: Use an image after apply canny function.
    cv::Mat cannyImage = cannyDetect(img);
    cv::imwrite(BASE_DIR + "cannyImageToUse.jpg", cannyImage);
    cannyImage = cv::imread(BASE_DIR + "cannyImageToUse.jpg");
    // End optimization

    
    findSquaresOfficial(cannyImage, squares);
    //findSquaresCustom(cannyImage, squares);
    drawSquares(image, squares);
    drawCornerPoints(image, squares);
    
    imwrite(BASE_DIR + "ImgWithSquaresDetected.jpg", image);
    return image;
}

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquaresOfficial( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();
    Mat pyr, timg, gray0(image.size(), CV_8U), gray;
    
    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    
    vector<vector<Point> > contours;
    //* delete after test
    imwrite(BASE_DIR + "timg.jpg", timg);
    //*
    
    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 0, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                // tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }
            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            vector<Point> approx;
            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                   fabs(contourArea(Mat(approx))) > MAX_AREA &&
                   isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;
                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( maxCosine < 0.3 )
                        squares.push_back(approx);
                }
            }
        }
    }
    for (int i = 0 ; i < squares.size() ; i++) {
        int tempX, tempY;
        tempX = (squares[i])[0].x;
        tempY = (squares[i])[0].y;
        cv::Rect tempR = cv::Rect(tempX - RADIUS,tempY - RADIUS,RADIUS * 2,RADIUS * 2);
        for (int j = i+1 ; j < squares.size(); j++) {
            if( tempR.contains(squares[j][0]) ){
                squares.erase(squares.begin() + j);
                j--;
            }
        }
    }
    printSquares(squares);
    cout << "squares found: " << squares.size() <<endl;
}

static void findSquaresCustom(const Mat& image, vector<vector<Point> >& squares)
{
    squares.clear();
    // blur will enhance edge detection
    Mat blurred(image);
    medianBlur(image, blurred, 9);
    
    Mat gray0(blurred.size(), CV_8U), gray;
    vector<vector<Point> > contours;
    //* delete after test
    imwrite(BASE_DIR + "gray0.jpg", gray0);
    imwrite(BASE_DIR + "blurred.jpg", blurred);
    //*
    
    // find squares in every color plane of the image
    for (int c = 0; c < 3; c++)
    {
        int ch[] = {c, 0};
        mixChannels(&blurred, 1, &gray0, 1, ch, 1);
        
        // try 2 threshold levels
        const int threshold_level = 2; //
        for (int l = 0; l < threshold_level; l++) //
        {
            // Use Canny instead of zero threshold level!
            // Canny helps to catch squares with gradient shading
            if (l == 0)
            {
                Canny(gray0, gray, 10, 20, 3); //
                
                // Dilate helps to remove potential holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                gray = gray0 >= (l+1) * 255 / threshold_level;
            }
            
            // Find contours and store them in a list
            findContours(gray, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
            
            // Test contours
            vector<Point> approx;
            for (size_t i = 0; i < contours.size(); i++)
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if (approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > MAX_AREA && //how big should be the square
                    isContourConvex(Mat(approx)))
                {
                    double maxCosine = 0;
                    
                    for (int j = 2; j < 5; j++)
                    {
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    
                    if (maxCosine < 0.3)
                        squares.push_back(approx);
                }
            }
        }
    }
    
    // remove edges that reference the same square
    cv::Rect threshold;
    cv::Point pTopLeft, pBottomRight;
    vector<Point>square;
    
    for (int i=0; i<squares.size(); i++) {
        square      = squares[i];
        pTopLeft    = Point(square[0].x - RADIUS, square[0].y + RADIUS);
        pBottomRight= Point(square[0].x + RADIUS, square[0].y - RADIUS);
        threshold   = cv::Rect(pTopLeft, pBottomRight);
        
        for (int j=i+1; j<squares.size(); j++) {
            if(threshold.contains(squares[j][0])){
                for(int k=0; k<square.size(); k++ ){
                    square[k].x = std::min(square[k].x, squares[j][k].x);
                    square[k].y =std::min(square[k].x, squares[j][k].x);
                }
                squares.erase(squares.begin() + j);
                j--;
            }
        }
    }
    printSquares(squares);
    cout << "squares found: " << squares.size() << endl;
}

cv::Mat Squares::cannyDetect(const cv::Mat &image)
{
    static const double threshold1 = 50;
    static const double threshold2 = 200;
    static const int kernelSize = 3;
    static const bool l2Gradient = false;
    cv::Mat result;
    image.copyTo(result);
    cv::Canny(image, result, threshold1, threshold1, kernelSize, l2Gradient);
    return result;
}

// the function draws all the squares in the image
void Squares::drawSquares( Mat& image, const vector<vector<Point> >& squares )
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, Scalar(0,255,0), 3,16);
    }
    //imshow(wndname, image);
}

static void printSquares(const vector<vector<Point>> & squares){
    string str;
    
    for (unsigned i=0; i<squares.size(); i++) {
        vector<Point> tempV = squares[i];
        
        str = "[";
        for (unsigned j=0; j<tempV.size(); j++) {
            str.append("(" + to_string(tempV.at(j).x) + "," + to_string(tempV.at(j).y) + "); ");
        }
        str.append("]\n");
        cout << str;
    }
}

static vector<Point> retrieveBiggestSquare(vector<vector<Point>> & squares){
    
    vector<cv::Point> biggest = squares[0];
    int biggestPosition = 0;
    cv::Rect biggestRect = transformPointsToRect(biggest);
    
    for (int i = 0 ; i < squares.size() -1 ; i++) {
        cv::Rect tempR2 = transformPointsToRect(squares[i+1]);
        if(biggestRect.width <= tempR2.width ){
            
            biggest = squares[i + 1];
            biggestPosition =  i+1;
            biggestRect = tempR2;
        }
    }
    return biggest;
    
}

static cv::Rect transformPointsToRect(vector<cv::Point> points)
{
    int x = points[0].x,y = points[0].y,x2 = points[0].x,y2 = points[0].y;
    for(int i = 0; i < 3; i++){
        if(x > points[i+1].x){
            x = points[i+1].x;
        }
        if(y > points[i+1].y){
            y = points[i+1].y;
        }
        if(x2 < points[i+1].x){
            x2 = points[i+1].x;
        }
        if(y2 < points[i+1].y){
            y2 = points[i+1].y;
        }
        
    }
    return cv::Rect(x, y, x2 - x, y2 -y);
}

// the functions draws all the corners in the image
static void drawCornerPoints(cv::Mat &image,  std::vector<std::vector<cv::Point>> squares){
    cv::Point pt = cv::Point(50, 50);//delete
    cv::circle(image, pt, 3, CV_RGB(36,205,224), 2); //delete
    for (int j=0; j<squares.size(); j++) {
        cv::circle(image, squares[j][0], 3, CV_RGB(255,0,0), 2);
        cv::circle(image, squares[j][1], 3, CV_RGB(0,255,0), 2);
        cv::circle(image, squares[j][2], 3, CV_RGB(0,0,255), 2);
        cv::circle(image, squares[j][3], 3, CV_RGB(255,255,255), 2);
        
        //cout << "pt: " << pt << "\n";
        cout << "TL: " << squares[j][0] << "\n";
        cout << "TR: " << squares[j][1] << "\n";
        cout << "BR: " << squares[j][2] << "\n";
        cout << "BL: " << squares[j][3] << "\n";
        
        /*for (int i=0; i<squares[j].size(); i++) {
         //Image,center,radius,color,
         cv::circle(image, squares[j][i], 3, CV_RGB(255,0,0), 2);
         }*/
    }
    std::cout << "corner points drawn\n";
}

std::vector<cv::Rect> Squares::squaresAsRects(){
    vector<Rect> rects;
    rects.clear();
    
    for (int i=0; i<squares.size(); i++)
        rects.push_back(transformPointsToRect(squares[i]));
    
    return rects;
}

std::vector<std::vector<cv::Point>> Squares::getSquares() {
    return squares;
}



