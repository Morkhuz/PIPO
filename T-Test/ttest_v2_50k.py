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

##### Traces_00005.dat
fixed_trace_00005 = read_file("Traces_fixed/Traces_00005.dat")
fixed_not_00005 = get_not("Traces_fixed/Traces_00005.dat")
#print(fixed_not_00005)

##### Traces_00006.dat
fixed_trace_00006 = read_file("Traces_fixed/Traces_00006.dat")
fixed_not_00006 = get_not("Traces_fixed/Traces_00006.dat")

##### Traces_00007.dat
fixed_trace_00007 = read_file("Traces_fixed/Traces_00007.dat")
fixed_not_00007 = get_not("Traces_fixed/Traces_00007.dat")

##### Traces_00008.dat
fixed_trace_00008 = read_file("Traces_fixed/Traces_00008.dat")
fixed_not_00008 = get_not("Traces_fixed/Traces_00008.dat")

##### Traces_00009.dat
fixed_trace_00009 = read_file("Traces_fixed/Traces_00009.dat")
fixed_not_00009 = get_not("Traces_fixed/Traces_00009.dat")

##### Traces_00010.dat
fixed_trace_00010 = read_file("Traces_fixed/Traces_00010.dat")
fixed_not_00010 = get_not("Traces_fixed/Traces_00010.dat")

##### Traces_00011.dat
fixed_trace_00011 = read_file("Traces_fixed/Traces_00011.dat")
fixed_not_00011 = get_not("Traces_fixed/Traces_00011.dat")

##### Traces_00012.dat
fixed_trace_00012 = read_file("Traces_fixed/Traces_00012.dat")
fixed_not_00012 = get_not("Traces_fixed/Traces_00012.dat")

##### Traces_00013.dat
fixed_trace_00013 = read_file("Traces_fixed/Traces_00013.dat")
fixed_not_00013 = get_not("Traces_fixed/Traces_00013.dat")

##### Traces_00014.dat
fixed_trace_00014 = read_file("Traces_fixed/Traces_00014.dat")
fixed_not_00014 = get_not("Traces_fixed/Traces_00014.dat")

##### Traces_00015.dat
fixed_trace_00015 = read_file("Traces_fixed/Traces_00015.dat")
fixed_not_00015 = get_not("Traces_fixed/Traces_00015.dat")

##### Traces_00016.dat
fixed_trace_00016 = read_file("Traces_fixed/Traces_00016.dat")
fixed_not_00016 = get_not("Traces_fixed/Traces_00016.dat")

##### Traces_00017.dat
fixed_trace_00017 = read_file("Traces_fixed/Traces_00017.dat")
fixed_not_00017 = get_not("Traces_fixed/Traces_00017.dat")

##### Traces_00018.dat
fixed_trace_00018 = read_file("Traces_fixed/Traces_00018.dat")
fixed_not_00018 = get_not("Traces_fixed/Traces_00018.dat")

##### Traces_00019.dat
fixed_trace_00019 = read_file("Traces_fixed/Traces_00019.dat")
fixed_not_00019 = get_not("Traces_fixed/Traces_00019.dat")

##### Traces_00020.dat
fixed_trace_00020 = read_file("Traces_fixed/Traces_00020.dat")
fixed_not_00020 = get_not("Traces_fixed/Traces_00020.dat")

##### Traces_00021.dat
fixed_trace_00021 = read_file("Traces_fixed/Traces_00021.dat")
fixed_not_00021 = get_not("Traces_fixed/Traces_00021.dat")

##### Traces_00022.dat
fixed_trace_00022 = read_file("Traces_fixed/Traces_00022.dat")
fixed_not_00022 = get_not("Traces_fixed/Traces_00022.dat")

##### Traces_00023.dat
fixed_trace_00023 = read_file("Traces_fixed/Traces_00023.dat")
fixed_not_00023 = get_not("Traces_fixed/Traces_00023.dat")

##### Traces_00024.dat
fixed_trace_00024 = read_file("Traces_fixed/Traces_00024.dat")
fixed_not_00024 = get_not("Traces_fixed/Traces_00024.dat")

##################################


########## Random plaintexts ##########
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

#### Traces_00005.dat
random_trace_00005 = read_file("Traces_random/Traces_00005.dat")
random_not_00005 = get_not("Traces_random/Traces_00005.dat")

