#include "picoscope.h"

static bool_t connected = BOOL_FALSE;
static bool_t configured = BOOL_FALSE;
static short h = 0;
static bool_t data_available = BOOL_FALSE;
static int16_t *buffer_a = 0;
static int16_t *buffer_b = 0;
static long timebase = 0;
static double sample_rate = 0;
static unsigned long pretrigger_samples = 0;
static unsigned long block_sizeA = 0;
static unsigned long block_sizeB = 0;
static unsigned long block_sizeAll = 0;
static double channel_adc_to_mv[2] = {0, 0};

static struct tTriggerChannelProperties source_details[3];
static unsigned int trigger_channels_configured = 0;


/****************************************************************************
 * Initialise unit' structure with Variant specific defaults
 ****************************************************************************/
void get_info(UNIT_MODEL * unit){
	char description [6][25]= { "Driver Version",
			                    "USB Version",
			                    "Hardware Version",
			                    "Variant Info",
			                    "Serial",
			                    "Error Code" };
	short i, r = 0;
	char line [80];
	int variant;

	if (unit->handle) {
		for (i = 0; i < 5; i++) {
			ps5000GetUnitInfo(unit->handle, line, sizeof (line), &r, i);
			if (i == 3) {
				variant = atoi(line);
			}
			printf("%s: %s\n", description[i], line);
		}

		switch (variant) {
			case MODEL_PS5203:
				unit->model = MODEL_PS5203;
				unit->external = BOOL_TRUE;
				unit->signalGenerator = BOOL_TRUE;
				unit->firstRange = PS5000_100MV;
				unit->lastRange = PS5000_20V;
				unit->ChannelCount = DUAL_SCOPE;
				break;
			case MODEL_PS5204:
				unit->model = MODEL_PS5204;
				unit->external = BOOL_TRUE;
				unit->signalGenerator = BOOL_TRUE;
				unit->firstRange = PS5000_100MV;
				unit->lastRange = PS5000_20V;
				unit->ChannelCount = DUAL_SCOPE;
				break;
			default:
				break;
		}
	}
}

bool_t picoscope_connect()

{
	if(connected) {
		printf("picoscope_connect(): Already connected\n");
		return BOOL_FALSE;
	}

	short handle = 0;
	PICO_STATUS status = ps5000OpenUnit(&handle);

	if (status != PICO_OK) {
		printf("picoscope_connect(): Error code: %d\n", (int)status);
		printf("picoscope_connect():");print_PicoError(status);
		return BOOL_FALSE;
	}

	h = handle;
	connected = BOOL_TRUE;
	data_available = BOOL_FALSE;

	memset(source_details, 0, sizeof(struct tTriggerChannelProperties) * 3);
	trigger_channels_configured = 0;


	return BOOL_TRUE;
}

bool_t picoscope_disconnect()
{
	if(!connected) {
		printf("picoscope_disconnect(): Not connected\n");
		return BOOL_FALSE;
	}

	picoscope_disarm();

	if(configured) {
		free(buffer_a);
		buffer_a = 0;
		free(buffer_b);
		buffer_b = 0;
	}

	ps5000CloseUnit(h);

	h = 0;
	connected = BOOL_FALSE;
	configured = BOOL_FALSE;
	data_available = BOOL_FALSE;

	return BOOL_TRUE;
}

bool_t picoscope_configure_channel(const PS5000_CHANNEL ch, const short enable,
	const short dc_coupling, const unsigned int range_mv, const short trigger_enabled,
	const double hysteresis, const double thresholdMinor, const double thresholdMajor,
	const enum enThresholdMode thresholdMode)

