//
//
//	ay8910.cpp
//

#include <math.h>
#include "typedefs.h"
#include "ay8910.h"

//#define LOG_SOUND

#ifdef LOG_SOUND
int last;
#endif

ay8910::ay8910()
{
    build_mixer_table();
    set_RC_filter(0,820,500,0,10000);
    SampleRate = 64000;
    AY8910_set_clock(1720000);
#ifdef LOG_SOUND
    fp = fopen("sound.txt", "w+");
#endif
	reset();
}

ay8910::~ay8910()
{
#ifdef LOG_SOUND
    fclose(fp);
#endif
}

void ay8910::reset()
{
  address = 0;
  muted = 0;
//  changed = 0;
  AY8910_reset();
#ifdef LOG_SOUND
  last = GetTickCount();
#endif
}

void ay8910::write_data(Byte d, Byte cs, Byte bdir, Byte bc1)
{
#ifdef LOG_SOUND
DWORD tickcount, diff;

  tickcount = GetTickCount();
#endif
//  changed = FALSE;
  porta_changed = portb_changed = false;

  if ( bc1 )
    address = d & 0xf;
  else if ( bdir && cs ) {
#ifdef LOG_SOUND
      diff = tickcount - last;
      if ( diff )
        fprintf( fp, "\n");
      fprintf( fp, "%08ld ", diff);
      switch ( address ) {
      case 0:
        fprintf( fp, "Chan A Fine   %02x\n", d);
        break;
      case 1:
        fprintf( fp, "Chan A Course %02x\n", d);
        break;
      case 2:
        fprintf( fp, "Chan B Fine   %02x\n", d);
        break;
      case 3:
        fprintf( fp, "Chan B Course %02x\n", d);
        break;
      case 4:
        fprintf( fp, "Chan C Fine   %02x\n", d);
        break;
      case 5:
        fprintf( fp, "Chan C Course %02x\n", d);
        break;
      case 6:
        fprintf( fp, "Noise Period  %02x\n", d);
        break;
      case 7:
        fprintf( fp, "Enable        %02x\n", d);
        break;
      case 8:
        fprintf( fp, "Chan A Volume %02x\n", d);
        break;
      case 9:
        fprintf( fp, "Chan B Volume %02x\n", d);
        break;
      case 10:
        fprintf( fp, "Chan C Volume %02x\n", d);
        break;
      case 11:
        fprintf( fp, "Envelope Fine %02x\n", d);
        break;
      case 12:
        fprintf( fp, "Envelope Cors %02x\n", d);
        break;
      case 13:
        fprintf( fp, "Envelope Shpe %02x\n", d);
        break;
      case 14:
        fprintf( fp, "I/O A         %02x\n", d);
        break;
      case 15:
        fprintf( fp, "I/O B         %02x\n", d);
        break;
      }
      last = tickcount;
#endif
      AYWriteReg(address, d);
//      reg = address;
//      value = d;
//      changed = TRUE;
  }
}
void ay8910::_AYWriteReg(int r, int v)
{
	int old;

	switch( r )
	{
      case AY_PORTA:
        porta_changed = Regs[r] ^ v;
        break;
      case AY_PORTB:
        portb_changed = Regs[r] ^ v;
        break;
    }

	Regs[r] = v;

	/* A note about the period of tones, noise and envelope: for speed reasons,*/
	/* we count down from the period to 0, but careful studies of the chip     */
	/* output prove that it instead counts up from 0 until the counter becomes */
	/* greater or equal to the period. This is an important difference when the*/
	/* program is rapidly changing the period to modulate the sound.           */
	/* To compensate for the difference, when the period is changed we adjust  */
	/* our internal counter.                                                   */
	/* Also, note that period = 0 is the same as period = 1. This is mentioned */
	/* in the YM2203 data sheets. However, this does NOT apply to the Envelope */
	/* period. In that case, period = 0 is half as period = 1. */
	switch( r )
	{
	case AY_AFINE:
	case AY_ACOARSE:
		Regs[AY_ACOARSE] &= 0x0f;
		old = PeriodA;
		PeriodA = (Regs[AY_AFINE] + 256 * Regs[AY_ACOARSE]) * UpdateStep;
		if (PeriodA == 0) PeriodA = UpdateStep;
		CountA += PeriodA - old;
		if (CountA <= 0) CountA = 1;
		break;
	case AY_BFINE:
	case AY_BCOARSE:
		Regs[AY_BCOARSE] &= 0x0f;
		old = PeriodB;
		PeriodB = (Regs[AY_BFINE] + 256 * Regs[AY_BCOARSE]) * UpdateStep;
		if (PeriodB == 0) PeriodB = UpdateStep;
		CountB += PeriodB - old;
		if (CountB <= 0) CountB = 1;
		break;
	case AY_CFINE:
	case AY_CCOARSE:
		Regs[AY_CCOARSE] &= 0x0f;
		old = PeriodC;
		PeriodC = (Regs[AY_CFINE] + 256 * Regs[AY_CCOARSE]) * UpdateStep;
		if (PeriodC == 0) PeriodC = UpdateStep;
		CountC += PeriodC - old;
		if (CountC <= 0) CountC = 1;
		break;
	case AY_NOISEPER:
		Regs[AY_NOISEPER] &= 0x1f;
		old = PeriodN;
		PeriodN = Regs[AY_NOISEPER] * UpdateStep;
		if (PeriodN == 0) PeriodN = UpdateStep;
		CountN += PeriodN - old;
		if (CountN <= 0) CountN = 1;
		break;
	case AY_AVOL:
		Regs[AY_AVOL] &= 0x1f;
		EnvelopeA = Regs[AY_AVOL] & 0x10;
		VolA = EnvelopeA ? VolE : VolTable[Regs[AY_AVOL] ? Regs[AY_AVOL]*2+1 : 0];
		break;
	case AY_BVOL:
		Regs[AY_BVOL] &= 0x1f;
		EnvelopeB = Regs[AY_BVOL] & 0x10;
		VolB = EnvelopeB ? VolE : VolTable[Regs[AY_BVOL] ? Regs[AY_BVOL]*2+1 : 0];
		break;
	case AY_CVOL:
		Regs[AY_CVOL] &= 0x1f;
		EnvelopeC = Regs[AY_CVOL] & 0x10;
		VolC = EnvelopeC ? VolE : VolTable[Regs[AY_CVOL] ? Regs[AY_CVOL]*2+1 : 0];
		break;
	case AY_EFINE:
	case AY_ECOARSE:
		old = PeriodE;
		PeriodE = ((Regs[AY_EFINE] + 256 * Regs[AY_ECOARSE])) * UpdateStep;
		if (PeriodE == 0) PeriodE = UpdateStep / 2;
		CountE += PeriodE - old;
		if (CountE <= 0) CountE = 1;
		break;
	case AY_ESHAPE:
		/* envelope shapes:
		C AtAlH
		0 0 x x  \___

		0 1 x x  /___

		1 0 0 0  \\\\

		1 0 0 1  \___

		1 0 1 0  \/\/
		          ___
		1 0 1 1  \

		1 1 0 0  ////
		          ___
		1 1 0 1  /

		1 1 1 0  /\/\

		1 1 1 1  /___

		The envelope counter on the AY-3-8910 has 16 steps. On the YM2149 it
		has twice the steps, happening twice as fast. Since the end result is
		just a smoother curve, we always use the YM2149 behaviour.
		*/
		Regs[AY_ESHAPE] &= 0x0f;
		Attack = (Regs[AY_ESHAPE] & 0x04) ? 0x1f : 0x00;
		if ((Regs[AY_ESHAPE] & 0x08) == 0)
		{
			/* if Continue = 0, map the shape to the equivalent one which has Continue = 1 */
			Hold = 1;
			Alternate = Attack;
		}
		else
		{
			Hold = Regs[AY_ESHAPE] & 0x01;
			Alternate = Regs[AY_ESHAPE] & 0x02;
		}
		CountE = PeriodE;
		CountEnv = 0x1f;
		Holding = 0;
		VolE = VolTable[CountEnv ^ Attack];
		if (EnvelopeA) VolA = VolE;
		if (EnvelopeB) VolB = VolE;
		if (EnvelopeC) VolC = VolE;
		break;
	case AY_PORTA:
		if ((Regs[AY_ENABLE] & 0x40) != 0)
          porta = v;
		break;
	case AY_PORTB:
		if ((Regs[AY_ENABLE] & 0x80) != 0)
          portb = v;
		break;
	}
}


