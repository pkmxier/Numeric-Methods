#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
from pprint import pprint


x_max = np.pi

u = []
analytic_u = []

with open(sys.argv[1], "r") as f, \
     open(sys.argv[2], "r") as f1:
    t_max = float(f.readline())
    tao, h = map(float, f.readline().split())
    Nk, Nx = map(int, f.readline().split())
    for i in range(Nk):
        cur = [float(x) for x in f.readline().split()]
        analytic_u.append(cur)

        cur = [float(x) for x in f1.readline().split()]
        u.append(cur)

times = [0, 0.5, 1]
colors = ['r', 'g', 'b']
patches = []

for i in range(len(times)):
    patches.append(mpatches.Patch(color=colors[i], \
                   label="t = " + "{:.2}".format((times[i] * Nk) * tao)))

plt.legend(handles=patches)

X = np.linspace(0, x_max, Nx)
T = np.linspace(0, t_max, Nk)

plt.subplot(2, 1, 1)
plt.xlabel("x")
plt.ylabel("u(x, t)")
for i in range(len(times)):
    arg = int(times[i] * Nk)
    if arg >= len(u):
        arg = len(u) - 1
    Y = u[arg]
    plt.plot(X, Y, color=colors[i])

    Y = analytic_u[arg]
    plt.plot(X, Y, linestyle=':', color='k')

plt.subplot(2, 1, 2)
plt.xlabel("t")
plt.ylabel("max error")
error = []
for i in range(len(T)):
    m = 0
    for j in range(len(u[i])):
        m = max(abs(u[i][j] - analytic_u[i][j]), m)
    error.append(m)
plt.plot(T, error, color='red')


plt.show()