{
	if(enable)
	{
		// enable and setup channel
		ps5000SetChannel(h, ch, 1, dc_coupling, picoscope_get_range_from_mv(range_mv));

		printf("Channel%d: Range: %d, coupling %d\n", ch, picoscope_get_range_from_mv(range_mv), dc_coupling);

		const double adc_to_mv = (double)(picoscope_get_mv_from_range(picoscope_get_range_from_mv(range_mv))) / PS5000_MAX_VALUE;

		printf("ADC->mV: %f\n", adc_to_mv);

		// store channel range
		channel_adc_to_mv[(unsigned int)ch] = adc_to_mv;

		// configure trigger channel settings
		if(trigger_enabled) {
			printf("Trigger enabled\n");

			source_details[trigger_channels_configured].channel = ch;

			source_details[trigger_channels_configured].hysteresis =
				(unsigned int)(hysteresis/adc_to_mv);

			source_details[trigger_channels_configured].thresholdMinor =
				(unsigned int)(thresholdMinor/adc_to_mv);

			source_details[trigger_channels_configured].thresholdMajor =
				(unsigned int)(thresholdMajor/adc_to_mv);

			source_details[trigger_channels_configured].thresholdMode =
				thresholdMode;

			// next channel to configure in source details
			trigger_channels_configured++;
		}
	}
	else {
		// disable channel
		ps5000SetChannel(h, ch, 0, 1, PS5000_20V);
	}

	return BOOL_TRUE;
}

bool_t picoscope_configure_basic(const short channelCount,const long sampleCount, const short oversample){
	if(!connected) {
		printf("picoscope_configure(): Not connected\n");
		return BOOL_FALSE;
	}

	if(configured) {
		free(buffer_a);
		buffer_a = 0;
		free(buffer_b);
		buffer_b = 0;
	}
	PICO_STATUS status;

	data_available = BOOL_FALSE;

	//timebase = picoscope_get_timebase(_sample_rate);

	// reset channels
	//ps5000SetChannel(h, PS5000_CHANNEL_A, 0, 1, PS5000_20V);
	//ps5000SetChannel(h, PS5000_CHANNEL_B, 0, 1, PS5000_20V);
	//ps5000SetChannel(h, PS5000_EXTERNAL, 0, 1, PS5000_20V);

	// disable ets
	ps5000SetEts(h, PS5000_ETS_OFF, 0, 0, NULL);

	//printf("Getting timebase (want %ld)\n", timebase);
	long timeInterval,maxSamples;

	/*  find the maximum number of samples, the time interval (in timeUnits),
	 * the most suitable time units, and the maximum oversample at the current timebase*/
	while ( (status=ps5000GetTimebase(h, timebase,channelCount*sampleCount,&timeInterval, oversample, &maxSamples, 0))){
	    printf("picoscope_configure_basic():");print_PicoError(status);
		timebase++;
	}
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;
    timebase++;



	printf("Using: timebase: %lu \t oversample: %hd \n", timebase, oversample);
	printf("     : timeintervall: %lu ns \t NrSample: %lu \n", timeInterval, maxSamples);
	printf("Want %d * %ul = %ul blocks and get %ul blocks\n",channelCount,sampleCount,channelCount*sampleCount,maxSamples);
	printf("Giving every channel %ul blocks\n",maxSamples/channelCount);
	block_sizeA=maxSamples/channelCount;
	block_sizeB=maxSamples/channelCount;


	//unsigned int block_sizeAll = (unsigned int)(maxSamples*sizeof(uint16_t));

	//if(ps5000GetTimebase(h, timebase, _block_size, &time_interval, 1, &max_samples, 0) != PICO_OK)	{
	//	printf("picoscope_configure(): Cannot sample %d values at %f\n", _block_size, _sample_rate);
	//	return BOOL_FALSE;
	//}else {
	//	double fs_actual = picoscope_get_sample_rate(timebase);

	//	if(fabs(fs_actual - _sample_rate) >= 1e-6) {
	//		printf("picoscope_configure(): Desired sample rate = %f  != actual sample rate = %f\n",
	//			_sample_rate, fs_actual);
	//	}
	//	sample_rate = fs_actual;
	//	point_to_ns = 1e9/sample_rate;
	//}


	printf("Alloc'ing buffers\n");

	// only do if channel a enabled
	buffer_a = (int16_t*)calloc(block_sizeA, sizeof(int16_t));
	if(buffer_a == 0) {
		printf("picoscope_configure_basic(): Alloc of buffer for channel A failed\n");
		return BOOL_FALSE;
	}

	printf("Alloc A done\n");

	buffer_b = (int16_t*)calloc(block_sizeB, sizeof(int16_t));
	if(buffer_b == 0) {
		printf("picoscope_configure_basic(): Alloc of buffer for channel B failed\n");

		free(buffer_a);
		buffer_a = 0;

		return BOOL_FALSE;
	}

	printf("Alloc B done\n");

	block_sizeAll = block_sizeA+block_sizeB;
	configured = BOOL_TRUE;

	return BOOL_TRUE;
}

