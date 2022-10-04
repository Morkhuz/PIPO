#imports
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
    plaintexts = np.zeros([number_of_traces, 10], dtype='B')

    #for eachtrace in the file
    for y in range(number_of_traces):
        offset = ((samplepoints + 22) * y)

        #write plaintexts into array
        for x in range(10):
            plaintexts[y][x] = trace[x+8+samplepoints+offset]
    return plaintexts

##################################
#####                        #####
#####      Define S-Box      #####
#####                        #####
##################################


def secand( a0,  b0,  a1,  b1,  m):
    # temp variables
    r4 = 0
    r5 = 0 

    r4 = b0    # step 01
    r4 &= a0   # step 02
    r4 ^= m    # step 03

    r5 = a0    # step 04
    r5 &= b1   # step 05
    r4 ^= r5   # step 06

    r5 = a1    # step 07
    r5 &= b0   # step 08
    r4 ^= r5   # step 09

    r5 = a1    # step 10
    r5 &= b1   # step 11
    r4 ^= r5   # step 12

    return r4

def secor(a0,  b0,  a1,  b1,  c):
    # temp variables
    r4 = 0
    r5 = 0
    r6 = 0 

    r4 = c      # step 1
    r4 ^= b1    # step 2
    r5 = b0     # step 3
    r5 |= a1    # step 4
    r5 ^= r4    # step 5

    r4 = b0     # step 6
    r4 &= a0    # step 7
    r6 = a0     # step 8
    r6 |= b1    # step 9
    r4 ^= r6    # step 10
    r4 ^= r5    # step 11

    return r4

def slayer (X,  m0,  m1,  m2):
    # declare helping variables
    
    T = np.array([0,0,0], dtype=int)
    temp = 0
    c = (m0 | m1)

    temp = secand(X[7], X[6], m0, m1, m2)   # Step 4
    X[5] = X[5] ^ temp                      # Step 5

    temp = secand(X[3], X[5], m2, m0, m1)   # Step 6
    X[4] = X[4] ^ temp                      # Step 7
    X[7] = X[7] ^ X[4]                      # Step 8
    X[6] = X[6] ^ X[3]                      # Step 9

    temp = secor(X[5], X[4], m0, m2, c)     # Step 10
    X[3] = X[3] ^ temp                      # Step 11
    X[5] = X[5] ^ X[7]                      # Step 12

    temp = secand(X[5], X[6], m2, m0, m1)   # Step 13
    X[4] = X[4] ^ temp                      # Step 14

    temp = secand(X[1], X[0], m1, m0, m2)   # Step 15
    X[2] = X[2] ^ temp                      # Step 16

    temp = secor(X[1], X[2], m1, m0, c)     # Step 17
    X[0] = X[0] ^ temp                      # Step 18

    temp = secor(X[2], X[0], m0, m2, c)     # Step 19
    X[1] = X[1] ^ temp                      # Step 20
    X[2] = ~X[2]                            # Step 21

    X[7] = X[7] ^ X[1]                      # Step 22
    X[3] = X[3] ^ X[2]                      # Step 23
    X[4] = X[4] ^ X[0]                      # Step 24
    T[0] = X[7]                             # Step 25
    T[1] = X[3]                             # Step 26
    T[2] = X[4]                             # Step 27

    temp = secand(X[5], T[0], m2, m0, m1)   # Step 28
    X[6] = X[6] ^ temp                      # Step 29

    T[0] = T[0] ^ X[6]                      # Step 30

    temp = secor(T[2], T[1], m1, m2, c)     # Step 31
    X[6] = X[6] ^ temp                      # Step 32

    T[1] ^= X[5] ^ m0                       # Step 33

    temp = secor(X[6], T[2], m0, m1, c)     # Step 34
    X[5] = X[5] ^ temp                      # Step 35

    temp = secand(T[1], T[0], m0, m1, m2)   # Step 36
    T[2] = T[2] ^ temp                      # Step 37

    X[2] = X[2] ^ T[0]                      # Step 38
    T[0] = X[1] ^ T[2]                      # Step 39
    X[1] = X[0] ^ T[1]                      # Step 40
    X[0] = X[7] ^ m1                        # Step 41
    X[7] = T[0]                             # Step 42
    T[1] = X[3]                             # Step 43
    X[3] = X[6]                             # Step 44
    X[6] = T[1]                             # Step 45
    T[2] = X[4]                             # Step 46
    X[4] = X[5]                             # Step 47
    X[5] = T[2]                             # Step 48

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
plaintexts = np.loadtxt('plaintexts_10k.txt', dtype=int)
print('Done')
print(' ')


##################################
#####                        #####
#####          CPA           #####
#####                        #####
##################################


# Matrix P contains the n power traces measured
print('Loading Matrix P and compute mean of P...')
matrix_P = np.loadtxt('powertraces_10k.txt')
number_of_traces = np.shape(matrix_P)[0]
matrix_P_mean = np.mean(matrix_P, axis=0)

np.savetxt('matrix_P_mean.txt', matrix_P_mean)
print('Done')
print(' ')

# Matrix H contains the Hemming Weight of Values of Output S-Box
print('Computing Matrix H and mean of H...')
matrix_H = np.zeros((number_of_traces, 256))
for y in range(number_of_traces):
    for k in range(256):
        X = plaintexts[y]

        # compute masks m0, m1 and m2
        m0 = X[8]
        m1 = X[9]
        m2 = m0 ^ m1

        # add whitening key for correkt key expect for X[0] -> there we use the key guess
        X[0] ^= k
        X[1] ^= 0x22 ^ m2
        X[2] ^= 0x15 ^ m2
        X[3] ^= 0x2E ^ m1
        X[4] ^= 0xAD ^ m2
        X[5] ^= 0x20 ^ m2
        X[6] ^= 0x1D ^ m2
        X[7] ^= 0x7E ^ m2

        # Fill matrix_H with hemming weights
        matrix_H[y][k] = HW(slayer(X, m0, m1, m2))

matrix_H_mean = np.mean(matrix_H, axis=0)

np.savetxt('matrix_H.txt', matrix_H, fmt='%i')
np.savetxt('matrix_H_mean.txt', matrix_H_mean)

print('Done')
print(' ')

# compute the correlation coefficient between each column of the matrix H and each column of P
print('Computing correlation coefficients...')
matrix_r_i_j = np.zeros((256, samplepoints))

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
            matrix_r_i_j[i][j] = 0
        else:
            matrix_r_i_j[i][j] = cov / divisor


np.savetxt('correlation_coefficients.txt', matrix_r_i_j)
print('Done')
print(' ')

# search for maximum in matrix_r_i_j
maximum = 0.0
for i in range(256):
    for j in range(samplepoints):
        temp = matrix_r_i_j[i][j]
        if temp > maximum:
            maximum = temp
            key = i

print('Most probable key: ' + str(hex(key)))


# plot results
x = np.arange(samplepoints, dtype=int)

for i in range(256):
    plt.plot(x, matrix_r_i_j[i])
plt.show()

