//---------------------------------------------------------------------------
#ifndef dotmatrixH
#define dotmatrixH
//---------------------------------------------------------------------------
//#include <vcl\Classes.hpp>
//---------------------------------------------------------------------------

#include "system.h"
#include "mc6809.h"
#include "alpha.h"

class TDOTMATRIX;

class DOTMATRIX : public mc6809 {

public:
	Byte			 read(Word);
	void			 write(Word, Byte);
    int			     execute(void);
    alpha			 display;
    int              refresh;
    int              fiftyHz;
    bool             irq;
    bool             firq;
    bool             updated;
    Byte             chr;
    int              pos;
    int              bcount;
    int              blocks;
    int              status;
    int              latch;
    Byte             segdata[9];
    Byte             map[9*21];
    Byte             memory[0x10000];
    Byte             ram[0x2000];
    Byte             queue[500];
    int              readpos;
    int              writepos;
    TDOTMATRIX *     parent;
};

class TDOTMATRIX  : public TSystem
{
public:
  DOTMATRIX board;

  protected:
	void __fastcall Run();
	void __fastcall Run2();
public:
	__fastcall TDOTMATRIX (String AGame, String AGamePath, TStringList *AROM_list);
    void __fastcall sendchar(Byte chr);
    void __fastcall QueueChar(Byte ch);
    void __fastcall hard_reset(void);
    void __fastcall load_intelhex();
    void __fastcall UpdateTrace(void);
    void __fastcall UpdateDebug(void);
    void __fastcall UpdateMatrix(void);
    int  __fastcall getpc(void);
    int  __fastcall getmemory(int address);
    void  __fastcall setmemory(int address, Byte value);
    int  __fastcall dasm(int pc, char *buffer);
    void  __fastcall ClearRAM(void);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};
extern TDOTMATRIX  *dotmatrix;
//---------------------------------------------------------------------------
#endif