bool_t picoscope_configure_trigger(struct tTriggerConditions* conditions,
	struct tTriggerDirections* directions, const double trigger_delay,
	const long auto_trigger_ms, const double pretrig_samples)
{
	const double point_to_ns = 1e9/sample_rate;
	const unsigned int trig_delay = (unsigned int)(trigger_delay * point_to_ns)/8;
	const short aux_enable = 0;

	pretrigger_samples = (unsigned long)(pretrig_samples/point_to_ns);

	// actually set trigger
	printf("Setting trigger, %d channels\n", trigger_channels_configured);

	//PICO_STATUS status = picoscope_set_trigger(, ,
	//	&conditions, 1, &directions, &pulse_width, trigger_delay, aux_enable, auto_trigger_ms);

	PICO_STATUS status;

	if ((status = ps5000SetTriggerChannelProperties(h,
					&(source_details[0]),
					trigger_channels_configured,
					aux_enable,
					auto_trigger_ms)) != PICO_OK) {
		printf("picoscope_configure_trigger(): Setting channel properties failed: Error code: %d\n", (int)status);
		printf("picoscope_configure_trigger():");print_PicoError(status);
		return BOOL_FALSE;
	}

	if ((status = ps5000SetTriggerChannelConditions(h, (TRIGGER_CONDITIONS*)conditions, 1)) != PICO_OK) {
		printf("picoscope_configure_trigger(): Setting channel conditions failed: Error code: %d\n", (int)status);
		printf("picoscope_configure_trigger():");print_PicoError(status);
		return BOOL_FALSE;
	}

	if ((status = ps5000SetTriggerChannelDirections(h,
					directions->channelA,
					directions->channelB,
					directions->channelC,
					directions->channelD,
					directions->ext,
					directions->aux)) != PICO_OK) {
		printf("picoscope_configure_trigger(): Setting channel directions failed: Error code: %d\n", (int)status);
		printf("picoscope_configure_trigger():");print_PicoError(status);
		return BOOL_FALSE;
	}

	if ((status = ps5000SetTriggerDelay(h, trig_delay)) != PICO_OK) {
		printf("picoscope_configure_trigger(): Setting trigger delay failed: Error code: %d\n", (int)status);
		printf("picoscope_configure_trigger():");print_PicoError(status);
		return BOOL_FALSE;
	}



	return BOOL_TRUE;
}

bool_t picoscope_configure_pwq(const short enable, struct tPwqConditions* cond_pwq,
	const enum enThresholdDirection direction, const double lower,
	const double upper, const enum enPulseWidthType type)
{
	PICO_STATUS status;
	const double point_to_ns = 1e9/sample_rate;

	// enable pwq
	if(enable) {
		// setup pwq
		if((status = ps5000SetPulseWidthQualifier(h, cond_pwq, 1, direction,
			(long)(lower/point_to_ns), (long)(upper/point_to_ns), type)) != PICO_OK)
		{
			printf("picoscope_configure_trigger(): Setting trigger delay failed: Error code: %d\n", (int)status);
			printf("picoscope_configure_trigger():");print_PicoError(status);
			return BOOL_FALSE;
		}
	}

	return BOOL_TRUE;
}

bool_t picoscope_disarm()
{
	if(!connected) {
		printf("picoscope_disarm(): Not connected\n");
		return BOOL_FALSE;
	}

	// stop scope
	ps5000Stop(h);

	return BOOL_TRUE;
}

