import numpy as np

# This controller translates position into velocity
#  using a proportional controller
class P(object):

    def __init__(self, kp, limit, tau):
        super(P, self).__init__()
        self.kp = kp
        self.limit = limit
        self.tau = tau

        self.xdot = 0
        self.error_d1 = 0
        self.x_d1 = 0

    def update(self, x_c, x, Tx, max_error_window=0.425):
        # compute the error
        use_window_error = (max_error_window is not 0)
        if use_window_error and abs(x_c - x) > max_error_window:
            error = max_error_window*np.sign(x_c - x)
        else:
            error = x_c - x

        # update delayed variables for next time
        self.error_d1 = error
        self.x_d1 = x		

        # compute the P control signal
        u_unsat = self.kp*error
        u = self._sat(u_unsat)		

        return u


    def _sat(self, val):
        out = 0

        if val > self.limit:
            out = self.limit
        elif val < -self.limit:
            out = -self.limit
        else:
            out = val

        return out
