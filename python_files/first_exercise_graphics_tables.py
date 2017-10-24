#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Oct 23 14:45:24 2017

@author: tclavoratti
"""

import numpy as np
import matplotlib.pyplot as plt


finiteVolumeData = np.loadtxt('../results/firstExerciseCenterMesh4NodesVC.csv',dtype=float,delimiter=',',skiprows=1)
analyticalSolutionData = np.loadtxt('../results/firstExerciseCenterMesh4NodesAN.csv',dtype=float,delimiter=',',skiprows=1)

x  = finiteVolumeData.transpose()[0]
y = finiteVolumeData.transpose()[1]

plt.plot(x,y,'bo',markersize=8)
y = analyticalSolutionData.transpose()[1]
plt.plot(x,y,'r+')
plt.grid(b=1, axis='both')
plt.xlabel('x',fontsize='large')
plt.ylabel('T(x)')
plt.savefig('../results/plot_analit_and_finite_volume_4_nodes_central_mesh.png')