/* write a register on AY8910 chip number 'n' */
void ay8910::AYWriteReg(int r,int v)
{

	if (r > 15) return;
	if (r < 14)
	{
		if (r == AY_ESHAPE || Regs[r] != v)
		{
			/* update the output buffer before changing the register */
//          Form1->UpdateSoundBuffermpu4();
		}
	}

	_AYWriteReg(r,v);

}



unsigned char ay8910::AYReadReg(int r)
{

	if (r > 15) return 0;

	switch (r)
	{
	case AY_PORTA:
		if ((Regs[AY_ENABLE] & 0x40) == 0)
          return porta_in;
		break;
	case AY_PORTB:
		if ((Regs[AY_ENABLE] & 0x80) == 0)
          return portb_in;
		break;
	}
	return Regs[r];
}


void ay8910::AY8910Write(int a,int data)
{

	if (a & 1)
	{	/* Data port */
		AYWriteReg(register_latch,data);
	}
	else
	{	/* Register port */
		register_latch = data & 0x0f;
	}
}

int ay8910::AY8910Read(void)
{

	return AYReadReg(register_latch);
}


/* AY8910 interface */
void ay8910::Update(short *ptr, int len, unsigned int clks)
{
	int outn;
    int length = len;
    short *buff_ptr1 = ptr;
    short out_a, out_b, out_c;
	/* The 8910 has three outputs, each output is the mix of one of the three */
	/* tone generators and of the (single) noise generator. The two are mixed */
	/* BEFORE going into the DAC. The formula to mix each channel is: */
	/* (ToneOn | ToneDisable) & (NoiseOn | NoiseDisable). */
	/* Note that this means that if both tone and noise are disabled, the output */
	/* is 1, not 0, and can be modulated changing the volume. */


	/* If the channels are disabled, set their output to 1, and increase the */
	/* counter, if necessary, so they will not be inverted during this update. */
	/* Setting the output to 1 is necessary because a disabled channel is locked */
	/* into the ON state (see above); and it has no effect if the volume is 0. */
	/* If the volume is 0, increase the counter, but don't touch the output. */
	if (Regs[AY_ENABLE] & 0x01)
	{
		if (CountA <= length*STEP) CountA += length*STEP;
		OutputA = 1;
	}
	else if (Regs[AY_AVOL] == 0)
	{
		/* note that I do count += length, NOT count = length + 1. You might think */
		/* it's the same since the volume is 0, but doing the latter could cause */
		/* interferencies when the program is rapidly modulating the volume. */
		if (CountA <= length*STEP) CountA += length*STEP;
	}
	if (Regs[AY_ENABLE] & 0x02)
	{
		if (CountB <= length*STEP) CountB += length*STEP;
		OutputB = 1;
	}
	else if (Regs[AY_BVOL] == 0)
	{
		if (CountB <= length*STEP) CountB += length*STEP;
	}
	if (Regs[AY_ENABLE] & 0x04)
	{
		if (CountC <= length*STEP) CountC += length*STEP;
		OutputC = 1;
	}
	else if (Regs[AY_CVOL] == 0)
	{
		if (CountC <= length*STEP) CountC += length*STEP;
	}

	/* for the noise channel we must not touch OutputN - it's also not necessary */
	/* since we use outn. */
	if ((Regs[AY_ENABLE] & 0x38) == 0x38)	/* all off */
		if (CountN <= length*STEP) CountN += length*STEP;

	outn = (OutputN | Regs[AY_ENABLE]);


	/* buffering loop */
	while (length)
	{
		int vola,volb,volc;
		int left;


		/* vola, volb and volc keep track of how long each square wave stays */
		/* in the 1 position during the sample period. */
		vola = volb = volc = 0;

		left = STEP;
		do
		{
			int nextevent;


			if (CountN < left) nextevent = CountN;
			else nextevent = left;

			if (outn & 0x08)
			{
				if (OutputA) vola += CountA;
				CountA -= nextevent;
				/* PeriodA is the half period of the square wave. Here, in each */
				/* loop I add PeriodA twice, so that at the end of the loop the */
				/* square wave is in the same status (0 or 1) it was at the start. */
				/* vola is also incremented by PeriodA, since the wave has been 1 */
				/* exactly half of the time, regardless of the initial position. */
				/* If we exit the loop in the middle, OutputA has to be inverted */
				/* and vola incremented only if the exit status of the square */
				/* wave is 1. */
				while (CountA <= 0)
				{
					CountA += PeriodA;
					if (CountA > 0)
					{
						OutputA ^= 1;
						if (OutputA) vola += PeriodA;
						break;
					}
					CountA += PeriodA;
					vola += PeriodA;
				}
				if (OutputA) vola -= CountA;
			}
			else
			{
				CountA -= nextevent;
				while (CountA <= 0)
				{
					CountA += PeriodA;
					if (CountA > 0)
					{
						OutputA ^= 1;
						break;
					}
					CountA += PeriodA;
				}
			}

			if (outn & 0x10)
			{
				if (OutputB) volb += CountB;
				CountB -= nextevent;
				while (CountB <= 0)
				{
					CountB += PeriodB;
					if (CountB > 0)
					{
						OutputB ^= 1;
						if (OutputB) volb += PeriodB;
						break;
					}
					CountB += PeriodB;
					volb += PeriodB;
				}
				if (OutputB) volb -= CountB;
			}
			else
			{
				CountB -= nextevent;
				while (CountB <= 0)
				{
					CountB += PeriodB;
					if (CountB > 0)
					{
						OutputB ^= 1;
						break;
					}
					CountB += PeriodB;
				}
			}

			if (outn & 0x20)
			{
				if (OutputC) volc += CountC;
				CountC -= nextevent;
				while (CountC <= 0)
				{
					CountC += PeriodC;
					if (CountC > 0)
					{
						OutputC ^= 1;
						if (OutputC) volc += PeriodC;
						break;
					}
					CountC += PeriodC;
					volc += PeriodC;
				}
				if (OutputC) volc -= CountC;
			}
			else
			{
				CountC -= nextevent;
				while (CountC <= 0)
				{
					CountC += PeriodC;
					if (CountC > 0)
					{
						OutputC ^= 1;
						break;
					}
					CountC += PeriodC;
				}
			}

			CountN -= nextevent;
			if (CountN <= 0)
			{
				/* Is noise output going to change? */
				if ((RNG + 1) & 2)	/* (bit0^bit1)? */
				{
					OutputN = ~OutputN;
					outn = (OutputN | Regs[AY_ENABLE]);
				}

				/* The Random Number Generator of the 8910 is a 17-bit shift */
				/* register. The input to the shift register is bit0 XOR bit2 */
				/* (bit0 is the output). */

				/* The following is a fast way to compute bit 17 = bit0^bit2. */
				/* Instead of doing all the logic operations, we only check */
				/* bit 0, relying on the fact that after two shifts of the */
				/* register, what now is bit 2 will become bit 0, and will */
				/* invert, if necessary, bit 16, which previously was bit 18. */
				if (RNG & 1) RNG ^= 0x28000;
				RNG >>= 1;
				CountN += PeriodN;
			}

			left -= nextevent;
		} while (left > 0);

		/* update envelope */
		if (Holding == 0)
		{
			CountE -= STEP;
			if (CountE <= 0)
			{
				do
				{
					CountEnv--;
					CountE += PeriodE;
				} while (CountE <= 0);

				/* check envelope current position */
				if (CountEnv < 0)
				{
					if (Hold)
					{
						if (Alternate)
							Attack ^= 0x1f;
						Holding = 1;
						CountEnv = 0;
					}
					else
					{
						/* if CountEnv has looped an odd number of times (usually 1), */
						/* invert the output. */
						if (Alternate && (CountEnv & 0x20))
 							Attack ^= 0x1f;

						CountEnv &= 0x1f;
					}
				}

				VolE = VolTable[CountEnv ^ Attack];
				/* reload volume */
				if (EnvelopeA) VolA = VolE;
				if (EnvelopeB) VolB = VolE;
				if (EnvelopeC) VolC = VolE;
			}
		}
        out_a = ((vola * VolA) / STEP);
        out_b = ((volb * VolB) / STEP);
        out_c = ((volc * VolC) / STEP);
        if ( muted )
          *buff_ptr1++ = 0;
        else
	      *buff_ptr1++ = (out_a/3) + (out_b/3) + (out_c/3);

		length--;
	}
    apply_RC_filter(0, ptr, len, SampleRate);
}


