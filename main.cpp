#include <iostream>
#include <opencv2/opencv.hpp>
#include <Driver/RMVideoCapture.hpp>
#include <zbar.h>
using namespace cv;
using namespace std;
using namespace zbar;

typedef struct
{
  string type;
  string data;
  vector <Point> location;
} decodedObject;

// Find and decode barcodes and QR codes
void decode(Mat &im, vector<decodedObject>&decodedObjects)
{

  // Create zbar scanner
  ImageScanner scanner;

  // Configure scanner
  scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

  // Convert image to grayscale
  Mat imGray;
  cvtColor(im, imGray,COLOR_BGR2GRAY);

  // Wrap image data in a zbar image
  Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);

  // Print results
  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    decodedObject obj;

    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();

    // Print type and data
//    cout << "Type : " << obj.type << endl;
//    cout << "Data : " << obj.data << endl << endl;

    // Obtain location
    for(int i = 0; i< symbol->get_location_size(); i++)
    {
      obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
}

// Display barcode and QR code location
void display(Mat &im, vector<decodedObject>&decodedObjects,Mat cam,Mat dis)
{
  // Loop over all decoded objects
  for(int i = 0; i < decodedObjects.size(); i++)
  {
    vector<Point> points = decodedObjects[i].location;
    vector<Point> hull;

    // If the points do not form a quad, find convex hull
    if(points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;
    vector<Point2f> pnts;
    // Number of points in the convex hull
    int n = hull.size();
    for(int j = 0; j < n; j++)
    {
      line(im, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
        pnts.push_back(Point2f(hull[j].x,hull[j].y));
    }
    #define HALF_LENGTH 29.5
    vector<Point3f> obj=vector<Point3f>{
            cv::Point3f(-HALF_LENGTH, -HALF_LENGTH, 0),	//tl
            cv::Point3f(HALF_LENGTH, -HALF_LENGTH, 0),	//tr
            cv::Point3f(HALF_LENGTH, HALF_LENGTH, 0),	//br
            cv::Point3f(-HALF_LENGTH, HALF_LENGTH, 0)	//bl
    };
    cv::Mat rVec = cv::Mat::zeros(3, 1, CV_64FC1);//init rvec
    cv::Mat tVec = cv::Mat::zeros(3, 1, CV_64FC1);//init tvec
    solvePnP(obj,pnts,cam,dis,rVec,tVec,false,SOLVEPNP_ITERATIVE);
    cout <<"tvec: "<<tVec<<endl;
  }

  // Display results
//  imshow("Results", im);
//  waitKey(0);

}

int main()
{
    RMVideoCapture cap;
    cap.open(1);
    cap.setVideoFormat(1920,1080);

    Mat cameraMatrix;
    Mat distCoeffs;

    FileStorage fs("./out_camera_data.xml", FileStorage::READ);
    fs["camera_matrix"] >> cameraMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    cap.startStream();
    Mat img;
    vector<vector<Point> > squares;
    while (waitKey(1) != 'q') {
        if(!cap.grab())continue;
        cap.retrieve(img);
        Mat demo;
        Mat gray;
        Mat edge;
        // Variable for decoded objects
        vector<decodedObject> decodedObjects;

        // Find and decode barcodes and QR codes
        decode(img, decodedObjects);

        // Display location
        display(img, decodedObjects,cameraMatrix,distCoeffs);

        resize(img,demo,Size(960,540));
        imshow("demo",demo);
    }
    return 0;
}
