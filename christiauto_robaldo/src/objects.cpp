#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose2D.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
using namespace std;

//Mat imgOriginal;//create matrix called imgOriginal
//int thresh = 100;
//int max_thresh = 255;
//RNG rng(12345);




//added callback function from vtest
//
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
      Mat imgOriginal;

  
      imgOriginal = cv_bridge::toCvShare(msg, "bgr8")->image;
      imgOriginal = Mat(imgOriginal, Rect(120, 100, 250, 300));
       

      Mat imgGRAY;
      Mat imgthresh;
      Mat imgmean;
      Mat imggaus;
      /// Convert image to gray and blur it
      cvtColor( imgOriginal, imgGRAY, COLOR_BGR2GRAY );
      imshow("gray", imgGRAY); //show the original image
                                      //threshold(imgGRAY, imgthresh, 185, 255, CV_THRESH_BINARY);  
                                      //adaptiveThreshold(imgGRAY, imgmean, 185, ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 3, 2);
                                      //adaptiveThreshold(imgGRAY, imggaus, 185, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY, 3, 2); 
                                      // imshow("imgthresh", imgthresh); //show the original image
                                      // imshow("imgmean", imgmean); //show the original image
                                      // imshow("imggaus", imggaus); //show the original image

      Mat canny_output;
      vector<vector<Point> > contours;
      vector<Vec4i> hierarchy;


      /// Detect edges using canny
      Canny( imgGRAY, canny_output, 75, 250, 3 );
      /// Find contours
      findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


      // Draw contours
      Mat drawing = Mat::zeros( imgOriginal.size(), CV_8UC3 );
      for( int i = 0; i< contours.size(); i++ )
         {
              drawContours( drawing, contours, i, 255, 1, 8, hierarchy, 0, Point() );

            

            double peri = arcLength(contours.at(i), 1);
            vector<Point> approx;
            approxPolyDP(contours.at(i), approx, .02*peri, 1);
            //ROS_INFO("%d, %lu", i, approx.size());


            Moments oMoments = moments(contours.at(i));
            double dM01 = oMoments.m01;//add up all pixel Y? positions
            double dM10 = oMoments.m10;//add up all pixel X? positions
            double dArea = contourArea(approx, 0);
                        //double dArea = oMoments.m00;//calculate area of color
                        //double dArea = contourArea(contours.at(i), 0);
            int posX = dM10 / dArea;
            int posY = dM01 / dArea; 
            ROS_INFO("area: %lf, x: %d, y: %d", dArea, posX, posY);
            



            i++;//THIS IS A HACK BECAUSE findContours() FINDS DUPLICATES OF EVERY CONTOUR. SO SKIP THE DUPLICATES!!!!!!!!!!
          
         }

           
      imshow( "Contours", drawing );

      cv::waitKey(30);
  }

  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }

}



 int main( int argc, char** argv )
 {

    ros::init(argc, argv, "objects");
    ros::NodeHandle n;
    ros::Publisher chatter_pub = n.advertise<geometry_msgs::Pose2D>("chatter",1000);



            /// Create Window
             //char* source_window = "Source";
             //namedWindow( source_window, CV_WINDOW_AUTOSIZE );
             //imshow( source_window, imgOriginal );

             //createTrackbar( " Canny thresh:", "Source", &thresh, max_thresh );

  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub = it.subscribe("/usb_cam_away/image_raw", 1, imageCallback);
  

  ros::spin();
  cv::destroyWindow("view");



   return 0;
}



