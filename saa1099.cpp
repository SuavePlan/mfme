/***************************************************************************

	Philips SAA1099 Sound driver

	By Juergen Buchmueller and Manuel Abadia

	SAA1099 register layout:
	========================

	offs | 7654 3210 | description
	-----+-----------+---------------------------
	0x00 | ---- xxxx | Amplitude channel 0 (left)
	0x00 | xxxx ---- | Amplitude channel 0 (right)
	0x01 | ---- xxxx | Amplitude channel 1 (left)
	0x01 | xxxx ---- | Amplitude channel 1 (right)
	0x02 | ---- xxxx | Amplitude channel 2 (left)
	0x02 | xxxx ---- | Amplitude channel 2 (right)
	0x03 | ---- xxxx | Amplitude channel 3 (left)
	0x03 | xxxx ---- | Amplitude channel 3 (right)
	0x04 | ---- xxxx | Amplitude channel 4 (left)
	0x04 | xxxx ---- | Amplitude channel 4 (right)
	0x05 | ---- xxxx | Amplitude channel 5 (left)
	0x05 | xxxx ---- | Amplitude channel 5 (right)
		 |			 |
	0x08 | xxxx xxxx | Frequency channel 0
	0x09 | xxxx xxxx | Frequency channel 1
	0x0a | xxxx xxxx | Frequency channel 2
	0x0b | xxxx xxxx | Frequency channel 3
	0x0c | xxxx xxxx | Frequency channel 4
	0x0d | xxxx xxxx | Frequency channel 5
		 |			 |
	0x10 | ---- -xxx | Channel 0 octave select
	0x10 | -xxx ---- | Channel 1 octave select
	0x11 | ---- -xxx | Channel 2 octave select
	0x11 | -xxx ---- | Channel 3 octave select
	0x12 | ---- -xxx | Channel 4 octave select
	0x12 | -xxx ---- | Channel 5 octave select
		 |			 |
	0x14 | ---- ---x | Channel 0 frequency enable (0 = off, 1 = on)
	0x14 | ---- --x- | Channel 1 frequency enable (0 = off, 1 = on)
	0x14 | ---- -x-- | Channel 2 frequency enable (0 = off, 1 = on)
	0x14 | ---- x--- | Channel 3 frequency enable (0 = off, 1 = on)
	0x14 | ---x ---- | Channel 4 frequency enable (0 = off, 1 = on)
	0x14 | --x- ---- | Channel 5 frequency enable (0 = off, 1 = on)
		 |			 |
  	0x15 | ---- ---x | Channel 0 noise enable (0 = off, 1 = on)
	0x15 | ---- --x- | Channel 1 noise enable (0 = off, 1 = on)
	0x15 | ---- -x-- | Channel 2 noise enable (0 = off, 1 = on)
	0x15 | ---- x--- | Channel 3 noise enable (0 = off, 1 = on)
	0x15 | ---x ---- | Channel 4 noise enable (0 = off, 1 = on)
	0x15 | --x- ---- | Channel 5 noise enable (0 = off, 1 = on)
		 |			 |
	0x16 | ---- --xx | Noise generator parameters 0
	0x16 | --xx ---- | Noise generator parameters 1
		 |			 |
	0x18 | --xx xxxx | Envelope generator 0 parameters
	0x18 | x--- ---- | Envelope generator 0 control enable (0 = off, 1 = on)
	0x19 | --xx xxxx | Envelope generator 1 parameters
	0x19 | x--- ---- | Envelope generator 1 control enable (0 = off, 1 = on)
		 |			 |
	0x1c | ---- ---x | All channels enable (0 = off, 1 = on)
	0x1c | ---- --x- | Synch & Reset generators

***************************************************************************/

#include "saa1099.h"
#include "typedefs.h"
#include <math.h>


#define LEFT	0x00
#define RIGHT	0x01


/* global parameters */
static double sample_rate;

static int amplitude_lookup[16] = {
	 0*32767/16,  1*32767/16,  2*32767/16,	3*32767/16,
	 4*32767/16,  5*32767/16,  6*32767/16,	7*32767/16,
	 8*32767/16,  9*32767/16, 10*32767/16, 11*32767/16,
	12*32767/16, 13*32767/16, 14*32767/16, 15*32767/16
};

static Byte envelope[8][64] = {
	/* zero amplitude */
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* maximum amplitude */
    {15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	 15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
	 15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
     15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,15, },
	/* single decay */
	{15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* repetitive decay */
	{15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	 15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	 15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	 15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
	/* single triangular */
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	 15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* repetitive triangular */
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	 15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
	  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	 15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 },
	/* single attack */
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	/* repetitive attack */
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,
	  0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15 }
};

