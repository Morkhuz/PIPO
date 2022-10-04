#imports
import numbers
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
                trace = np.fromfile(f, dtype)
        return trace

def get_not(filepath):
        with open(filepath, 'rb') as f:
                number_of_traces = int.from_bytes(f.read(4), byteorder='little')
        return number_of_traces

def get_trace_array(trace, number_of_traces):
        #create numpy array for fixed_traces
        tracepoints = np.zeros([number_of_traces, samplepoints], dtype)

        #for each trace in the file
        for y in range(number_of_traces):
                offset = ((samplepoints + 22) * y)
                

                # write tracepoints into array
                for x in range(samplepoints):
                        tracepoints[y][x] = trace[x+8+offset]
        return tracepoints

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
#####    T-Test functions    #####
#####                        #####
##################################

def t_test(mean1, mean2, sd1, sd2, rows1, rows2):
        t = np.arange(samplepoints, dtype=float)
        diff_of_means = 0.0
        for x in range(samplepoints):
                diff_of_means = mean1[x] - mean2[x]
                if diff_of_means == 0:
                        t[x] = 0
                else:
                        summand1 = ((sd1[x] * sd1[x]) / rows1)
                        summand2 = ((sd2[x] * sd2[x]) / rows2)
                        root = math.sqrt(summand1 + summand2)
                        t[x] = diff_of_means / root
        return t
                


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
#print(fixed_not_00000)

##### Traces_00001.dat
fixed_trace_00001 = read_file("Traces_fixed/Traces_00001.dat")
fixed_not_00001 = get_not("Traces_fixed/Traces_00001.dat")
#print(fixed_not_00001)

##### Traces_00002.dat
fixed_trace_00002 = read_file("Traces_fixed/Traces_00002.dat")
fixed_not_00002 = get_not("Traces_fixed/Traces_00002.dat")
#print(fixed_not_00002)

##### Traces_00003.dat
fixed_trace_00003 = read_file("Traces_fixed/Traces_00003.dat")
fixed_not_00003 = get_not("Traces_fixed/Traces_00003.dat")
#print(fixed_not_00003)

##### Traces_00004.dat
fixed_trace_00004 = read_file("Traces_fixed/Traces_00004.dat")
fixed_not_00004 = get_not("Traces_fixed/Traces_00004.dat")
#print(fixed_not_00004)


##################################


########## Random plaintexts ##########
#### Traces_00000.dat
random_trace_00000 = read_file("Traces_random/Traces_00000.dat")
random_not_00000 = get_not("Traces_random/Traces_00000.dat")

#### Traces_00001.dat
random_trace_00001 = read_file("Traces_random/Traces_00001.dat")
random_not_00001 = get_not("Traces_random/Traces_00001.dat")

#### Traces_00002.dat
random_trace_00002 = read_file("Traces_random/Traces_00002.dat")
random_not_00002 = get_not("Traces_random/Traces_00002.dat")

#### Traces_00003.dat
random_trace_00003 = read_file("Traces_random/Traces_00003.dat")
random_not_00003 = get_not("Traces_random/Traces_00003.dat")

#### Traces_00004.dat
random_trace_00004 = read_file("Traces_random/Traces_00004.dat")
random_not_00004 = get_not("Traces_random/Traces_00004.dat")



##################################
#####                        #####
#####    Tracepoint Array    #####
#####                        #####
##################################

#get overall fixed_traces
print('Generating fixed_traces...')
fixed_trace_array_00000 = get_trace_array(fixed_trace_00000, fixed_not_00000)
fixed_trace_array_00001 = get_trace_array(fixed_trace_00001, fixed_not_00001)
fixed_trace_array_00002 = get_trace_array(fixed_trace_00002, fixed_not_00002)
fixed_trace_array_00003 = get_trace_array(fixed_trace_00003, fixed_not_00003)
fixed_trace_array_00004 = get_trace_array(fixed_trace_00004, fixed_not_00004)

fixed_traces = np.vstack((fixed_trace_array_00000, fixed_trace_array_00001, fixed_trace_array_00002, fixed_trace_array_00003, fixed_trace_array_00004))
print('The array for fixed_traces was successfully generated.')
print('Number of (rows,columns) = ', np.shape(fixed_traces))
print(" ")


