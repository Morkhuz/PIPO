#include "Scopetool.h"
#include <ps5000Api.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> //log for timebases
#include <algorithm> //min for timebases
#include "windows.h" //sleep for savetraces
#include <string>




Scopetool* Scopetool::theScope = 0;
bool Scopetool::ready=false;

Scopetool::Scopetool()
{
    maxsamples = 0;
    samplerate = 250 MS;
    pretriggersamples=0;
    posttriggersamples=0;
    numofsamples=pretriggersamples+posttriggersamples;
    ready=false;
    status = PICO_OK;
    status = ps5000OpenUnit(&(PicoHardware.handle));
    if (status != PICO_OK)
    {
        printf("Unable to open device\n");
        printf("Error code : %d\n", (int) status);
        print_PicoError(status);
        exit(-1);
    }
    else
        printf("Scope opened successfully.\n");

    getDeviceInfo();

    buffer[0] = new short[PicoHardware.Memory];
    buffer[1] = new short[PicoHardware.Memory];
    for (unsigned int i=0; i<PicoHardware.Memory;i++)
    {
        buffer[0][i]=0;
        buffer[1][i]=0;
    }

    printf("Initializing default setup:\n");
    status = ps5000SetEts(PicoHardware.handle, PS5000_ETS_OFF, 0, 0, NULL);
    if (status != PICO_OK)
    {
        printf("Unable to disable ETS\n");
        printf("Error code : %d\n", (int) status);
        print_PicoError(status);
        exit(-1);
    }
    else
        printf("ETS disabled successfully.\n");

    setChannel(PS5000_CHANNEL_A, TRUE, FALSE, PS5000_5V);
    setChannel(PS5000_CHANNEL_B, TRUE, FALSE, PS5000_5V);
    ps5000SetDataBuffer(PicoHardware.handle, PS5000_CHANNEL_A, buffer[0], PicoHardware.Memory);
    ps5000SetDataBuffer(PicoHardware.handle, PS5000_CHANNEL_B, buffer[1], PicoHardware.Memory);
    setSamplerate(&samplerate);
    disableTrigger();
    printf("Initialization complete.\n\n");


}


Scopetool::Scopetool(const Scopetool&)
{
// deny copy constructor
}


Scopetool::~Scopetool()
{
    status = ps5000Stop(PicoHardware.handle);
    if (status != PICO_OK)
    {
        printf("Unable to stop device\n");
        printf("Error code : %d\n", (int) status);
        print_PicoError(status);
        exit(-1);
    }
    else
        status= ps5000CloseUnit(PicoHardware.handle);
    if (status != PICO_OK)
    {
        printf("Unable to close device\n");
        printf("Error code : %d\n", (int) status);
        print_PicoError(status);
        exit(-1);
    }
    else
        printf("\nScope closed successfully.\n");

}

void Scopetool::getDeviceInfo()
{
    char description [7][25]= { "Driver Version",
                                "USB Version",
                                "Hardware Version",
                                "Variant Info",
                                "Serial",
                                "Calibration date",
                                "Kernel Driver Version"
                              };
    short itemnumber, lengthofreply = 0;
    char buffer [80];
    int variant=0;

    if (PicoHardware.handle)
    {
        for (itemnumber = 0; itemnumber < 7; itemnumber++)
        {
            status = ps5000GetUnitInfo(PicoHardware.handle, buffer, sizeof (buffer), &lengthofreply, itemnumber);
            if (itemnumber == 3)
            {
                variant = atoi(buffer);
            }

            printf("%s: %s\n", description[itemnumber], buffer);
        }

        switch (variant)
        {
        case MODEL_PS5203:
            PicoHardware.model = MODEL_PS5203;
            PicoHardware.external = TRUE;
            PicoHardware.signalGenerator = TRUE;
            PicoHardware.firstRange = PS5000_100MV;
            PicoHardware.lastRange = PS5000_20V;
            PicoHardware.ChannelCount = DUAL_SCOPE;
            PicoHardware.Memory = 32 * 1024 *1024;
            break;
        case MODEL_PS5204:
            PicoHardware.model = MODEL_PS5204;
            PicoHardware.external = TRUE;
            PicoHardware.signalGenerator = TRUE;
            PicoHardware.firstRange = PS5000_100MV;
            PicoHardware.lastRange = PS5000_20V;
            PicoHardware.ChannelCount = DUAL_SCOPE;
            PicoHardware.Memory = 123 * 1024 *1024;
            break;
        default:
            break;
        }

        if (status != PICO_OK)
        {
            printf("Cannot Read Hardware Info.\n");
            printf("Error code : %d\n", (int) status);
            print_PicoError(status);
            exit(-1);
        }
        else
            printf("Finished reading hardware info.\n");

    }
    else
    {
        printf("No Scope found.\n");
        print_PicoError(status);
        exit(-1);
    }
}

