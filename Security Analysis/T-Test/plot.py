#imports
from time import time
from unittest import TextTestResult
import numpy as np
import matplotlib.pyplot as plt

samplepoints = 103000

# plot ttest
ttest = np.loadtxt('ttest.txt')

### Output values, which are above 4.5 or below -4.5
counter = 0
for element in range(ttest.size):
    if ((ttest[element] > 4.5) or (ttest[element] < -4.5)):
        micro_time = ((element - 1000) / 500)
        print('Sample ' + str(element) + ' has the value ' + str(ttest[element]) + ' at time ' + str(micro_time))
        counter += 1
print('NUmber of "wrong" elements: '+ str(counter))

x = np.arange(samplepoints, dtype=float)
plt.axhline(y = 4.5, color = 'r', linestyle = '-')
plt.axhline(y = -4.5, color = 'r', linestyle = '-')
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
