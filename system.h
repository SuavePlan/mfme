//---------------------------------------------------------------------------
#ifndef systemH
#define systemH

#include <stdio.h>

#define TRACE_SIZE 4000

#define savestate(x,y) memcpy( x, &y, sizeof(y)); x+=sizeof(y)
#define readstate(x,y) memcpy( &y, x, sizeof(y)); x+=sizeof(y)

#define savestatesize(x,y,z) memcpy( x, &y, z); x+=z
#define readstatesize(x,y,z) memcpy( &y, x, z); x+=z

enum Arch_Types {
  UNKNOWN,
  BARCREST_MPU3,
  BARCREST_MPU4,
  JPM_IMPACT,
  MAYGAY_M1A,
  JPM_MPS2,
  ACE_SYSTEM1,
  BFM_SCORPION2,
  BFM_DOTMATRIX,
  JPM_SYS80,
  JPM_SYS5,
  BARCREST_MPU4VIDEO,
  BFM_SCORPION1,
  PROJECT_PROCONN,
  ACE_SPACE
};

enum Machine_states {
  MS_RUNNING,
  MS_STEPPING,
  MS_READ_BP,
  MS_WRITE_BP,
  MS_EXECUTE_BP,
  MS_RESET,
  MS_RESUME,
  MS_HALTED
};

class lamps;
class led;
class triacs;
class meters;
class jpmreels;
class reels;
class SEC;
class alpha;
class BFMalpha;
class BFMDotAlpha;

