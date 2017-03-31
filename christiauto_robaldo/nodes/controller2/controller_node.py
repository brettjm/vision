#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist, Pose2D
# from std_srvs.srv import Trigger, TriggerResponse

from christiauto_robaldo.msg import PIDInfo, RobotState

import numpy as np

import Controller
import Relationship
import CommandPSOC

# Steps to execute in controller_node:
#  1. Get actual position and desired position
#  2. Run positions through a P controller to get linear velocities
#  3. Run linear velocities through M matrix to get wheel_velocities
#  4. Convert wheel_velocities to rev/sec
#  5. Send wheel_velocities (rev/sec) to PSOC (has a PI controller) to get PWM

_ctrl_period = 1.0/100

_xhat = 0
_yhat = 0
_thetahat = 0

_xhat_ball = 0
_yhat_ball = 0

_ctrl_on = True
_initializing = True

#  1. Get actual position and desired position
def _handle_robot_state(msg):
    global _xhat, _yhat, _thetahat, _initializing, _ctrl_on
    _xhat = msg.x
    _yhat = msg.y
    _thetahat = msg.theta

    #if _initializing:
    #    _initializing = False
    #    _ctrl_on = True

        #x = rospy.get_param('x_init')
        #y = rospy.get_param('y_init')
        #theta = rospy.get_param('theta_init')

        #Controller.set_commanded_position(x, y, theta)# 

def _handle_ball_state(msg):
    global _xhat_ball, _yhat_ball
    _xhat = msg.x
    _yhat = msg.y


def _handle_desired_position(msg):
    global _ctrl_on
    Controller.set_commanded_position(msg.x, msg.y, msg.theta)
    _ctrl_on = True


def main():
    rospy.init_node('controller', anonymous=False)

    # Sub/Pub
    rospy.Subscriber('robot_state', Pose2D, _handle_robot_state)
    rospy.Subscriber('ball_state_yellow', Pose2D, _handle_ball_state)
    #rospy.Subscriber('path_position', Pose2D, _handle_desired_position)
    
    #pub_PIDInfo = rospy.Publisher('pidinfo', PIDInfo, queue_size=10)

    # Get the correct PID stuff
    #gains = rospy.get_param('gains') # returns as a dict
    # {'x': {'P': 0, 'I': 0, 'D': 0}, ... }

    # initialize the controller and PSOC
    #Controller.init(gains)
    CommandPSOC.init()

    rate = rospy.Rate(int(1/_ctrl_period))
    while not rospy.is_shutdown():
        print ("BALL: ", _xhat_ball, _yhat_ball)
        print ("CURRENT ROBOT: ", _xhat, _yhat, _thetahat)
        print ("-------------------")
        '''
        global _ctrl_on

        if _ctrl_on:
			#  2. Run positions through a P controller to get linear velocities
            (vx, vy, w) = Controller.update(_ctrl_period, _xhat, _yhat, _thetahat)

			#  3. Run linear velocities through M matrix to get wheel_velocities
			(v1, v2, v3) = Relationship.world_to_wheels(vx, vy, w)

			#  4. Convert wheel_velocities to rev/sec
			(rps1, rps2, rps3) = Relationship.v_to_rps(v1, v2, v3)			

			#  5. Send wheel_velocities (rev/sec) to PSOC (has a PI controller) to get PWM
			CommandPSOC.setWheelVelocities(rps1, rps2, rps3)

            # Publish PID Info
            msg = PIDInfo()
            msg.error.x = Controller.PID_x.error_d1
            msg.error.y = Controller.PID_y.error_d1
            msg.error.theta = Controller.PID_theta.error_d1
            set_point = Controller.get_commanded_position()
            msg.desired.x = set_point[0]
            msg.desired.y = set_point[1]
            msg.desired.theta = set_point[2]
            msg.actual.x = _xhat
            msg.actual.y = _yhat
            msg.actual.theta = _thetahat
            pub_PIDInfo.publish(msg)
        '''

        rate.sleep()

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    main()
