#imports
from unittest import TextTestResult
import numpy as np
import matplotlib.pyplot as plt

# plot ttest
ttest = np.loadtxt('ttest.txt')
x = np.arange(55500, dtype=float)

plt.plot(x, ttest)
plt.show()

#Plot ttest, mean and standard deviation
"""
##### Plot t-test result 
ttest = np.loadtxt('ttest.txt')
x = np.arange(55500, dtype=float)

plt.subplot(3,1,1)
plt.plot(x, ttest)


##### Plot mean result 
fixed_mean = np.loadtxt('fixed_mean.txt')
random_mean = np.loadtxt('random_mean.txt')
x = np.arange(55500, dtype=float)

plt.subplot(3,1,2)
plt.plot(x, fixed_mean)
plt.plot(x, random_mean)
plt.legend(['fixed_mean', 'random_mean'])



##### Plot standard deviation result 
fixed_sd = np.loadtxt('fixed_standard_deviation.txt')
random_sd = np.loadtxt('random_standard_deviation.txt')
x = np.arange(55500, dtype=float)

plt.subplot(3,1,3)
plt.plot(x, fixed_sd)
plt.plot(x, random_sd)
plt.legend(['fixed_standard_deviation', 'random_standard_deviation'])


plt.show()
"""
