//---------------------------------------------------------------------------
#ifndef sampleH
#define sampleH
//---------------------------------------------------------------------------

#define BUFFLEN1 12000
#define BUFFLEN2 3000

struct UPD7759voice
{
	int signal;             /* current ADPCM signal */
	int last;             /* previous ADPCM signal */
   int step;               /* current ADPCM step */
};

/* struct describing a single playing ADPCM voice */
struct YMZ280BVoice
{
	int signal;			/* current ADPCM signal */
	int step;				/* current ADPCM step */
};

struct YMZVoice
{
  int playing;
  int start;
  int stop;
  int mode;
  int loop_start;
  int loop_end;
  int keyon;
  int looping;
  int rate;
  int pan;
  int vol;
  int tunenum;
};

class Sample
{
protected:
private:
    struct adpcm_status {
      short last;
      short step_index;
    };
    int m_writePos;
    int m_writePos2;
    int m_writePos3;
    int s1_writePos;
    int s2_writePos;
    int Length[200];
    int RealLength[200];
    int SampleRate;
    int Stereo;
    int old_volume;
    bool muted;
    void __fastcall adpcm_init( struct adpcm_status *stat );
    char __fastcall adpcm_encode( short samp, struct adpcm_status *stat );
    short __fastcall adpcm_decode( char code, struct adpcm_status *stat );
    short __fastcall step_adjust ( char code );
    void __fastcall NECDecode(int val, UPD7759voice *voice);
    void __fastcall ymz280b_initadpcm(void);
    short __fastcall ymz280b_decode(struct YMZ280BVoice *voice, unsigned char val);
    void __fastcall Read32(TMemoryStream *tms, unsigned int *vtr);
    void __fastcall Read24(TMemoryStream *tms, unsigned int *vtr);
    void __fastcall Read16(TMemoryStream *tms, unsigned short *vtr);
    void __fastcall ymz_start(int channum);
    void __fastcall ymz_stop(int channum);
public:
	__fastcall Sample(void);
    __fastcall ~Sample(void);
    void __fastcall LoadSound(TStringList *SoundList, int volume, int Rate);
    void __fastcall LoadJPMSound(TStringList *SoundList, int volume, int rate);
    void __fastcall LoadYMZSound(TStringList *SoundList, int volume);
    bool __fastcall Init(HWND hwnd, int *ASampleRate);
    void __fastcall Play(int nbr, int Channel);
    void __fastcall UpdateSoundBuff(void (__closure *func)(short *,int, unsigned int), unsigned int);
    void __fastcall UpdateSoundBuff2(void (__closure *func)(short *,int, unsigned int), unsigned int);
    void __fastcall UpdateSoundBuff3(void (__closure *func)(short *,int, unsigned int), unsigned int);
    void __fastcall Stop(void);
    void __fastcall ClearBuffer(void);
    void __fastcall PlaySample(int nbr);
    void __fastcall PlaySample(Byte nbr, Byte Channel);
    void __fastcall PlaySample2(Byte nbr, Byte Channel);
     int __fastcall Details(int nbr);
     int __fastcall RealDetails(int nbr);
    bool __fastcall SetSampleRate(int SampleRate);
     int __fastcall Playing(int nbr);
    void __fastcall SaveToFile(int nbr, String Filename);
    void __fastcall DeleteTunes(void);
    void __fastcall AdjustRate(int rate);
    void __fastcall ymz280b_reset(void);
    void __fastcall ymz280b_writereg(int regnum);
    void __fastcall ymz280b_writedata(unsigned char data);
     int __fastcall ymz280b_readstatus(void);
     int __fastcall ymz280b_readdata(void);
      int __fastcall ymz280b_checkirq(void);
    void __fastcall SetMasterVolume(int level);
    void __fastcall SetStereo(int stereo);
    bool __fastcall Sample::Mute(void);
};
extern Sample *Sound;
#endif