void ay8910::AY8910_set_clock(int clock)
{
	/* the step clock for the tone and noise generators is the chip clock    */
	/* divided by 8; for the envelope generator of the AY-3-8910, it is half */
	/* that much (clock/16), but the envelope of the YM2149 goes twice as    */
	/* fast, therefore again clock/8.                                        */
	/* Here we calculate the number of steps which happen during one sample  */
	/* at the given sample rate. No. of events = sample rate / (clock/8).    */
	/* STEP is a multiplier used to turn the fraction into a fixed point     */
	/* number.                                                               */
	UpdateStep = ((double)STEP * SampleRate * 8) / clock;
}


void ay8910::build_mixer_table(void)
{
	int i;
	double out;


	/* calculate the volume->voltage conversion table */
	/* The AY-3-8910 has 16 levels, in a logarithmic scale (3dB per step) */
	/* The YM2149 still has 16 levels for the tone generators, but 32 for */
	/* the envelope generator (1.5dB per step). */
	out = MAX_OUTPUT;
	for (i = 31;i > 0;i--)
	{
		VolTable[i] = out + 0.5;	/* round to nearest */

		out /= 1.188502227;	/* = 10 ^ (1.5/20) = 1.5dB */
	}
	VolTable[0] = 0;
}



void ay8910::AY8910_reset(void)
{
	int i;

	register_latch = 0;
	RNG = 1;
	OutputA = 0;
	OutputB = 0;
	OutputC = 0;
	OutputN = 0xff;
    porta_changed = portb_changed = false;
    porta = portb = 0;
	for (i = 0;i < AY_PORTA;i++)
		_AYWriteReg(i,0);	/* AYWriteReg() uses the timer system; we cannot */
								/* call it at this time because the timer system */
								/* has not been initialized. */
}

