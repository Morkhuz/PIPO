#include <stdio.h> //printf
extern "C" {
 #include <ps5000Api.h>
}


#ifndef Scopetool_H
#define Scopetool_H

#define TRUE true
#define FALSE false
#define QUAD_SCOPE 4
#define DUAL_SCOPE 2
#define kS *1E3
#define MS *1E6
#define GS *1E9
#define V
#define mV *1E-3


typedef struct
{
    short DCcoupled;
    PS5000_RANGE range;
    short enabled;
} CHANNEL_SETTINGS;

typedef enum
{
    MODEL_NONE = 0,
    MODEL_PS5203 = 5203,
    MODEL_PS5204 = 5204
} MODEL_TYPE;


typedef struct
{
    short handle;
    MODEL_TYPE model;
    PS5000_RANGE firstRange;
    PS5000_RANGE lastRange;
    unsigned char signalGenerator;
    unsigned char external;
    short ChannelCount;
    CHANNEL_SETTINGS channelSettings [2];
    PS5000_RANGE triggerRange;
    unsigned long Memory;
} UNIT_MODEL;


inline double min(double a, double b)
{
  return (a < b) ? a : b;
}

inline double max(double a, double b)
{
  return (a > b) ? a : b;
}

class Scopetool
{
private:
    static Scopetool* theScope;
    //friend void CallBackGlobal(short handle, PICO_STATUS status, void * Parameter);
    friend PICO_STATUS ps5000RunBlock(short handle, PICO_STATUS status, void * Parameter);
    static void __stdcall CallBackBlock(short handle, PICO_STATUS status, void * Parameter);
    UNIT_MODEL PicoHardware;
    PICO_STATUS status;
    short overflow;
    unsigned long numofsamples;
    static bool ready;
    long maxsamples;
    long posttriggersamples;
    long pretriggersamples;
    short *buffer[2];
    double samplerate;
    unsigned long timebase;
    void getDeviceInfo();
    Scopetool();
    Scopetool(const Scopetool&);
    ~Scopetool();
public:

    static Scopetool& initialize();
    static void destroy();
    void setChannel(PS5000_CHANNEL channel, short enabled, short dc, PS5000_RANGE range);
    void setSamplerate(double *samplerate);
    long getMaxsamples();
    void setTrigger(PS5000_CHANNEL channel, short voltagelevel, THRESHOLD_DIRECTION direction, unsigned long delay, short autoTrigger_ms);
    void disableTrigger();
    void arm();
    void savetrace(PS5000_CHANNEL channel, char * destination);
    void setlength(long pretriggersamples, long posttriggersamples);
};


void CallBackGlobal(short handle, PICO_STATUS status, void * Parameter);
void Testarm();




#endif // Scopetool_H