static void PREF4 picoscope_callback_wrapper(short handle, PICO_STATUS status, void * p)
{
	printf("picoscope_callback_wrapper(): Data there!\n");

	// TODO: Lock
	// set flag
	data_available = BOOL_TRUE;
}
/*
void picoscope_fetchDataRAPID(const short channel, double* out, const unsigned long max_size)
{
	const int16_t * channel_buffer = (const int16_t*)(picoscope_fetchRawDataRAPID(channel));

	if(channel_buffer == 0)
		return;

	// convert to mv and copy tofetch out
	const double adc_to_mv = channel_adc_to_mv[channel];

	unsigned long s;
	unsigned long channel_blocks;
	if (channel==0){
        channel_blocks=block_sizeA;
	}else if(channel==1){
	    channel_blocks=block_sizeB;
	}else{
	     printf("picoscope_fetchDataRAPID(): invalid channel\n");
	}

	for(s = 0; s < channel_blocks && s < max_size; s++) {
		out[s] = adc_to_mv * channel_buffer[s];
	}
}

const uint8_t* picoscope_fetchRawDataRAPID(const short channel)
{
	picoscope_disarm();

	unsigned long channel_blocks;
	if (channel==0){
        channel_blocks=block_sizeA;
	}else if(channel==1){
	    channel_blocks=block_sizeB;
	}else{
	     printf("picoscope_fetchRawDataRAPID(): invalid channel\n");
	}
	short overflow = 0;

	ps5000GetValuesBulk(h, 1, 0, 0, &overflow);

	if(channel == 0)
		return (const uint8_t*)(buffer_a);
	else if(channel == 1)
		return (const uint8_t*)(buffer_b);
	else {
		printf("picoscope_fetchRawData(): Channel %d not configured or available\n", channel);
		return 0;
	}

}
*/
void picoscope_fetchData(const short channel, double* out, const unsigned long max_size)
{
	const int16_t * channel_buffer = (const int16_t*)(picoscope_fetchRawData(channel));

	if(channel_buffer == 0)
		return;

	// convert to mv and copy tofetch out
	const double adc_to_mv = channel_adc_to_mv[channel];

	unsigned long s;
	unsigned long channel_blocks;
	if (channel==0){
        channel_blocks=block_sizeA;
	}else if(channel==1){
	    channel_blocks=block_sizeB;
	}else{
	     printf("picoscope_fetchData(): invalid channel\n");
	}

	for(s = 0; s < channel_blocks && s < max_size; s++) {
		out[s] = adc_to_mv * channel_buffer[s];
	}
}

const uint8_t* picoscope_fetchRawData(const short channel)
{
	picoscope_disarm();

	unsigned long channel_blocks;
	if (channel==0){
        channel_blocks=block_sizeA;
	}else if(channel==1){
	    channel_blocks=block_sizeB;
	}else{
	     printf("picoscope_fetchRawData(): invalid channel\n");
	}
	short overflow = 0;

	ps5000GetValues(h, 0, &channel_blocks, 1, RATIO_MODE_NONE, 0, &overflow);

	if(channel == 0)
		return (const uint8_t*)(buffer_a);
	else if(channel == 1)
		return (const uint8_t*)(buffer_b);
	else {
		printf("picoscope_fetchRawData(): Channel %d not configured or available\n", channel);
		return 0;
	}

}

bool_t picoscope_has_data()
{
	// TODO: Lock
	return data_available;
}