/*
signal >--R1--+--R2--+
              |      |
              C      R3---> amp
              |      |
             GND    GND
*/

/* R1, R2, R3 in Ohm; C in pF */
/* set C = 0 to disable the filter */
void ay8910::set_RC_filter(int channel,int R1,int R2,int R3,int C)
{
	r1[channel] = R1;
	r2[channel] = R2;
	r3[channel] = R3;
	c[channel] = C;
}

void ay8910::apply_RC_filter(int channel,short *buf,int len,int sample_rate)
{
	float R1,R2,R3,C;
	float Req;
	int K;
	int i;


	if (c[channel] == 0) return;	/* filter disabled */

	R1 = r1[channel];
	R2 = r2[channel];
	R3 = r3[channel];
	C = (float)c[channel] * 1E-12;	/* convert pF to F */

	/* Cut Frequency = 1/(2*Pi*Req*C) */

	Req = (R1*(R2+R3))/(R1+R2+R3);

	K = 0x10000 * exp(-1 / (Req * C) / sample_rate);

	buf[0] = buf[0] + (memory[channel] - buf[0]) * K / 0x10000;

	for (i = 1;i < len;i++)
		buf[i] = buf[i] + (buf[i-1] - buf[i]) * K / 0x10000;

	memory[channel] = buf[len-1];
}

