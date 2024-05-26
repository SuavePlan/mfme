#ifndef saa1099_h
#define saa1099_h

/**********************************************
	Philips SAA1099 Sound driver
**********************************************/

/* this structure defines a channel */
struct saa1099_channel
{
	int frequency;			/* frequency (0x00..0xff) */
	int freq_enable;		/* frequency enable */
	int noise_enable;		/* noise enable */
	int octave; 			/* octave (0x00..0x07) */
	int amplitude[2];		/* amplitude (0x00..0x0f) */
	int envelope[2];		/* envelope (0x00..0x0f or 0x10 == off) */

	/* vars to simulate the square wave */
	double counter;
	double freq;
	int level;
};

/* this structure defines a noise channel */
struct saa1099_noise
{
	/* vars to simulate the noise generator output */
	double counter;
	double freq;
	int level;						/* noise polynomal shifter */
};
/* this structure defines a SAA1099 chip */
class saa1099
{
	int stream;						/* our stream */
	int noise_params[2];			/* noise generators parameters */
	int env_enable[2];				/* envelope generators enable */
	int env_reverse_right[2];		/* envelope reversed for right channel */
	int env_mode[2];				/* envelope generators mode */
	int env_bits[2];				/* non zero = 3 bits resolution */
	int env_clock[2];				/* envelope clock mode (non-zero external) */
    int env_step[2];                /* current envelope step */
	int all_ch_enable;				/* all channels enable */
	int sync_state;					/* sync all channels */
	int selected_reg;				/* selected register */
	struct saa1099_channel channels[6];    /* channels */
	struct saa1099_noise noise[2];	/* noise generators */
    void saa1099_envelope(int ch);
public:
    void Update(short *buffer, int length, unsigned int cycles);
    void control(int data );
    void data(int data );
    void reset();
    saa1099();
};

#endif

