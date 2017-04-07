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

void imageCallback(const sensor_msgs::ImageConstPtr& msg);
void redChatterCallback(const geometry_msgs::Pose2D& msg);
void purpleChatterCallback(const geometry_msgs::Pose2D& msg);
void yellowChatterCallback(const geometry_msgs::Pose2D& msg);
void blueChatterCallback(const geometry_msgs::Pose2D& msg);
void pinkChatterCallback(const geometry_msgs::Pose2D& msg);

int main(int argc, char** argv) {
	ros::init(argc, argv, "visionTester");
	ros::NodeHandle n;

	image_transport::ImageTransport it(n);
	image_transport::Subscriber sub =
		it.subscribe("/usb_cam_away/image_raw", 1, imageCallback);

	ros::Subscriber redChatterSub = n.subscribe("red_chatter", 1, redChatterCallback);
	ros::Subscriber purpleChatterSub = n.subscribe("purple_chatter", 1, purpleChatterCallback);
	ros::Subscriber yellowChatterSub = n.subscribe("yellow_chatter", 1, yellowChatterCallback);
	ros::Subscriber blueChatterSub = n.subscribe("blue_chatter", 1, blueChatterCallback);
	ros::Subscriber pinkChatterSub = n.subscribe("pink_chatter", 1, pinkChatterCallback);
	ros::spin();
	return 0;
}

const int smallestX = 91;
const int largestX = 730;
const int smallestY = 18;
const int largestY = 438;

int redChatterX = -1;
int redChatterY = -1;
int purpleChatterX = -1;
int purpleChatterY = -1;
int yellowChatterX = -1;
int yellowChatterY = -1;
int blueChatterX = -1;
int blueChatterY = -1;
int pinkChatterX = -1;
int pinkChatterY = -1;

void imageCallback(const sensor_msgs::ImageConstPtr& msg) {
	Mat imgOriginal;
	imgOriginal = cv_bridge::toCvShare(msg, "bgr8")->image;
	imgOriginal = Mat(imgOriginal, Rect(smallestX, smallestY, largestX - smallestX, largestY - smallestY));
	if (redChatterX >= 0 && redChatterY >= 0) {
		circle( imgOriginal, // RED
    	     Point(redChatterX, (largestY - smallestY) - redChatterY),
    	     10.0,
    	     Scalar( 0, 0, 255 ),
    	     -1,
    	     8 );
	}
	if (purpleChatterX >= 0 && purpleChatterY >= 0) {
		circle( imgOriginal, // PURPLE
    	     Point(purpleChatterX, (largestY - smallestY) - purpleChatterY),
    	     10.0,
    	     Scalar( 255, 0, 255 ),
    	     -1,
    	     8 );
	}
	if (yellowChatterX >= 0 && yellowChatterY >= 0) {
		circle( imgOriginal, // YELLOW
	         Point(yellowChatterX, (largestY - smallestY) - yellowChatterY),
	         10.0,
	         Scalar( 0, 120, 120 ),
	         -1,
	         8 );
	}
	if (blueChatterX >= 0 && blueChatterY >= 0) {
		circle( imgOriginal, // BLUE
	         Point(blueChatterX, (largestY - smallestY) - blueChatterY),
	         10.0,
	         Scalar( 255, 0, 0 ),
	         -1,
	         8 );
	}
	if (pinkChatterX >= 0 && pinkChatterY >= 0) {
		circle( imgOriginal, // PINK
 	        Point(pinkChatterX, (largestY - smallestY) - pinkChatterY),
 	        5.0,
 	        Scalar( 147, 20, 255 ),
 	        -1,
 	        8 );
	}
	imshow("original", imgOriginal);
	cv::waitKey(15);
}

void redChatterCallback(const geometry_msgs::Pose2D& msg) {
	redChatterX = msg.x;
	redChatterY = msg.y;
}

void purpleChatterCallback(const geometry_msgs::Pose2D& msg) {
	purpleChatterX = msg.x;
	purpleChatterY = msg.y;
}

void yellowChatterCallback(const geometry_msgs::Pose2D& msg) {
	yellowChatterX = msg.x;
	yellowChatterY = msg.y;
}

void blueChatterCallback(const geometry_msgs::Pose2D& msg) {
	blueChatterX = msg.x;
	blueChatterY = msg.y;
}

void pinkChatterCallback(const geometry_msgs::Pose2D& msg) {
	pinkChatterX = msg.x;
	pinkChatterY = msg.y;
}