#### Traces_00006.dat
random_trace_00006 = read_file("Traces_random/Traces_00006.dat")
random_not_00006 = get_not("Traces_random/Traces_00006.dat")

#### Traces_00007.dat
random_trace_00007 = read_file("Traces_random/Traces_00007.dat")
random_not_00007 = get_not("Traces_random/Traces_00007.dat")

#### Traces_00008.dat
random_trace_00008 = read_file("Traces_random/Traces_00008.dat")
random_not_00008 = get_not("Traces_random/Traces_00008.dat")

#### Traces_00009.dat
random_trace_00009 = read_file("Traces_random/Traces_00009.dat")
random_not_00009 = get_not("Traces_random/Traces_00009.dat")

#### Traces_00010.dat
random_trace_00010 = read_file("Traces_random/Traces_00010.dat")
random_not_00010 = get_not("Traces_random/Traces_00010.dat")

#### Traces_00011.dat
random_trace_00011 = read_file("Traces_random/Traces_00011.dat")
random_not_00011 = get_not("Traces_random/Traces_00011.dat")

#### Traces_00012.dat
random_trace_00012 = read_file("Traces_random/Traces_00012.dat")
random_not_00012 = get_not("Traces_random/Traces_00012.dat")

#### Traces_00013.dat
random_trace_00013 = read_file("Traces_random/Traces_00013.dat")
random_not_00013 = get_not("Traces_random/Traces_00013.dat")

#### Traces_00014.dat
random_trace_00014 = read_file("Traces_random/Traces_00014.dat")
random_not_00014 = get_not("Traces_random/Traces_00014.dat")

#### Traces_00015.dat
random_trace_00015 = read_file("Traces_random/Traces_00015.dat")
random_not_00015 = get_not("Traces_random/Traces_00015.dat")

#### Traces_00016.dat
random_trace_00016 = read_file("Traces_random/Traces_00016.dat")
random_not_00016 = get_not("Traces_random/Traces_00016.dat")

#### Traces_00017.dat
random_trace_00017 = read_file("Traces_random/Traces_00017.dat")
random_not_00017 = get_not("Traces_random/Traces_00017.dat")

#### Traces_00018.dat
random_trace_00018 = read_file("Traces_random/Traces_00018.dat")
random_not_00018 = get_not("Traces_random/Traces_00018.dat")

#### Traces_00019.dat
random_trace_00019 = read_file("Traces_random/Traces_00019.dat")
random_not_00019 = get_not("Traces_random/Traces_00019.dat")

#### Traces_00020.dat
random_trace_00020 = read_file("Traces_random/Traces_00020.dat")
random_not_00020 = get_not("Traces_random/Traces_00020.dat")

#### Traces_00021.dat
random_trace_00021 = read_file("Traces_random/Traces_00021.dat")
random_not_00021 = get_not("Traces_random/Traces_00021.dat")

#### Traces_00022.dat
random_trace_00022 = read_file("Traces_random/Traces_00022.dat")
random_not_00022 = get_not("Traces_random/Traces_00022.dat")

#### Traces_00023.dat
random_trace_00023 = read_file("Traces_random/Traces_00023.dat")
random_not_00023 = get_not("Traces_random/Traces_00023.dat")

#### Traces_00024.dat
random_trace_00024 = read_file("Traces_random/Traces_00024.dat")
random_not_00024 = get_not("Traces_random/Traces_00024.dat")


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
fixed_trace_array_00005 = get_trace_array(fixed_trace_00005, fixed_not_00005)
fixed_trace_array_00006 = get_trace_array(fixed_trace_00006, fixed_not_00006)
fixed_trace_array_00007 = get_trace_array(fixed_trace_00007, fixed_not_00007)
fixed_trace_array_00008 = get_trace_array(fixed_trace_00008, fixed_not_00008)
fixed_trace_array_00009 = get_trace_array(fixed_trace_00009, fixed_not_00009)
fixed_trace_array_00010 = get_trace_array(fixed_trace_00010, fixed_not_00010)