class TSystem : public TThread {

// Internal registers

public:
    Arch_Types board_type;
    Machine_states state;
    bool in_int;
    unsigned int bp;
    unsigned int page_bp;
    unsigned int read_bp;
    unsigned int read_mask;
    int read_addr;
    int read_value;
    int read_count;
    bool readbp;
    unsigned int read_value_bp;
    unsigned int read_value_mask;
    unsigned int write_bp;
    unsigned int write_mask;
    int write_addr;
    int write_value;
    int write_count;
    bool writebp;
    unsigned int write_value_bp;
    unsigned int write_value_mask;
    unsigned int sts;
    bool errors;
    struct trace_buf {
      unsigned int pc;
      unsigned char page;
      unsigned char a;
      unsigned char b;
      unsigned short x;
      unsigned short y;
      unsigned short u;
      unsigned short s;
    } trace[4000];
    bool Stopped;
    unsigned int Total_cycles;
    unsigned int Last_cycles;
    unsigned int lamp_updates;
    unsigned int led_updates;
    unsigned int alpha_updates;
    unsigned int reel_updates;
    unsigned int sound_updates;
    int reeltimer;
    String CurrGamePath;
    String Game;
    TStringList *ROMList;
    int Delay;
    int Delay2;
protected:

// Read and write functions
public:

// Public constructor and destructor
         __fastcall TSystem(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall do_meter(void);
    void __fastcall do_triac(void);
    void __fastcall do_lamp(void);
    void __fastcall do_reel_lamp(void);
    void __fastcall do_display(void);
    void __fastcall do_dot_display(void);
    void __fastcall do_screen(void);
    void __fastcall do_row(void);
    void __fastcall do_matrix(void);
    void __fastcall do_reel(void);
    void __fastcall do_led_display(void);
    void __fastcall do_debug(void);
    void __fastcall do_wave(void);
    void __fastcall do_pak(void);
    void __fastcall do_trace(void);
    void __fastcall do_status(void);
    void __fastcall do_io_meters(void);
    void __fastcall do_io_triacs(void);
    void __fastcall do_io_sinks(void);
    void __fastcall do_io_selects(void);
    void __fastcall do_io_reels(void);
    void __fastcall do_sec(void);
    void __fastcall do_bitmap(void);
    int __fastcall LatchState();
    void __fastcall Execute();
    virtual void __fastcall Run();
    virtual void __fastcall Run2();
    virtual void __fastcall load_intelhex();
    void __fastcall saveram(Byte *ptr, int size);
    virtual  void __fastcall saveram();
    virtual void __fastcall UpdateDebug(void);
    virtual void __fastcall UpdateStatus(void);
    void __fastcall UpdateStatuss(Byte status);
    virtual void __fastcall UpdateDisplay(void);
    virtual void __fastcall UpdateScreen(void);
    virtual void __fastcall UpdateRow(void);
    void __fastcall UpdateScreens(Byte *map, Word *rowtable);
    void __fastcall UpdateRows(Byte *palette, Byte *map, Word *rowtable, bool updated, Byte row);
    void __fastcall UpdateDisplays(alpha *alpha_ptr, int nbr);
    void __fastcall UpdateDisplays(BFMalpha *alpha_ptr, int nbr);
    void __fastcall UpdateDisplays(BFMDotAlpha *alpha_ptr, int nbr);
    virtual void __fastcall UpdateMatrix(void);
    void __fastcall UpdateMatrixs(Byte *map);
    void __fastcall UpdateACEMatrix(Byte *map);
    void __fastcall UpdatePROCONNMatrix(Byte *map);
    virtual void __fastcall UpdateReel(void);
    void __fastcall UpdateReels(jpmreels *reel_ptr);
    void __fastcall UpdateReels(reels *reel_ptr);
    virtual void __fastcall UpdateReelLamp(void);
    void __fastcall UpdateReelLamps(jpmreels *ptr);
    virtual void __fastcall UpdateLamp(void);
    void __fastcall UpdateLamps(lamps *lamp_ptr, int nbr );
    virtual void __fastcall UpdateMeter(void);
    void __fastcall UpdateMeters(meters *meter_ptr);
    void __fastcall MeterSensed(meters *meter_ptr, Byte state);
    virtual void __fastcall UpdateTriac(void);
    void __fastcall UpdateTriacs(triacs *triac_ptr);
    void __fastcall UpdateSenseTriacs(triacs *triac_ptr);
    virtual void __fastcall UpdateTrace(void);
    virtual void __fastcall UpdateLedDisplay(void);
    void __fastcall UpdateLedDisplays(led *led_ptr);
    virtual void __fastcall UpdateWave(void);
    void __fastcall UpdateWaves(int tune, int channel);
    virtual void __fastcall UpdatePak(void);
    virtual void __fastcall UpdateIOMeters(void);
    virtual void __fastcall UpdateIOTriacs(void);
    virtual void __fastcall UpdateIOSinks(void);
    virtual void __fastcall UpdateIOSelects(void);
    virtual void __fastcall UpdateIOReels(void);
    virtual void __fastcall UpdateSEC(void);
    virtual void __fastcall UpdateMouse(void);
    void __fastcall UpdateSECs(SEC *sec_ptr);
    virtual void __fastcall hard_reset(void);
    virtual int __fastcall getpc(void) = 0;
    virtual int __fastcall getstack();
    virtual int __fastcall getstacktop();
    virtual int __fastcall getmemory(int address) = 0;
    virtual void __fastcall setmemory(int address, Byte value) = 0;
    virtual int __fastcall dasm(int pc, char *buffer) = 0;
    virtual void __fastcall samplefinished(int Channel);
    virtual void __fastcall ClearRAM(void) = 0;
    virtual void __fastcall SaveToStream(Byte *&ptr);
    virtual void __fastcall LoadFromStream(Byte *&ptr);
    virtual bool __fastcall AutoLamp(int nbr, int &reel, int &symbol);
    void __fastcall WriteCharToMPU4(char ch);
    void __fastcall WriteCharToVideo(char ch);
    void __fastcall MPU4CharReceived();
    void __fastcall VideoCharReceived();
    void __fastcall SetMPU4DCD(Byte value);
    void __fastcall SetMPU4CTS(Byte value);
    void __fastcall CreateAutoLamp(void);
    void __fastcall IntelHexRead(byte *ptr, String filename);
    int __fastcall get_next(FILE *fp, int size, int *csum);
    int __fastcall SetReels(reels *reel_ptr, int nbr, int adjust);
    int __fastcall SetReels(jpmreels *reel_ptr, int nbr, int adjust);
};
extern TSystem *Fruit;
extern TSystem *Display;
//---------------------------------------------------------------------------
#endif
