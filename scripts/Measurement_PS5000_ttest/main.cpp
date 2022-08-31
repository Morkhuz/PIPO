
#include "Scopetool.h"
#include "windows.h"
#include "AtfFileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include "ComClass.h"
#include <time.h>


const unsigned int      ComPort = 12;
const DWORD			    BaudRate = 115200;

const unsigned long int PreTriggerSamples = 1000; // configure this
const unsigned long int PostTriggerSamples = 54500; // and this
const unsigned long int NumberOfTraces = 10000;

const uint64_t 			k1 = 0x6DC416DD779428D2;
const uint64_t 			k0 = 0x7E1D20AD2E152297;

#define ENABLE_SCOPE


uint8_t secand (uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t m) {
    // declare variables
    uint8_t r4, r5; // temp variables

    r4 = b0;    // step 01
    r4 &= a0;   // step 02
    r4 ^= m;    // step 03

    r5 = a0;    // step 04
    r5 &= b1;   // step 05
    r4 ^= r5;   // step 06

    r5 = a1;    // step 07
    r5 &= b0;   // step 08
    r4 ^= r5;   // step 09

    r5 = a1;    // step 10
    r5 &= b1;   // step 11
    r4 ^= r5;   // step 12

    return r4;
}

uint8_t secor (uint8_t a0, uint8_t b0, uint8_t a1, uint8_t b1, uint8_t m) {
    // declare variables
    uint8_t r4, r5, r6; // temp variables

    r4 = m;  // step 1
    r4 ^= b1;   // step 2
    r5 = b0;    // step 3
    r5 |= a1;   // step 4
    r5 ^= r4;   // step 5

    r4 = b0;    // step 6
    r4 &= a0;   // step 7
    r6 = a0;    // step 8
    r6 |= b1;   // step 9
    r4 ^= r6;   // step 10
    r4 ^= r5;   // step 11

    return r4;
}

void rlayer (uint8_t *array) {
    array[1] = ((array[1] << 7)) | ((array[1] >> 1));
    array[2] = ((array[2] << 4)) | ((array[2] >> 4));
    array[3] = ((array[3] << 3)) | ((array[3] >> 5));
    array[4] = ((array[4] << 6)) | ((array[4] >> 2));
    array[5] = ((array[5] << 5)) | ((array[5] >> 3));
    array[6] = ((array[6] << 1)) | ((array[6] >> 7));
    array[7] = ((array[7] << 2)) | ((array[7] >> 6));
}

void slayer (uint8_t *X, uint8_t m0, uint8_t m1, uint8_t m2) {
    ///////////
    // S-Box //
    ///////////

    // declare helping variables
    uint8_t T[3] = {0};
    uint8_t temp = 0;
    uint8_t c = (m0 | m1);

    temp = secand(X[7], X[6], m0, m1, m2);  // Step 4
    X[5] = X[5] ^ temp;                     // Step 5

    temp = secand(X[3], X[5], m2, m0, m1);  // Step 6
    X[4] = X[4] ^ temp;                     // Step 7
    X[7] = X[7] ^ X[4];                     // Step 8
    X[6] = X[6] ^ X[3];                     // Step 9

    temp = secor(X[5], X[4], m0, m2, c);    // Step 10
    X[3] = X[3] ^ temp;                     // Step 11
    X[5] = X[5] ^ X[7];                     // Step 12

    temp = secand(X[5], X[6], m2, m0, m1);  // Step 13
    X[4] = X[4] ^ temp;                     // Step 14

    temp = secand(X[1], X[0], m1, m0, m2);  // Step 15
    X[2] = X[2] ^ temp;                     // Step 16

    temp = secor(X[1], X[2], m1, m0, c);    // Step 17
    X[0] = X[0] ^ temp;                     // Step 18

    temp = secor(X[2], X[0], m0, m2, c);    // Step 19
    X[1] = X[1] ^ temp;                     // Step 20
    X[2] = ~X[2];                           // Step 21

    X[7] = X[7] ^ X[1];                     // Step 22
    X[3] = X[3] ^ X[2];                     // Step 23
    X[4] = X[4] ^ X[0];                     // Step 24
    T[0] = X[7];                            // Step 25
    T[1] = X[3];                            // Step 26
    T[2] = X[4];                            // Step 27

    temp = secand(X[5], T[0], m2, m0, m1);  // Step 28
    X[6] = X[6] ^ temp;                     // Step 29

    T[0] = T[0] ^ X[6];                     // Step 30

    temp = secor(T[2], T[1], m1, m2, c);    // Step 31
    X[6] = X[6] ^ temp;                     // Step 32

    T[1] ^= X[5] ^ m0;                      // Step 33

    temp = secor(X[6], T[2], m0, m1, c);    // Step 34
    X[5] = X[5] ^ temp;                     // Step 35

    temp = secand(T[1], T[0], m0, m1, m2);  // Step 36
    T[2] = T[2] ^ temp;                     // Step 37

    X[2] = X[2] ^ T[0];                     // Step 38
    T[0] = X[1] ^ T[2];                     // Step 39
    X[1] = X[0] ^ T[1];                     // Step 40
    X[0] = X[7] ^ m1;                       // Step 41
    X[7] = T[0];                            // Step 42
    T[1] = X[3];                            // Step 43
    X[3] = X[6];                            // Step 44
    X[6] = T[1];                            // Step 45
    T[2] = X[4];                            // Step 46
    X[4] = X[5];                            // Step 47
    X[5] = T[2];                            // Step 48
}

