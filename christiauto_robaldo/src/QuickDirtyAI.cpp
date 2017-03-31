#include <iostream>
#include <cmath>
#include "ros/ros.h"
#include "geometry_msgs/Pose2D.h"

double pi = 3.14159265358979;
bool attack = false;

int robot_x;
int robot_y;
int robot_theta;

int ball_x;
int ball_y;
int ball_theta;

ros::Publisher pub;

void robot_state_callback(const geometry_msgs::Pose2D& msg) {
	robot_x = msg.x;
	robot_y = msg.y;
	robot_theta = msg.theta;
}

void ball_state_callback(const geometry_msgs::Pose2D& msg) {
	ball_x = msg.x;
	ball_y = msg.y;
	ball_theta = msg.theta;

	geometry_msgs::Pose2D pub_msg;

    	if (attack) {
    		pub_msg.x = ball_x + 10;
    		pub_msg.y = ball_y;
    		pub_msg.theta = 180;
    	}
    	else {
    		pub_msg.x = 70;
    		if (ball_y >= 170 && ball_y <= 260) {
    			pub_msg.y = ball_y;
	    	}
	    	else if (ball_y < 170) {
	    		pub_msg.y = 170;
	    	}
	    	else {
	    		pub_msg.y = 260;
	    	}
    		pub_msg.theta = 0;
    	}

    	pub.publish(pub_msg);
}

int main( int argc, char** argv ) {
    ros::init(argc, argv, "QuickDirtyAI");
    ros::NodeHandle n;

    pub = n.advertise<geometry_msgs::Pose2D>("path_planner",100);

    ros::Subscriber robotStateSub = n.subscribe("purple_chatter", 1, robot_state_callback);
    ros::Subscriber ballStateSub = n.subscribe("red_chatter", 1, ball_state_callback);

    ros::spin();
}
