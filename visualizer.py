#!/usr/bin/env python3

import matplotlib
matplotlib.use('Agg') # Set non GUI based backend
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# Read particles from input file
particles = {
        0: [(1,1), (2,2), (3,3)],
        1: [(2,2), (3,3), (4,4)],
        2: [(3,3), (4,4), (5,5)],
        3: [(4,4), (5,5), (6,6)],
        4: [(5,5), (6,6), (7,7)]
}

fig, ax = plt.subplots()
ax.set_xlim(0, 10)
ax.set_ylim(0, 10)
ln, = ax.plot([], [], 'ro', lw=2)

def init():
    ln.set_data([], [])
    return ln,

def update(frame):
    xdata = [ x for x, _ in particles[frame] ]
    ydata = [ y for _, y in particles[frame] ]
    ln.set_data(xdata, ydata)
    return ln,

ani = FuncAnimation(fig, update, frames=5, interval=2000, init_func=init, blit=True, repeat=False)
ani.save("example.gif", writer="imagemagick", fps=1)

#plt.show() # plot when GUI based backend is used
