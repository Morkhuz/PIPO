/**
* @file picoscope.hpp
* @brief This file contains all the easy2use functions for the scope.
*
*/


#ifndef _PICOSCOPE_H
#define _PICOSCOPE_H

// includes
#include <inttypes.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "ps5000Api.h"
#include "picoErrors.h"
// defines
#define MAX_CHANNELS 4
#define QUAD_SCOPE 4
#define DUAL_SCOPE 2
#define SEGMEM		10

#define mv2adc(mv,ch) ((mv * PS5000_MAX_VALUE)/inputRanges[ch])

//unsigned short inputRanges [PS5000_MAX_RANGES] = { 10,20,50,100,200,500,1000,2000,5000,10000,20000,50000 };

typedef enum {BOOL_FALSE = 0, BOOL_TRUE = 1} bool_t;

typedef struct tTriggerDirections{
	enum enThresholdDirection channelA;
	enum enThresholdDirection channelB;
	enum enThresholdDirection channelC;
	enum enThresholdDirection channelD;
	enum enThresholdDirection ext;
	enum enThresholdDirection aux;
} trigger_directions_t;


typedef struct tPwqSettings{
	struct tPwqConditions * conditions;
	short nConditions;
	enum enThresholdDirection direction;
	unsigned long lower;
	unsigned long upper;
	enum enPulseWidthType type;
} pwq_t;

typedef enum{
	MODEL_NONE = 0,
	MODEL_PS5203 = 5203,
	MODEL_PS5204 = 5204
} MODEL_TYPE;

typedef struct{
	short DCcoupled;
	short range;
	short enabled;
}CHANNEL_SETTINGS;

typedef struct{
	short handle;
	MODEL_TYPE model;
	PS5000_RANGE firstRange;
	PS5000_RANGE lastRange;
	unsigned char signalGenerator;
	unsigned char external;
	short ChannelCount;
	CHANNEL_SETTINGS channelSettings [MAX_CHANNELS];
	PS5000_RANGE triggerRange;
}UNIT_MODEL;

typedef struct tPwq{
	struct tPwqConditions * conditions;
	short nConditions;
	enum enThresholdDirection direction;
	unsigned long lower;
	unsigned long upper;
	enum enPulseWidthType type;
}PWQ;

/**
*@brief This function connects the PC to the picoscope.
*The function checks if there already is a connection to the
*picoscope. If not it configures the memory for the storage of
*the properties of the trigger channel and opens a connection
*to the picoscope.
*@param h will contain the handle of the picoscope connected.
*@param connected will be set true if connected and false if not.
*/
bool_t picoscope_connect();


bool_t picoscope_disconnect();


void get_info(UNIT_MODEL * unit);
bool_t picoscope_configure_basic(const short channelCount, const long sampleCount,const short oversample);
bool_t picoscope_configure_channel(const PS5000_CHANNEL ch, const short enable,
	const short dc_coupling, const unsigned int range_mv, const short trigger_enabled,
	const double hysteresis, const double thresholdMinor, const double thresholdMajor,
	const enum enThresholdMode thresholdMode);

/**
* @brief Function for configuring the trigger.
* This function takes a channel and the fitting parameters
* and configures the channel.
*/
bool_t picoscope_configure_trigger(struct tTriggerConditions* conditions,
	struct tTriggerDirections* directions, const double trigger_delay,
	const long auto_trigger_ms, const double pretrig_samples);


bool_t picoscope_configure_pwq(const short enable, struct tPwqConditions* cond_pwq,
	const enum enThresholdDirection direction, const double lower,
	const double upper, const enum enPulseWidthType type);
/**
* @brief This turns to true if the callback wrapper tells the
* program that there is data to fetch
*/
bool_t picoscope_has_data();

/**
* @brief Starts the picoscope and makes it ready for measurement.
*/
bool_t picoscope_arm();

/**
* @brief Stops the scope. From now on it wont react on any trigger.
*/
bool_t picoscope_disarm();

void picoscope_fetchData(const short channel, double* out, const unsigned long max_size);
const uint8_t* picoscope_fetchRawData(const short channel);

/**
* @brief Turns Integervalue in Milivolt into Picoscope Range Parameter
*/
PS5000_RANGE picoscope_get_range_from_mv(const unsigned int r);

long picoscope_get_timebase(const double rate);

unsigned int picoscope_getRawDataWidth();

unsigned int picoscope_get_mv_from_range(const PS5000_RANGE r);

double picoscope_get_sample_rate(const long timebase);

#endif
