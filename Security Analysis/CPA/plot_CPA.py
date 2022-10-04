#imports
from optparse import Values
from time import time
from unittest import TextTestResult
import numpy as np
import matplotlib.pyplot as plt

samplepoints = 103000

# import matrix_p_i_j
matrix_p_i_j = np.loadtxt('correlation_coefficients.txt')
print('Number of (rows,columns) = ', np.shape(matrix_p_i_j))

max_for_i = np.arange(256, dtype=float)


#create maximum for each key guess

for i in range(256):
    maximum = 0.0
    temp = 0.0
    for j in range(samplepoints):
        if matrix_p_i_j[i][j] < 0:
            temp = matrix_p_i_j[i][j] * -1
        else:
            temp = matrix_p_i_j[i][j]
        
        if temp > maximum:
            maximum = temp
    max_for_i[i] = maximum

maximum = 0.0
for i in range(256):
    if max_for_i[i] > maximum:
        maximum = max_for_i[i]
        key = i
print('maximum : ' + str(maximum))
print('key: ' + hex(key))    
print('value at correct key 0x97: ' + str(max_for_i[0x97]))

# plot results
x = np.arange(256, dtype=int)

plt.plot(x, max_for_i, ms=2, linewidth=1)
plt.show()

