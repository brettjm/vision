#ifndef TRACKER_H
#define TRACKER_H

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



class color_tracker
{
	public:

		 ros::Publisher chatter_pub;

		 double iLastX; 
		 double iLastY;
		 double iLastTheta;
		 double iLastArea;

		 double smallBlobY;
		 double smallBlobX;
		 double bigBlobY;
		 double bigBlobX;

		 double smallBlobArea;
		 double bigBlobArea;

		 double color_pos_X;
		 double color_pos_Y;
		 double color_pos_theta;

		 int LowH;
		 int HighH;
		 int  LowS;
		 int  HighS;
		 int  LowV;
		 int  HighV;

		 Scalar line_color;
		 string hue;
	
	private:

};


void imageCallback(const sensor_msgs::ImageConstPtr& );
void threshold(Mat , color_tracker& );
void color_pos(Mat , color_tracker& );
void contour_pos(Mat, color_tracker& );
void background_sub(Mat&);

#endif