saa1099::saa1099()
{
  sample_rate = 64000;
}

void saa1099::reset()
{
int i;

  for ( i = 0; i < 6; i++ ) {
    channels[i].counter = 0;
    channels[i].freq = 0;
  }
  all_ch_enable = 0;
  env_enable[0] = env_enable[1] = 0;
}

void saa1099::saa1099_envelope(int ch)
{
	if (env_enable[ch])
	{
		int step, mode, mask;
        mode = env_mode[ch];
		/* step from 0..63 and then loop in steps 32..63 */
		step = env_step[ch] =
			((env_step[ch] + 1) & 0x3f) | (env_step[ch] & 0x20);

		mask = 15;
        if (env_bits[ch])
			mask &= ~1; 	/* 3 bit resolution, mask LSB */

        channels[ch*3+0].envelope[ LEFT] =
		channels[ch*3+1].envelope[ LEFT] =
		channels[ch*3+2].envelope[ LEFT] = envelope[mode][step] & mask;
		if (env_reverse_right[ch] & 0x01)
		{
			channels[ch*3+0].envelope[RIGHT] =
			channels[ch*3+1].envelope[RIGHT] =
			channels[ch*3+2].envelope[RIGHT] = (15 - envelope[mode][step]) & mask;
		}
		else
		{
			channels[ch*3+0].envelope[RIGHT] =
			channels[ch*3+1].envelope[RIGHT] =
			channels[ch*3+2].envelope[RIGHT] = envelope[mode][step] & mask;
        }
	}
	else
	{
		/* envelope mode off, set all envelope factors to 16 */
        channels[ch*3+0].envelope[ LEFT] =
		channels[ch*3+1].envelope[ LEFT] =
		channels[ch*3+2].envelope[ LEFT] =
		channels[ch*3+0].envelope[RIGHT] =
		channels[ch*3+1].envelope[RIGHT] =
		channels[ch*3+2].envelope[RIGHT] = 16;
    }
}


void saa1099::Update(short *buffer, int length, unsigned int cycles)
{
    int j, ch;

	/* if the channels are disabled we're done */
	if (!all_ch_enable)
	{
		/* init output data */
		memset(buffer,0,length*sizeof(short));
//		memset(buffer[RIGHT],0,length*sizeof(short));
        return;
	}

    for (ch = 0; ch < 2; ch++)
    {
		switch (noise_params[ch])
		{
		case 0: noise[ch].freq = 31250.0 * 2; break;
		case 1: noise[ch].freq = 15625.0 * 2; break;
		case 2: noise[ch].freq =  7812.5 * 2; break;
		case 3: noise[ch].freq = channels[ch * 3].freq; break;
		}
	}

    /* fill all data needed */
	for( j = 0; j < length; j++ )
	{
		int output_l = 0, output_r = 0;

		/* for each channel */
		for (ch = 0; ch < 6; ch++)
		{
            if (channels[ch].freq == 0.0)
                channels[ch].freq = (double)((2 * 15625) << channels[ch].octave) /
                    (511.0 - (double)channels[ch].frequency);

            /* check the actual position in the square wave */
            channels[ch].counter -= channels[ch].freq;
			while (channels[ch].counter < 0)
			{
				/* calculate new frequency now after the half wave is updated */
				channels[ch].freq = (double)((2 * 15625) << channels[ch].octave) /
					(511.0 - (double)channels[ch].frequency);

				channels[ch].counter += sample_rate;
				channels[ch].level ^= 1;

				/* eventually clock the envelope counters */
				if (ch == 1 && env_clock[0] == 0)
					saa1099_envelope(0);
				if (ch == 4 && env_clock[1] == 0)
					saa1099_envelope(1);
			}

			/* if the noise is enabled */
			if (channels[ch].noise_enable)
			{
				/* if the noise level is high (noise 0: chan 0-2, noise 1: chan 3-5) */
				if (noise[ch/3].level & 1)
				{
					/* subtract to avoid overflows, also use only half amplitude */
					output_l -= channels[ch].amplitude[ LEFT] * channels[ch].envelope[ LEFT] / 16 / 2;
					output_r -= channels[ch].amplitude[RIGHT] * channels[ch].envelope[RIGHT] / 16 / 2;
				}
			}

			/* if the square wave is enabled */
			if (channels[ch].freq_enable)
			{
				/* if the channel level is high */
				if (channels[ch].level & 1)
				{
					output_l += channels[ch].amplitude[ LEFT] * channels[ch].envelope[ LEFT] / 16;
					output_r += channels[ch].amplitude[RIGHT] * channels[ch].envelope[RIGHT] / 16;
				}
			}
		}

		for (ch = 0; ch < 2; ch++)
		{
			/* check the actual position in noise generator */
			noise[ch].counter -= noise[ch].freq;
			while (noise[ch].counter < 0)
			{
				noise[ch].counter += sample_rate;
				if( ((noise[ch].level & 0x4000) == 0) == ((noise[ch].level & 0x0040) == 0) )
					noise[ch].level = (noise[ch].level << 1) | 1;
				else
					noise[ch].level <<= 1;
			}
		}
        /* write sound data to the buffer */
		buffer[j] = (output_l / 12) + (output_r / 12);
//		buffer[RIGHT][j] = C;
	}
}

