#!/usr/bin/env python
# -*- coding: utf-8 -*-

from mpl_toolkits import mplot3d
import sys
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
from pprint import pprint


x_max = 1
y_max = np.pi / 2

u = []
analytic_u = []

with open(sys.argv[1], "r") as f, \
     open(sys.argv[2], "r") as f1:
    h_x, h_y = map(float, f.readline().split())
    Nx, Ny = map(int, f.readline().split())
    for i in range(Ny):
        cur = [float(x) for x in f.readline().split()]
        analytic_u.append(cur)

        cur = [float(x) for x in f1.readline().split()]
        u.append(cur)

times = [0, 0.5, 1]
colors = ['r', 'g', 'b']
#patches = []

#for i in range(len(times)):
#    patches.append(mpatches.Patch(color=colors[i], \
#                   label="x = " + "{:.2}".format((times[i] * Nx) * h_x)))

#plt.legend(handles=patches)

X = np.linspace(0, x_max, Nx)
Y = np.linspace(0, y_max, Ny)

Y, X = np.meshgrid(Y, X)
Z = np.array(u)

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.contour3D(X, Y, Z, 50)
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_zlabel('z');

#plt.subplot(2, 1, 1)
#plt.xlabel("y")
#plt.ylabel("u(x, y)")
#for i in range(len(times)):
#    arg = int(times[i] * Nx)
#    if arg >= len(u):
#        arg = len(u) - 1
#    Y = u[arg]
#    plt.plot(X, Y, color=colors[i])

#    Y = analytic_u[arg]
#    plt.plot(X, Y, linestyle=':', color='k')

#plt.subplot(2, 1, 2)
#plt.xlabel("t")
#plt.ylabel("max error")
#error = []
#for i in range(len(X)):
#    m = 0
#    for j in range(len(u[i])):
#        m = max(abs(u[i][j] - analytic_u[i][j]), m)
#    error.append(m)
#plt.plot(X, error, color='red')


plt.show()