void ay8910::mute(Byte Amuted)
{
  muted = Amuted;
}

void __fastcall ay8910::SaveToStream(Byte *&ptr)
{
  savestate(ptr, register_latch);
  savestate(ptr, Regs);
  savestate(ptr, UpdateStep);
  savestate(ptr, PeriodA);
  savestate(ptr, PeriodB);
  savestate(ptr, PeriodC);
  savestate(ptr, PeriodN);
  savestate(ptr, PeriodE);
  savestate(ptr, CountA);
  savestate(ptr, CountB);
  savestate(ptr, CountC);
  savestate(ptr, CountN);
  savestate(ptr, CountE);
  savestate(ptr, VolA);
  savestate(ptr, VolB);
  savestate(ptr, VolC);
  savestate(ptr, VolE);
  savestate(ptr, EnvelopeA);
  savestate(ptr, EnvelopeB);
  savestate(ptr, EnvelopeC);
  savestate(ptr, OutputA);
  savestate(ptr, OutputB);
  savestate(ptr, OutputC);
  savestate(ptr, OutputN);
  savestate(ptr, CountEnv);
  savestate(ptr, Hold);
  savestate(ptr, Alternate);
  savestate(ptr, Attack);
  savestate(ptr, Holding);
  savestate(ptr, RNG);
  savestate(ptr, VolTable);
  savestate(ptr, r1);
  savestate(ptr, r2);
  savestate(ptr, r3);
  savestate(ptr, c);
  savestate(ptr, memory);
  savestate(ptr, address);
}

