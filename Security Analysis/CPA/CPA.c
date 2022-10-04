#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define SAMPLEPOINTS        103000
#define NUMBER_OF_TRACES    10000

//int matrix_P[NUMBER_OF_TRACES][SAMPLEPOINTS];
float matrix_P_mean[SAMPLEPOINTS];
int matrix_H[NUMBER_OF_TRACES][256];
float matrix_H_mean[256];
int plaintexts[NUMBER_OF_TRACES][10];
double matrix_r_i_j[256][SAMPLEPOINTS];


int main () {
    /////////////////////////
    ///   Import Traces   ///
    /////////////////////////

    int *matrix_P = malloc(sizeof(int) * NUMBER_OF_TRACES * SAMPLEPOINTS);
    if (NULL == matrix_P) return 1;
    // matrix p[i*number_of_traces+j]

    FILE *myfile;
    int myvariable;
    float myvar2;
    
    // Matrix P
    myfile=fopen("powertraces_10k.txt", "r");
    for(int i = 0; i < NUMBER_OF_TRACES; i++) {
        for (int j = 0 ; j < SAMPLEPOINTS; j++) {
            fscanf(myfile,"%d",&myvariable);
            matrix_P[i*NUMBER_OF_TRACES+j] = myvariable;
        }
    }
    fclose(myfile);

    printf("P[0][0] = %d", matrix_P[0]);

/*
    // plaintexts + m0 + m1
    myfile=fopen("plaintexts_10k.txt", "r");
    for(int i = 0; i < NUMBER_OF_TRACES; i++) {
        for (int j = 0 ; j < 10; j++) {
            fscanf(myfile,"%d",&myvariable);
            plaintexts[i][j] = myvariable;
        }
    }
    fclose(myfile);


    // Matrix H
    myfile=fopen("matrix_H.txt", "r");
    for(int i = 0; i < NUMBER_OF_TRACES; i++) {
        for (int j = 0 ; j < 256; j++) {
            fscanf(myfile,"%d",&myvariable);
            matrix_H[i][j] = myvariable;
        }
    }
    fclose(myfile);

    // Matrix P mean
    for (int x = 0; x < SAMPLEPOINTS; x++) {
        float sum = 0;
        for (int y = 0; y < NUMBER_OF_TRACES; y++) {
            sum += matrix_P[y][x];
        matrix_P_mean[x] = sum / NUMBER_OF_TRACES;
        }
    }

    // Matrix H mean
    myfile=fopen("matrix_H_mean.txt", "r");
    for(int i = 0; i < 1; i++) {
        for (int j = 0 ; j < 256; j++) {
            fscanf(myfile,"%d",&myvariable);
            matrix_H_mean[j] = myvariable;
        }
    }
    fclose(myfile);



    // compute correlation
    double divisor = 0;
    double cov;
    for (int i=0; i<256; i++) {
        print('i = %d', i);
        for (int j=0; j<SAMPLEPOINTS; j++) {
            // set divisor_left and devisor_right to zero again
            double divisor_left = 0;
            double divisor_right = 0;
            cov = 0;

            for (int l=0; l<NUMBER_OF_TRACES; l++) {
                // compute covariance
                cov += (matrix_H[l][i] - matrix_H_mean[i]) * (matrix_P[l][j] - matrix_P_mean[j]);
                divisor_left += pow(matrix_H[l][i] - matrix_H_mean[i], 2);
                divisor_right += pow(matrix_P[l][j] - matrix_P_mean[j], 2);
            }
            divisor = sqrt(divisor_left * divisor_right);

            if (divisor == 0) {
                matrix_r_i_j[i][j] = 0;
            }
            else {
                matrix_r_i_j[i][j] = cov / divisor;
            }
        }
    }

    // search in matrix_p_i_j for maximum
    double maximum_tmp = 0;
    double maximum = 0;
    unsigned int key_candidate;

    for (int i=0; i<256; i++) {
        for (int j=0; j<SAMPLEPOINTS; j++) {
            maximum_tmp = matrix_r_i_j[i][j];
            // check if there is a larger amount
            if (maximum_tmp > maximum){
                maximum = maximum_tmp;
                key_candidate = i;
            }
        }
    }

*/

free(matrix_P);
}
