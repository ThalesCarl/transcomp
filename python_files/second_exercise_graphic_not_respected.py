#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Nov 24 08:26:53 2017

@author: tclavoratti
"""


import numpy as np
import matplotlib.pyplot as plt


finiteVolumeDataEQ = np.loadtxt('../results/.csv',dtype=float,delimiter=',',skiprows=1)
finiteVolumeDataLI = np.loadtxt('../results/secondExerciseDivisionNotRespectedLinearInterpolationVC.csv',dtype=float,delimiter=',',skiprows=1)
analyticalSolutionData = np.loadtxt('../results/secondExerciseDivisionNotRespectedAN.csv',dtype=float,delimiter=',',skiprows=1)

x = analyticalSolutionData.transpose()[0]
y = analyticalSolutionData.transpose()[1]

plt.plot(x,y,label = "Solução analítica")
x = finiteVolumeDataEQ.transpose()[0]
y = finiteVolumeDataEQ.transpose()[1]
plt.plot(x,y,'r+', label = "Volumes finitos com resistência equivalente")
y=finiteVolumeDataLI.transpose()[1]
plt.plot(x,y,'k+',label = "Volumes finitos com interpolação linear")
plt.xlabel('x')
plt.ylabel('T(x)')
plt.grid(b=1)
#plt.ylim([300,370])
legend = plt.legend(loc='botton left')
legend.get_frame().set_facecolor('#D3D3D3')

plt.savefig('../results/plot_not_respected.png')