fixed_trace_array_00011 = get_trace_array(fixed_trace_00011, fixed_not_00011)
fixed_trace_array_00012 = get_trace_array(fixed_trace_00012, fixed_not_00012)
fixed_trace_array_00013 = get_trace_array(fixed_trace_00013, fixed_not_00013)
fixed_trace_array_00014 = get_trace_array(fixed_trace_00014, fixed_not_00014)
fixed_trace_array_00015 = get_trace_array(fixed_trace_00015, fixed_not_00015)
fixed_trace_array_00016 = get_trace_array(fixed_trace_00016, fixed_not_00016)
fixed_trace_array_00017 = get_trace_array(fixed_trace_00017, fixed_not_00017)
fixed_trace_array_00018 = get_trace_array(fixed_trace_00018, fixed_not_00018)
fixed_trace_array_00019 = get_trace_array(fixed_trace_00019, fixed_not_00019)
fixed_trace_array_00020 = get_trace_array(fixed_trace_00020, fixed_not_00020)

fixed_trace_array_00021 = get_trace_array(fixed_trace_00021, fixed_not_00021)
fixed_trace_array_00022 = get_trace_array(fixed_trace_00022, fixed_not_00022)
fixed_trace_array_00023 = get_trace_array(fixed_trace_00023, fixed_not_00023)
fixed_trace_array_00024 = get_trace_array(fixed_trace_00024, fixed_not_00024)

fixed_traces = np.vstack((fixed_trace_array_00000, fixed_trace_array_00001, fixed_trace_array_00002, fixed_trace_array_00003, fixed_trace_array_00004, fixed_trace_array_00005, fixed_trace_array_00006, fixed_trace_array_00007, fixed_trace_array_00008, fixed_trace_array_00009, fixed_trace_array_00010, fixed_trace_array_00011, fixed_trace_array_00012, fixed_trace_array_00013, fixed_trace_array_00014, fixed_trace_array_00015, fixed_trace_array_00016, fixed_trace_array_00017, fixed_trace_array_00018, fixed_trace_array_00019, fixed_trace_array_00020, fixed_trace_array_00021, fixed_trace_array_00022, fixed_trace_array_00023, fixed_trace_array_00024))
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
random_trace_array_00006 = get_trace_array(random_trace_00006, random_not_00006)
random_trace_array_00007 = get_trace_array(random_trace_00007, random_not_00007)
random_trace_array_00008 = get_trace_array(random_trace_00008, random_not_00008)
random_trace_array_00009 = get_trace_array(random_trace_00009, random_not_00009)
random_trace_array_00010 = get_trace_array(random_trace_00010, random_not_00010)

random_trace_array_00011 = get_trace_array(random_trace_00011, random_not_00011)
random_trace_array_00012 = get_trace_array(random_trace_00012, random_not_00012)
random_trace_array_00013 = get_trace_array(random_trace_00013, random_not_00013)
random_trace_array_00014 = get_trace_array(random_trace_00014, random_not_00014)
random_trace_array_00015 = get_trace_array(random_trace_00015, random_not_00015)
random_trace_array_00016 = get_trace_array(random_trace_00016, random_not_00016)
random_trace_array_00017 = get_trace_array(random_trace_00017, random_not_00017)
random_trace_array_00018 = get_trace_array(random_trace_00018, random_not_00018)
random_trace_array_00019 = get_trace_array(random_trace_00019, random_not_00019)
random_trace_array_00020 = get_trace_array(random_trace_00020, random_not_00020)

random_trace_array_00021 = get_trace_array(random_trace_00021, random_not_00021)
random_trace_array_00022 = get_trace_array(random_trace_00022, random_not_00022)
random_trace_array_00023 = get_trace_array(random_trace_00023, random_not_00023)
random_trace_array_00024 = get_trace_array(random_trace_00024, random_not_00024)

random_traces = np.vstack((random_trace_array_00000, random_trace_array_00001, random_trace_array_00002, random_trace_array_00003, random_trace_array_00004, random_trace_array_00005, random_trace_array_00006, random_trace_array_00007, random_trace_array_00008, random_trace_array_00009, random_trace_array_00010, random_trace_array_00011, random_trace_array_00012, random_trace_array_00013, random_trace_array_00014, random_trace_array_00015, random_trace_array_00016, random_trace_array_00017, random_trace_array_00018, random_trace_array_00019, random_trace_array_00020, random_trace_array_00021, random_trace_array_00022, random_trace_array_00023, random_trace_array_00024))
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
np.savetxt('ttest_50k.txt', t)
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