print('Generating random_traces...')
random_trace_array_00000 = get_trace_array(random_trace_00000, random_not_00000)
random_trace_array_00001 = get_trace_array(random_trace_00001, random_not_00001)
random_trace_array_00002 = get_trace_array(random_trace_00002, random_not_00002)
random_trace_array_00003 = get_trace_array(random_trace_00003, random_not_00003)
random_trace_array_00004 = get_trace_array(random_trace_00004, random_not_00004)

random_traces = np.vstack((random_trace_array_00000, random_trace_array_00001, random_trace_array_00002, random_trace_array_00003, random_trace_array_00004))
print('The array for random_traces was successfully generated.')
print('Number of (rows,columns) = ', np.shape(random_traces))
print(" ")

"""
##################################
#####                        #####
#####    Powertrace Array    #####
#####                        #####
##################################
print('Generating power traces...')
powertraces = np.vstack((fixed_traces, random_traces))
np.savetxt('powertraces_10k.txt', powertraces, fmt='%i')
print('The array for powertraces was successfully stored.')
print('Number of (rows,columns) = ', np.shape(powertraces))
print(" ")

##################################
#####                        #####
#####    PLaintexts Array    #####
#####                        #####
##################################
print('Generating plaintexts...')
# fixed plaintexts
plaintext_fixed_00000 = get_trace_plaintexts(fixed_trace_00000, fixed_not_00000)
plaintext_fixed_00001 = get_trace_plaintexts(fixed_trace_00001, fixed_not_00001)
plaintext_fixed_00002 = get_trace_plaintexts(fixed_trace_00002, fixed_not_00002)
plaintext_fixed_00003 = get_trace_plaintexts(fixed_trace_00003, fixed_not_00003)
plaintext_fixed_00004 = get_trace_plaintexts(fixed_trace_00004, fixed_not_00004)

plaintexts_fixed = np.vstack((plaintext_fixed_00000, plaintext_fixed_00001, plaintext_fixed_00002, plaintext_fixed_00003, plaintext_fixed_00004))

# random plaintexts
plaintext_random_00000 = get_trace_plaintexts(random_trace_00000, random_not_00000)
plaintext_random_00001 = get_trace_plaintexts(random_trace_00001, random_not_00001)
plaintext_random_00002 = get_trace_plaintexts(random_trace_00002, random_not_00002)
plaintext_random_00003 = get_trace_plaintexts(random_trace_00003, random_not_00003)
plaintext_random_00004 = get_trace_plaintexts(random_trace_00004, random_not_00004)

plaintexts_random = np.vstack((plaintext_random_00000, plaintext_random_00001, plaintext_random_00002, plaintext_random_00003, plaintext_random_00004))
plaintexts = np.vstack((plaintexts_fixed, plaintexts_random))
np.savetxt('plaintexts_10k.txt', plaintexts, fmt='%i')
print('The array for plaintexts was successfully stored.')
print('Number of (rows,columns) = ', np.shape(plaintexts))
print(' ')
"""


##################################
#####                        #####
#####         T-Test         #####
#####                        #####
##################################

# get number of rows for both trace arrays
fixed_rows = np.shape(fixed_traces)[0]
random_rows = np.shape(random_traces)[0]

# get mean of fixed values
print('Generating fixed_mean values...')
fixed_mean = np.mean(fixed_traces, axis=0)
#np.savetxt('fixed_mean.txt', fixed_mean)
print('Done.')
print(" ")


# get mean of random values
print('Generating random_mean values...')
random_mean = np.mean(random_traces, axis=0)
#np.savetxt('random_mean.txt', random_mean)
print('Done.')
print(" ")



# get standard deviation of fixed values
print('Generating fixed_standard_deviation values...')
fixed_sd = np.std(fixed_traces, axis=0)
#np.savetxt('fixed_standard_deviation.txt', fixed_sd)
print('Done.')
print(" ")

# get standard deviation of random values
print('Generating random_standard_deviation values...')
random_sd = np.std(random_traces, axis=0)
#np.savetxt('random_standard_deviation.txt', random_sd)
print('Done.')
print(" ")

# computing t-test
print('Computing the T-Test...')
t = t_test(fixed_mean, random_mean, fixed_sd, random_sd, fixed_rows, random_rows)
np.savetxt('ttest_10k.txt', t)
print('Done.')



##################################
#####                        #####
#####          Plot          #####
#####                        #####
##################################

x = np.arange(samplepoints, dtype=float)
plt.axhline(y = 4.5, color = 'r', linestyle = '-')
plt.axhline(y = -4.5, color = 'r', linestyle = '-')
plt.plot(x, t)
plt.show()



