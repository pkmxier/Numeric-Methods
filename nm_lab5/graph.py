#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
import numpy as np
from pprint import pprint


x_max = np.pi
t_max = 1

u = []
analytic_u = []

with open(sys.argv[1], "r") as f, \
     open(sys.argv[2], "r") as f1:
    tao, h = map(float, f.readline().split())
    Nk, Nx = map(int, f.readline().split())
    for i in range(Nk):
        cur = [float(x) for x in f.readline().split()]
        u.append(cur)

        cur = [float(x) for x in f1.readline().split()]
        analytic_u.append(cur)

times = [0, 0.3, 0.7]
colors = ['r', 'g', 'b']
patches = []

for i in range(len(times)):
    patches.append(mpatches.Patch(color=colors[i], \
                   label="t = " + "{:.2}".format((times[i] * Nk) * tao)))

plt.legend(handles=patches)

X = [x for x in np.linspace(0, np.pi, Nx)]
T = [t for t in np.linspace(0, 1, Nk)]

for i in range(len(times)):
    Y = [y for y in analytic_u[int(times[i] * Nk)]]
    plt.plot(X, Y, color=colors[i])

    Y = [y for y in u[int(times[i] * Nk)]]
    plt.plot(X, Y, color=colors[i], linestyle=':')

plt.show()
