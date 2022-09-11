#imports
import numbers
from pydoc import plain
import numpy as np
import math
import matplotlib.pyplot as plt

##################################
#####                        #####
#####    trace functions     #####
#####                        #####
##################################
samplepoints = 103000

def read_file(filepath):
        with open(filepath, 'rb') as f:
                trace = np.fromfile(f, dtype='B')
        return trace

def get_not(filepath):
        with open(filepath, 'rb') as f:
                number_of_traces = int.from_bytes(f.read(4), byteorder='little')
        return number_of_traces


def get_trace_plaintexts(trace, number_of_traces):
    #create numpy array for plaintexts
    plaintexts = np.zeros([number_of_traces, 8], dtype='B')

    #for eachtrace in the file
    for y in range(number_of_traces):
        offset = ((samplepoints + 20) * y)

        #write plaintexts into array
        for x in range(8):
            plaintexts[y][x] = trace[x+8+samplepoints+offset]
    return plaintexts
    
def mean(matrix, rows, columns):
        mean = np.arange(columns, dtype=float)
        for x in range(columns):
                sum = 0.0
                for y in range(rows):
                        sum += matrix[y][x]
                mean[x] = sum / rows
        return mean

##################################
#####                        #####
#####      Define S-Box      #####
#####                        #####
##################################

Sbox_values = [0x5E, 0xF9, 0xFC, 0x00, 0x3F, 0x85, 0xBA, 0x5B, 0x18, 0x37, 0xB2, 0xC6, 0x71, 0xC3, 0x74, 0x9D, 0xA7, 0x94, 0x0D, 0xE1, 0xCA, 0x68, 0x53, 0x2E, 0x49, 0x62, 0xEB, 0x97, 0xA4, 0x0E, 0x2D, 0xD0, 0x16, 0x25, 0xAC, 0x48, 0x63, 0xD1, 0xEA, 0x8F, 0xF7, 0x40, 0x45, 0xB1, 0x9E, 0x34, 0x1B, 0xF2, 0xB9, 0x86, 0x03, 0x7F, 0xD8, 0x7A, 0xDD, 0x3C, 0xE0, 0xCB, 0x52, 0x26, 0x15, 0xAF, 0x8C, 0x69, 0xC2, 0x75, 0x70, 0x1C, 0x33, 0x99, 0xB6, 0xC7, 0x04, 0x3B, 0xBE, 0x5A, 0xFD, 0x5F, 0xF8, 0x81, 0x93, 0xA0, 0x29, 0x4D, 0x66, 0xD4, 0xEF, 0x0A, 0xE5, 0xCE, 0x57, 0xA3, 0x90, 0x2A, 0x09, 0x6C, 0x22, 0x11, 0x88, 0xE4, 0xCF, 0x6D, 0x56, 0xAB, 0x7B, 0xDC, 0xD9, 0xBD, 0x82, 0x38, 0x07, 0x7E, 0xB5, 0x9A, 0x1F, 0xF3, 0x44, 0xF6, 0x41, 0x30, 0x4C, 0x67, 0xEE, 0x12, 0x21, 0x8B, 0xA8, 0xD5, 0x55, 0x6E, 0xE7, 0x0B, 0x28, 0x92, 0xA1, 0xCC, 0x2B, 0x08, 0x91, 0xED, 0xD6, 0x64, 0x4F, 0xA2, 0xBC, 0x83, 0x06, 0xFA, 0x5D, 0xFF, 0x58, 0x39, 0x72, 0xC5, 0xC0, 0xB4, 0x9B, 0x31, 0x1E, 0x77, 0x01, 0x3E, 0xBB, 0xDF, 0x78, 0xDA, 0x7D, 0x84, 0x50, 0x6B, 0xE2, 0x8E, 0xAD, 0x17, 0x24, 0xC9, 0xAE, 0x8D, 0x14, 0xE8, 0xD3, 0x61, 0x4A, 0x27, 0x47, 0xF0, 0xF5, 0x19, 0x36, 0x9C, 0xB3, 0x42, 0x1D, 0x32, 0xB7, 0x43, 0xF4, 0x46, 0xF1, 0x98, 0xEC, 0xD7, 0x4E, 0xAA, 0x89, 0x23, 0x10, 0x65, 0x8A, 0xA9, 0x20, 0x54, 0x6F, 0xCD, 0xE6, 0x13, 0xDB, 0x7C, 0x79, 0x05, 0x3A, 0x80, 0xBF, 0xDE, 0xE9, 0xD2, 0x4B, 0x2F, 0x0C, 0xA6, 0x95, 0x60, 0x0F, 0x2C, 0xA5, 0x51, 0x6A, 0xC8, 0xE3, 0x96, 0xB0, 0x9F, 0x1A, 0x76, 0xC1, 0x73, 0xC4, 0x35, 0xFE, 0x59, 0x5C, 0xB8, 0x87, 0x3D, 0x02, 0xFB]
sbox = np.array(Sbox_values)

