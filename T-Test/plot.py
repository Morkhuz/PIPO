#imports
from unittest import TextTestResult
import numpy as np
import matplotlib.pyplot as plt

# Choose what to plot: ttest, ttest_post, mean, sd
choice = 'ttest_post'


##### Plot T-test result with pre- post trigger samples
if choice == 'ttest':
    ttest = np.loadtxt('ttest.txt')
    x = np.arange(55500, dtype=float)

    plt.plot(x, ttest)
    plt.show()
elif choice == 'ttest_post':
    ttest = np.loadtxt('ttest.txt')
    x = np.arange(54500, dtype=float)
    y = np.arange(54500, dtype=float)
    for i in range(54500):
        y[i] = ttest[i+1000]

    plt.plot(x, y)
    plt.show()
elif choice == 'mean':
    fixed_mean = np.loadtxt('fixed_mean.txt')
    random_mean = np.loadtxt('random_mean.txt')
    x = np.arange(55500, dtype=float)

    plt.plot(x, fixed_mean)
    plt.plot(x, random_mean)
    plt.legend(['fixed_mean', 'random_mean'])
    plt.show()


elif choice == 'sd':
    fixed_sd = np.loadtxt('fixed_standard_deviation.txt')
    random_sd = np.loadtxt('random_standard_deviation.txt')
    x = np.arange(55500, dtype=float)

    plt.plot(x, fixed_sd)
    plt.plot(x, random_sd)
    plt.legend(['fixed_standard_deviation', 'random_standard_deviation'])
    plt.show()
else: 
    print('Wrong choice!')