void __fastcall ay8910::LoadFromStream(Byte *&ptr)
{
  readstate(ptr, register_latch);
  readstate(ptr, Regs);
  readstate(ptr, UpdateStep);
  readstate(ptr, PeriodA);
  readstate(ptr, PeriodB);
  readstate(ptr, PeriodC);
  readstate(ptr, PeriodN);
  readstate(ptr, PeriodE);
  readstate(ptr, CountA);
  readstate(ptr, CountB);
  readstate(ptr, CountC);
  readstate(ptr, CountN);
  readstate(ptr, CountE);
  readstate(ptr, VolA);
  readstate(ptr, VolB);
  readstate(ptr, VolC);
  readstate(ptr, VolE);
  readstate(ptr, EnvelopeA);
  readstate(ptr, EnvelopeB);
  readstate(ptr, EnvelopeC);
  readstate(ptr, OutputA);
  readstate(ptr, OutputB);
  readstate(ptr, OutputC);
  readstate(ptr, OutputN);
  readstate(ptr, CountEnv);
  readstate(ptr, Hold);
  readstate(ptr, Alternate);
  readstate(ptr, Attack);
  readstate(ptr, Holding);
  readstate(ptr, RNG);
  readstate(ptr, VolTable);
  readstate(ptr, r1);
  readstate(ptr, r2);
  readstate(ptr, r3);
  readstate(ptr, c);
  readstate(ptr, memory);
  readstate(ptr, address);
}