def convert(X):
    T = np.array([0, 0, 0, 0, 0, 0, 0, 0])

    for i in range(8):
        for j in range(8):
            T[i] |= (((X[j] & (1<<i))>>i) << j)
    
    for i in range(8):
        X[i] = T[i]


def sbox_TLU(X, k):
    X[0] = X[0] ^ k
    convert(X)
    for i in range(8):
        X[0] = sbox[X[0]]
    convert(X)
    return X[0]

##################################
#####                        #####
#####     Hemming Weight     #####
#####                        #####
##################################

def HW(x):
    weight = 0
    for i in range(8):
        if (x & (1 << i)):
            weight += 1
    return weight

##################################
#####                        #####
#####    Read Trace Files    #####
#####                        #####
##################################

#set options to print np array as hex
np.set_printoptions(formatter={'int':lambda x:hex(int(x))})
dtype = np.dtype('b')



########## Fixed plaintexts ##########
##### Traces_00000.dat
fixed_trace_00000 = read_file("Traces_fixed/Traces_00000.dat")
fixed_not_00000 = get_not("Traces_fixed/Traces_00000.dat")

#get overall fixed_traces
print('Generating plaintexts...')
plaintexts = get_trace_plaintexts(fixed_trace_00000, fixed_not_00000)
print('Done')
print(' ')


##################################
#####                        #####
#####          CPA           #####
#####                        #####
##################################


# Matrix P contains the n power traces measured
print('Loading Matrix P and compute mean of P...')
matrix_P = np.loadtxt('powertraces.txt')
number_of_traces = 100 #np.shape(matrix_P)[0]
matrix_P_mean = mean(matrix_P, number_of_traces, samplepoints)
print('Done')
print(' ')
output = np.savetxt('matrix_P_mean.txt', matrix_P_mean)
print(output)

# Matrix H contains the Hemming Weight of Values of Output S-Box
print('Computing Matrix H and mean of H...')
matrix_H = np.zeros((number_of_traces, 256))
for y in range(number_of_traces):
    for k in range(256):
        matrix_H[y][k] = HW(sbox_TLU(plaintexts[y], k))

matrix_H_mean = mean(matrix_H, number_of_traces, 256)
print('Done')
print(' ')

output = np.savetxt('matrix_H.txt', matrix_H)
print(output)

output = np.savetxt('matrix_H_mean.txt', matrix_H_mean)
print(output)

# compute the correlation coefficient between each column of the matrix H and each column of P
print('Computing correlation coefficients...')
matrix_p_i_j = np.zeros((256, samplepoints))

divisor = 0.0
for i in range(256):
    print('i = ' + str(i))
    for j in range(samplepoints):
        divisor_left = 0.0
        divisor_right = 0.0
        cov = 0.0

        for l in range(number_of_traces):
            cov += (matrix_H[l][i] - matrix_H_mean[i]) * (matrix_P[l][j] - matrix_P_mean[j])  
            divisor_left += math.pow((matrix_H[l][i] - matrix_H_mean[i]), 2)
            divisor_right += math.pow((matrix_P[l][j] - matrix_P_mean[j]), 2)
        
        divisor = math.sqrt(divisor_left * divisor_right)
        

        if divisor == 0:
            matrix_p_i_j[i][j] = 0
        else:
            matrix_p_i_j[i][j] = cov / divisor


coefficients = np.savetxt('correlation_coefficients.txt', matrix_p_i_j)
print(coefficients)
print('Done')
print(' ')

# search for maximum in matrix_p_i_j
maximum = 0.0
for i in range(256):
    for j in range(samplepoints):
        temp = matrix_p_i_j[i][j]
        if temp > maximum:
            maximum = temp
            key = i

print('Most probable key: ' + str(hex(key)))


# plot results
x = np.arange(samplepoints, dtype=int)

for i in range(256):
    plt.plot(x, matrix_p_i_j[i])
plt.show()