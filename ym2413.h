#ifndef _EMU2413_H_
#define _EMU2413_H_

#undef int8
#undef int16
#undef int32
#undef uint
#undef uint8
#undef uint16
#undef uint

#define int8   signed char			/* ASG: changed from char to signed char */
#define uint8  unsigned char
#define int16  short
#define uint16 unsigned short
#define int32  long
#define uint   unsigned int

/* voice data */
typedef struct {
  unsigned int TL,FB,EG,ML,AR,DR,SL,RR,KR,KL,AM,PM,WF ;
  int update ; /* for real-time update */
} OPLL_PATCH ;

/* slot */
typedef struct {

  OPLL_PATCH *patch;

  int type ;          /* 0 : modulator 1 : carrier */
  int update ;

  /* OUTPUT */
  int output[2] ;      /* Output value of slot */

  /* for Phase Generator (PG) */
  int *sintbl ;     /* Wavetable */
  uint phase ;      /* Phase */
  uint dphase ;     /* Phase increment amount */
  uint pgout ;      /* output */

  /* for Envelope Generator (EG) */
  int fnum ;          /* F-Number */
  int block ;         /* Block */
  int volume ;        /* Current volume */
  int sustine ;       /* Sustine 1 = ON, 0 = OFF */
  uint tll ;	      /* Total Level + Key scale level*/
  uint rks ;        /* Key scale offset (Rks) */
  int eg_mode ;       /* Current state */
  uint eg_phase ;   /* Phase */
  uint eg_dphase ;  /* Phase increment amount */
  uint egout ;      /* output */

  /* LFO (refer to opll->*) */
  int *plfo_am ;
  int *plfo_pm ;

} OPLL_SLOT ;

/* Channel */
typedef struct {

  int patch_number ;
  int key_status ;
  OPLL_SLOT *mod, *car ;

#ifdef OPLL_ENABLE_DEBUG
  int debug_keyonpatch[4] ;
#endif

} OPLL_CH ;

/* opll */
typedef struct {

  int output[4] ;

  /* Register */
  unsigned char reg[0x40] ;
  int slot_on_flag[18] ;

  /* Rythm Mode : 0 = OFF, 1 = ON */
  int rythm_mode ;

  /* Pitch Modulator */
  uint pm_phase ;
  uint pm_dphase ;
  int lfo_pm ;

  /* Amp Modulator */
  uint am_phase ;
  uint am_dphase ;
  int lfo_am ;

  /* Noise Generator */
  uint whitenoise ;

  /* Channel & Slot */
  OPLL_CH *ch[9] ;
  OPLL_SLOT *slot[18] ;

  /* Voice Data */
  OPLL_PATCH *patch[19] ;
  int user_patch_update[2] ; /* flag for check patch update */

  int mask[10] ; /* mask[9] = RYTHM */

  int masterVolume ; /* 0min -- 64 -- 127 max (Liner) */
  int rythmVolume ;  /* 0min -- 64 -- 127 max (Liner) */

} OPLL ;

