import numpy as np

def world_to_wheels(vx, vy, w):
	v1 = 0 	# Wheel 1 velocity
	v2 = 0  # Wheel 2 velocity
	v3 = 0 	# Wheel 3 velocity

	# TODO: Figure out the M matrix

	wheel_velocities = (v1, v2, v3)
	return wheel_velocities


def v_to_rps(v1, v2, v3):
	rps1 = 0 # Wheel 1 rev/sec
	rps2 = 0 # Wheel 2 rev/sec
	rps3 = 0 # Wheel 3 rev/sec

	twopi = np.pi*2
	rps1 = v1/(twopi)
	rps2 = v2/(twopi)
	rps3 = v3/(twopi)

	wheel_velocities_rps = (rps1, rps2, rps3)
	return wheel_velocities_rps