void Scopetool::setChannel(PS5000_CHANNEL channel, short enabled, short dc, PS5000_RANGE range)
{
    status = ps5000SetChannel(PicoHardware.handle, channel, enabled, dc, range);
    if (status != PICO_OK)
    {
        printf("Unable to set channel\n");
        printf("Error code : %d\n", (int) status);
        print_PicoError(status);
        exit(-1);
    }
    else
        printf("Channel %d configured successfully.\n", channel);
    PicoHardware.channelSettings[channel].DCcoupled=dc;
    PicoHardware.channelSettings[channel].range=range;
    PicoHardware.channelSettings[channel].enabled=enabled;
    ps5000GetTimebase(PicoHardware.handle, 25, 1, NULL, 0, &maxsamples,0 );
    status = PICO_OK;
}

void Scopetool::setSamplerate(double *samplerate)
{
    if (*samplerate >= 1 MS)
        printf("Setting sample rate... requested: %4.2f MS/s\n", *samplerate/1E6);
    else if(*samplerate >= 1 kS)
        printf("Setting sample rate... requested: %4.2f kS/s\n", *samplerate/1E3);
    else
        printf("Setting sample rate... requested: %4.4f Samples/s\n", *samplerate);

    if (*samplerate>1 GS) *samplerate=1 GS;
    if ((*samplerate>5E8)&&(PicoHardware.channelSettings[0].enabled!=0)&&(PicoHardware.channelSettings[1].enabled!=0))
    {
        *samplerate=5E8;
        printf("With both channels enabled the sample rate is limited to %4.2f MS/s.\n", *samplerate/1E6);
    }

    const double intervall=1/ *samplerate;

    if (intervall<8E-9)
        timebase=floor(log2(intervall*1E9));
    else
        timebase=min(floor((intervall*125E6)+2), pow(2,32)-1);

    if (timebase<3)
        *samplerate = 1E9/pow(2,timebase);
    else
        *samplerate = 125E6/(timebase-2);

    if (*samplerate >= 1 MS)
        printf("Trying to configure using  %4.2f MS/s.\n", *samplerate/1E6);
    else if(*samplerate >= 1 kS)
        printf("Trying to configure using  %4.2f kS/s.\n", *samplerate/1E3);
    else
        printf("Trying to configure using  %4.4f Samples/s.\n", *samplerate);

    status = ps5000GetTimebase(PicoHardware.handle, timebase, 1, NULL, 0, &maxsamples,0 );
    if (status != PICO_OK)
    {
        printf("Unable to set Samplerate\n");
        printf("Error code : %d\n", (int) status);
        print_PicoError(status);
        exit(-1);
    }
    else
    {
        if (*samplerate >= 1 MS)
            printf("Samplerate %4.2f MS/s configured successfully.\n", *samplerate/1E6);
        else if(*samplerate >= 1 kS)
            printf("Samplerate %4.2f kS/s configured successfully.\n", *samplerate/1E3);
        else
            printf("Samplerate %4.4f Samples/s configured successfully.\n", *samplerate);
    }
}

long Scopetool::getMaxsamples()
{
    return maxsamples;
}

void Scopetool::setTrigger(PS5000_CHANNEL channel, short voltagelevel, THRESHOLD_DIRECTION direction, unsigned long delay, short autoTrigger_ms)
{
    unsigned short inputRanges [12] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000, 20000, 50000};
    double adclvl=voltagelevel*32512/inputRanges[PicoHardware.channelSettings[channel].range];

    status = ps5000SetSimpleTrigger(PicoHardware.handle, 1,  channel, (int)adclvl  , direction, delay, autoTrigger_ms);
    if (status != PICO_OK)
        {
            printf("Cannot Set Trigger.\n");
            printf("Error code : %d\n", (int) status);
            print_PicoError(status);
            exit(-1);
        }
        else
            printf("Trigger configured successfully.\n");
}

