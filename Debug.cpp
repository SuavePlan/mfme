//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "system.h"
#include "Debug.h"
#include "mpu3.h"
#include "mpu4.h"
#include "impact.h"
#include "scorpion1.h"
#include "scorpion2.h"
#include "sys5.h"
#include "proconn.h"
#include "m1a.h"
#include "ace1.h"
#include "mps2.h"
#include "sys80.h"
#include "space.h"
#include "dotmatrix.h"
#include "mpu4video.h"
#include "shared.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TDebugForm *MainDebug;
TDebugForm *MatrixDebug;
TDebugForm *VideoDebug;
//---------------------------------------------------------------------------
__fastcall TDebugForm::TDebugForm(TComponent* Owner)
    : TForm(Owner)
{
  offset = 0;
  calls = 0;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::SetProcess(TSystem *Process, Arch_Types Type)
{
  process = Process;
  type = Type;
  switch ( type ) {
    default:
      Caption = "MainBoard";
      switch ( type ) {
        case ACE_SPACE:
        case BFM_SCORPION1:
        case BFM_SCORPION2:
          ScrollBar1->Max = 0xff80 / 16;
          break;
        default:
          ScrollBar1->Max = 0xFFFFFF8;
          break;
      }
    break;
    case BFM_DOTMATRIX:
      Caption = "DotMatrix Board";
      break;
    case BARCREST_MPU4VIDEO:
      Caption = "Video Board";
      break;
  }
  FormShow(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ClearProcess()
{
  process = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::StopClick(TObject *Sender)
{
  if ( process != NULL ) {
    Timer2->Enabled = false;
    process->state = MS_EXECUTE_BP;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::StepClick(TObject *Sender)
{
  if ( process != NULL ) {
    process->in_int = 0;
    process->state = MS_STEPPING;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::RunClick(TObject *Sender)
{
  if ( process != NULL ) {
    process->state = MS_RUNNING;
    Run->Enabled = FALSE;
    Step->Enabled = FALSE;
    FastStep->Enabled = FALSE;
    Reset->Enabled = FALSE;
    Stop->Enabled = TRUE;
    if ( Update->Checked )
      Timer1->Enabled = TRUE;
  }
}
//---------------------------------------------------------------------------


void __fastcall TDebugForm::Timer1Timer(TObject *Sender)
{
  if ( process != NULL )
    process->do_debug();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ResetClick(TObject *Sender)
{
  if ( process != NULL ) {
    Timer2->Enabled = false;
    process->hard_reset();
    process->do_debug();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CodeClick(TObject *Sender)
{
String address;

  address = Code->Items->Strings[Code->ItemIndex];
  Break1->Text = address.SubString(1,address.Pos(" ")-1);
  CbExecute->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CbExecuteClick(TObject *Sender)
{

  if ( process != NULL ) {
    if ( !CbExecute->Checked ) {
      process->bp = -1;
    } else {
      if ( Break1->Text.SubString(3,1) == ":" ) {
        process->page_bp = Break1->Text.SubString(1,2).ToIntDef(0);
        process->bp = StrToHex( Break1->Text.SubString(4, Break1->Text.Length() - 3));
      } else {
        process->bp = StrToHex(Break1->Text);
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Halt(void)
{
  if ( !Timer2->Enabled )
    Stop->Enabled = false;
  Step->Enabled = true;
  FastStep->Enabled = true;
  Run->Enabled = true;
  Reset->Enabled = true;
  Timer1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::RegChange(TObject *Sender)
{
DWORD value;


  if ( process != NULL && !updating ) {
    value = StrToHex(((TEdit *)Sender)->Text);
    if ( Sender == M6809_A )
      ((TMPU4 *)process)->board.dd.b.h = value;
    else if ( Sender == M6809_B )
      ((TMPU4 *)process)->board.dd.b.l = value;
    else if ( Sender == M6809_D )
      ((TMPU4 *)process)->board.dd.w.l = value;
    else if ( Sender == M6809_X )
      ((TMPU4 *)process)->board.x.w.l = value;
    else if ( Sender == M6809_Y )
      ((TMPU4 *)process)->board.y.w.l = value;
    else if ( Sender == M6809_U )
      ((TMPU4 *)process)->board.u.w.l = value;
    else if ( Sender == M6809_S )
      ((TMPU4 *)process)->board.s.w.l = value;
    else if ( Sender == M6809_PC )
      ((TMPU4 *)process)->board.pc.w.l = value;
    else if ( Sender == M6809_flags )
      ((TMPU4 *)process)->board.cc = StrToBin(M6809_flags->Text.c_str());
    else if ( Sender == Pagelatch )
      ((TMPU4 *)process)->board.page = value;
    else if ( Sender == M6800_A )
      ((TMPU3 *)process)->board.d.b.h = value;
    else if ( Sender == M6800_B )
      ((TMPU3 *)process)->board.d.b.l = value;
    else if ( Sender == M6800_D )
      ((TMPU3 *)process)->board.d.w.l = value;
    else if ( Sender == M6800_X )
      ((TMPU3 *)process)->board.x.w.l = value;
    else if ( Sender == M6800_S )
      ((TMPU3 *)process)->board.s.w.l = value;
    else if ( Sender == M6800_PC )
      ((TMPU3 *)process)->board.pc.w.l = value;
    else if ( Sender == M6800_flags )
      ((TMPU3 *)process)->board.cc = StrToBin(M6800_flags->Text.c_str());
    else if ( Sender == M68000PC )
      ((TIMPACT *)process)->board.m68k_cpu.pc = value;
    else if ( Sender == M68000D0 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[0] = value;
    else if ( Sender == M68000D1 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[1] = value;
    else if ( Sender == M68000D2 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[2] = value;
    else if ( Sender == M68000D3 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[3] = value;
    else if ( Sender == M68000D4 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[4] = value;
    else if ( Sender == M68000D5 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[5] = value;
    else if ( Sender == M68000D6 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[6] = value;
    else if ( Sender == M68000D7 )
      ((TIMPACT *)process)->board.m68k_cpu.dr[7] = value;
    else if ( Sender == M68000A0 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[0] = value;
    else if ( Sender == M68000A1 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[1] = value;
    else if ( Sender == M68000A2 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[2] = value;
    else if ( Sender == M68000A3 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[3] = value;
    else if ( Sender == M68000A4 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[4] = value;
    else if ( Sender == M68000A5 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[5] = value;
    else if ( Sender == M68000A6 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[6] = value;
    else if ( Sender == M68000A7 )
      ((TIMPACT *)process)->board.m68k_cpu.ar[7] = value;
    else if ( Sender == TMS9995_PC )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_PC, value);
    else if ( Sender == TMS9995_WP )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_WP, value);
    else if ( Sender == TMS9995_R0 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R0, value);
    else if ( Sender == TMS9995_R1 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R1, value);
    else if ( Sender == TMS9995_R2 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R2, value);
    else if ( Sender == TMS9995_R3 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R3, value);
    else if ( Sender == TMS9995_R4 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R4, value);
    else if ( Sender == TMS9995_R5 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R5, value);
    else if ( Sender == TMS9995_R6 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R6, value);
    else if ( Sender == TMS9995_R7 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R7, value);
    else if ( Sender == TMS9995_R8 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R8, value);
    else if ( Sender == TMS9995_R9 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R9, value);
    else if ( Sender == TMS9995_R10 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R10, value);
    else if ( Sender == TMS9995_R11 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R11, value);
    else if ( Sender == TMS9995_R12 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R12, value);
    else if ( Sender == TMS9995_R13 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R13, value);
    else if ( Sender == TMS9995_R14 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R14, value);
    else if ( Sender == TMS9995_R15 )
      ((TMPS2 *)Fruit)->board.set_reg(TMS9900_R15, value);
    else if ( Sender == Z80_PC )
      ((TACE1 *)Fruit)->board.R.PC.D = value;
    else if ( Sender == Z80_AF )
      ((TACE1 *)Fruit)->board.R.AF.D = value;
    else if ( Sender == Z80_BC )
      ((TACE1 *)Fruit)->board.R.BC.D = value;
    else if ( Sender == Z80_DE )
      ((TACE1 *)Fruit)->board.R.DE.D = value;
    else if ( Sender == Z80_HL )
      ((TACE1 *)Fruit)->board.R.HL.D = value;
    else if ( Sender == Z80_AAF )
      ((TACE1 *)Fruit)->board.R.AF2.D = value;
    else if ( Sender == Z80_ABC )
      ((TACE1 *)Fruit)->board.R.BC2.D = value;
    else if ( Sender == Z80_ADE )
      ((TACE1 *)Fruit)->board.R.DE2.D = value;
    else if ( Sender == Z80_AHL )
      ((TACE1 *)Fruit)->board.R.HL2.D = value;
    else if ( Sender == Z80_IX )
      ((TACE1 *)Fruit)->board.R.IX.D = value;
    else if ( Sender == Z80_IY )
      ((TACE1 *)Fruit)->board.R.IY.D = value;
    else if ( Sender == Z80_SP )
      ((TACE1 *)Fruit)->board.R.SP.D = value;
    //else if ( Sender == Z80_FLAGS )
    //  ((TACE1 *)Fruit)->board.R.AF.B.l = StrToBin(Z80_FLAGS->Text);
  }

}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::UpdateClick(TObject *Sender)
{
  if ( !Update->Checked )
    Timer1->Enabled = FALSE;
  else
    if ( process != NULL )
      if ( process->state == 0 )
        Timer1->Enabled = TRUE;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::BreakChange(TObject *Sender)
{
unsigned int addr, mask;
String value = ((TEdit *)Sender)->Text;

  if ( process != NULL ) {
    if ( Sender == Break1 ) {
      if ( CbExecute->Checked ) {
        if ( value.SubString(3,1) == ":" ) {
          process->page_bp = value.SubString(1,2).ToIntDef(0);
          StrToHex( value.SubString(4, value.Length() - 3), &addr, &mask);
          process->bp = addr;
        } else {
          StrToHex( value, &addr, &mask);
          process->bp = addr;
        }
      }
    } else {
      StrToHex( value, &addr, &mask);
      if ( Sender == Break2 ) {
        process->read_bp = addr;
        process->read_mask = mask;
      } else if ( Sender == Break3 ) {
        process->write_bp = addr;
        process->write_mask = mask;
      } else if ( Sender == ReadValue ) {
        process->read_value_bp = addr;
        process->read_value_mask = mask;
      } else if ( Sender == WriteValue ) {
        process->write_value_bp = addr;
        process->write_value_mask = mask;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::TraceClick(TObject *Sender)
{
  if ( process != NULL )
    process->do_trace();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Debug_Display(void)
{
char buffer[80];
int count;
int addr;

  Raddr->Text = "";
  Rvalue->Text = "";
  Waddr->Text = "";
  Wvalue->Text = "";

  if ( !process )
    return;
    
  ReadCount->Text = IntToStr(process->read_count);
  WriteCount->Text = IntToStr(process->write_count);
  switch ( process->state ) {
    case 2:
      Raddr->Text = IntToHex(process->read_addr,8);
      Rvalue->Text = IntToHex(process->read_value,8);
      break;
    case 3:
      Waddr->Text = IntToHex(process->write_addr,8);
      Wvalue->Text = IntToHex(process->write_value,8);
      break;
  }

  addr = process->getpc();
  Code->Items->Clear();
  for ( count = 0; count < 20; count++ ) {
    addr += process->dasm( addr, buffer);
    Code->Items->Add(buffer);
  }
  Cycles->Text = IntToStr(process->Total_cycles);
//  Speed->Text = IntToStr(Form1->Delay);
  Display_RAM();
  Display_STACK();

  updating = true;
  switch ( process->board_type ) {
    case BARCREST_MPU4:
      {
      MPU4 *ptr = &((TMPU4 *)process)->board;
      M6809_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6809_A->Text      = IntToHex(ptr->dd.b.h,2);
      M6809_B->Text      = IntToHex(ptr->dd.b.l,2);
      M6809_DP->Text     = IntToHex(ptr->dp.b.h,2);
      M6809_D->Text      = IntToHex(ptr->dd.w.l,4);
      M6809_X->Text      = IntToHex(ptr->x.w.l,4);
      M6809_Y->Text      = IntToHex(ptr->y.w.l,4);
      M6809_U->Text      = IntToHex(ptr->u.w.l,4);
      M6809_S->Text      = IntToHex(ptr->s.w.l,4);
      M6809_flags->Text  = IntToBin(ptr->cc);
      MPU4_mux->Text     = IntToHex(ptr->decoder.column,1);
      Pagelatch->Text    = IntToHex(ptr->page,2);
      PTM_CNT1->Text     = IntToHex(ptr->ptm.cnt1,4);
      PTM_CNT2->Text     = IntToHex(ptr->ptm.cnt2,4);
      PTM_CNT3->Text     = IntToHex(ptr->ptm.cnt3,4);
      PTM_LAT1->Text     = IntToHex(ptr->ptm.lat1,4);
      PTM_LAT2->Text     = IntToHex(ptr->ptm.lat2,4);
      PTM_LAT3->Text     = IntToHex(ptr->ptm.lat3,4);
      PTM_CR1->Text      = IntToHex(ptr->ptm.cr1,2);
      PTM_CR2->Text      = IntToHex(ptr->ptm.cr2,2);
      PTM_CR3->Text      = IntToHex(ptr->ptm.cr3,2);
      PTM_SR->Text       = IntToHex(ptr->ptm.sr,2);
        mc6850st->Text     = IntToHex(ptr->acia.status,2);
        mc6850ctl->Text     = IntToHex(ptr->acia.control,2);
        mc6850tr->Text     = IntToHex(ptr->acia.tdr,2);
        mc6850rc->Text     = IntToHex(ptr->acia.rdr,2);
        mc6850dcd->Text     = IntToHex(ptr->acia.status & 4, 1);
        mc6850rts->Text     = IntToHex(ptr->acia.rts,2);
        mc6850cts->Text     = IntToHex(ptr->acia.status & 8,1);
        TrackballIn->Text   = IntToHex(ptr->pia3.ora_in, 2);
        XCount->Text   = IntToStr(ptr->trackXcnt);
        YCount->Text   = IntToStr(ptr->trackYcnt);
      }
      break;
    case BARCREST_MPU3:
      {
      MPU3 *ptr = &((TMPU3 *)process)->board;
      M6800_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6800_A->Text      = IntToHex(ptr->d.b.h,2);
      M6800_B->Text      = IntToHex(ptr->d.b.l,2);
      M6800_D->Text      = IntToHex(ptr->d.w.l,4);
      M6800_X->Text      = IntToHex(ptr->x.w.l,4);
      M6800_S->Text      = IntToHex(ptr->s.w.l,4);
      M6800_flags->Text  = IntToBin(ptr->cc);
      MPU3_mux->Text     = IntToHex(ptr->decoder.column,1);
      }
      break;
    case ACE_SPACE:
      {
      SPACE *ptr = &((TSPACE *)process)->board;
      M6800_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6800_A->Text      = IntToHex(ptr->d.b.h,2);
      M6800_B->Text      = IntToHex(ptr->d.b.l,2);
      M6800_D->Text      = IntToHex(ptr->d.w.l,4);
      M6800_X->Text      = IntToHex(ptr->x.w.l,4);
      M6800_S->Text      = IntToHex(ptr->s.w.l,4);
      M6800_flags->Text  = IntToBin(ptr->cc);
      //MPU3_mux->Text     = IntToHex(ptr->decoder.column,1);
      }
      break;
    case JPM_IMPACT:
      {
      IMPACT *ptr = &((TIMPACT *)process)->board;
      M68000PC->Text     = IntToHex((int)ptr->m68k_cpu.pc,8);
      M68000D0->Text     = IntToHex((int)ptr->m68k_cpu.dr[0],8);
      M68000D1->Text     = IntToHex((int)ptr->m68k_cpu.dr[1],8);
      M68000D2->Text     = IntToHex((int)ptr->m68k_cpu.dr[2],8);
      M68000D3->Text     = IntToHex((int)ptr->m68k_cpu.dr[3],8);
      M68000D4->Text     = IntToHex((int)ptr->m68k_cpu.dr[4],8);
      M68000D5->Text     = IntToHex((int)ptr->m68k_cpu.dr[5],8);
      M68000D6->Text     = IntToHex((int)ptr->m68k_cpu.dr[6],8);
      M68000D7->Text     = IntToHex((int)ptr->m68k_cpu.dr[7],8);
      M68000SCR->Text    = IntToHex((int)ptr->getccr(),4);
      M68000CCR->Text    = IntToBin((unsigned char)(ptr->getccr() & 0xff));
      M68000A0->Text     = IntToHex((int)ptr->m68k_cpu.ar[0],8);
      M68000A1->Text     = IntToHex((int)ptr->m68k_cpu.ar[1],8);
      M68000A2->Text     = IntToHex((int)ptr->m68k_cpu.ar[2],8);
      M68000A3->Text     = IntToHex((int)ptr->m68k_cpu.ar[3],8);
      M68000A4->Text     = IntToHex((int)ptr->m68k_cpu.ar[4],8);
      M68000A5->Text     = IntToHex((int)ptr->m68k_cpu.ar[5],8);
      M68000A6->Text     = IntToHex((int)ptr->m68k_cpu.ar[6],8);
      M68000A7->Text     = IntToHex((int)ptr->m68k_cpu.ar[7],8);
      M68000SFC->Text    = IntToHex((int)ptr->m68k_cpu.sfc,2);
      M68000DFC->Text    = IntToHex((int)ptr->m68k_cpu.dfc,2);
      ctur->Text         = IntToHex((int)ptr->duart.ctur,2);
      ctlr->Text         = IntToHex((int)ptr->duart.ctlr,2);
      ctr->Text          = IntToHex((int)ptr->duart.counter,4);
      isr->Text          = IntToHex((int)ptr->duart.isr,2);
      imr->Text          = IntToHex((int)ptr->duart.imr,2);
      acr->Text          = IntToHex((int)ptr->duart.acr,2);
      }
      break;
    case MAYGAY_M1A:
      {
      M1A *ptr = &((TM1A *)process)->board;
      M6809_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6809_A->Text      = IntToHex(ptr->dd.b.h,2);
      M6809_B->Text      = IntToHex(ptr->dd.b.l,2);
      M6809_DP->Text     = IntToHex(ptr->dp.b.h,2);
      M6809_D->Text      = IntToHex(ptr->dd.w.l,4);
      M6809_X->Text      = IntToHex(ptr->x.w.l,4);
      M6809_Y->Text      = IntToHex(ptr->y.w.l,4);
      M6809_U->Text      = IntToHex(ptr->u.w.l,4);
      M6809_S->Text      = IntToHex(ptr->s.w.l,4);
      M6809_flags->Text  = IntToBin(ptr->cc);
      scnCnt->Text       = IntToHex(ptr->duart.counter,4);
//      MPU4_mux->Text     = IntToHex(ptr->decoder.column,1);
//      MPU4_page->Text    = IntToHex(ptr->page,2);
//      PTM_CNT1->Text     = IntToHex(ptr->ptm.cnt1,4);
//      PTM_CNT2->Text     = IntToHex(ptr->ptm.cnt2,4);
//      PTM_CNT3->Text     = IntToHex(ptr->ptm.cnt3,4);
//      PTM_LAT1->Text     = IntToHex(ptr->ptm.lat1,4);
//      PTM_LAT2->Text     = IntToHex(ptr->ptm.lat2,4);
//      PTM_LAT3->Text     = IntToHex(ptr->ptm.lat3,4);
//      PTM_CR1->Text      = IntToHex(ptr->ptm.cr1,2);
//      PTM_CR2->Text      = IntToHex(ptr->ptm.cr2,2);
//      PTM_CR3->Text      = IntToHex(ptr->ptm.cr3,2);
//     PTM_SR->Text       = IntToHex(ptr->ptm.sr,2);
      }
      break;
    case BFM_SCORPION1:
      {
      SCORPION1 *ptr = &((TSCORPION1 *)process)->board;
      M6809_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6809_A->Text      = IntToHex(ptr->dd.b.h,2);
      M6809_B->Text      = IntToHex(ptr->dd.b.l,2);
      M6809_DP->Text     = IntToHex(ptr->dp.b.h,2);
      M6809_D->Text      = IntToHex(ptr->dd.w.l,4);
      M6809_X->Text      = IntToHex(ptr->x.w.l,4);
      M6809_Y->Text      = IntToHex(ptr->y.w.l,4);
      M6809_U->Text      = IntToHex(ptr->u.w.l,4);
      M6809_S->Text      = IntToHex(ptr->s.w.l,4);
      M6809_flags->Text  = IntToBin(ptr->cc);
      Pagelatch->Text    = IntToHex(ptr->page,2);
      }
      break;
    case BFM_SCORPION2:
      {
      SCORPION2 *ptr = &((TSCORPION2 *)process)->board;
      M6809_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6809_A->Text      = IntToHex(ptr->dd.b.h,2);
      M6809_B->Text      = IntToHex(ptr->dd.b.l,2);
      M6809_DP->Text     = IntToHex(ptr->dp.b.h,2);
      M6809_D->Text      = IntToHex(ptr->dd.w.l,4);
      M6809_X->Text      = IntToHex(ptr->x.w.l,4);
      M6809_Y->Text      = IntToHex(ptr->y.w.l,4);
      M6809_U->Text      = IntToHex(ptr->u.w.l,4);
      M6809_S->Text      = IntToHex(ptr->s.w.l,4);
      M6809_flags->Text  = IntToBin(ptr->cc);
      Pagelatch->Text    = IntToHex(ptr->page,2);
      }
      break;
    case JPM_MPS2:
      {
      MPS2 *ptr = &((TMPS2 *)process)->board;
      TMS9995_PC->Text   = IntToHex(ptr->get_reg(TMS9900_PC),4);
      TMS9995_WP->Text   = IntToHex(ptr->get_reg(TMS9900_WP),4);
      TMS9995_R0->Text   = IntToHex(ptr->get_reg(TMS9900_R0),4);
      TMS9995_R1->Text   = IntToHex(ptr->get_reg(TMS9900_R1),4);
      TMS9995_R2->Text   = IntToHex(ptr->get_reg(TMS9900_R2),4);
      TMS9995_R3->Text   = IntToHex(ptr->get_reg(TMS9900_R3),4);
      TMS9995_R4->Text   = IntToHex(ptr->get_reg(TMS9900_R4),4);
      TMS9995_R5->Text   = IntToHex(ptr->get_reg(TMS9900_R5),4);
      TMS9995_R6->Text   = IntToHex(ptr->get_reg(TMS9900_R6),4);
      TMS9995_R7->Text   = IntToHex(ptr->get_reg(TMS9900_R7),4);
      TMS9995_R8->Text   = IntToHex(ptr->get_reg(TMS9900_R8),4);
      TMS9995_R9->Text   = IntToHex(ptr->get_reg(TMS9900_R9),4);
      TMS9995_R10->Text  = IntToHex(ptr->get_reg(TMS9900_R10),4);
      TMS9995_R11->Text  = IntToHex(ptr->get_reg(TMS9900_R11),4);
      TMS9995_R12->Text  = IntToHex(ptr->get_reg(TMS9900_R12),4);
      TMS9995_R13->Text  = IntToHex(ptr->get_reg(TMS9900_R13),4);
      TMS9995_R14->Text  = IntToHex(ptr->get_reg(TMS9900_R14),4);
      TMS9995_R15->Text  = IntToHex(ptr->get_reg(TMS9900_R15),4);
      DuartTimer1->Text  = IntToHex(ptr->duart1.prescaler,6);
      DuartTimer2->Text  = IntToHex(ptr->duart2.prescaler,6);
      Count1->Text       = IntToHex(ptr->duart1.timer,2);
      Count2->Text       = IntToHex(ptr->duart2.timer,2);
      Pending1->Text     = IntToHex(ptr->duart1.pending,2);
      Pending2->Text     = IntToHex(ptr->duart2.pending,2);
      ST->Text           = IntToBin(ptr->STATUS);
      Dec->Text          = IntToHex(ptr->decrementer/16,4);
      }
      break;
    case JPM_SYS80:
      {
      SYS80 *ptr = &((TSYS80 *)Fruit)->board;
      TMS9995_PC->Text   = IntToHex(ptr->get_reg(TMS9900_PC),4);
      TMS9995_WP->Text   = IntToHex(ptr->get_reg(TMS9900_WP),4);
      TMS9995_R0->Text   = IntToHex(ptr->get_reg(TMS9900_R0),4);
      TMS9995_R1->Text   = IntToHex(ptr->get_reg(TMS9900_R1),4);
      TMS9995_R2->Text   = IntToHex(ptr->get_reg(TMS9900_R2),4);
      TMS9995_R3->Text   = IntToHex(ptr->get_reg(TMS9900_R3),4);
      TMS9995_R4->Text   = IntToHex(ptr->get_reg(TMS9900_R4),4);
      TMS9995_R5->Text   = IntToHex(ptr->get_reg(TMS9900_R5),4);
      TMS9995_R6->Text   = IntToHex(ptr->get_reg(TMS9900_R6),4);
      TMS9995_R7->Text   = IntToHex(ptr->get_reg(TMS9900_R7),4);
      TMS9995_R8->Text   = IntToHex(ptr->get_reg(TMS9900_R8),4);
      TMS9995_R9->Text   = IntToHex(ptr->get_reg(TMS9900_R9),4);
      TMS9995_R10->Text  = IntToHex(ptr->get_reg(TMS9900_R10),4);
      TMS9995_R11->Text  = IntToHex(ptr->get_reg(TMS9900_R11),4);
      TMS9995_R12->Text  = IntToHex(ptr->get_reg(TMS9900_R12),4);
      TMS9995_R13->Text  = IntToHex(ptr->get_reg(TMS9900_R13),4);
      TMS9995_R14->Text  = IntToHex(ptr->get_reg(TMS9900_R14),4);
      TMS9995_R15->Text  = IntToHex(ptr->get_reg(TMS9900_R15),4);
//      DuartTimer1->Text  = IntToHex(ptr->duart1.prescaler,6);
//      DuartTimer2->Text  = IntToHex(ptr->duart2.prescaler,6);
//      Count1->Text       = IntToHex(ptr->duart1.timer,2);
//      Count2->Text       = IntToHex(ptr->duart2.timer,2);
      ST->Text           = IntToBin(ptr->STATUS);
      Dec->Text          = IntToHex(ptr->decrementer/16,4);
      }
      break;
    case BFM_DOTMATRIX:
      {
      DOTMATRIX *ptr = &((TDOTMATRIX *)process)->board;
      M6809_PC->Text     = IntToHex(ptr->pc.w.l,4);
      M6809_A->Text      = IntToHex(ptr->dd.b.h,2);
      M6809_B->Text      = IntToHex(ptr->dd.b.l,2);
      M6809_DP->Text     = IntToHex(ptr->dp.b.h,2);
      M6809_D->Text      = IntToHex(ptr->dd.w.l,4);
      M6809_X->Text      = IntToHex(ptr->x.w.l,4);
      M6809_Y->Text      = IntToHex(ptr->y.w.l,4);
      M6809_U->Text      = IntToHex(ptr->u.w.l,4);
      M6809_S->Text      = IntToHex(ptr->s.w.l,4);
      M6809_flags->Text  = IntToBin(ptr->cc);
      }
      break;
    case JPM_SYS5:
      {
        SYS5 *ptr = &((TSYS5 *)process)->board;
        M68000PC->Text     = IntToHex((int)ptr->m68k_cpu.pc,8);
        M68000D0->Text     = IntToHex((int)ptr->m68k_cpu.dr[0],8);
        M68000D1->Text     = IntToHex((int)ptr->m68k_cpu.dr[1],8);
        M68000D2->Text     = IntToHex((int)ptr->m68k_cpu.dr[2],8);
        M68000D3->Text     = IntToHex((int)ptr->m68k_cpu.dr[3],8);
        M68000D4->Text     = IntToHex((int)ptr->m68k_cpu.dr[4],8);
        M68000D5->Text     = IntToHex((int)ptr->m68k_cpu.dr[5],8);
        M68000D6->Text     = IntToHex((int)ptr->m68k_cpu.dr[6],8);
        M68000D7->Text     = IntToHex((int)ptr->m68k_cpu.dr[7],8);
        M68000SCR->Text    = IntToHex((int)ptr->getccr(),4);
        M68000CCR->Text    = IntToBin((unsigned char)(ptr->getccr() & 0xff));
        M68000A0->Text     = IntToHex((int)ptr->m68k_cpu.ar[0],8);
        M68000A1->Text     = IntToHex((int)ptr->m68k_cpu.ar[1],8);
        M68000A2->Text     = IntToHex((int)ptr->m68k_cpu.ar[2],8);
        M68000A3->Text     = IntToHex((int)ptr->m68k_cpu.ar[3],8);
        M68000A4->Text     = IntToHex((int)ptr->m68k_cpu.ar[4],8);
        M68000A5->Text     = IntToHex((int)ptr->m68k_cpu.ar[5],8);
        M68000A6->Text     = IntToHex((int)ptr->m68k_cpu.ar[6],8);
        M68000A7->Text     = IntToHex((int)ptr->m68k_cpu.ar[7],8);
        M68000SFC->Text    = IntToHex((int)ptr->m68k_cpu.sfc,2);
        M68000DFC->Text    = IntToHex((int)ptr->m68k_cpu.dfc,2);
        PTM_CNT1->Text     = IntToHex(ptr->ptm.cnt1,4);
        PTM_CNT2->Text     = IntToHex(ptr->ptm.cnt2,4);
        PTM_CNT3->Text     = IntToHex(ptr->ptm.cnt3,4);
        PTM_LAT1->Text     = IntToHex(ptr->ptm.lat1,4);
        PTM_LAT2->Text     = IntToHex(ptr->ptm.lat2,4);
        PTM_LAT3->Text     = IntToHex(ptr->ptm.lat3,4);
        PTM_CR1->Text      = IntToHex(ptr->ptm.cr1,2);
        PTM_CR2->Text      = IntToHex(ptr->ptm.cr2,2);
        PTM_CR3->Text      = IntToHex(ptr->ptm.cr3,2);
        PTM_SR->Text       = IntToHex(ptr->ptm.sr,2);
      }
      break;
    case PROJECT_PROCONN:
      {
        Proconn *ptr2 = &((TProconn *)process)->board;
        Z80CTC_Cnt1->Text = IntToHex(ptr2->ctc.down[0], 4);
        Z80CTC_Cnt2->Text = IntToHex(ptr2->ctc.down[1], 4);
        Z80CTC_Cnt3->Text = IntToHex(ptr2->ctc.down[2], 4);
        Z80CTC_Cnt4->Text = IntToHex(ptr2->ctc.down[3], 4);
        Z80CTC_Const1->Text = IntToHex(ptr2->ctc.tconst[0], 4);
        Z80CTC_Const2->Text = IntToHex(ptr2->ctc.tconst[1], 4);
        Z80CTC_Const3->Text = IntToHex(ptr2->ctc.tconst[2], 4);
        Z80CTC_Const4->Text = IntToHex(ptr2->ctc.tconst[3], 4);
        Z80CTC_IState1->Text = IntToHex(ptr2->ctc.int_state[0], 4);
        Z80CTC_IState2->Text = IntToHex(ptr2->ctc.int_state[1], 4);
        Z80CTC_IState3->Text = IntToHex(ptr2->ctc.int_state[2], 4);
        Z80CTC_IState4->Text = IntToHex(ptr2->ctc.int_state[3], 4);
        Z80CTC_IVector->Text = IntToHex(ptr2->ctc.vector, 2);
        Z80CTC_NoTimer->Text = IntToHex(ptr2->ctc.notimer, 2);
      }
    case ACE_SYSTEM1:
      {
      ACE1 *ptr = &((TACE1 *)process)->board;
      Z80_PC->Text     = IntToHex(ptr->R.PC.W.l,4);
      Z80_AF->Text      = IntToHex(ptr->R.AF.W.l,4);
      Z80_BC->Text      = IntToHex(ptr->R.BC.W.l,4);
      Z80_DE->Text     = IntToHex(ptr->R.DE.W.l,4);
      Z80_HL->Text      = IntToHex(ptr->R.HL.W.l,4);
      Z80_IX->Text      = IntToHex(ptr->R.IX.W.l,4);
      Z80_IY->Text      = IntToHex(ptr->R.IY.W.l,4);
      Z80_SP->Text      = IntToHex(ptr->R.SP.W.l,4);
      Z80_FLAGS->Text  = IntToBin(ptr->R.AF.B.l);
      Z80_AAF->Text      = IntToHex(ptr->R.AF2.W.l,4);
      Z80_ABC->Text      = IntToHex(ptr->R.BC2.W.l,4);
      Z80_ADE->Text     = IntToHex(ptr->R.DE2.W.l,4);
      Z80_AHL->Text      = IntToHex(ptr->R.HL2.W.l,4);
      Z80_IM->Text     = IntToHex((int)ptr->R.IM,1);
      Z80_I->Text      = IntToHex((int)ptr->R.I,2);
      }
      break;
    case BARCREST_MPU4VIDEO:
      {
      MPU4VIDEO *ptr = &((TMPU4VIDEO *)process)->board;
      M68000PC->Text     = IntToHex((int)ptr->m68k_cpu.pc,8);
      M68000D0->Text     = IntToHex((int)ptr->m68k_cpu.dr[0],8);
      M68000D1->Text     = IntToHex((int)ptr->m68k_cpu.dr[1],8);
      M68000D2->Text     = IntToHex((int)ptr->m68k_cpu.dr[2],8);
      M68000D3->Text     = IntToHex((int)ptr->m68k_cpu.dr[3],8);
      M68000D4->Text     = IntToHex((int)ptr->m68k_cpu.dr[4],8);
      M68000D5->Text     = IntToHex((int)ptr->m68k_cpu.dr[5],8);
      M68000D6->Text     = IntToHex((int)ptr->m68k_cpu.dr[6],8);
      M68000D7->Text     = IntToHex((int)ptr->m68k_cpu.dr[7],8);
      M68000SCR->Text    = IntToHex((int)ptr->getccr(),4);
      M68000CCR->Text    = IntToBin((unsigned char)(ptr->getccr() & 0xff));
      M68000A0->Text     = IntToHex((int)ptr->m68k_cpu.ar[0],8);
      M68000A1->Text     = IntToHex((int)ptr->m68k_cpu.ar[1],8);
      M68000A2->Text     = IntToHex((int)ptr->m68k_cpu.ar[2],8);
      M68000A3->Text     = IntToHex((int)ptr->m68k_cpu.ar[3],8);
      M68000A4->Text     = IntToHex((int)ptr->m68k_cpu.ar[4],8);
      M68000A5->Text     = IntToHex((int)ptr->m68k_cpu.ar[5],8);
      M68000A6->Text     = IntToHex((int)ptr->m68k_cpu.ar[6],8);
      M68000A7->Text     = IntToHex((int)ptr->m68k_cpu.ar[7],8);
      M68000SFC->Text    = IntToHex((int)ptr->m68k_cpu.sfc,2);
      M68000DFC->Text    = IntToHex((int)ptr->m68k_cpu.dfc,2);
      PTM_CNT1->Text     = IntToHex(ptr->ptm.cnt1,4);
      PTM_CNT2->Text     = IntToHex(ptr->ptm.cnt2,4);
      PTM_CNT3->Text     = IntToHex(ptr->ptm.cnt3,4);
      PTM_LAT1->Text     = IntToHex(ptr->ptm.lat1,4);
      PTM_LAT2->Text     = IntToHex(ptr->ptm.lat2,4);
      PTM_LAT3->Text     = IntToHex(ptr->ptm.lat3,4);
      PTM_CR1->Text      = IntToHex(ptr->ptm.cr1,2);
      PTM_CR2->Text      = IntToHex(ptr->ptm.cr2,2);
      PTM_CR3->Text      = IntToHex(ptr->ptm.cr3,2);
      PTM_SR->Text       = IntToHex(ptr->ptm.sr,2);
      scnir0->Text       = IntToHex(ptr->vdc.ir[0],2);
      scnir1->Text       = IntToHex(ptr->vdc.ir[1],2);
      scnir2->Text       = IntToHex(ptr->vdc.ir[2],2);
      scnir3->Text       = IntToHex(ptr->vdc.ir[3],2);
      scnir4->Text       = IntToHex(ptr->vdc.ir[4],2);
      scnir5->Text       = IntToHex(ptr->vdc.ir[5],2);
      scnir6->Text       = IntToHex(ptr->vdc.ir[6],2);
      scnir7->Text       = IntToHex(ptr->vdc.ir[7],2);
      scnir8->Text       = IntToHex(ptr->vdc.ir[8],2);
      scnir9->Text       = IntToHex(ptr->vdc.ir[9],2);
      scnir10->Text      = IntToHex(ptr->vdc.ir[10],2);
      scnir11->Text      = IntToHex(ptr->vdc.ir[11],2);
      scnir12->Text      = IntToHex(ptr->vdc.ir[12],2);
      scnir13->Text      = IntToHex(ptr->vdc.ir[13],2);
      scnir14->Text      = IntToHex(ptr->vdc.ir[14],2);
      scnss1->Text       = IntToHex(ptr->vdc.regs[2] + 256 * ptr->vdc.regs[3],4);
      scnss2->Text       = IntToHex(ptr->vdc.regs[6] + 256 * ptr->vdc.regs[7],4);
      scnca->Text        = IntToHex(ptr->vdc.regs[4] + 256 * ptr->vdc.regs[5],4);
      scnirq->Text       = IntToHex(ptr->vdc.regs[0],2);
      scnstatus->Text    = IntToHex(ptr->vdc.regs[1],2);
      scnmask->Text      = IntToHex(ptr->vdc.intmask,2);
        mc6850st->Text     = IntToHex(ptr->acia.status,2);
        mc6850ctl->Text     = IntToHex(ptr->acia.control,2);
        mc6850tr->Text     = IntToHex(ptr->acia.tdr,2);
        mc6850rc->Text     = IntToHex(ptr->acia.rdr,2);
        mc6850dcd->Text     = IntToHex(ptr->acia.status & 4, 1);
        mc6850rts->Text     = IntToHex(ptr->acia.rts,2);
        mc6850cts->Text     = IntToHex(ptr->acia.status & 8,1);
        Graphics->Text      = IntToHex((int)ptr->vdc.graphics, 2);
        Display->Text       = IntToHex((int)ptr->vdc.display, 2);
        TrackballIn->Text   = IntToHex((int)ptr->acia_int, 2);
      }
      break;
  }
  LampUpdates->Text = IntToStr(process->lamp_updates);
  ReelUpdates->Text = IntToStr(process->reel_updates);
  LEDUpdates->Text = IntToStr(process->led_updates);
  AlphaUpdates->Text = IntToStr(process->alpha_updates);
  SoundUpdates->Text = IntToStr(process->sound_updates);
  CPUUpdates->Text = IntToStr(process->Total_cycles - process->Last_cycles);
  process->Last_cycles = process->Total_cycles;
  calls++;
  Calls->Text = IntToStr(calls);
  process->sound_updates = 0;
  updating = false;

}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Display_RAM(void)
{
unsigned int addr;
unsigned int count, count2;
char *ptr;
char buffer[100];
int mem, i;

  if ( process != NULL ) {
    Data->Items->Clear();
    if ( locktopc->Checked )
      addr = process->getpc();
    else
      addr = StrToHex(MemAddress->Text);
    count = addr;
    for ( i = 0; i < 8; i++, count+=16 ) {
      ptr = buffer;
      ptr += sprintf(ptr, "%08X ", count);
      for ( count2 = 0; count2 < 16; count2++ ) {
        mem = process->getmemory(count2+count);
        if ( mem >= 0 )
          ptr+=sprintf(ptr, " %02X", mem);
        else
          ptr+=sprintf(ptr, " --");
      }
      ptr+=sprintf(ptr, "  ");
      for ( count2 = 0; count2 < 16; count2++ ) {
        unsigned char c = process->getmemory(count2+count);
        if ( isprint(c) )
          ptr+=sprintf(ptr, "%c", c);
        else
          ptr+=sprintf(ptr,".");
      }
      Data->Items->Add(buffer);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Display_STACK(void)
{
unsigned int addr, top;
unsigned int count, count2;
char *ptr;
char buffer[100];
int mem, i;

  if ( process != NULL ) {
    Stack->Items->Clear();
    addr = process->getstack();
    top  = process->getstacktop();
    count = addr;
    for ( i = 0; i < 8 && (count + i < top); i++, count+=16 ) {
      ptr = buffer;
      ptr += sprintf(ptr, "%08X ", count);
      for ( count2 = 0; count2 < 16 && (count + i + count2) < top; count2++ ) {
        mem = process->getmemory(count2+count);
        if ( mem >= 0 )
          ptr+=sprintf(ptr, " %02X", mem);
        else
          ptr+=sprintf(ptr, " --");
      }
      for ( ; count2 < 16; count2++ )
          ptr+=sprintf(ptr, "   ");
      ptr+=sprintf(ptr, "  ");
      for ( count2 = 0; count2 < 16 && (count + i + count2) < top; count2++ ) {
        unsigned char c = process->getmemory(count2+count);
        if ( isprint(c) )
          ptr+=sprintf(ptr, "%c", c);
        else
          ptr+=sprintf(ptr,".");
      }
      Stack->Items->Add(buffer);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::MemAddressChange(TObject *Sender)
{
unsigned int addr;

  updating = true;
  addr = StrToHex(MemAddress->Text);
  offset = addr % 16;
  ScrollBar1->Position = addr / 16;
  updating = false;
  Display_RAM();
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::WriteClick(TObject *Sender)
{
int addr;

  if ( process ) {
    addr = StrToHex(WriteAddress->Text);
    if ( DWordWise->Checked ) {
      process->setmemory(addr, StrToHex(Value->Text) >> 24);
      process->setmemory(addr+1, (StrToHex(Value->Text) >> 16) & 0xff);
      process->setmemory(addr+2, (StrToHex(Value->Text) >> 8) & 0xff);
      process->setmemory(addr+3, StrToHex(Value->Text) & 0xff);
    } else if ( WordWise->Checked ) {
      process->setmemory(addr, StrToHex(Value->Text) >> 8);
      process->setmemory(addr+1, StrToHex(Value->Text) & 0xff);
    } else
      process->setmemory(addr, StrToHex(Value->Text) & 0xff);
    Display_RAM();
  }

}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::FormShow(TObject *Sender)
{
  if ( process != NULL ) {
    m6809->Enabled = false;
    m6800->Enabled = false;
    m68000->Enabled = false;
    tms9995->Enabled = false;
    Z80->Enabled=false;
    switch ( process->board_type) {
      case BARCREST_MPU3:
      case ACE_SPACE:
        Registers->ActivePage = m6800;
        m6800->Enabled = true;
        break;
      case BARCREST_MPU4:
      case BFM_SCORPION1:
      case BFM_SCORPION2:
      case BFM_DOTMATRIX:
      case MAYGAY_M1A:
        Registers->ActivePage = m6809;
        m6809->Enabled = true;
        break;
      case JPM_IMPACT:
      case JPM_SYS5:
      case BARCREST_MPU4VIDEO:
        Registers->ActivePage = m68000;
        m68000->Enabled = true;
        break;
      case JPM_MPS2:
      case JPM_SYS80:
        Registers->ActivePage = tms9995;
        tms9995->Enabled = true;
        break;
      case ACE_SYSTEM1:
      case PROJECT_PROCONN:
        Registers->ActivePage = Z80;
        Z80->Enabled = true;
        break;
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CodeAddressChange(TObject *Sender)
{
char buffer[80];
int count;
int addr;

  if ( process != NULL ) {
    addr = StrToHex(CodeAddress->Text);
    Code->Items->Clear();
    for ( count = 0; count < 40; count++ ) {
      addr += process->dasm( addr, buffer);
      Code->Items->Add(buffer);
    }
  }

}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::StrToHex(String s, unsigned int *addr, unsigned int *mask)
{
int count, mask2;
char *ch_ptr;

  count = 0;
  mask2 = 0xffffffff;
  ch_ptr = s.c_str();
  if ( *ch_ptr ) {
    while ( *ch_ptr ) {
      if ( isxdigit(*ch_ptr) ) {
        count = count * 16 + ( *ch_ptr >= '0' &&
                               *ch_ptr <= '9' ? *ch_ptr-'0' :
                               toupper(*ch_ptr)-'A' + 10);
      } else if ( *ch_ptr == '*' ) {
        count = count * 16;
        mask2 = mask2 << 4;
      }
      ch_ptr++;
    }
  } else
    mask2 = 0;
  *addr = count;
  *mask = mask2;
}
//---------------------------------------------------------------------------
void __fastcall TDebugForm::CbReadClick(TObject *Sender)
{
  if ( process != NULL )
    process->readbp = CbRead->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CbWriteClick(TObject *Sender)
{
  if ( process != NULL )
    process->writebp = CbWrite->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::Timer2Timer(TObject *Sender)
{
  if ( process != NULL ) {
    process->state = MS_STEPPING;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::FastStepClick(TObject *Sender)
{
  if ( process != NULL ) {
    Timer2->Enabled = true;
    Stop->Enabled = true;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ClearReadClick(TObject *Sender)
{
  if ( process ) {
    process->read_count = 0;
    ReadCount->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ClearWriteClick(TObject *Sender)
{
  if ( process ) {
    process->write_count = 0;
    WriteCount->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::CatchErrorsClick(TObject *Sender)
{
  if ( process ) {
    if ( CatchErrors->Checked )
      process->errors = true;
    else
      process->errors = false;
  }
}
//--------------------------------------------------------------------------

void __fastcall TDebugForm::TraceonClick(TObject *Sender)
{
  Trace->Enabled = Traceon->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ScrollBar1Change(TObject *Sender)
{
int addr = offset + ScrollBar1->Position * 16;

  if ( !updating )
    MemAddress->Text = IntToHex(addr,8);
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::ClearCyclesClick(TObject *Sender)
{
  if ( process ) {
    process->Total_cycles = 0;
    Cycles->Text = "0";
  }
}
//---------------------------------------------------------------------------

void __fastcall TDebugForm::SpeedButton1Click(TObject *Sender)
{
Byte value;

//  value = (Byte)StrToHex(Edit1->Text);
//  Form1->SendChar(value);
}
//---------------------------------------------------------------------------
int __fastcall TDebugForm::StrToHex(String s)
{
int count;
char *ch_ptr;

  count = 0;
  ch_ptr = s.c_str();
  if ( *ch_ptr ) {
    while ( *ch_ptr ) {
      if ( isxdigit(*ch_ptr) )
        count = count * 16 + ( *ch_ptr >= '0' &&
                               *ch_ptr <= '9' ? *ch_ptr-'0' :
                               toupper(*ch_ptr)-'A' + 10);
      ch_ptr++;
    }
    return count;
  } else
    return -1;
}
//---------------------------------------------------------------------------

String __fastcall TDebugForm::IntToBin(unsigned char value)
{
int count;
char buff[9];

  buff[8] = '\0';

  for ( count = 0; count < 8; count++ ) {
    if ( value & 128 )
      buff[count] = '1';
    else
      buff[count] = '0';
    value = value << 1;
  }

  return AnsiString(buff);
}
//---------------------------------------------------------------------------






void __fastcall TDebugForm::locktopcClick(TObject *Sender)
{
  Display_RAM();    
}
//---------------------------------------------------------------------------

