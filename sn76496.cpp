/***************************************************************************

  sn76496.c

  Routines to emulate the Texas Instruments SN76489 / SN76496 programmable
  tone /noise generator. Also known as (or at least compatible with) TMS9919.

  Noise emulation is not accurate due to lack of documentation. The noise
  generator uses a shift register with a XOR-feedback network, but the exact
  layout is unknown. It can be set for either period or white noise; again,
  the details are unknown.

***************************************************************************/
#include <math.h>
#include "typedefs.h"
#include "sn76496.h"


sn76496::sn76496()
{
	por();
}

sn76496::~sn76496()
{
}

void sn76496::por()
{
  SampleRate = 64000;
  volume=0xff;
  baseclock=1250000;
  SN76496_init(baseclock,volume & 0xff,SampleRate);
  SN76496_set_gain((volume >> 8) & 0xff);
}

void sn76496::SN76496Write(int data)
{
	/* update the output buffer before changing the registers */
//	stream_update(Channel,0);

	if (data & 0x80)
	{
		int r = (data & 0x70) >> 4;
		int c = r/2;

		LastRegister = r;
		Register[r] = (Register[r] & 0x3f0) | (data & 0x0f);
		switch (r)
		{
			case 0:	/* tone 0 : frequency */
			case 2:	/* tone 1 : frequency */
			case 4:	/* tone 2 : frequency */
				Period[c] = UpdateStep * Register[r];
				if (Period[c] == 0) Period[c] = UpdateStep;
				if (r == 4)
				{
					/* update noise shift frequency */
					if ((Register[6] & 0x03) == 0x03)
						Period[3] = 2 * Period[2];
				}
				break;
			case 1:	/* tone 0 : volume */
			case 3:	/* tone 1 : volume */
			case 5:	/* tone 2 : volume */
			case 7:	/* noise  : volume */
				Volume[c] = VolTable[data & 0x0f];
				break;
			case 6:	/* noise  : frequency, mode */
				{
					int n = Register[6];
					NoiseFB = (n & 4) ? SN_FB_WNOISE : SN_FB_PNOISE;
					n &= 3;
					/* N/512,N/1024,N/2048,Tone #3 output */
					Period[3] = (n == 3) ? 2 * Period[2] : (UpdateStep << (5+n));

					/* reset noise shifter */
					RNG = SN_NG_PRESET;
					Output[3] = RNG & 1;
				}
				break;
		}
	}
	else
	{
		int r = LastRegister;
		int c = r/2;

		switch (r)
		{
			case 0:	/* tone 0 : frequency */
			case 2:	/* tone 1 : frequency */
			case 4:	/* tone 2 : frequency */
				Register[r] = (Register[r] & 0x0f) | ((data & 0x3f) << 4);
				Period[c] = UpdateStep * Register[r];
				if (Period[c] == 0) Period[c] = UpdateStep;
				if (r == 4)
				{
					/* update noise shift frequency */
					if ((Register[6] & 0x03) == 0x03)
						Period[3] = 2 * Period[2];
				}
				break;
		}
	}
}

