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

Mat imgOriginal;//create matrix called imgOriginal

int smallestX = 91; // 0
int largestX = 730; // 864
int smallestY = 18; // 0
int largestY = 438; // 480

int oldSmallestX;
int oldLargestX;
int oldSmallestY;
int oldLargestY;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try {
    imgOriginal = cv_bridge::toCvShare(msg, "bgr8")->image;
    if (smallestX >= largestX) {
      smallestX = oldSmallestX;
      largestX = oldLargestX;
      setTrackbarPos("SmallestX", "Control", smallestX);
      setTrackbarPos("LargestX", "Control", largestX);
    }
    else {
      oldSmallestX = smallestX;
      oldLargestX = largestX;
    }
    if (smallestY >= largestY) {
      smallestY = oldSmallestY;
      largestY = oldLargestY;
      setTrackbarPos("SmallestY", "Control", smallestY);
      setTrackbarPos("LargestY", "Control", largestY);
    }
    else {
      oldSmallestY = smallestY;
      oldLargestY = largestY;
    }
    imgOriginal = Mat(imgOriginal, Rect(smallestX, smallestY, largestX - smallestX, largestY - smallestY));
    imshow( "CroppedImage", imgOriginal ); // Show in a window
    cv::waitKey(30); // Allow OpenCV to draw image in window
  }
  catch (cv_bridge::Exception& e) {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main( int argc, char** argv ) {
  ros::init(argc, argv, "contours");
  ros::NodeHandle n;

  namedWindow("Control", CV_WINDOW_AUTOSIZE); // Create a window called "Control"
  createTrackbar("SmallestX", "Control", &smallestX, 864); // Create trackbars in "Control" window
  createTrackbar("LargestX", "Control", &largestX, 864);
  createTrackbar("SmallestY", "Control", &smallestY, 480);
  createTrackbar("LargestY", "Control", &largestY, 480);

  namedWindow( "CroppedImage", CV_WINDOW_AUTOSIZE ); // Create a window called "CroppedImage"

  image_transport::ImageTransport it(n);
  image_transport::Subscriber sub = it.subscribe("/usb_cam_away/image_raw", 1, imageCallback);
  
  ros::spin();
  cv::destroyWindow("CroppedImage");
  cv::destroyWindow("Control");

  return 0;
}