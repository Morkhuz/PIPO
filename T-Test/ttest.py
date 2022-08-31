#imports
import numpy as np
import math
import matplotlib.pyplot as plt

##################################
#####                        #####
#####    trace functions     #####
#####                        #####
##################################

def read_file(filepath):
        with open(filepath, 'rb') as f:
                trace = np.fromfile(f, dtype)
        return trace

def get_not(trace):
        number_of_traces = (trace[3] << 24) + (trace[2] << 16) + (trace[1] << 8) + trace[0]
        return number_of_traces

def get_trace_array(trace, number_of_traces):
        #create numpy array for fixed_traces
        tracepoints = np.zeros([number_of_traces, 55500], dtype)

        #for each trace in the file
        for y in range(number_of_traces):
                # check if number of points is 55500
                offset = (55520 * y)
                #print(y)
                number_of_points = (trace[7+offset] << 24) + (trace[6+offset] << 16) + (trace[5+offset] << 8) + trace[4+offset]
                if number_of_points != 55500:
                        print('ERROR: Trace ' + str(y) + ' has ' + str(number_of_points) + ' points, instead of 55500!')
                        break

                # write tracepoints into array
                for x in range(55500):
                        tracepoints[y][x] = trace[x+8]
        return tracepoints



##################################
#####                        #####
#####    T-Test functions    #####
#####                        #####
##################################

def mean(trace_array, rows):
        mean = np.arange(55500, dtype=float)
        for x in range(55500):
                sum = 0.0
                for y in range(rows):
                        sum += trace_array[y][x]
                mean[x] = sum / rows
        return mean

def standard_deviation(trace_array, rows, mean):
        sigma = np.arange(55500, dtype=float)
        for x in range(55500):
                sum = 0.0
                for y in range(rows):
                        sum += ((trace_array[y][x] - mean[x]) ** 2)
                variance = sum / (rows - 1)
                sigma[x] = math.sqrt(variance)
        return sigma

def t_test(mean1, mean2, sd1, sd2, rows1, rows2):
        t = np.arange(55500, dtype=float)
        diff_of_means = 0.0
        for x in range(55500):
                diff_of_means = mean1[x] - mean2[x]
                
                # test if difference of means is negative. If so, make it positive
                #if diff_of_means < 0:
                #        diff_of_means *= -1

                t[x] = diff_of_means / (math.sqrt(((sd1[x] * sd1[x]) / rows1) + ((sd2[x] * sd2[x]) / rows2)))
        return t
                


##################################
#####                        #####
#####    Read Trace Files    #####
#####                        #####
##################################

#set options to print np array as hex
np.set_printoptions(formatter={'int':lambda x:hex(int(x))})
dtype = np.dtype('B')


########## Fixed plaintexts ##########
##### Traces_00000.dat
fixed_trace_00000 = read_file("Traces_fixed/Traces_00000.dat")
fixed_not_00000 = get_not(fixed_trace_00000)

##### Traces_00001.dat
fixed_trace_00001 = read_file("Traces_fixed/Traces_00001.dat")
fixed_not_00001 = get_not(fixed_trace_00001)

##### Traces_00002.dat
fixed_trace_00002 = read_file("Traces_fixed/Traces_00002.dat")
fixed_not_00002 = get_not(fixed_trace_00002)

##### Traces_00003.dat
fixed_trace_00003 = read_file("Traces_fixed/Traces_00003.dat")
fixed_not_00003 = get_not(fixed_trace_00003)

##### Traces_00001.dat
fixed_trace_00004 = read_file("Traces_fixed/Traces_00004.dat")
fixed_not_00004 = get_not(fixed_trace_00004)


##################################


########## Random plaintexts ##########
#### Traces_00000.dat
random_trace_00000 = read_file("Traces_random/Traces_00000.dat")
random_not_00000 = get_not(random_trace_00000)

random_trace_00001 = read_file("Traces_random/Traces_00001.dat")
random_not_00001 = get_not(random_trace_00001)

random_trace_00002 = read_file("Traces_random/Traces_00002.dat")
random_not_00002 = get_not(random_trace_00002)

random_trace_00003 = read_file("Traces_random/Traces_00003.dat")
random_not_00003 = get_not(random_trace_00003)

random_trace_00004 = read_file("Traces_random/Traces_00004.dat")
random_not_00004 = get_not(random_trace_00004)

random_trace_00005 = read_file("Traces_random/Traces_00005.dat")
random_not_00005 = get_not(random_trace_00005)


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
random_trace_array_00005 = get_trace_array(random_trace_00005, random_not_00005)

random_traces = np.vstack((random_trace_array_00000, random_trace_array_00001, random_trace_array_00002, random_trace_array_00003, random_trace_array_00004, random_trace_array_00005))
print('The array for random_traces was successfully generated.')
print('Number of (rows,columns) = ', np.shape(random_traces))
print(" ")



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
fixed_mean = mean(fixed_traces, fixed_rows)
output = np.savetxt('fixed_mean.txt', fixed_mean)
print(output)
print('Done.')
print(" ")


# get mean of random values
print('Generating random_mean values...')
random_mean = mean(random_traces, random_rows)
output = np.savetxt('random_mean.txt', random_mean)
print(output)
print('Done.')
print(" ")



# get standard deviation of fixed values
print('Generating fixed_standard_deviation values...')
fixed_sd = standard_deviation(fixed_traces, fixed_rows, fixed_mean)
output = np.savetxt('fixed_standard_deviation.txt', fixed_sd)
print(output)
print('Done.')
print(" ")

# get standard deviation of random values
print('Generating random_standard_deviation values...')
random_sd = standard_deviation(random_traces, random_rows, random_mean)
output = np.savetxt('random_standard_deviation.txt', random_sd)
print(output)
print('Done.')
print(" ")

# computing t-test
print('Computing the T-Test...')
t = t_test(fixed_mean, random_mean, fixed_sd, random_sd, fixed_rows, random_rows)
output = np.savetxt('ttest.txt', t)
print(output)

print('Done.')



##################################
#####                        #####
#####          Plot          #####
#####                        #####
##################################


x = np.arange(55500)
plt.plot(x, t)
plt.show()

print('Press any button to exit...')
x = input()
