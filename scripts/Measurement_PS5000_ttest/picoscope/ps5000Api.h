/****************************************************************************
 *
 * Filename:    ps5000Api.h
 * Copyright:   Pico Technology Limited 2002 - 2007
 * Author:      MAS
 * Description:
 *
 * This header defines the interface to driver routines for the
 *	PicoScope5000 range of PC Oscilloscopes.
 *
 ****************************************************************************/
#ifndef __PS5000API_H__
#define __PS5000API_H__

#include "picoStatus.h"

#ifdef PREF0
  #undef PREF0
#endif
#ifdef PREF1
  #undef PREF1
#endif
#ifdef PREF2
  #undef PREF2
#endif
#ifdef PREF3
  #undef PREF3
#endif

#ifdef __cplusplus
	#define PREF0 extern "C"
#else
	#define PREF0
#endif

#ifdef WIN32
/*	If you are dynamically linking PS5000.DLL into your project #define DYNLINK here
 */
#ifdef DYNLINK
  #define PREF1 typedef
	#define PREF2
	#define PREF3(x) (__stdcall *x)
#else
  #define PREF1
	#ifdef _USRDLL
		#define PREF2 __declspec(dllexport) __stdcall
	#else
		#define PREF2 __declspec(dllimport) __stdcall
	#endif
  #define PREF3(x) x
#endif
	#define PREF4 __stdcall
#else

	/* Define a 64-bit integer type */
	#include <stdint.h>
	//typedef int64_t __int64;

	#ifdef DYNLINK
		#define PREF1 typedef
		#define PREF2
		#define PREF3(x) (*x)
	#else
		#ifdef _USRDLL
			#define PREF1 __attribute__((visibility("default")))
		#else
			#define PREF1
		#endif
		#define PREF2
		#define PREF3(x) x
	#endif
	#define PREF4
#endif

#define PS5000_MAX_OVERSAMPLE_8BIT 256

/* Although the PS5000 uses an 8-bit ADC, it is usually possible to
 * oversample (collect multiple readings at each time) by up to 256.
 * the results are therefore ALWAYS scaled up to 16-bits, even if
 * oversampling is not used.
 *
 * The maximum and minimum values returned are therefore as follows:
 */
#define PS5000_MAX_VALUE 32512
#define PS5000_MIN_VALUE -32512
#define PS5000_LOST_DATA -32768

#define PS5000_EXT_MAX_VALUE  32767
#define PS5000_EXT_MIN_VALUE -32767

#define MAX_PULSE_WIDTH_QUALIFIER_COUNT 16777215L
#define MAX_DELAY_COUNT 8388607L

#define MAX_SIG_GEN_BUFFER_SIZE 8192
#define MIN_SIG_GEN_BUFFER_SIZE 10
#define MIN_DWELL_COUNT				10
#define MAX_SWEEPS_SHOTS				((1 << 30) - 1)

typedef enum enPS5000Channel
{
	PS5000_CHANNEL_A,
	PS5000_CHANNEL_B,
	PS5000_CHANNEL_C,
	PS5000_CHANNEL_D,
	PS5000_EXTERNAL,
	PS5000_MAX_CHANNELS = PS5000_EXTERNAL,
	PS5000_TRIGGER_AUX,
	PS5000_MAX_TRIGGER_SOURCES
}	PS5000_CHANNEL;

typedef enum enChannelBufferIndex
{
	PS5000_CHANNEL_A_MAX,
	PS5000_CHANNEL_A_MIN,
	PS5000_CHANNEL_B_MAX,
	PS5000_CHANNEL_B_MIN,
	PS5000_CHANNEL_C_MAX,
	PS5000_CHANNEL_C_MIN,
	PS5000_CHANNEL_D_MAX,
	PS5000_CHANNEL_D_MIN,
	PS5000_MAX_CHANNEL_BUFFERS
} PS5000_CHANNEL_BUFFER_INDEX;

typedef enum enPS5000Range
{
	PS5000_10MV,
	PS5000_20MV,
	PS5000_50MV,
	PS5000_100MV,
	PS5000_200MV,
	PS5000_500MV,
	PS5000_1V,
	PS5000_2V,
	PS5000_5V,
	PS5000_10V,
	PS5000_20V,
	PS5000_50V,
	PS5000_MAX_RANGES
}	PS5000_RANGE;


typedef enum enPS5000EtsMode
  {
  PS5000_ETS_OFF,             // ETS disabled
  PS5000_ETS_FAST,            // Return ready as soon as requested no of interleaves is available
  PS5000_ETS_SLOW,            // Return ready every time a new set of no_of_cycles is collected
  PS5000_ETS_MODES_MAX
  }	PS5000_ETS_MODE;

