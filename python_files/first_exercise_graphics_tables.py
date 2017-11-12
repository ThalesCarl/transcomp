#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 23 14:45:24 2017

@author: tclavoratti
"""

import numpy as np
import matplotlib.pyplot as plt


finiteVolumeDataCenter = np.loadtxt('../results/first_exercise/centerMesh4NodesVC.csv',dtype=float,delimiter=',',skiprows=1)
analyticalSolutionDataCenter = np.loadtxt('../results/first_exercise/centerMesh4NodesAN.csv',dtype=float,delimiter=',',skiprows=1)

x  = finiteVolumeDataCenter.transpose()[0]
y = finiteVolumeDataCenter.transpose()[1]

plt.plot(x,y,'bo',markersize=8,label="Solução usando volumes finitos e malha central")
y = analyticalSolutionDataCenter.transpose()[1]
plt.plot(x,y,'r+',label = "Solução analítica para malha central")
plt.grid(b=1, axis='both')
plt.xlabel('x',fontsize='large')
plt.ylabel('T(x)')




finiteVolumeDataBoth = np.loadtxt('../results/first_exercise/bothMesh4NodesVC.csv',dtype=float,delimiter=',',skiprows=1)
analyticalSolutionDataBoth = np.loadtxt('../results/first_exercise/bothMesh4NodesAN.csv',dtype=float,delimiter=',',skiprows=1)

x  = finiteVolumeDataBoth.transpose()[0]
y = finiteVolumeDataBoth.transpose()[1]

plt.plot(x,y,'go',markersize=8,label = "Solução usando volumes finitos e malha both")
y = analyticalSolutionDataBoth.transpose()[1]
plt.plot(x,y,'y+',label = "Solução analítica para malha both")
plt.grid(b=1, axis='both')
plt.xlabel('x',fontsize='large')
plt.ylabel('T(x)')
plt.ylim([0,170])
plt.xlim([-0.005,0.11])
legend = plt.legend(loc='upper right')
legend.get_frame().set_facecolor('#D3D3D3')
plt.savefig('../results/first_exercise/plot_4nodes_dualGridType.png')
