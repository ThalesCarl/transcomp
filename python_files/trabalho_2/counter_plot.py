#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu Apr  5 20:05:15 2018

@author: tclavoratti
"""

import numpy as np
import matplotlib.pyplot as plt

counterData = np.loadtxt('../../results/second_task/counter.csv',dtype=int,delimiter=',',skiprows=1)

numberOfNodes = counterData.transpose()[0]
counterEQ = counterData.transpose()[1]
counterLI = counterData.transpose()[2]

plt.plot(numberOfNodes,counterEQ,'ro',label='Resistência equivalente')
plt.plot(numberOfNodes,counterLI,'k+',markersize = 12,label='Interpolação Linear')
plt.xlabel('Número de volumes de controle')
plt.ylabel('Número de iterações')
plt.axis([0,180,0,13])
legend = plt.legend(loc='lower right')
legend.get_frame().set_facecolor('#D3D3D3')
                
plt.savefig('../../results/second_task/counterCurve.png')


