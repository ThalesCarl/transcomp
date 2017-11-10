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

plt.plot(x,y,'bo',markersize=8)
y = analyticalSolutionDataCenter.transpose()[1]
plt.plot(x,y,'r+')
plt.grid(b=1, axis='both')
plt.xlabel('x',fontsize='large')
plt.ylabel('T(x)')
plt.savefig('../results/first_exercise_plot_analit_and_finite_volume_4_nodes_central_mesh.png')
"""
plt.clf();

finiteVolumeDataBoth = np.loadtxt('../results/first_exercise/bothMesh4NodesVC.csv',dtype=float,delimiter=',',skiprows=1)
analyticalSolutionDataBoth = np.loadtxt('../results/first_exercise/bothMesh4NodesAN.csv',dtype=float,delimiter=',',skiprows=1)

x  = finiteVolumeDataBoth.transpose()[0]
y = finiteVolumeDataBoth.transpose()[1]

plt.plot(x,y,'bo',markersize=8)
y = analyticalSolutionDataBoth.transpose()[1]
plt.plot(x,y,'r+')
plt.grid(b=1, axis='both')
plt.xlabel('x',fontsize='large')
plt.ylabel('T(x)')
plt.savefig('../results/first_exercise/plot_analit_and_finite_volume_4_nodes_both_mesh.png')

"""