typedef enum enPS5000TimeUnits
  {
  PS5000_FS,
  PS5000_PS,
  PS5000_NS,
  PS5000_US,
  PS5000_MS,
  PS5000_S,
  PS5000_MAX_TIME_UNITS,
  }	PS5000_TIME_UNITS;

typedef enum enSweepType
{
	UP,
	DOWN,
	UPDOWN,
	DOWNUP,
	MAX_SWEEP_TYPES
} SWEEP_TYPE;

typedef enum enWaveType
{
	PS5000_SINE,
	PS5000_SQUARE,
	PS5000_TRIANGLE,
	PS5000_RAMP_UP,
	PS5000_RAMP_DOWN,
	PS5000_SINC,
	PS5000_GAUSSIAN,
	PS5000_HALF_SINE,
	PS5000_DC_VOLTAGE,
	PS5000_WHITE_NOISE,
	MAX_WAVE_TYPES
} WAVE_TYPE;

#define PS5000_SINE_MAX_FREQUENCY				20000000.f
#define PS5000_SQUARE_MAX_FREQUENCY			20000000.f
#define PS5000_TRIANGLE_MAX_FREQUENCY		20000000.f
#define PS5000_SINC_MAX_FREQUENCY				20000000.f
#define PS5000_RAMP_MAX_FREQUENCY				20000000.f
#define PS5000_HALF_SINE_MAX_FREQUENCY	20000000.f
#define PS5000_GAUSSIAN_MAX_FREQUENCY		20000000.f
#define PS5000_MIN_FREQUENCY									 0.03f

typedef enum enSigGenTrigType
{
	SIGGEN_RISING,
	SIGGEN_FALLING,
	SIGGEN_GATE_HIGH,
	SIGGEN_GATE_LOW
} SIGGEN_TRIG_TYPE;

typedef enum enSigGenTrigSource
{
	SIGGEN_NONE,
	SIGGEN_SCOPE_TRIG,
	SIGGEN_AUX_IN,
	SIGGEN_EXT_IN,
	SIGGEN_SOFT_TRIG
} SIGGEN_TRIG_SOURCE;

typedef enum enIndexMode
{
	SINGLE,
	DUAL,
	QUAD,
	MAX_INDEX_MODES
} INDEX_MODE;

typedef enum enThresholdMode
{
	LEVEL,
	WINDOW
} THRESHOLD_MODE;

typedef enum enThresholdDirection
{
	ABOVE,
	BELOW,
	RISING,
	FALLING,
	RISING_OR_FALLING,
	INSIDE = ABOVE,
	OUTSIDE = BELOW,
	ENTER = RISING,
	EXIT = FALLING,
	ENTER_OR_EXIT = RISING_OR_FALLING,
  NONE = RISING
} THRESHOLD_DIRECTION;

typedef enum enTriggerState
{
  CONDITION_DONT_CARE,
  CONDITION_TRUE,
  CONDITION_FALSE,
	CONDITION_MAX
} TRIGGER_STATE;

#pragma pack(1)
typedef struct tTriggerConditions
{
  TRIGGER_STATE channelA;
  TRIGGER_STATE channelB;
  TRIGGER_STATE channelC;
  TRIGGER_STATE channelD;
  TRIGGER_STATE external;
  TRIGGER_STATE aux;
	TRIGGER_STATE pulseWidthQualifier;
} TRIGGER_CONDITIONS;
#pragma pack()

#pragma pack(1)
typedef struct tPwqConditions
{
  TRIGGER_STATE channelA;
  TRIGGER_STATE channelB;
  TRIGGER_STATE channelC;
  TRIGGER_STATE channelD;
  TRIGGER_STATE external;
  TRIGGER_STATE aux;
} PWQ_CONDITIONS;
#pragma pack()


#pragma pack(1)
typedef struct tTriggerChannelProperties
{
  short thresholdMajor;
  short thresholdMinor;
  unsigned short hysteresis;
	PS5000_CHANNEL channel;
  THRESHOLD_MODE thresholdMode;
} TRIGGER_CHANNEL_PROPERTIES;
#pragma pack()

typedef enum enRatioMode
{
	RATIO_MODE_NONE,
	RATIO_MODE_AGGREGATE
} RATIO_MODE;

typedef enum enPulseWidthType
{
	PW_TYPE_NONE,
  PW_TYPE_LESS_THAN,
	PW_TYPE_GREATER_THAN,
	PW_TYPE_IN_RANGE,
	PW_TYPE_OUT_OF_RANGE
} PULSE_WIDTH_TYPE;