void add_mask (uint8_t *x, const uint8_t *m) {
    x[0] ^= m[1];
    x[1] ^= m[0];
    x[2] ^= m[0];
    x[3] ^= m[0];
    x[4] ^= m[1];
    x[5] ^= m[0];
    x[6] ^= m[0];
    x[7] ^= m[1];
}

void remove_mask (uint8_t *x, uint8_t *m) {
    x[0] ^= m[0];
    x[1] ^= m[1];
    x[2] ^= m[1];
    x[3] ^= m[2];
    x[4] ^= m[0];
    x[5] ^= m[1];
    x[6] ^= m[1];
    x[7] ^= m[0];
}



int main()
{
	///////////////////////////////////////////////////////////////////set seed for rand() and init arrays
	srand(time(NULL));
	uint8_t m[10];
	uint8_t X[8];

    ///////////////////////////////////////////////////////////////////Scope
    #ifdef ENABLE_SCOPE
        //initialize the Scope framework
        Scopetool& Scope=Scopetool::initialize();
        printf("Maxsamples: %ld \n",Scope.getMaxsamples());
        //configure the Channels (always config both to have a defined state)
        //PICO_STATUS setChannel(channel, enabled?, dccoupling?, Voltage Range)
        Scope.setChannel(PS5000_CHANNEL_A, TRUE, TRUE, PS5000_100MV);
        Scope.setChannel(PS5000_CHANNEL_B, TRUE, TRUE, PS5000_2V);

        //request desired sample rate here
        double samplerate = 500 MS;

        //this requests a samplerate and returns the sample rate that is actually configured
        Scope.setSamplerate(&samplerate);

        //set a Trigger
        //void setTrigger(PS5000_CHANNEL channel, short voltagelevel, THRESHOLD_DIRECTION direction, unsigned long delay, short autoTrigger_ms);
        Scope.setTrigger(PS5000_CHANNEL_B, 1000 , RISING, 0, 10000 );

        //Arguemtns: (number of pretrigger samples, number of posttrigger samples)
        Scope.setlength(PreTriggerSamples, PostTriggerSamples);
    #endif


    /////////////////////////////////////////////////////////////////// Init Device
    SFile com;
    com.Open(ComPort, BaudRate);
    com.SetReadTimeOut(10000);

    DWORD writtenCtr, receiveCtr;
	uint8_t ReadBuffer[10];

    /////////////////////////////////////////////////////////////////// FileIO

    //AtfFileSystem fs("./Traces", 1000, 8, 8, PreTriggerSamples+PostTriggerSamples);
    AtfFileSystem fs_fixed("./Traces_fixed", 1000, 8, 8, PreTriggerSamples+PostTriggerSamples);
    AtfFileSystem fs_random("./Traces_random", 1000, 8, 8, PreTriggerSamples+PostTriggerSamples);


    /////////////////////////////////////////////////////////////////Measurement

    for(int TRACENUMBER=0; TRACENUMBER<NumberOfTraces; TRACENUMBER++)
    {
        printf("%5.5d \r",TRACENUMBER);
        int random = rand() % 2;

        if (random == 0) {
            uint8_t * plaintext = fs_fixed.getPlaintextBuffer();
            uint8_t * ciphertext = fs_fixed.getCiphertextBuffer();
            char * traceBuffer = (char *) fs_fixed.getTraceBuffer();

        //**********************CODE HERE***********************************//
        ///////////////////////////////////////////////////////////////////Mask generation
            m[0] = rand() % 256;
            m[1] = rand() % 256;
            m[2] = m[0] ^ m[1];
            m[3] = ((m[1] >> 1) | (m[1] << 7)) ^ m[1];
            m[4] = ((m[2] >> 4) | (m[2] << 4)) ^ m[1];
            m[5] = ((m[0] >> 5) | (m[0] << 3)) ^ m[2];
            m[6] = ((m[1] >> 2) | (m[1] << 6)) ^ m[0];
            m[7] = ((m[1] >> 3) | (m[1] << 5)) ^ m[1];
            m[8] = ((m[2] >> 7) | (m[2] << 1)) ^ m[1];
            m[9] = ((m[1] >> 6) | (m[1] << 2)) ^ m[0];

        //Set Mask of device
            com.WriteByte(0x01);
            com.Write(m, 10, writtenCtr);
            com.Read(ReadBuffer, 10, receiveCtr);

            if (receiveCtr != 10)
            {
                printf("receive counter: %d\n", receiveCtr);
                return -2;

            }
            for (size_t i = 0; i < 10; i++)
            {
                if (ReadBuffer[i] != m[i])
                {
                    return -3;
                }
            }

        // select plaintext here for t test... use rand() function
            plaintext[0] = 0x26;
            plaintext[1] = 0x00;
            plaintext[2] = 0x27;
            plaintext[3] = 0x1E;
            plaintext[4] = 0xF6;
            plaintext[5] = 0x52;
            plaintext[6] = 0x85;
            plaintext[7] = 0x09;


            // Add Mask
            add_mask(plaintext, m);

        //******************************************************************//

            for(int i = 0; i < 8; i++)
            {
                X[i] = plaintext[i];
            }

        //******************************************************************//
            // PIPO cipher until 1. Round out to test results of board

            uint8_t *round_key = (uint8_t *)&k0;
            // XOR masked Whitening Key
            X[0] ^= round_key[0] ^ m[2];
            X[1] ^= round_key[1] ^ m[2];
            X[2] ^= round_key[2] ^ m[2];
            X[3] ^= round_key[3] ^ m[1];
            X[4] ^= round_key[4] ^ m[2];
            X[5] ^= round_key[5] ^ m[2];
            X[6] ^= round_key[6] ^ m[2];
            X[7] ^= round_key[7] ^ m[2];

        // Round 1
            // S-Boxes
            slayer(X, m[0], m[1], m[2]);

            // R-Layer
            rlayer(X);

            // Add Round Key XOR round constant
            round_key = (uint8_t *)&k1;
            X[0] ^= round_key[0] ^ m[1] ^ 1;
            for (int j = 1; j < 8; j++) {
                X[j] ^= round_key[j] ^ m[j + 2];
            }

            // remove mask
            //remove_mask(X, m);

        //******************************************************************//

            #ifdef ENABLE_SCOPE
                //prepare Scope for Measurement
                Scope.arm();
            #endif


            com.WriteByte(0x02);
            com.Write(plaintext, 8, writtenCtr);
            com.Read(ciphertext, 8, receiveCtr);

            // remove mask
            //remove_mask(ciphertext, m);

            for(int i = 0; i < 8; i++)
            {
                if (X[i] != ciphertext[i])
                {
                    printf("\nincorrect ciphertext\n");
                    return -4;
                }
            }

            #ifdef ENABLE_SCOPE
                //read Measurement as soon as it is ready
                Scope.savetrace(PS5000_CHANNEL_A, traceBuffer);
                fs_fixed.nextMeasurement();
            #endif


        } else {
            uint8_t * plaintext = fs_random.getPlaintextBuffer();
            uint8_t * ciphertext = fs_random.getCiphertextBuffer();
            char * traceBuffer = (char *) fs_random.getTraceBuffer();

        //**********************CODE HERE***********************************//
        ///////////////////////////////////////////////////////////////////Mask generation
            m[0] = rand() % 256;
            m[1] = rand() % 256;
            m[2] = m[0] ^ m[1];
            m[3] = ((m[1] >> 1) | (m[1] << 7)) ^ m[1];
            m[4] = ((m[2] >> 4) | (m[2] << 4)) ^ m[1];
            m[5] = ((m[0] >> 5) | (m[0] << 3)) ^ m[2];
            m[6] = ((m[1] >> 2) | (m[1] << 6)) ^ m[0];
            m[7] = ((m[1] >> 3) | (m[1] << 5)) ^ m[1];
            m[8] = ((m[2] >> 7) | (m[2] << 1)) ^ m[1];
            m[9] = ((m[1] >> 6) | (m[1] << 2)) ^ m[0];

        //Set Mask of device
            com.WriteByte(0x01);
            com.Write(m, 10, writtenCtr);
            com.Read(ReadBuffer, 10, receiveCtr);

            if (receiveCtr != 10)
            {
                printf("receive counter: %d\n", receiveCtr);
                return -2;

            }
            for (size_t i = 0; i < 10; i++)
            {
                if (ReadBuffer[i] != m[i])
                {
                    return -3;
                }
            }

        // select plaintext here for t test... use rand() function
            for (int i = 0; i < 8; i++) {
                plaintext[i] = rand() % 256;
            }


            // Add Mask
            add_mask(plaintext, m);

        //******************************************************************//

            for(int i = 0; i < 8; i++)
            {
                X[i] = plaintext[i];
            }

        //******************************************************************//
            // PIPO cipher until 1. Round out to test results of board

            uint8_t *round_key = (uint8_t *)&k0;
            // XOR masked Whitening Key
            X[0] ^= round_key[0] ^ m[2];
            X[1] ^= round_key[1] ^ m[2];
            X[2] ^= round_key[2] ^ m[2];
            X[3] ^= round_key[3] ^ m[1];
            X[4] ^= round_key[4] ^ m[2];
            X[5] ^= round_key[5] ^ m[2];
            X[6] ^= round_key[6] ^ m[2];
            X[7] ^= round_key[7] ^ m[2];

        // Round 1
            // S-Boxes
            slayer(X, m[0], m[1], m[2]);

            // R-Layer
            rlayer(X);

            // Add Round Key XOR round constant
            round_key = (uint8_t *)&k1;
            X[0] ^= round_key[0] ^ m[1] ^ 1;
            for (int j = 1; j < 8; j++) {
                X[j] ^= round_key[j] ^ m[j + 2];
            }

            // remove mask
            //remove_mask(X, m);

        //******************************************************************//

            #ifdef ENABLE_SCOPE
                //prepare Scope for Measurement
                Scope.arm();
            #endif


            com.WriteByte(0x02);
            com.Write(plaintext, 8, writtenCtr);
            com.Read(ciphertext, 8, receiveCtr);

            // remove mask
            //remove_mask(ciphertext, m);

            for(int i = 0; i < 8; i++)
            {
                if (X[i] != ciphertext[i])
                {
                    printf("\nincorrect ciphertext\n");
                    return -4;
                }
            }

            #ifdef ENABLE_SCOPE
                //read Measurement as soon as it is ready
                Scope.savetrace(PS5000_CHANNEL_A, traceBuffer);
                fs_random.nextMeasurement();
            #endif

        }

    }

    #ifdef ENABLE_SCOPE
        Scopetool::destroy();
    #endif

}

