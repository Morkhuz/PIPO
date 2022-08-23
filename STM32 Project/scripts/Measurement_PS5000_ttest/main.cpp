
#include "Scopetool.h"
#include "windows.h"
#include "AtfFileSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include "ComClass.h"
#include <time.h>


const unsigned int      ComPort = 12;
const DWORD			    BaudRate=115200;

const unsigned long int PreTriggerSamples = 0; // configure this
const unsigned long int PostTriggerSamples = 0; // and this
const unsigned long int NumberOfTraces = 10000;

const uint64_t 			k1 = 0x6DC416DD779428D2;
const uint64_t 			k0 = 0x7E1D20AD2E152297;

#define ENABLE_SCOPE


extern void rlayer (uint8_t *X);
extern void slayer (uint8_t *X, uint8_t *m, uint8_t *T, uint8_t c);

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
	///////////////////////////////////////////////////////////////////set seed for rand() and init mask array
	srand(time(NULL));
	uint8_t m[10];

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

    AtfFileSystem fs("./Traces", 1000, 16, 16, PreTriggerSamples+PostTriggerSamples);


    /////////////////////////////////////////////////////////////////Measurement

    for(int TRACENUMBER=0; TRACENUMBER<NumberOfTraces; TRACENUMBER++)
    {
        printf("%5.5d \r",TRACENUMBER);

        uint8_t * plaintext = fs.getPlaintextBuffer();
        uint8_t * ciphertext = fs.getCiphertextBuffer();
        char * traceBuffer = (char *) fs.getTraceBuffer();
        uint8_t X[8];

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
        int random = rand() % 2;

        if (random == 0) {
        	plaintext[0] = 0x26;
        	plaintext[1] = 0x00;
        	plaintext[2] = 0x27;
        	plaintext[3] = 0x1E;
        	plaintext[4] = 0xF6;
        	plaintext[5] = 0x52;
        	plaintext[6] = 0x85;
        	plaintext[7] = 0x09;
        } else {
        	for (int i = 0; i < 8; i++) {
        		plaintext[i] = rand() % 256;
        	}
        }

    //******************************************************************//

        for(int i = 0; i < 8; i++)
        {
        	X[i] = plaintext[i];
        }


	//******************************************************************//
		// PIPO cipher until 1. Round out to test results of board
		// Preparation of Slayer
		uint8_t T[3] = {0};
		uint8_t c = (m[0] | m[1]);

		// Add Mask
		add_mask(X, m);

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
		slayer(X, m, T, c);

		// R-Layer
		rlayer(X);

		// Add Round Key XOR round constant
		round_key = (uint8_t *)&k1;
		X[0] ^= round_key[0] ^ m[1] ^ i;
		for (int j = 1; j < 8; j++) {
			X[j] ^= round_key[j] ^ m[j + 2];
		}

		// remove mask
		remove_mask(X, m);

	//******************************************************************//

        #ifdef ENABLE_SCOPE
            //prepare Scope for Measurement
            Scope.arm();
        #endif


        com.WriteByte(0x02);
        com.Write(plaintext, 8, writtenCtr);
        com.Read(ciphertext, 8, receiveCtr);


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
            fs.nextMeasurement();
        #endif

    }

    #ifdef ENABLE_SCOPE
        Scopetool::destroy();
    #endif

}

