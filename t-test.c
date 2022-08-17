#include <stdio.h>
#include <math.h>

float Mean(float arr[], int n) {
    float sum = 0;
    for ( int i = 0; i < n; i++) {
        sum += arr[i];  
    }
    return sum / n;
}

float StandardDeviation (float arr[], float mean, int n) {
    float variance, sd, sum = 0; 
    for ( int i = 0; i < n; i++) {
        sum += (arr[i] - mean)*(arr[i] - mean);
    }
    return sqrt(sum / (n-1));
}

float t_test (float arr1[], int n1, float arr2[], int n2){
    // Arithmetic Mean
    float mean1 = Mean(arr1, n1);
    float mean2 = Mean(arr2, n2);
    
    // standard Deviation
    float sd1 = StandardDeviation(arr1, mean1, n1);
    float sd2 = StandardDeviation(arr2, mean2, n2);
    
    // t-test
    float t = (mean1 - mean2) / sqrt((sd1 * sd1) / n1 + (sd2 * sd2) / n2);
    return t;
}


int main () {

    float arr1[] = {10, 20, 30, 40, 50};
    int n1 = sizeof(arr1) / sizeof(arr1[0]); // number of Elements in Dataset1
    
    float arr2[] = {1, 29, 46, 78, 99};
    int n2 = sizeof(arr2) / sizeof(arr2[0]); // number of Elements in Dataset2
    
    float t = t_test(arr1, n1, arr2, n2);


    printf("\nt = %f\n", t);
}