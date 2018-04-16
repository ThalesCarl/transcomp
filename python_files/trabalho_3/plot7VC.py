#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  5 20:05:15 2018

@author: tclavoratti
"""

import numpy as np
import matplotlib.pyplot as plt

dataCV = np.loadtxt('../../results/third_task/controlVolume7VC.csv',dtype=float,delimiter=',',skiprows=0)
dataAN = np.loadtxt('../../results/third_task/analytical_solution7.csv',dtype=float,delimiter=',',skiprows=0)

x = dataCV[0][1:8];


for i in range(len(dataCV)):
    if(i==0):
        continue
    yCV = dataCV[i][1:8];       
    plt.plot(x,yCV,'-',label = str(dataCV[i][0]));


plt.xlabel('x[m]')
plt.ylabel('T(x)[ºC]')
plt.grid(b=1)
plt.axis([0,0.65,-1,100])
legend = plt.legend(loc='center right')
legend.get_frame().set_facecolor('#D3D3D3')

plt.savefig('../../results/third_task/temperature_field_7VC.png')