typedef void (PREF4 *ps5000BlockReady)
	(
		short											handle,
		PICO_STATUS								status,
		void										*	pParameter
	);

typedef void (PREF4 *ps5000StreamingReady)
	(
		short    									handle,
		long     									noOfSamples,
		unsigned long							startIndex,
		short    									overflow,
		unsigned long							triggerAt,
		short    									triggered,
		short    									autoStop,
		void										*	pParameter
	);

typedef void (PREF4 *ps5000DataReady)
	(
		short    									handle,
		long     									noOfSamples,
		short    									overflow,
		unsigned long							triggerAt,
		short    									triggered,
		void										*	pParameter
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000OpenUnit)
  (
	  short											* handle
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000OpenUnitAsync)
  (
	  short											* status
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000OpenUnitProgress)
	(
	  short 										* handle,
	  short 										* progressPercent,
	  short 										* complete
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetUnitInfo)
 	(
	  short     								  handle,
	  char      								* string,
	  short     								  stringLength,
	  short     								* requiredSize,
	  PICO_INFO 								  info
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000FlashLed)
	(
	  short 											handle,
		short 											start
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000IsLedFlashing)
	(
	  short 										  handle,
		short 										* status
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000CloseUnit)
	(
	  short												handle
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000MemorySegments)
	(
	  short												handle,
		unsigned short							nSegments,
		long											* nMaxSamples
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetChannel)
 	(
	  short												handle,
		PS5000_CHANNEL							channel,
	  short												enabled,
	  short												dc,
		PS5000_RANGE								range
	);


PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetTimebase)
	(
	   short											handle,
	   unsigned long							timebase,
	   long												noSamples,
	   long											* timeIntervalNanoseconds,
	   short											oversample,
		 long											* maxSamples,
		 unsigned short							segmentIndex
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetTimebase2)
	(
	   short											handle,
	   unsigned long							timebase,
	   long												noSamples,
	   float										* timeIntervalNanoseconds,
	   short											oversample,
		 long											* maxSamples,
		 unsigned short							segmentIndex
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetSigGenArbitrary)
	(
	 	short												handle,
	 	long												offsetVoltage,
	 	unsigned long								pkToPk,
	 	unsigned long								startDeltaPhase,
	 	unsigned long								stopDeltaPhase,
	 	unsigned long								deltaPhaseIncrement,
	 	unsigned long								dwellCount,
	 	short											*	arbitraryWaveform,
	 	long												arbitraryWaveformSize,
		SWEEP_TYPE									sweepType,
		short												whiteNoise,
		INDEX_MODE									indexMode,
		unsigned long								shots,
		unsigned long								sweeps,
		SIGGEN_TRIG_TYPE						triggerType,
		SIGGEN_TRIG_SOURCE					triggerSource,
		short												extInThreshold
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3(ps5000SetSigGenBuiltIn)
	(
		short												handle,
		long												offsetVoltage,
		unsigned long								pkToPk,
		short												waveType,
		float												startFrequency,
		float												stopFrequency,
		float												increment,
		float												dwellTime,
		SWEEP_TYPE									sweepType,
		short												whiteNoise,
		unsigned long								shots,
		unsigned long								sweeps,
		SIGGEN_TRIG_TYPE						triggerType,
		SIGGEN_TRIG_SOURCE					triggerSource,
		short												extInThreshold
		);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SigGenSoftwareControl)
	(
		short												handle,
		short												state
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetEts)
  (
		short												handle,
		PS5000_ETS_MODE							mode,
		short												etsCycles,
		short												etsInterleave,
		long											* sampleTimePicoseconds
	);


PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetTriggerChannelProperties)
	(
		short																handle,
		TRIGGER_CHANNEL_PROPERTIES *				channelProperties,
		short																nChannelProperties,
		short																auxOutputEnable,
		long																autoTriggerMilliseconds
	);


PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetTriggerChannelConditions)
	(
		short												handle,
		TRIGGER_CONDITIONS	*	conditions,
		short												nConditions
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetTriggerChannelDirections)
	(
		short									handle,
		THRESHOLD_DIRECTION		channelA,
		THRESHOLD_DIRECTION		channelB,
		THRESHOLD_DIRECTION		channelC,
		THRESHOLD_DIRECTION		channelD,
		THRESHOLD_DIRECTION		ext,
		THRESHOLD_DIRECTION		aux
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetSimpleTrigger)
	(
		short handle,
		short enable,
		PS5000_CHANNEL source,
		short threshold,
		THRESHOLD_DIRECTION direction,
		unsigned long delay,
		short autoTrigger_ms
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetTriggerDelay)
	(
		short									handle,
		unsigned long					delay
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetPulseWidthQualifier)
	(
		short											handle,
		PWQ_CONDITIONS					*	conditions,
		short											nConditions,
		THRESHOLD_DIRECTION				direction,
		unsigned long							lower,
		unsigned long							upper,
		PULSE_WIDTH_TYPE					type
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000IsTriggerOrPulseWidthQualifierEnabled)
	(
		short 								handle,
		short 							* triggerEnabled,
		short 							* pulseWidthQualifierEnabled
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetTriggerTimeOffset)
	(
		short									handle,
		unsigned long 			* timeUpper,
		unsigned long 			* timeLower,
		PS5000_TIME_UNITS		*	timeUnits,
		unsigned short				segmentIndex
	);


PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetTriggerTimeOffset64)
	(
		short									handle,
		__int64							* time,
		PS5000_TIME_UNITS		*	timeUnits,
		unsigned short				segmentIndex
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetValuesTriggerTimeOffsetBulk)
	(
	  short								handle,
		unsigned long			*	timesUpper,
		unsigned long			* timesLower,
		PS5000_TIME_UNITS	*	timeUnits,
		unsigned short			fromSegmentIndex,
		unsigned short			toSegmentIndex
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetValuesTriggerTimeOffsetBulk64)
	(
	  short								handle,
		__int64			*	times,
		PS5000_TIME_UNITS	*	timeUnits,
		unsigned short			fromSegmentIndex,
		unsigned short			toSegmentIndex
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetDataBuffers)
(
   short									handle,
	 PS5000_CHANNEL 			  channel,
	 short								* bufferMax,
	 short								* bufferMin,
   long										bufferLth
);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetDataBuffer)
(
   short									handle,
	 PS5000_CHANNEL 			  channel,
	 short								* buffer,
   long										bufferLth
);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetDataBufferBulk)
(
   short									handle,
	 PS5000_CHANNEL 			  channel,
	 short							*		buffer,
   long										bufferLth,
	 unsigned short					waveform
);


PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetEtsTimeBuffer)
(
   short									handle,
	 __int64 *							buffer,
	 long										bufferLth
);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetEtsTimeBuffers)
(
   short									handle,
	 unsigned long				* timeUpper,
	 unsigned long				* timeLower,
	 long										bufferLth
);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000IsReady)
	(
		short handle,
		short * ready
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000RunBlock)
	(
		short									handle,
		long									noOfPreTriggerSamples,
		long									noOfPostTriggerSamples,
		unsigned long					timebase,
		short									oversample,
		long								* timeIndisposedMs,
		unsigned short				segmentIndex,
		ps5000BlockReady			lpReady,
		void								* pParameter
	);


PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000RunStreaming)
  (
	  short									handle,
		unsigned long				* sampleInterval,
		PS5000_TIME_UNITS			sampleIntervalTimeUnits,
	  unsigned long					maxPreTriggerSamples,
	  unsigned long					maxPostPreTriggerSamples,
		short									autoStop,
		unsigned long					downSampleRatio,
    unsigned long					overviewBufferSize
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetStreamingLatestValues)
  (
    short									handle,
    ps5000StreamingReady	lpPs5000Ready,
		void								* pParameter
  );

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000NoOfStreamingValues)
	(
	  short								handle,
		unsigned long			*	noOfValues
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetMaxDownSampleRatio)
	(
	  short								handle,
		unsigned long 			noOfUnaggreatedSamples,
		unsigned long 		* maxDownSampleRatio,
		short								downSampleRatioMode,
		unsigned short			segmentIndex
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetValues)
	(
	  short								handle,
		unsigned long 			startIndex,
	  unsigned long			*	noOfSamples,
	  unsigned long				downSampleRatio,
		short								downSampleRatioMode,
		unsigned short			segmentIndex,
		short							* overflow
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetValuesBulk)
	(
	  short								handle,
		unsigned long 	 	*	noOfSamples,
		unsigned short			fromSegmentIndex,
		unsigned short			toSegmentIndex,
		short							* overflow
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000GetValuesAsync)
	(
	  short								handle,
		unsigned long				startIndex,
	  unsigned long				noOfSamples,
	  unsigned long				downSampleRatio,
		short								downSampleRatioMode,
		unsigned short			segmentIndex,
	  void							*	lpDataReady,
		void							*	pParameter
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000Stop)
	(
	  short handle
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000PingUnit)
	(
	  short handle
	);

PREF0 PREF1 PICO_STATUS PREF2 PREF3 (ps5000SetNoOfCaptures) (short handle, unsigned short nCaptures);

#endif