/*
bool_t picoscope_arm(unsigned int wavefoind)
{
	if(!connected) {
		printf("picoscope_arm(): Not connected\n");
		return BOOL_FALSE;
	}

	if(!configured) {
		printf("picoscope_arm(): Not configured\n");
		return BOOL_FALSE;
	}

	data_available = BOOL_FALSE;
	PICO_STATUS status;

	// only do if channel a enabled
	if(buffer_a) {
		status = ps5000SetDataBufferBulk(h, PS5000_CHANNEL_A, buffer_a, block_sizeA);
		if(status != PICO_OK) {
			printf("picoscope_arm(): Setting data buffer A: Error code: %d\n", (int)status);
            printf("picoscope_arm():");print_PicoError(status);
			return BOOL_FALSE;
		}
	}

	// only do if channel b enabled
	if(buffer_b) {
		status = ps5000SetDataBuffer(h, PS5000_CHANNEL_B, buffer_b, block_sizeB);
		if(status != PICO_OK) {
			printf("picoscope_arm(): Setting data buffer B: Error code: %d\n", (int)status);
			printf("picoscope_arm():");print_PicoError(status);
			return BOOL_FALSE;
		}
	}

	long time_indisposed = 0;
	status = ps5000RunBlock(h, pretrigger_samples, block_sizeA+block_sizeB - pretrigger_samples, timebase, 1, &time_indisposed, 0, picoscope_callback_wrapper, 0);
	if (status != PICO_OK) {
		printf("picoscope_arm(): Block sampling: Error code: 0x%02x\n", (int)status);
        printf("picoscope_arm():");print_PicoError(status);
		return BOOL_FALSE;
	}

	return BOOL_TRUE;
}
*/
bool_t picoscope_arm()
{
	if(!connected) {
		printf("picoscope_arm(): Not connected\n");
		return BOOL_FALSE;
	}

	if(!configured) {
		printf("picoscope_arm(): Not configured\n");
		return BOOL_FALSE;
	}

	data_available = BOOL_FALSE;
	PICO_STATUS status;

	// only do if channel a enabled
	if(buffer_a) {
		status = ps5000SetDataBuffer(h, PS5000_CHANNEL_A, buffer_a, block_sizeA);
		if(status != PICO_OK) {
			printf("picoscope_arm(): Setting data buffer A: Error code: %d\n", (int)status);
            printf("picoscope_arm():");print_PicoError(status);
			return BOOL_FALSE;
		}
	}

	// only do if channel b enabled
	if(buffer_b) {
		status = ps5000SetDataBuffer(h, PS5000_CHANNEL_B, buffer_b, block_sizeB);
		if(status != PICO_OK) {
			printf("picoscope_arm(): Setting data buffer B: Error code: %d\n", (int)status);
			printf("picoscope_arm():");print_PicoError(status);
			return BOOL_FALSE;
		}
	}

	long time_indisposed = 0;
	status = ps5000RunBlock(h, pretrigger_samples, block_sizeA+block_sizeB - pretrigger_samples, timebase, 1, &time_indisposed, 0, picoscope_callback_wrapper, 0);
	if (status != PICO_OK) {
		printf("picoscope_arm(): Block sampling: Error code: 0x%02x\n", (int)status);
        printf("picoscope_arm():");print_PicoError(status);
		return BOOL_FALSE;
	}

	return BOOL_TRUE;
}


PS5000_RANGE picoscope_get_range_from_mv(const unsigned int r)
{
	PS5000_RANGE result;
	switch(r) {
		case 100:
			result = PS5000_100MV;
		break;
		case 200:
			result = PS5000_200MV;
		break;
		case 500:
			result = PS5000_500MV;
		break;
		case 1000:
			result = PS5000_1V;
		break;
		case 2000:
			result = PS5000_2V;
		break;
		case 5000:
			result = PS5000_5V;
		break;
		case 10000:
			result = PS5000_10V;
		break;
		case 20000:
			result = PS5000_20V;
		break;
		default:
			result = PS5000_100MV;
		break;
	}

	return result;
}

long picoscope_get_timebase(const double rate)
{
	long timebase = 0;
	// sampling interval
	const double Ts = 1/rate;
	// actual samling rate
	double fs_actual = 0;

	if(Ts <= 1e-9) {
		timebase = 0;
		fs_actual = 1/1e-9;
	}
	else if(Ts <= 2e-9) {
		timebase = 1;
		fs_actual = 1/2e-9;
	}
	else if(Ts <= 4e-9) {
		timebase = 2;
		fs_actual = 1/4e-9;
	}
	else {
		double base = floor(Ts * 125000000 + 2);
		timebase = (long)base;
		fs_actual = 1.0/((base - 2)/125000000);
	}

	return timebase;
}

unsigned int picoscope_getRawDataWidth()
{
	return sizeof(int16_t);
}

unsigned int picoscope_get_mv_from_range(const PS5000_RANGE r)
{
	unsigned int conv = 0;
	switch(r) {
		case PS5000_100MV:
			conv = 100;
		break;
		case PS5000_200MV:
			conv = 200;
		break;
		case PS5000_500MV:
			conv = 500;
		break;
		case PS5000_1V:
			conv = 1000;
		break;
		case PS5000_2V:
			conv = 2000;
		break;
		case PS5000_5V:
			conv = 5000;
		break;
		case PS5000_10V:
			conv = 10000;
		break;
		case PS5000_20V:
			conv = 20000;
		break;
		default:
			conv = 0;
		break;
	}

	return conv;
}

double picoscope_get_sample_rate(const long timebase)
{
	if(timebase == 0) {
		return 1/1e-9;
	}
	else if(timebase == 1) {
		return 1/2e-9;
	}
	else {
		return 1.0/((timebase - 2.0)/125000000);
	}
}

