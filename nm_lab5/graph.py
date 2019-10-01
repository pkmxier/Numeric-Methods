#!/usr/bin/env python
# -*- coding: utf-8 -*-

import matplotlib.pyplot as plt
import numpy as np

def convert(pred):
    res = 0
    for x in pred:
        res = res * 2 + x
    return res

train_data = np.array([[-2.8, 1.4], [-0.2, -3.5], [2.8, -4],
                       [-2.1, -2.7], [0.3, -4.1], [-1, -4]])
train_labels = np.array([[0], [1], [1], [0], [1], [0]])

plt.scatter([x[0] for x in train_data], [x[1] for x in train_data],
            c=[convert(x) for x in train_labels])
plt.show()