void Scopetool::disableTrigger()
{
 status = ps5000SetSimpleTrigger(PicoHardware.handle, 0,  PS5000_CHANNEL_B, 0, RISING_OR_FALLING, 0, 0);
    if (status != PICO_OK)
        {
            printf("Cannot disable Trigger.\n");
            printf("Error code : %d\n", (int) status);
            print_PicoError(status);
            exit(-1);
        }
        else
            printf("Trigger disabled successfully.\n");

}

void __stdcall Scopetool::CallBackBlock(short handle, PICO_STATUS status, void * Parameter)
{
    if (status != PICO_OK)
        {
            printf("CallBackBlock catched an error.\n");
            printf("Error code : %d\n", (int) status);
            print_PicoError(status);
            exit(-1);
        }
    //printf("CallBackCalled.\n");
    ready=true;

}


void Scopetool::arm()
{
    Scopetool::ready=false;
    status=ps5000RunBlock(PicoHardware.handle, pretriggersamples, posttriggersamples, timebase, 0, NULL, 0, Scopetool::CallBackBlock, NULL);
    if (status != PICO_OK)
        {
            printf("Error arming the Scope.\n");
            printf("Error code : %d\n", (int) status);
            print_PicoError(status);
            exit(-1);
        }
    //printf("Armed successfully.\n");
}

void Scopetool::savetrace(PS5000_CHANNEL channel, char * destination)
{
    while(!ready){Sleep(0);};

    status=ps5000GetValues(PicoHardware.handle, 0, &numofsamples,0, RATIO_MODE_NONE, 0, &overflow );

    if (status != PICO_OK)
        {
            printf("Error reading data from scope.\n");
            printf("Error code : %d\n", (int) status);
            print_PicoError(status);
            exit(-1);
        }

    //FILE *  F;
    //F=fopen("test.txt", "w");
    //printf("Samples successfully read: %ld\n", numofsamples);
    for (int i=0;i<pretriggersamples+posttriggersamples;i++)
    {
        destination[i]= buffer[channel][i]>>8;
        //fprintf(F, "%5d, %5d \n", destination[i], buffer[channel][i]);
        //printf("%2X %5d, %8X %5d \n", destination[i],destination[i], buffer[channel][i],buffer[channel][i]>>8);
    }
    // fclose(F);
    //printf("Converted.\n");

}

void Scopetool::setlength(long pretriggersamples, long posttriggersamples)
{
    if (Scopetool::maxsamples>=pretriggersamples+posttriggersamples)
    {
        Scopetool::pretriggersamples = pretriggersamples;
        Scopetool::posttriggersamples = posttriggersamples;
    }
    else if (Scopetool::maxsamples>=posttriggersamples)
    {
        Scopetool::pretriggersamples = Scopetool::maxsamples - posttriggersamples;
        Scopetool::posttriggersamples = posttriggersamples;
        printf("Adjusted Samples to:\n  Pre-Trigger: %ld\n  Post-Trigger: %ld\n", Scopetool::pretriggersamples, Scopetool::posttriggersamples);
    }
    else if (Scopetool::maxsamples>=pretriggersamples)
    {
        Scopetool::posttriggersamples = Scopetool::maxsamples - pretriggersamples;
        Scopetool::pretriggersamples = pretriggersamples;
        printf("Adjusted Samples to:\n  Pre-Trigger: %ld\n  Post-Trigger: %ld\n", Scopetool::pretriggersamples, Scopetool::posttriggersamples);
    }
    else
    {
        Scopetool::pretriggersamples = 0;
        Scopetool::posttriggersamples = Scopetool::maxsamples;
        printf("Adjusted Samples to:\n  Pre-Trigger: %ld\n  Post-Trigger: %ld\n", Scopetool::pretriggersamples, Scopetool::posttriggersamples);
    }
    numofsamples=pretriggersamples+posttriggersamples;
}

Scopetool& Scopetool::initialize()
{
    if ( !theScope )
    {
        theScope = new Scopetool();
    }
    return *theScope;
}


void Scopetool::destroy()
{
    if ( theScope )
    {
        delete theScope;
    }
    theScope = 0;
}



