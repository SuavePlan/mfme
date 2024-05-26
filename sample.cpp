//---------------------------------------------------------------------------
#include <classes.hpp>
#pragma hdrstop

#include <stdio.h>
#include "sample.h"
#include "system.h"
#include "mmsystem.h"
#include "dsound.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#define FALL_OFF(n) 	(n-((n+7)/8))

#define SIGNAL_BITS 	15	/* signal range */
#define SIGNAL_MAX		(0x7fff >> (15-SIGNAL_BITS))
#define SIGNAL_MIN		-SIGNAL_MAX

#define STEP_MAX		32
#define STEP_MIN		0

/* step size index shift table */
#define INDEX_SHIFT_MAX 16

class Twomdone : public TThread
{
__published:
protected:
    int  objects;
	void __fastcall Execute();
private:
public:
	__fastcall Twomdone(bool createsuspended);
    void __fastcall finished(void);
};

extern char buffer[];

struct wave_header {
  char header[4];
  unsigned int length;
  char wave_header[4];
  char format_hdr[4];
  unsigned int fmt_length;
  unsigned short fmt;
  unsigned short nbr_channels;
  unsigned int sample_rate;
  unsigned int bytes_per_second;
  unsigned short bytes_per_sample;
  unsigned short bits_per_sample;
  char data[4];
  unsigned int data_length;
};

struct wave_header header1 = {
  "RIFF", 0, "WAVE", "fmt ", 0x10, 1, 1, 16000,
  32000, 2, 16, "data", 0
};

const static int UPD7759_step[16][16] = {
	{ 0,  0,  1,  2,  3,   5,   7,  10,  0,   0,  -1,  -2,  -3,   -5,   -7,  -10 },
	{ 0,  1,  2,  3,  4,   6,   8,  13,  0,  -1,  -2,  -3,  -4,   -6,   -8,  -13 },
	{ 0,  1,  2,  4,  5,   7,  10,  15,  0,  -1,  -2,  -4,  -5,   -7,  -10,  -15 },
	{ 0,  1,  3,  4,  6,   9,  13,  19,  0,  -1,  -3,  -4,  -6,   -9,  -13,  -19 },
	{ 0,  2,  3,  5,  8,  11,  15,  23,  0,  -2,  -3,  -5,  -8,  -11,  -15,  -23 },
	{ 0,  2,  4,  7, 10,  14,  19,  29,  0,  -2,  -4,  -7, -10,  -14,  -19,  -29 },
	{ 0,  3,  5,  8, 12,  16,  22,  33,  0,  -3,  -5,  -8, -12,  -16,  -22,  -33 },
	{ 1,  4,  7, 10, 15,  20,  29,  43, -1,  -4,  -7, -10, -15,  -20,  -29,  -43 },
	{ 1,  4,  8, 13, 18,  25,  35,  53, -1,  -4,  -8, -13, -18,  -25,  -35,  -53 },
	{ 1,  6, 10, 16, 22,  31,  43,  64, -1,  -6, -10, -16, -22,  -31,  -43,  -64 },
	{ 2,  7, 12, 19, 27,  37,  51,  76, -2,  -7, -12, -19, -27,  -37,  -51,  -76 },
	{ 2,  9, 16, 24, 34,  46,  64,  96, -2,  -9, -16, -24, -34,  -46,  -64,  -96 },
	{ 3, 11, 19, 29, 41,  57,  79, 117, -3, -11, -19, -29, -41,  -57,  -79, -117 },
	{ 4, 13, 24, 36, 50,  69,  96, 143, -4, -13, -24, -36, -50,  -69,  -96, -143 },
	{ 4, 16, 29, 44, 62,  85, 118, 175, -4, -16, -29, -44, -62,  -85, -118, -175 },
	{ 6, 20, 36, 54, 76, 104, 144, 214, -6, -20, -36, -54, -76, -104, -144, -214 },
};

const static int UPD7759_state[16] = { -1, -1, 0, 0, 1, 2, 2, 3, -1, -1, 0, 0, 1, 2, 2, 3 };

static short step_size[49] = { 16, 17, 19, 21, 23, 25, 28, 31, 34, 37, 41,
     45, 50, 55, 60, 66, 73, 80, 88, 97, 107, 118, 130, 143, 157, 173,
     190, 209, 230, 253, 279, 307, 337, 371, 408, 449, 494, 544, 598, 658,
     724, 796, 876, 963, 1060, 1166, 1282, 1408, 1552 };

/* YMZ280 step size index shift table */
static int index_scale[8] = { 0x0e6, 0x0e6, 0x0e6, 0x0e6, 0x133, 0x199, 0x200, 0x266 };

/* YMZ380 lookup table for the precomputed difference */
static int diff_lookup[16];

int playing[10];

    LPDIRECTSOUND               lpds;
    LPDIRECTSOUNDBUFFER         lpdsbPrimary;
    LPDIRECTSOUNDNOTIFY         lpdsNotify;
    LPDIRECTSOUNDBUFFER         tune[200];
    LPDIRECTSOUNDBUFFER         soundbuff;
    LPDIRECTSOUNDBUFFER         soundbuff2;
    LPDIRECTSOUNDBUFFER         soundbuff3;
//    LPDIRECTSOUNDBUFFER         sample1;
//    LPDIRECTSOUNDBUFFER         sample2;
    DSBPOSITIONNOTIFY           rgdsbpn[2];
    HANDLE                      rghEvent[2];
//    unsigned char               *play1, *play2;
//    int                         length1, length2;
//    bool                        changed1, changed2;
    bool AppCreateBasicBuffer(
        LPDIRECTSOUND lpDirectSound,
        LPDIRECTSOUNDBUFFER *lplpDsb,
        DWORD freq,
        DWORD length,
        bool notify = false);
    bool AppWriteDataToBuffer(
        LPDIRECTSOUNDBUFFER lpDsb,  // the DirectSound buffer
        DWORD dwOffset,             // our own write cursor
        LPBYTE lpbSoundData,        // start of our data
        DWORD dwSoundBytes);        // size of block to copy
    UPD7759voice ADPCMStatFlag;
    Byte lookup[8][256];
    TMemoryStream *tunes[200];
    int ymz_rates[200];
    int ymz_lookup[200];
    int ymz_number;
    int rates[200];
    int ymz_curr_reg;
    unsigned int ymz_addr;
    TMemoryStream *sound; // Contains the whole ROM
    LPDIRECTSOUNDBUFFER buf_tune[8];
    unsigned char ymz_status;
    struct YMZVoice ymzvoices[8];
    int ymz_keyon_enable;
    Byte  ymz_irqmask;

