//---------------------------------------------------------------------------
#ifndef mpu4videoH
#define mpu4videoH

#include "m68kcpu.h"
#include "mc6840.h"
#include "mc6850.h"
#include "saa1099.h"
#include "scn2674.h"

class TMPU4VIDEO;

class MPU4VIDEO : public mc68000 {

public:
  int  m68k_read_immediate_8(uint address, int cs);
  int  m68k_read_immediate_16(uint address, int cs);
  int  m68k_read_immediate_32(uint address, int cs);

/* Read an instruction (16-bit word immeditately after PC) */
  int  m68k_read_instruction(uint address, int cs);

/* Write to anywhere */
  void m68k_write_memory_8(uint address, int value, int cs);
  void m68k_write_memory_16(uint address, int value, int cs);
  void m68k_write_memory_32(uint address, int value, int cs);
  int m68k_read_memory_8(uint address, int cs);
  int m68k_read_memory_16(uint address, int cs);
  int m68k_read_memory_32(uint address, int cs);
  void           buildrowtable();
  int			 execute(void);
  mc6840		 ptm;
  saa1099        sound;
  mc6850         acia;
  scn2674        vdc;
  int            refresh;
  Byte           status;
  int            divider;
  bool           acia_int;
  unsigned char  rom[0x800000];
  unsigned char  ram[0x10000];
  unsigned char  video[0x20000];
  unsigned short rowtable[64 * 37];
  Byte           palette[32];
  Byte           palettepos;
  bool           paletteupdated;
  bool           gfx_written;
  TMPU4VIDEO     *parent;
};

class TMPU4VIDEO : public TSystem
{
public:
  MPU4VIDEO board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
//	int            execute(void);
//    void        step(void);
//    void        halt(void);
public:
	__fastcall TMPU4VIDEO(String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall hard_reset(void);
    void __fastcall saveram();
    void __fastcall load_intelhex();
    void __fastcall UpdateDebug(void);
    void __fastcall UpdateScreen(void);
    void __fastcall UpdateRow(void);
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateWave(void);
    int  __fastcall getpc(void);
    int  __fastcall getstack(void);
    int  __fastcall getstacktop(void);
    int  __fastcall getmemory(int address);
    void __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
//---------------------------------------------------------------------------
#endif