void sn76496::SN76496Update(short *buffer,int length, unsigned int cycles)
{
	int i;

	/* If the volume is 0, increase the counter */
	for (i = 0;i < 4;i++)
	{
		if (Volume[i] == 0)
		{
			/* note that I do count += length, NOT count = length + 1. You might think */
			/* it's the same since the volume is 0, but doing the latter could cause */
			/* interferencies when the program is rapidly modulating the volume. */
			if (Count[i] <= length*SN_STEP) Count[i] += length*SN_STEP;
		}
	}

	while (length > 0)
	{
		int vol[4];
		unsigned int out;
		int left;


		/* vol[] keeps track of how long each square wave stays */
		/* in the 1 position during the sample period. */
		vol[0] = vol[1] = vol[2] = vol[3] = 0;

		for (i = 0;i < 3;i++)
		{
			if (Output[i]) vol[i] += Count[i];
			Count[i] -= SN_STEP;
			/* Period[i] is the half period of the square wave. Here, in each */
			/* loop I add Period[i] twice, so that at the end of the loop the */
			/* square wave is in the same status (0 or 1) it was at the start. */
			/* vol[i] is also incremented by Period[i], since the wave has been 1 */
			/* exactly half of the time, regardless of the initial position. */
			/* If we exit the loop in the middle, Output[i] has to be inverted */
			/* and vol[i] incremented only if the exit status of the square */
			/* wave is 1. */
			while (Count[i] <= 0)
			{
				Count[i] += Period[i];
				if (Count[i] > 0)
				{
					Output[i] ^= 1;
					if (Output[i]) vol[i] += Period[i];
					break;
				}
				Count[i] += Period[i];
				vol[i] += Period[i];
			}
			if (Output[i]) vol[i] -= Count[i];
		}

		left = SN_STEP;
		do
		{
			int nextevent;


			if (Count[3] < left) nextevent = Count[3];
			else nextevent = left;

			if (Output[3]) vol[3] += Count[3];
			Count[3] -= nextevent;
			if (Count[3] <= 0)
			{
				if (RNG & 1) RNG ^= NoiseFB;
				RNG >>= 1;
				Output[3] = RNG & 1;
				Count[3] += Period[3];
				if (Output[3]) vol[3] += Period[3];
			}
			if (Output[3]) vol[3] -= Count[3];

			left -= nextevent;
		} while (left > 0);

		out = vol[0] * Volume[0] + vol[1] * Volume[1] +
				vol[2] * Volume[2] + vol[3] * Volume[3];

		if (out > SN_MAX_OUTPUT * SN_STEP) out = SN_MAX_OUTPUT * SN_STEP;

		*(buffer++) = out / SN_STEP;

		length--;
	}
}

void sn76496::SN76496_set_clock(int clock)
{
	/* the base clock for the tone generators is the chip clock divided by 16; */
	/* for the noise generator, it is clock / 256. */
	/* Here we calculate the number of steps which happen during one sample */
	/* at the given sample rate. No. of events = sample rate / (clock/16). */
	/* STEP is a multiplier used to turn the fraction into a fixed point */
	/* number. */
	UpdateStep = ((double)SN_STEP * SampleRate * 16) / clock;
}

void sn76496::SN76496_set_gain(int gain)
{
	int i;
	double out;

	gain &= 0xff;

	/* increase max output basing on gain (0.2 dB per step) */
	out = SN_MAX_OUTPUT / 3;
	while (gain-- > 0)
		out *= 1.023292992;	/* = (10 ^ (0.2/20)) */

	/* build volume table (2dB per step) */
	for (i = 0;i < 15;i++)
	{
		/* limit volume to avoid clipping */
		if (out > SN_MAX_OUTPUT / 3) VolTable[i] = SN_MAX_OUTPUT / 3;
		else VolTable[i] = out;

		out /= 1.258925412;	/* = 10 ^ (2/20) = 2dB */
	}
	VolTable[15] = 0;
}

int sn76496::SN76496_init(int clock,int volume,int sample_rate)
{
	int i;

	if (Channel == -1)
		return 1;

	SampleRate = sample_rate;
	SN76496_set_clock(clock);

	for (i = 0;i < 4;i++) Volume[i] = 0;

	LastRegister = 0;
	for (i = 0;i < 8;i+=2)
	{
		Register[i] = 0;
		Register[i + 1] = 0x0f;	/* volume = 0 */
	}

	for (i = 0;i < 4;i++)
	{
		Output[i] = 0;
		Period[i] = Count[i] = UpdateStep;
	}
	RNG = SN_NG_PRESET;
	Output[3] = RNG & 1;

	return 0;
}

void __fastcall sn76496::SaveToStream(Byte *&ptr)
{
  savestate(ptr,baseclock);
  savestate(ptr,volume);
  savestate(ptr,Channel);
  savestate(ptr,SampleRate);
  savestate(ptr,UpdateStep);
  savestate(ptr,VolTable);
  savestate(ptr,Register);
  savestate(ptr,LastRegister);
  savestate(ptr,Volume);
  savestate(ptr,RNG);
  savestate(ptr,NoiseFB);
  savestate(ptr,Period);
  savestate(ptr,Count);
  savestate(ptr,Output);
}

void __fastcall sn76496::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,baseclock);
  readstate(ptr,volume);
  readstate(ptr,Channel);
  readstate(ptr,SampleRate);
  readstate(ptr,UpdateStep);
  readstate(ptr,VolTable);
  readstate(ptr,Register);
  readstate(ptr,LastRegister);
  readstate(ptr,Volume);
  readstate(ptr,RNG);
  readstate(ptr,NoiseFB);
  readstate(ptr,Period);
  readstate(ptr,Count);
  readstate(ptr,Output);
}