class ym2413 {
private:

private:
  OPLL *opll;
  int ym_latch;
  void __fastcall OPLL_init(int c, int r) ;
  void __fastcall OPLL_close(void) ;
  OPLL *__fastcall OPLL_new(void) ;
  void __fastcall OPLL_reset(OPLL *) ;
  void __fastcall OPLL_reset_patch(OPLL *) ;
  void __fastcall OPLL_delete(OPLL *) ;
  void __fastcall OPLL_writeReg(OPLL *, uint, uint) ;
  int16 __fastcall OPLL_calc(OPLL *) ;
  void __fastcall OPLL_setPatch(OPLL *, int, OPLL_PATCH *) ;
  void __fastcall OPLL_forceRefresh(OPLL *) ;
  void __fastcall dump2patch(unsigned char *dump, OPLL_PATCH *patch) ;
  int __fastcall Max(int i,int j);
  int __fastcall Min(int i,int j);
  void __fastcall makeAdjustTable(void);
  void __fastcall makeDB2LinTable(void);
  int __fastcall lin2db(double d);
  void __fastcall makeSinTable(void);
  void __fastcall makePmTable(void);
  void __fastcall makeDphaseTable(void);
  void __fastcall makeAmTable(void);
  void __fastcall makeTllTalbe(void);
  void __fastcall makeDphaseARTable(void);
  void __fastcall makeDphaseDRTable(void);
  void __fastcall makeRksTable(void);
  void __fastcall makeDefaultPatch(void);
  uint __fastcall calc_eg_dphase(OPLL_SLOT *slot);
  void __fastcall refresh(OPLL_SLOT *slot);
  void __fastcall slotOn(OPLL_SLOT *slot);
  void __fastcall slotOff(OPLL_SLOT *slot);
  void __fastcall keyOn(OPLL *opll, int i);
  void __fastcall keyOff(OPLL *opll, int i);
  void __fastcall keyOn_BD(OPLL *opll);
  void __fastcall keyOn_SD(OPLL *opll);
  void __fastcall keyOn_TOM(OPLL *opll);
  void __fastcall keyOn_HH(OPLL *opll);
  void __fastcall keyOn_CYM(OPLL *opll);
  void __fastcall keyOff_BD(OPLL *opll);
  void __fastcall keyOff_SD(OPLL *opll);
  void __fastcall keyOff_TOM(OPLL *opll);
  void __fastcall keyOff_HH(OPLL *opll);
  void __fastcall keyOff_CYM(OPLL *opll);
  void __fastcall setPatch(OPLL *opll, int i, int num);
  void __fastcall setSlotPatch(OPLL_SLOT *slot, OPLL_PATCH *patch);
  void __fastcall setSustine(OPLL *opll, int c, int sustine);
  void __fastcall setVolume(OPLL *opll, int c, int volume);
  void __fastcall setSlotVolume(OPLL_SLOT *slot, int volume);
  void __fastcall setVolume_BD(OPLL *opll, int volume);
  void __fastcall setVolume_HH(OPLL *opll, int volume);
  void __fastcall setVolume_SD(OPLL *opll, int volume);
  void __fastcall setVolume_TOM(OPLL *opll, int volume);
  void __fastcall setVolume_CYM(OPLL *opll, int volume);
  void __fastcall setBlock(OPLL *opll, int c, int block);
  void __fastcall setRythmMode(OPLL *opll, int mode);
  void __fastcall OPLL_copyPatch(OPLL *opll, int num, OPLL_PATCH *patch);
  void __fastcall setFnumber(OPLL *opll, int c, int fnum);
  void __fastcall OPLL_SLOT_reset(OPLL_SLOT *slot);
  OPLL_SLOT * __fastcall OPLL_SLOT_new(void);
  void __fastcall OPLL_SLOT_delete(OPLL_SLOT *slot);
  void __fastcall OPLL_CH_reset(OPLL_CH *ch);
  OPLL_CH * __fastcall OPLL_CH_new(void);
  void __fastcall OPLL_CH_delete(OPLL_CH *ch);
  uint __fastcall mrand(void);
  void __fastcall update_noise(OPLL *opll);
  void __fastcall update_ampm(OPLL *opll);
  uint __fastcall calc_phase(OPLL_SLOT *slot, int lfo_pm);
  uint __fastcall calc_envelope(OPLL_SLOT *slot, uint lfo_am);
  int __fastcall calc_slot_car(OPLL_SLOT *slot, int fm);
  int __fastcall calc_slot_mod(OPLL_SLOT *slot);
  int __fastcall calc_slot_tom(OPLL_SLOT *slot);
  int __fastcall calc_slot_snare(OPLL_SLOT *slot, uint whitenoise);
  int __fastcall calc_slot_hat(OPLL_SLOT *slot, int fm, int whitenoise);
public:
  void __fastcall write_reg(int data);
  void __fastcall write_data(int data);
  void __fastcall reset(void);
  void __fastcall stop(void);
  void Update(short *buffer, int length, uint clks);
__fastcall ym2413::ym2413(void);
};
#endif



