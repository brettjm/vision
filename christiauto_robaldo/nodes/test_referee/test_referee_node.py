#!/usr/bin/env python
import sys

import rospy

from christiauto_robaldo.msg import GameState

import numpy as np

game_state = GameState()

def main():
    rospy.init_node('test_referee', anonymous=False)

    pub_game_state = rospy.Publisher('/game_state', GameState, queue_size=10, latch=True)
    game_state.play = True
    game_state.reset_field = False

    rate = rospy.Rate(100) #100 Hz

    while not rospy.is_shutdown():
        pub_game_state.publish(game_state)

        rate.sleep()

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    main()