//#define LOG_SOUND
#ifdef LOG_SOUND
    FILE             *stream;
#endif

//---------------------------------------------------------------------------

 __fastcall Twomdone::Twomdone(bool CreateSuspended)
	: TThread(CreateSuspended)
{
  FreeOnTerminate = true;
//  Priority = tpNormal;
}
//---------------------------------------------------------------------------

void __fastcall Twomdone::Execute()
{

  while ( !Terminated ) {
    objects = WaitForMultipleObjects( 2, rghEvent, false, INFINITE);
    Synchronize(finished);
  }

}
//---------------------------------------------------------------------------
void __fastcall Twomdone::finished(void)
{
int object;

#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();
#endif

  switch ( objects ) {
    case WAIT_OBJECT_0:
    case WAIT_OBJECT_0 + 1:
      object = objects - WAIT_OBJECT_0;
      if ( playing[object] ) {
#ifdef LOG_SOUND
        fprintf(stream,"%08d Sample finished %d \r\n", tickcount, playing[object]);
#endif
        playing[object] = 0;
        if ( Fruit )
          Fruit->samplefinished(object);
      }
      break;
    default:
     break;
  }
}
//---------------------------------------------------------------------------
__fastcall Sample::Sample(void)
{
int loop;

    for ( loop = 0; loop < 2; loop++ ) {
      rghEvent[loop] = CreateEvent(NULL, FALSE, FALSE, NULL);
      rgdsbpn[loop].dwOffset = DSBPN_OFFSETSTOP;
      rgdsbpn[loop].hEventNotify = rghEvent[loop];
    }
    for ( loop = 0; loop < 10; loop++ )
      playing[loop] = 0;
    Stereo = false;
    muted = false;
}
//---------------------------------------------------------------------------
__fastcall Sample::~Sample(void)
{
  Stop();
  if (lpdsNotify)
    lpdsNotify->Release();
  if (lpds)
    lpds->Release();
#ifdef LOG_SOUND
    fclose(stream);
#endif
}
//---------------------------------------------------------------------------
bool __fastcall Sample::Init(HWND hwnd, int *ASampleRate)
{
DSBUFFERDESC dsbdesc;

   // Create DirectSound
    SampleRate = *ASampleRate;

    if FAILED(DirectSoundCreate(NULL, &lpds, NULL))
        return FALSE;

    // Set co-op level
 
    if FAILED(IDirectSound_SetCooperativeLevel(
            lpds, hwnd, DSSCL_PRIORITY))
        return FALSE;

 // Obtain primary buffer
 
    ZeroMemory(&dsbdesc, sizeof(DSBUFFERDESC));
    dsbdesc.dwSize = sizeof(DSBUFFERDESC);
    dsbdesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
    if FAILED(lpds->CreateSoundBuffer(&dsbdesc, &lpdsbPrimary, NULL))
        return FALSE;

 // Set primary buffer format

    if ( SetSampleRate(SampleRate) == false )
      return FALSE;

//    lpdsbPrimary->SetVolume(0); // Too damn loud !
    
    AppCreateBasicBuffer( lpds, &soundbuff, 64000, BUFFLEN1);
    soundbuff->Play(0,0,DSBPLAY_LOOPING);
    AppCreateBasicBuffer( lpds, &soundbuff2, 64000, BUFFLEN1);
    soundbuff2->Play(0,0,DSBPLAY_LOOPING);
    AppCreateBasicBuffer( lpds, &soundbuff3, 64000, BUFFLEN1);
    soundbuff3->Play(0,0,DSBPLAY_LOOPING);
    m_writePos = 0;
    m_writePos2 = 0;
    m_writePos3 = 0;
      new Twomdone(false);   // For M1a remove ***********
#ifdef LOG_SOUND
    stream = fopen("sound.txt", "w+");
#endif
    return TRUE;
}
//---------------------------------------------------------------------------
void __fastcall Sample::LoadSound(TStringList *SoundList, int volume, int Rate)
{
TMemoryStream *file_ptr, *ptr;
int count;
unsigned char bp[3];
int address, Size, loop;
short sample;
unsigned char value, length;
struct adpcm_status stat;
int Reallength;
int rate;

  if ( SoundList->Count ) {
    file_ptr = new TMemoryStream();
    sound = new TMemoryStream();
    SoundList->Sort();
    for ( count = 0; count < SoundList->Count; count++ ) {
      file_ptr->LoadFromFile(SoundList->Strings[count]);
      sound->CopyFrom(file_ptr, 0);
    }
    delete file_ptr;
    for ( count = 0; count < 120; count++ ) {
      sound->Position = count * 4;
      sound->Read( bp, 3);
      address = (bp[0] << 16) + (bp[1] << 8) + bp[2];
//      rate = (16 - ((bp[0] & 0xc0) >> 4)) * 1000;
      rate = Rate;
      address &= 0x1fffff;
      Size = 0;
      Reallength = 0;
      ptr = new TMemoryStream();
      if ( address > 0 && address < sound->Size ) {
        adpcm_init( &stat);
        sound->Position = address;
        sound->Read( &length, 1);
        Reallength = 1;
        length &= 0x7f;
        while ( length  ) {
          for ( loop = 0; loop < length; loop++ ) {
            sound->Read( &value, 1);
            sample = volume * adpcm_decode( (value >> 4) & 0xF, &stat);
            ptr->Write( &sample, 2);
            sample = volume * adpcm_decode( value & 0xF, &stat);
            ptr->Write( &sample, 2);
            Size += 4;
            Reallength++;
          }
          sound->Read( &length, 1);
          length &= 0x7f;
          Reallength++;
//          if ( length == 0xc4 )
//            printf("oh oh\n");
        }
      } else {
        if ( count == 0 ) {
          Size = 8;
          sample = 0;
          for ( loop = 0; loop < Size / 2; loop++ )
            ptr->Write( &sample, 2);
        }
      }
      RealLength[count] = Reallength;
      if ( Size > 0 ) {
        Length[count] = Size;
        ptr->Position = 0;
//        samples[count] = ptr;
        if ( AppCreateBasicBuffer( lpds, &tune[count],rate ? rate : Rate, Size, true) )
            AppWriteDataToBuffer( tune[count], 0, (unsigned char *)ptr->Memory, Size);
        rates[count] = rate ? rate : Rate;
      } else
        Length[count] = 0;
      tunes[count] = ptr;
//      rates[count] = Rate;
//      delete ptr;
    }
    if ( muted ) {
      muted = !muted;
      Mute();
    }
  }
}
//---------------------------------------------------------------------------
/*
* Initialze the data used by the coder.
*/
void __fastcall Sample::adpcm_init( struct adpcm_status *stat )
{
    stat->last = 0;
    stat->step_index = 0;
}

