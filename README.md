# pnpTest
使用solvePnP来测量二维码相对于摄像头的位置。二维码扫码代码参考：https://github.com/spmallick/learnopencv/tree/master/barcode-QRcodeScanner

解算位置的代码为以下几行：
```c
#define HALF_LENGTH 29.5
//自定义的物体世界坐标，单位为mm
vector<Point3f> obj=vector<Point3f>{
    cv::Point3f(-HALF_LENGTH, -HALF_LENGTH, 0),	//tl
    cv::Point3f(HALF_LENGTH, -HALF_LENGTH, 0),	//tr
    cv::Point3f(HALF_LENGTH, HALF_LENGTH, 0),	//br
    cv::Point3f(-HALF_LENGTH, HALF_LENGTH, 0)	//bl
};
cv::Mat rVec = cv::Mat::zeros(3, 1, CV_64FC1);//init rvec
cv::Mat tVec = cv::Mat::zeros(3, 1, CV_64FC1);//init tvec
//进行位置解算
solvePnP(obj,pnts,cam,dis,rVec,tVec,false,SOLVEPNP_ITERATIVE);
//输出平移向量
cout <<"tvec: "<<tVec<<endl;
```
