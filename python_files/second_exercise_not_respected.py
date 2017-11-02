#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Nov  2 14:13:20 2017

@author: tclavoratti
"""

import numpy as np
import matplotlib.pyplot as plt


finiteVolumeDataEQ = np.loadtxt('../results/secondExerciseDivisionNotRespectedEquivalentResistanceVC.csv',dtype=float,delimiter=',',skiprows=1)
finiteVolumeDataLI = np.loadtxt('../results/secondExerciseDivisionNotRespectedLinearInterpolationVC.csv',dtype=float,delimiter=',',skiprows=1)
analyticalSolutionData = np.loadtxt('../results/secondExerciseDivisionNotRespectedAN.csv',dtype=float,delimiter=',',skiprows=1)

x = analyticalSolutionData.transpose()[0]
y = analyticalSolutionData.transpose()[1]

plt.plot(x,y)
x = finiteVolumeDataEQ.transpose()[0]
y = finiteVolumeDataEQ.transpose()[1]
plt.plot(x,y,'r+')
y=finiteVolumeDataLI.transpose()[1]
plt.plot(x,y,'k+')
plt.xlabel('x')
plt.ylabel('T(x)')
plt.grid(b=1)

plt.savefig('../results/plot_not_respected.png')