//---------------------------------------------------------------------------
/*
* Encode linear to ADPCM
*/
char __fastcall Sample::adpcm_encode( short samp, struct adpcm_status *stat )
{
    short code;
    short diff, E, SS;

    SS = step_size[stat->step_index];
    code = 0x00;
    if( (diff = samp - stat->last) < 0 )
        code = 0x08;
    E = diff < 0 ? -diff : diff;
    if( E >= SS ) {
        code = code | 0x04;
        E -= SS;
    }
    if( E >= SS/2 ) {
        code = code | 0x02;
        E -= SS/2;
    }
    if( E >= SS/4 ) {
        code = code | 0x01;
    }
/*    stat->step_index += step_adjust( code );
    if( stat->step_index < 0 ) stat->step_index = 0;
    if( stat->step_index > 48 ) stat->step_index = 48;
*/
    /*
    * Use the decoder to set the estimate of last sample.
    * It also will adjust the step_index for us.
    */
    stat->last = adpcm_decode(code, stat);
    return( code );
}

//---------------------------------------------------------------------------
/*
* Decode Linear to ADPCM
*/
short __fastcall Sample::adpcm_decode( char code, struct adpcm_status *stat )
{
    short diff, E, SS, samp;

    SS = step_size[stat->step_index];
    E = SS/8;
    if ( code & 0x01 )
        E += SS/4;
    if ( code & 0x02 )
        E += SS/2;
    if ( code & 0x04 )
        E += SS;
    diff = (code & 0x08) ? -E : E;
    samp = stat->last + diff;

    /*
    *  Clip the values to +/- 2^11 (supposed to be 12 bits)
    */
    if( samp > 2048 ) samp = 2048;
    if( samp < -2048 ) samp = -2048;

    stat->last = samp;
    stat->step_index += step_adjust( code );
    if( stat->step_index < 0 ) stat->step_index = 0;
    if( stat->step_index > 48 ) stat->step_index = 48;

    return( samp );
}