void saa1099::control(int data )
{
    if ((data & 0xff) > 0x1c)
	{
		/* Error! */
        //        logerror("%04x: (SAA1099 #%d) Unknown register selected\n",activecpu_get_pc(), chip);
	}

    selected_reg = data & 0x1f;
	if (selected_reg == 0x18 || selected_reg == 0x19)
	{
		/* clock the envelope channels */
        if (env_clock[0])
			saa1099_envelope(0);
		if (env_clock[1])
			saa1099_envelope(1);
    }
}


void saa1099::data(int data )
{
	int reg = selected_reg;
	int ch;

	/* first update the stream to this point in time */
//	stream_update(stream, 0);

	switch (reg)
	{
	/* channel i amplitude */
	case 0x00:	case 0x01:	case 0x02:	case 0x03:	case 0x04:	case 0x05:
		ch = reg & 7;
		channels[ch].amplitude[LEFT] = amplitude_lookup[data & 0x0f];
		channels[ch].amplitude[RIGHT] = amplitude_lookup[(data >> 4) & 0x0f];
		break;
	/* channel i frequency */
	case 0x08:	case 0x09:	case 0x0a:	case 0x0b:	case 0x0c:	case 0x0d:
		ch = reg & 7;
		channels[ch].frequency = data & 0xff;
		break;
	/* channel i octave */
	case 0x10:	case 0x11:	case 0x12:
		ch = (reg - 0x10) << 1;
		channels[ch + 0].octave = data & 0x07;
		channels[ch + 1].octave = (data >> 4) & 0x07;
		break;
	/* channel i frequency enable */
	case 0x14:
		channels[0].freq_enable = data & 0x01;
		channels[1].freq_enable = data & 0x02;
		channels[2].freq_enable = data & 0x04;
		channels[3].freq_enable = data & 0x08;
		channels[4].freq_enable = data & 0x10;
		channels[5].freq_enable = data & 0x20;
		break;
	/* channel i noise enable */
	case 0x15:
		channels[0].noise_enable = data & 0x01;
		channels[1].noise_enable = data & 0x02;
		channels[2].noise_enable = data & 0x04;
		channels[3].noise_enable = data & 0x08;
		channels[4].noise_enable = data & 0x10;
		channels[5].noise_enable = data & 0x20;
		break;
	/* noise generators parameters */
	case 0x16:
		noise_params[0] = data & 0x03;
		noise_params[1] = (data >> 4) & 0x03;
		break;
	/* envelope generators parameters */
	case 0x18:	case 0x19:
		ch = reg - 0x18;
		env_reverse_right[ch] = data & 0x01;
		env_mode[ch] = (data >> 1) & 0x07;
		env_bits[ch] = data & 0x10;
		env_clock[ch] = data & 0x20;
		env_enable[ch] = data & 0x80;
		/* reset the envelope */
		env_step[ch] = 0;
		break;
	/* channels enable & reset generators */
	case 0x1c:
		all_ch_enable = data & 0x01;
		sync_state = data & 0x02;
		if (data & 0x02)
		{
			int i;

			/* Synch & Reset generators */
//			logerror("%04x: (SAA1099 #%d) -reg 0x1c- Chip reset\n",activecpu_get_pc(), chip);
			for (i = 0; i < 6; i++)
			{
                channels[i].level = 0;
				channels[i].counter = 0.0;
			}
		}
		break;
	default:	/* Error! */
        ;
//		logerror("%04x: (SAA1099 #%d) Unknown operation (reg:%02x, data:%02x)\n",activecpu_get_pc(), chip, reg, data);
	}
}


