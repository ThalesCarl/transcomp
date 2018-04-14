#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  5 20:05:15 2018

@author: tclavoratti
"""

import numpy as np
import matplotlib.pyplot as plt

dataCV = np.loadtxt('../../results/third_task/controlVolume.csv',dtype=float,delimiter=',',skiprows=0)
dataAN = np.loadtxt('../../results/third_task/analytical_solution.csv',dtype=float,delimiter=',',skiprows=0)

x = dataCV[0];
for i in range(len(dataAN)):
    if i==0:
        continue;
    yAN = dataAN[i];
    if i==2:
         plt.plot(x,yAN,'k-',label ='Solução analítica');
    plt.plot(x,yAN,'k-');
    
for i in range(len(dataCV)):
    if i==0:
        continue;
    yCV = dataCV[i];
    if i==1:
         plt.plot(x,yCV,'k+',label = 'Solução numérica');
    
    plt.plot(x,yCV,'k+');

#
#plt.plot(numberOfNodes,counterEQ,'ro',label='Resistência equivalente')
#plt.plot(numberOfNodes,counterLI,'k+',markersize = 12,label='Interpolação Linear')
plt.xlabel('x[m]')
plt.ylabel('T(x)[ºC]')
plt.grid(b=1)
plt.axis([-0.01,0.41,-1,100])
legend = plt.legend(loc='lower right')
legend.get_frame().set_facecolor('#D3D3D3')

plt.savefig('../../results/third_task/temperature_field_5VC.png')
#
#