//---------------------------------------------------------------------------
/*
* adjust the step for use on the next sample.
*/
short __fastcall Sample::step_adjust ( char code )
{
short reply;

    switch( code & 0x07 ) {
        case 0x00:
            reply = -1;
            break;
        case 0x01:
            reply = -1;
            break;
        case 0x02:
            reply = -1;
            break;
        case 0x03:
            reply = -1;
            break;
        case 0x04:
            reply = 2;
            break;
        case 0x05:
            reply = 4;
            break;
        case 0x06:
            reply = 6;
            break;
        case 0x07:
            reply = 8;
            break;
    }
    return reply;
}
//---------------------------------------------------------------------------
bool AppCreateBasicBuffer(
        LPDIRECTSOUND lpDirectSound,
        LPDIRECTSOUNDBUFFER *lplpDsb,
        DWORD freq,
        DWORD length,
        bool notify)
{
    WAVEFORMATEX wfx;
    DSBUFFERDESC dsbdesc;
    HRESULT hr;
    // Set up wave format structure.
    memset(&wfx, 0, sizeof(WAVEFORMATEX));
    wfx.wFormatTag = WAVE_FORMAT_PCM;
    wfx.nChannels = 1;
    wfx.nSamplesPerSec = freq;
    wfx.wBitsPerSample = 16;
    wfx.nBlockAlign = (wfx.nChannels * wfx.wBitsPerSample) / 8;
    wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
    // Set up DSBUFFERDESC structure.
    memset(&dsbdesc, 0, sizeof(DSBUFFERDESC)); // Zero it out. 
    dsbdesc.dwSize = sizeof(DSBUFFERDESC); 
    // Need default controls (pan, volume, frequency).
    dsbdesc.dwFlags =  DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLPAN | DSBCAPS_CTRLVOLUME;
    if ( notify ) {
        dsbdesc.dwFlags |= DSBCAPS_CTRLPOSITIONNOTIFY;
        dsbdesc.dwFlags |= DSBCAPS_LOCSOFTWARE;
    } else {
        dsbdesc.dwFlags |= DSBCAPS_GETCURRENTPOSITION2;
    }
    // 3-second buffer.
    dsbdesc.dwBufferBytes = length;
    dsbdesc.lpwfxFormat = &wfx;
    // Create buffer.
    hr = lpDirectSound->CreateSoundBuffer( &dsbdesc, lplpDsb, NULL);
    if SUCCEEDED(hr)
    { 
        // Valid interface is in *lplpDsb. 
        (*lplpDsb)->SetPan(0);
        (*lplpDsb)->SetVolume(0);
        return TRUE;
    }
    else 
    { 
        // Failed. 
        *lplpDsb = NULL; 
        return FALSE;
    } 
}
//---------------------------------------------------------------------------
bool AppWriteDataToBuffer(
    LPDIRECTSOUNDBUFFER lpDsb,  // the DirectSound buffer
    DWORD dwOffset,             // our own write cursor
    LPBYTE lpbSoundData,        // start of our data
    DWORD dwSoundBytes)         // size of block to copy
{
    LPVOID  lpvPtr1;
    DWORD   dwBytes1; 
    LPVOID  lpvPtr2; 
    DWORD   dwBytes2; 
    HRESULT hr;
    DWORD  dwAudio1, dwAudio2;

    // Obtain memory address of write block. This will be in two parts
    // if the block wraps around.
    hr = lpDsb->Lock(dwOffset, dwSoundBytes, &lpvPtr1,
        &dwBytes1, &lpvPtr2, &dwBytes2, 0);

    // If DSERR_BUFFERLOST is returned, restore and retry lock. 
    if (DSERR_BUFFERLOST == hr)
    { 
        lpDsb->Restore();
        hr = lpDsb->Lock(dwOffset, dwSoundBytes,
            &lpvPtr1, &dwAudio1, &lpvPtr2, &dwAudio2, 0);
    } 
    if SUCCEEDED(hr) 
    { 
        // Write to pointers. 
        CopyMemory(lpvPtr1, lpbSoundData, dwBytes1);
        if (NULL != lpvPtr2) 
        { 
            CopyMemory(lpvPtr2, lpbSoundData+dwBytes1, dwBytes2); 
        } 
        // Release the data back to DirectSound. 
        hr = lpDsb->Unlock(lpvPtr1, dwBytes1, lpvPtr2, dwBytes2);
        if SUCCEEDED(hr) 
        { 
            // Success. 
            return TRUE; 
        } 
    }
    // Lock, Unlock, or Restore failed.
    return FALSE; 
}
//---------------------------------------------------------------------------
void __fastcall Sample::Play(int nbr, int Channel)
{
/*
  if ( nbr == 0 ) {
//    changed1 = changed2 = true;
    play1 = play2 = NULL;
    notify(0);
    notify(1);
  } else {
    if ( samples[nbr] && Length[nbr] > 0 ) {
      if ( Channel == 0 ) {
//        if ( !play1 )
//          changed1 = true;
        length1 = Length[nbr];
        play1 = (unsigned char *)samples[nbr]->Memory;
      } else {
//        if ( !play2 )
//          changed2 = true;
        length2 = Length[nbr];
        play2 = (unsigned char *)samples[nbr]->Memory;
      }
    } else
      notify(Channel);
  } */
}
//---------------------------------------------------------------------------
void __fastcall Sample::UpdateSoundBuff(void (__closure *func)(short *,int, unsigned int), unsigned int cycles)
{
DWORD	playPos,unusedWriteCursor;
DWORD	writeLen;
LPVOID	p1,p2;
DWORD	l1,l2;
HRESULT hRes;

  hRes = soundbuff->GetCurrentPosition(&playPos,&unusedWriteCursor);
  if (hRes != DS_OK) {
    playPos = 0;
  }
  if (m_writePos < playPos) {
    writeLen = playPos - m_writePos;
  } else {
    writeLen = BUFFLEN1 - (m_writePos - playPos);
  }
  while (DS_OK != soundbuff->Lock(m_writePos,writeLen,&p1,&l1,&p2,&l2,0))
  {
    soundbuff->Restore();
    soundbuff->Play(0, 0, DSBPLAY_LOOPING);
  }

  if ((p1) && (l1>0))
    func((short *)p1,l1/2, cycles);
  if ((p2) && (l2>0))
    func((short *)p2,l2/2, cycles);

  soundbuff->Unlock(p1,l1,p2,l2);
  m_writePos += writeLen;

  if (m_writePos >= BUFFLEN1) m_writePos -= BUFFLEN1;
}
//---------------------------------------------------------------------------
void __fastcall Sample::UpdateSoundBuff2(void (__closure *func)(short *,int, unsigned int), unsigned int cycles)
{
DWORD	playPos,unusedWriteCursor;
DWORD	writeLen;
LPVOID	p1,p2;
DWORD	l1,l2;
HRESULT hRes;

  hRes = soundbuff2->GetCurrentPosition(&playPos,&unusedWriteCursor);
  if (hRes != DS_OK) playPos = 0;

  if (m_writePos2 < playPos)
    writeLen = playPos - m_writePos2;
  else
    writeLen = BUFFLEN1 - (m_writePos2 - playPos);

  while (DS_OK != soundbuff2->Lock(m_writePos2,writeLen,&p1,&l1,&p2,&l2,0))
  {
    soundbuff2->Restore();
    soundbuff2->Play(0, 0, DSBPLAY_LOOPING);
  }

  if ((p1) && (l1>0))
    func((short *)p1,l1/2, cycles);
  if ((p2) && (l2>0))
    func((short *)p2,l2/2, cycles);

  soundbuff2->Unlock(p1,l1,p2,l2);
  m_writePos2 += writeLen;

  if (m_writePos2 >= BUFFLEN1) m_writePos2 -= BUFFLEN1;
}
//---------------------------------------------------------------------------
void __fastcall Sample::UpdateSoundBuff3(void (__closure *func)(short *,int, unsigned int), unsigned int cycles)
{
DWORD	playPos,unusedWriteCursor;
DWORD	writeLen;
LPVOID	p1,p2;
DWORD	l1,l2;
HRESULT hRes;

  hRes = soundbuff3->GetCurrentPosition(&playPos,&unusedWriteCursor);
  if (hRes != DS_OK) playPos = 0;

  if (m_writePos3 < playPos)
    writeLen = playPos - m_writePos3;
  else
    writeLen = BUFFLEN1 - (m_writePos3 - playPos);

  while (DS_OK != soundbuff3->Lock(m_writePos3,writeLen,&p1,&l1,&p2,&l2,0))
  {
    soundbuff3->Restore();
    soundbuff3->Play(0, 0, DSBPLAY_LOOPING);
  }

  if ((p1) && (l1>0))
    func((short *)p1,l1/2, cycles);
  if ((p2) && (l2>0))
    func((short *)p2,l2/2, cycles);

  soundbuff3->Unlock(p1,l1,p2,l2);
  m_writePos3 += writeLen;

  if (m_writePos3 >= BUFFLEN1) m_writePos3 -= BUFFLEN1;
}
//---------------------------------------------------------------------------
void __fastcall Sample::ClearBuffer(void)
{
LPVOID	p1,p2;
DWORD	l1,l2;
int i;

  soundbuff->Stop();

  while (DS_OK != soundbuff->Lock(0,BUFFLEN1,&p1,&l1,&p2,&l2,0))
  {
    soundbuff->Restore();
  }

  if ((p1) && (l1>0))
    memset(p1, 0, l1);
  if ((p2) && (l2>0))
    memset(p2 ,0, l2);

  soundbuff->Unlock(p1,l1,p2,l2);

  soundbuff->Play(0,0,DSBPLAY_LOOPING);

  soundbuff2->Stop();

  while (DS_OK != soundbuff2->Lock(0,BUFFLEN1,&p1,&l1,&p2,&l2,0))
  {
    soundbuff2->Restore();
  }

  if ((p1) && (l1>0))
    memset(p1, 0, l1);
  if ((p2) && (l2>0))
    memset(p2 ,0, l2);

  soundbuff2->Unlock(p1,l1,p2,l2);

  soundbuff2->Play(0,0,DSBPLAY_LOOPING);

  for ( i = 0; i < 10; i++ )
    playing[i] = 0;
    
//  Stop();
  DeleteTunes();

}
//---------------------------------------------------------------------------
void __fastcall Sample::Stop(void)
{
  if ( playing[0] )
    tune[playing[0]]->Stop();
  if ( playing[1] )
    tune[playing[1]]->Stop();
}
//---------------------------------------------------------------------------
void __fastcall Sample::PlaySample(int nbr)
{
  if ( tune[nbr] ) {
    tune[nbr]->SetCurrentPosition(0);
    tune[nbr]->Play(0, 0, 0);
  }
}
//---------------------------------------------------------------------------
int __fastcall Sample::Details(int nbr)
{
  if ( tune[nbr] )
    return Length[nbr];
  return 0;
}
//---------------------------------------------------------------------------
int __fastcall Sample::RealDetails(int nbr)
{
  if ( tune[nbr] )
    return RealLength[nbr];
  return 0;
}
//---------------------------------------------------------------------------
void __fastcall Sample::PlaySample(Byte nbr, Byte Channel)
{
HRESULT result;
int loop;

#ifdef LOG_SOUND
DWORD tickcount;

  tickcount = GetTickCount();

  fprintf(stream,"%08d Play sample %d Chan %d\r\n", tickcount, nbr, Channel);
#endif
  if ( nbr == 0 ) {
    if ( playing[0] ) {
      tune[playing[0]]->Stop();
      playing[0] = 0;
      }
    if ( Fruit )
      Fruit->samplefinished(0);
    if ( playing[1] ) {
      tune[playing[1]]->Stop();
      playing[1] = 0 ;
      }
    if ( Fruit )
      Fruit->samplefinished(1);
  } else {
    if ( playing[Channel] ) {
      tune[playing[Channel]]->Stop();
      playing[Channel] = 0 ;
    }
      if ( ( result = tune[nbr]->QueryInterface(
               IID_IDirectSoundNotify, (LPVOID *)&lpdsNotify)) != DS_OK )
      {
        printf("Notify Failed\n");
        return;
      }

      rgdsbpn[Channel].dwOffset = /*DSBPN_OFFSETSTOP;*/Length[nbr]- 10;

      loop = 0;
      do {
        result = lpdsNotify->SetNotificationPositions(1, &rgdsbpn[Channel]);
//      result = IDirectSoundNotify_SetNotificationPositions(
//              lpdsNotify, 1, &rgdsbpn[Channel]);
        if ( result != DS_OK )
          tune[nbr]->Stop();
        loop++;
      } while ( result != DS_OK && loop < 10 );

      if ( loop == 10 ) {
        if ( result == DSERR_UNSUPPORTED )
          printf("Notify Failed\n");
        if ( result == DSERR_INVALIDCALL  )
          printf("Notify Failed\n");
        Fruit->samplefinished(Channel);
        printf("Notify Failed\n");
        return;
      }
      tune[nbr]->SetCurrentPosition(0);
      tune[nbr]->Play(0, 0, 0);
      playing[Channel] = nbr;
    }
}
//---------------------------------------------------------------------------
void __fastcall Sample::PlaySample2(Byte nbr, Byte Channel)
{

  if ( nbr == 0 ) {
    if ( playing[0] ) {
      tune[playing[0]]->Stop();
      playing[0] = 0;
      }
    if ( playing[1] ) {
      tune[playing[1]]->Stop();
      playing[1] = 0 ;
      }
  } else {
    if ( playing[Channel] ) {
      tune[playing[Channel]]->Stop();
      playing[Channel] = 0 ;
    }
    tune[nbr]->SetCurrentPosition(0);
    tune[nbr]->Play(0, 0, 0);
    playing[Channel] = nbr;
  }
}
//---------------------------------------------------------------------------
void __fastcall Sample::LoadJPMSound(TStringList *SoundList, int volume, int rate)
{
TMemoryStream *file_ptr, *ptr, *Sound;
int count;
unsigned char bp[3];
int endaddr, address, Size, pages, nibbles, nibblecount;
short sample;
unsigned char value;
Byte samples;
int header;
int page;
int nbrsamples = 1;
int zeros;
int myrate;
int repeat = 0;
int repeat_offset;
bool valid_header;

  if ( SoundList->Count ) {
    file_ptr = new TMemoryStream();
    Sound = new TMemoryStream();
    SoundList->Sort();
    for ( count = 0; count < SoundList->Count; count++ ) {
      file_ptr->LoadFromFile(SoundList->Strings[count]);
      Sound->CopyFrom(file_ptr, 0);
    }
    delete file_ptr;
    for ( pages = 0; pages < 4; pages++ ) { // Scan pages
      page = pages * 0x20000;
      if ( page >= Sound->Size )
        break;
      Sound->Position = page;
      Sound->Read( &samples, 1);
      Sound->Read( &header, 4);
      if ((header != 0x5569A55A) || (samples == 0))
        return;
      for ( count = 0; count <= samples; count++ ) {
        repeat = 0;
        myrate = rate;
        Sound->Position = page + 5 + count * 2;
        Sound->Read( bp, 2);
        address = (bp[0] << 8) + bp[1];
        address <<= 1;
        if (count<samples) {
          Sound->Position = page + 5 + ((count+1) * 2);
          Sound->Read( bp, 2);
          endaddr = (bp[0] << 8) + bp[1];
          endaddr <<= 1;
          endaddr += page;
        } else
          endaddr = page + 0x20000;
        Sound->Position = page + address;
        Size = 0;
        ADPCMStatFlag.signal==0;
        ADPCMStatFlag.step=0; // 15 ?
        ADPCMStatFlag.last=0;
        ptr = new TMemoryStream();
        Sound->Read( &value, 1); // Start Byte
        valid_header = false;
        do {
          if ( repeat ) {
            repeat--;
            Sound->Position = repeat_offset;
          }
          Sound->Read( &value, 1);
          zeros = 0;
          switch ( value & 0xc0 ) {
            case 0: // Silence
              if ( ( value & 0x3f ) == 0 && valid_header )
                Sound->Position = endaddr;
              else {
                // Generate value & 0x3f's worth of silence
                valid_header = true;
                int silence_length = ( value & 0x3f ) * 20;
                ADPCMStatFlag.signal==0;
                ADPCMStatFlag.step=0; // 15 ?
                ADPCMStatFlag.last=0;
                for ( int i = 0; i < silence_length; i++ ) {
                  NECDecode(0, &ADPCMStatFlag);
                  NECDecode(0, &ADPCMStatFlag);
                  sample = ADPCMStatFlag.signal;
                  ptr->Write(&sample, 2);
                  Size += 2;
                }
              }
              nibbles = 0;
              break;
            case 0x40:  // 256 Nibbles
              if (myrate == 0)
                myrate = 160000 / ((value & 0x3f) + 1);
              nibbles = 256;
              valid_header = true;
              break;
            case 0x80: // n Nibbles
              if (myrate == 0)
                myrate = 160000 / ((value & 0x3f) + 1);
              Sound->Read( &value, 1);
              nibbles = value + 1;
              valid_header = true;
              break;
            case 0xc0: // Repeat Loop
              repeat = (value & 0x7) + 1;
              repeat_offset = Sound->Position;
              valid_header = true;
              break;
          }
/*          while ( (value & 0xc0) == 0 && zeros < 2 ) {
            Sound->Read( &value, 1);
            zeros++;
          }
          if ( (value & 0xc0) == 0xc0 || zeros == 2 )
            break;
          if ( value & 0x80 ) {
            if (myrate == 0)
              myrate = 160000 / ((value & 0x1f) + 1);
            Sound->Read( &value, 1);
            nibbles = value + 1;
          } else if ( value & 0x40 ) {
            if (myrate == 0)
              myrate = 160000 / ((value & 0x1f) + 1);
            nibbles = 256;
          } else
            nibbles = 0;   */
          if ( nibbles ) {
            nibblecount = 0;
            while ( nibblecount < nibbles ) {
              if ( nibblecount & 1 )
                NECDecode(value & 0xf, &ADPCMStatFlag);
              else {
                Sound->Read( &value, 1);
                NECDecode((value >> 4) & 0xf, &ADPCMStatFlag);
              }
              sample = ADPCMStatFlag.signal;
              ptr->Write(&sample, 2);
              Size += 2;
              nibblecount++;
            }
          }
        } while ( Sound->Position < endaddr);
        if ( Size > 0 ) {
          Length[nbrsamples] = Size;
          ptr->Position = 0;
          if ( AppCreateBasicBuffer( lpds, &tune[nbrsamples], myrate, Size, true) )
            AppWriteDataToBuffer( tune[nbrsamples], 0, (unsigned char *)ptr->Memory, Size);
        } else
          Length[nbrsamples] = 0;
        RealLength[ nbrsamples ] = Size;
        lookup[pages][count] = nbrsamples;
        tunes[nbrsamples] = ptr;
        rates[nbrsamples] = myrate;
        nbrsamples++;
//        delete ptr;
      }
    }
    if ( muted ) {
      muted = !muted;
      Mute();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall Sample::NECDecode(int val, UPD7759voice *voice)
{
   int smpval;
   smpval=(voice->last)+UPD7759_step[voice->step][val];
   voice->step+=UPD7759_state[val];
   if (voice->step < 0)
      voice->step=0;
   else if (voice->step > 15)
      voice->step=15;
   if (smpval>255)
      smpval=255;
   if (smpval<-255) smpval=-255;

   voice->last=smpval;
   smpval<<=7;
   if (smpval>SIGNAL_MAX)
      smpval=SIGNAL_MAX;
   else if (smpval<SIGNAL_MIN)
      smpval=SIGNAL_MIN;
   voice->signal=smpval;
}
//---------------------------------------------------------------------------
bool __fastcall Sample::SetSampleRate(int SampleRate)
{
WAVEFORMATEX wfx;

   memset(&wfx, 0, sizeof(WAVEFORMATEX));
   wfx.wFormatTag = WAVE_FORMAT_PCM;
   wfx.nChannels = 1;
   wfx.nSamplesPerSec = SampleRate;
   wfx.wBitsPerSample = 16;
   wfx.nBlockAlign = (wfx.wBitsPerSample * wfx.nChannels ) / 8;
   wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;

   if ( lpdsbPrimary->SetFormat(&wfx) != DS_OK )
     return false;

   return true;
}
//---------------------------------------------------------------------------
int __fastcall Sample::Playing(int nbr)
{
unsigned long status;

  if ( tune[nbr] && tune[nbr]->GetStatus(&status) == DS_OK ) {
    if ( status & DSBSTATUS_BUFFERLOST )
      printf("Buffer %d lost\r\n", nbr);
    if ( status & DSBSTATUS_PLAYING )
      return 1;
  }
  return 0;
}
//---------------------------------------------------------------------------
void __fastcall Sample::SaveToFile(int nbr, String Filename)
{
struct wave_header head;
TMemoryStream *ptr;
int length;

  head = header1;
  ptr = new TMemoryStream();
  length = tunes[nbr]->Size;
  head.sample_rate = rates[nbr];
  head.bytes_per_second = rates[nbr]*2;
  head.length = 36 + length;
  head.data_length = length;
  ptr->Write( &head, sizeof(head));
  tunes[nbr]->Position = 0;
  ptr->CopyFrom(tunes[nbr], length);
  ptr->SaveToFile(Filename);
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall Sample::DeleteTunes(void)
{
int i;

  for ( i = 0; i < 200; i++ ) {
    if ( tune[i] ) {
      tune[i]->Release();
      tune[i] = NULL;
      delete tunes[i];
      tunes[i] = NULL;
    }
    Length[i] = 0;
    RealLength[i] = 0;
  }
}

//---------------------------------------------------------------------------

void __fastcall Sample::AdjustRate(int Rate)
{
int i;

  for ( i = 0; i < 200; i++ ) {
    if ( tune[i] )
      tune[i]->SetFrequency(Rate);
  }
}

/****************************************************************

  Yamaha YMZ280B Support Routines

*****************************************************************/

void __fastcall Sample::Read32(TMemoryStream *tms, unsigned int *vtr)
{
   unsigned char sc, lp;

   *vtr=0;
   for (lp=0; lp<4; lp++)
   {
      tms->Read(&sc, 1);
      *vtr=*vtr+(sc << (8*(3-lp)));
   }
}

void __fastcall Sample::Read24(TMemoryStream *tms, unsigned int *vtr)
{
   unsigned char sc, lp;

   *vtr=0;
   for (lp=0; lp<3; lp++)
   {
      tms->Read(&sc, 1);
      *vtr=*vtr+(sc << (8*(2-lp)));
   }
}

void __fastcall Sample::Read16(TMemoryStream *tms, unsigned short *vtr)
{
   unsigned char sc, lp;

   *vtr=0;
   for (lp=0; lp<2; lp++)
   {
      tms->Read(&sc, 1);
      *vtr=*vtr+(sc << (8*(1-lp)));
   }
}

void __fastcall Sample::ymz280b_initadpcm(void)
{
	int nib;

	/* loop over all nibbles and compute the difference */
	for (nib = 0; nib < 16; nib++) {
		int value = (nib & 0x07) * 2 + 1;
		diff_lookup[nib] = (nib & 0x08) ? -value : value;
	}
}

short __fastcall Sample::ymz280b_decode(struct YMZ280BVoice *voice, unsigned char val)
{
	int signal = voice->signal;
	int step = voice->step;

   /* compute the new amplitude and update the current step */
   signal += (step * diff_lookup[val & 15]) / 8;

   /* clamp to the maximum */
   if (signal > 32767)
      signal = 32767;
   else if (signal < -32768)
      signal = -32768;

   /* adjust the step size and clamp */
   step = (step * index_scale[val & 7]) >> 8;

   if (step > 0x6000)
      step = 0x6000;
   else if (step < 0x7f)
      step = 0x7f;

	voice->signal = signal;
	voice->step = step;

	return signal;
}

void __fastcall Sample::LoadYMZSound(TStringList *SoundList, int volume)
{
TMemoryStream *file_ptr, *ptr;
int count;
unsigned int buffptr, keeppos, sampptr;
struct YMZ280BVoice YMZVoice;
int nbrsamples = 1;
unsigned char data;
short vsampl1, vsampl2;
unsigned short wdata;

  if ( SoundList->Count ) {
    file_ptr = new TMemoryStream();
    sound = new TMemoryStream();
    SoundList->Sort();
    for ( count = 0; count < SoundList->Count; count++ ) {
      file_ptr->LoadFromFile(SoundList->Strings[count]);
      sound->CopyFrom(file_ptr, 0);
    }
    delete file_ptr;

    sound->Position = 0;
    for (count=0; count<250; count++) {
        Read16(sound, &wdata);
        ymz_rates[nbrsamples] = (((wdata & 0xff00) >> 8) / 6) * 1000;
        sound->Read(&data, 1);
        buffptr = (data << 16);
        sound->Read(&data, 1);
        sampptr = (data << 16);
        sound->Read(&data, 1);
        buffptr |= (data << 8);
        sound->Read(&data, 1);
        sampptr |= (data << 8);
        sound->Read(&data, 1);
        buffptr |= data;
        sound->Read(&data, 1);
        sampptr |= data;

        if ((sampptr == 0) || (buffptr == 0))
          break;

        ymz_lookup[nbrsamples] = buffptr; // Found start address
        Length[nbrsamples] = sampptr - ymz_lookup[nbrsamples];
        sound->Position = sound->Position + 8;

        // Set up the next sample buffer
        ptr = new TMemoryStream();
        ymz280b_initadpcm();

        // Read in Sample
        YMZVoice.signal = 0;
        YMZVoice.step = 0x7f;
        keeppos = sound->Position;

        sound->Position = ymz_lookup[nbrsamples];

        for (sampptr = 0; sampptr < Length[nbrsamples]; sampptr++) {
            sound->Read(&data, 1);
            vsampl1 = ymz280b_decode(&YMZVoice, (data & 0xf0)>>4);
            vsampl2 = ymz280b_decode(&YMZVoice, (data & 0xf));
            ptr->Write(&vsampl2, 2);
            ptr->Write(&vsampl1, 2);
        }

        Length[nbrsamples] *= 2;
        ptr->Position = 0;
        if ( AppCreateBasicBuffer( lpds, &tune[nbrsamples], ymz_rates[nbrsamples], Length[nbrsamples] * 2, true) )
          AppWriteDataToBuffer( tune[nbrsamples], 0, (unsigned char *)ptr->Memory, Length[nbrsamples] * 2);
        tunes[nbrsamples] = ptr;
        RealLength[nbrsamples] = Length[nbrsamples];
        //ptr->Free();

        sound->Position=keeppos;

        // Last Sample?
        //if (buffptr & 0xff)
        //    break;

        nbrsamples++;
    }
  }
  nbrsamples = nbrsamples;
}

void __fastcall Sample::ymz280b_reset(void)
{
  ymz_curr_reg = 0;
  ymz_addr = 0;

  // stop playback etc
  for (int i = 0; i < 8; i++) {
    ymz_stop(i);
    ymzvoices[i].tunenum = 0;
    ymzvoices[i].looping = 0;
  }
}

void __fastcall Sample::ymz280b_writereg(int regnum)
{
  ymz_curr_reg = regnum;
}

void __fastcall Sample::ymz_start(int channum)
{
  int loop;
  HRESULT hr;

  for (loop = 0; loop < 200; loop++) {
    if ( ymzvoices[channum].start && ((ymzvoices[channum].start/2) == ymz_lookup[loop])) {
      ymzvoices[channum].tunenum = loop;
      if ( buf_tune[channum] )
        buf_tune[channum]->Release();
      hr = IDirectSound_DuplicateSoundBuffer( lpds, tune[loop], &buf_tune[channum]);
      //buf_tune[channum] = tune[loop];
      if ( Stereo )
        hr = buf_tune[channum]->SetPan((ymzvoices[channum].pan - 8) * 1200);
      else
        hr = buf_tune[channum]->SetPan(0);
      if ( !muted )
        hr = buf_tune[channum]->SetVolume((255 - ymzvoices[channum].vol) * -2);
      else
        hr = buf_tune[channum]->SetVolume(DSBVOLUME_MIN);
      buf_tune[channum]->Stop();
      buf_tune[channum]->SetCurrentPosition(0);
      if (ymzvoices[channum].looping)
        buf_tune[channum]->Play(0, 0, DSBPLAY_LOOPING);
      else
        buf_tune[channum]->Play(0, 0, 0);
      break;
    }
  }
}

void __fastcall Sample::ymz_stop(int channum)
{
  ymzvoices[channum].playing = 0;
  if (buf_tune[channum] != NULL)
    buf_tune[channum]->Stop();
}

void __fastcall Sample::ymz280b_writedata(unsigned char data)
{
  struct YMZVoice *thisvoice;
  int i;

	/* lower registers follow a pattern */
  if (ymz_curr_reg < 0x80) {
    thisvoice = &ymzvoices[(ymz_curr_reg >> 2) & 7];

      switch (ymz_curr_reg & 0xe3) {
        case 0x00:		/* pitch low 8 bits */
          thisvoice->rate = (thisvoice->rate & 0x100) | (data & 0xff);
          break;
        case 0x01:		/* pitch upper 1 bit, loop, key on, mode */
          thisvoice->rate = (thisvoice->rate & 0xff) | ((data & 0x01) << 8);
          thisvoice->looping = (data & 0x10) >> 4;
          thisvoice->mode = (data & 0x60) >> 5;
          if (!thisvoice->keyon && (data & 0x80) && ymz_keyon_enable) {
            thisvoice->playing = 1;
            ymz_start((ymz_curr_reg >> 2) & 7);
					//thisvoice->position = thisvoice->start;
					//thisvoice->signal = thisvoice->loop_signal = 0;
					//thisvoice->step = thisvoice->loop_step = 0x7f;
					//thisvoice->loop_count = 0;
          }
          if (thisvoice->keyon && !(data & 0x80) && !thisvoice->looping) {
            thisvoice->playing = 0;
            ymz_stop((ymz_curr_reg >> 2) & 7);
//					chip->status_register &= ~(1 << ((chip->current_register >> 2) & 7));
//					chip->status_register |= (1 << ((chip->current_register >> 2) & 7));
          }
          thisvoice->keyon = (data & 0x80) >> 7;
          break;
        case 0x02:		/* total level */
          thisvoice->vol = data;
          if (buf_tune[(ymz_curr_reg >> 2) & 7] != NULL)
            if ( !muted )
              buf_tune[(ymz_curr_reg >> 2) & 7]->SetVolume((255 - thisvoice->vol) * -2);
            else
              buf_tune[(ymz_curr_reg >> 2) & 7]->SetVolume(DSBVOLUME_MIN);

          break;
        case 0x03:		/* pan */
          thisvoice->pan = data & 0x0f;
          if (buf_tune[(ymz_curr_reg >> 2) & 7] != NULL)
            buf_tune[(ymz_curr_reg >> 2) & 7]->SetPan((thisvoice->pan - 8) * 1200);
          break;
        case 0x20:		/* start address high */
          thisvoice->start = (thisvoice->start & (0x00ffff << 1)) | (data << 17);
          break;
        case 0x21:		/* loop start address high */
          thisvoice->loop_start = (thisvoice->loop_start & (0x00ffff << 1)) | (data << 17);
          break;
        case 0x22:		/* loop end address high */
          thisvoice->loop_end = (thisvoice->loop_end & (0x00ffff << 1)) | (data << 17);
          break;
        case 0x23:		/* stop address high */
          thisvoice->stop = (thisvoice->stop & (0x00ffff << 1)) | (data << 17);
          break;
        case 0x40:		/* start address middle */
          thisvoice->start = (thisvoice->start & (0xff00ff << 1)) | (data << 9);
          break;
        case 0x41:		/* loop start address middle */
          thisvoice->loop_start = (thisvoice->loop_start & (0xff00ff << 1)) | (data << 9);
          break;
        case 0x42:		/* loop end address middle */
          thisvoice->loop_end = (thisvoice->loop_end & (0xff00ff << 1)) | (data << 9);
          break;
        case 0x43:		/* stop address middle */
          thisvoice->stop = (thisvoice->stop & (0xff00ff << 1)) | (data << 9);
          break;
        case 0x60:		/* start address low */
          thisvoice->start = (thisvoice->start & (0xffff00 << 1)) | (data << 1);
          break;
        case 0x61:		/* loop start address low */
          thisvoice->loop_start = (thisvoice->loop_start & (0xffff00 << 1)) | (data << 1);
          break;
        case 0x62:		/* loop end address low */
          thisvoice->loop_end = (thisvoice->loop_end & (0xffff00 << 1)) | (data << 1);
          break;
        case 0x63:		/* stop address low */
          thisvoice->stop = (thisvoice->stop & (0xffff00 << 1)) | (data << 1);
          break;
      }
  } else {
    // upper registers are special
    switch (ymz_curr_reg) {
      case 0x84:		/* rom address high */
        ymz_addr = (ymz_addr & 0x00ffff) | (data << 16);
        break;
      case 0x85:		/* rom address middle */
        ymz_addr = (ymz_addr & 0xff00ff) | (data << 8);
        break;
      case 0x86:		/* rom address low */
        ymz_addr = (ymz_addr & 0xffff00) | data;
        break;
      case 0xfe:   // IRQ Mask
        ymz_irqmask = data;
        break;
      case 0xff:		/* IRQ enable, test, etc */
        if (ymz_keyon_enable && !(data & 0x80)) {
          for (i = 0; i < 8; i++) {
            ymzvoices[i].playing = 0;
            ymz_stop(i);
          }
        }
        if (!ymz_keyon_enable && (data & 0x80)) {
          for (i = 0; i < 8; i++) {
          if (ymzvoices[i].keyon && ymzvoices[i].looping) {
            ymzvoices[i].playing = 1;
            ymz_start(i);  // start playback
          }
        }
      }
      ymz_keyon_enable = (data & 0x80) >> 7;
      break;
    }
  }
}

int __fastcall Sample::ymz280b_readstatus(void)
{
  int loop;
  unsigned long PlayCursor;
  unsigned char dat;

  ymz_status = 0;
  for (loop = 0; loop < 8; loop ++) {
     if ((buf_tune[loop] != NULL) && (ymzvoices[loop].playing)) {
         buf_tune[loop]->GetStatus(&PlayCursor);
         if (!(PlayCursor & DSBSTATUS_PLAYING)) {
          ymz_status |= (1 << loop);
          ymzvoices[loop].playing = 0;
         }
     }
  }

  dat = ymz_status;
  ymz_status = 0x00;
  return dat;
}

int __fastcall Sample::ymz280b_checkirq(void)
{
  int loop;
  unsigned long PlayCursor;
  unsigned char dat;

  dat = 0;
  for (loop = 0; loop < 8; loop ++) {
     if ((buf_tune[loop] != NULL) && (ymzvoices[loop].playing)) {
         buf_tune[loop]->GetStatus(&PlayCursor);
         if (!(PlayCursor & DSBSTATUS_PLAYING)) {
          dat |= (1 << loop);
         }
     }
  }

  return dat & ymz_irqmask;
}

int __fastcall Sample::ymz280b_readdata(void)
{
  unsigned char dat = 0;

  if ( sound ) {
    sound->Position = ymz_addr;
    sound->Read(&dat, 1);
  }
  ymz_addr++;
  return (int)dat;
}

void __fastcall Sample::SetMasterVolume(int level)
{
  lpdsbPrimary->SetVolume((level-100) * 30);
}

void __fastcall Sample::SetStereo(int Astereo)
{
  Stereo = Astereo;
}

bool __fastcall Sample::Mute()
{
int volume;

  if ( !muted )
    volume = DSBVOLUME_MIN;
  else
    volume = 0;
  soundbuff->SetVolume(volume);
  soundbuff2->SetVolume(volume);
  for ( int i = 0; i < 200; i++ ) {
    if ( tune[i] )
      tune[i]->SetVolume(volume);
  }
  for ( int i = 0; i < 8; i++ ) {
    if ( buf_tune[i] )
      buf_tune[i]->SetVolume(volume);
  }
  muted = !muted;

  return muted;
}


