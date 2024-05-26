//---------------------------------------------------------------------------

#include <sys/stat.h>
#include <stdlib.h>
#include <io.h>
#include "interface.h"
#include "debug.h"
#include "shared.h"
#include "lamp.h"
#include "led.h"
#include "meter.h"
#include "triac.h"
#include "jpmreels.h"
#include "reels.h"
#include "sec.h"
#include "alpha.h"
#include "prefs.h"
#include "BFMalpha.h"


__fastcall TSystem::TSystem(String AGame, String AGamePath, TStringList *AROM_list)
     : TThread(true)
{
   Game = AGame;
   CurrGamePath = AGamePath;
   ROMList = new TStringList();
   ROMList->Assign(AROM_list);
   lamp_updates = 0;
   led_updates = 0;
   alpha_updates = 0;
   reel_updates = 0;
   sound_updates = 0;
   Last_cycles = 0;
   Total_cycles = 0;
   Priority = tpTimeCritical; //tpHighest;
   reeltimer = 0;
}

//---------------------------------------------------------------------------
void __fastcall TSystem::Execute()
{
  FreeOnTerminate = true;
  bp = -1;
  read_bp = -1;
  read_mask = 0;
  write_bp = -1;
  load_intelhex();
  hard_reset();
  if ( GeneralConfig->HighPriority->Checked )
    Run2();
  else
    Run();
  MainDebug->ClearProcess();
  VideoDebug->ClearProcess();
  MatrixDebug->ClearProcess();
  saveram();
}
//---------------------------------------------------------------------------
void __fastcall TSystem::Run()
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::Run2()
{
  Run();
}
//---------------------------------------------------------------------------
void __fastcall TSystem::load_intelhex()
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::saveram()
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::saveram(Byte *ptr, int size)
{
FILE		*fp;
String      filename;

  filename = CurrGamePath + Game.SubString(1, Game.Pos(".")) + "ram";
  chmod(filename.c_str(),S_IWRITE);
  fp = fopen(filename.c_str(), "wb");
  if (fp) {
    fwrite( ptr, size, 1, fp);
    fclose(fp);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_meter(void)
{
  Synchronize(UpdateMeter);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_triac(void)
{
  Synchronize(UpdateTriac);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_debug(void)
{
  Synchronize(UpdateDebug);

}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_display(void)
{
  alpha_updates++;
  Synchronize(UpdateDisplay);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_led_display(void)
{
  led_updates++;
  Synchronize(UpdateLedDisplay);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_reel(void)
{
  reel_updates++;
  Synchronize(UpdateReel);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_lamp(void)
{
  lamp_updates++;
  Synchronize(UpdateLamp);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_reel_lamp(void)
{
  Synchronize(UpdateReelLamp);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_trace(void)
{
  Synchronize(UpdateTrace);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_wave(void)
{
  Synchronize(UpdateWave);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_pak(void)
{
  Synchronize(UpdatePak);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_matrix(void)
{
  alpha_updates++;
  Synchronize(UpdateMatrix);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_screen(void)
{
  Synchronize(UpdateScreen);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_row(void)
{
  Synchronize(UpdateRow);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdatePak(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_io_meters(void)
{
  Synchronize(UpdateIOMeters);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_io_triacs(void)
{
  Synchronize(UpdateIOTriacs);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_io_sinks(void)
{
  Synchronize(UpdateIOSinks);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_io_selects(void)
{
  Synchronize(UpdateIOSelects);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_io_reels(void)
{
  Synchronize(UpdateIOReels);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_sec(void)
{
  Synchronize(UpdateSEC);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::do_status(void)
{
  Synchronize(UpdateStatus);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateStatus(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateStatuss(Byte state)
{
  Form1->UpdateStatus(state);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateDisplay(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateScreen(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateScreens(Byte *map, Word *rowtable)
{
  if ( Form1->screen ) {
    Form1->screen->WriteMemory(map);
    Form1->screen->WriteRowTable(rowtable);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateRow(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateRows(Byte *palette, Byte *map, Word *rowtable, bool updated, Byte row)
{
  if ( Form1->screen ) {
    if ( updated )
      Form1->screen->WritePalette(palette);
    Form1->screen->WriteMemory(map);
    Form1->screen->WriteRowTable(rowtable);
    Form1->screen->DrawRow(row);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateDisplays(alpha *alpha_ptr, int nbr)
{
int i;
TAlpha *ptr;

  for ( i = 0; i < nbr; i++ ) {
    if ( ( ptr = Form1->display[i] ) != NULL ) {
      if ( alpha_ptr[i].display_changed ) {
        ptr->Intensity = alpha_ptr[i].intensity;
        ptr->SetDisplay2(alpha_ptr[i].chs);
      }
      if ( alpha_ptr[i].intensity_changed ) {
        ptr->Intensity = alpha_ptr[i].intensity;
      }
    }
    alpha_ptr[i].display_changed = 0;
    alpha_ptr[i].intensity_changed = 0;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateDisplays(BFMalpha *alpha_ptr, int nbr)
{
 if ( nbr == 0 ) {
  if ( Form1->display[0] != NULL ) {
    if ( alpha_ptr->display_changed ) {
      if ( alpha_ptr->flash )
        Form1->display[0]->SetDisplay2(alpha_ptr->flashchs);
      else
      Form1->display[0]->SetDisplay2(alpha_ptr->chs);
    }
  }
  if ( Form1->bfmalpha != NULL ) {
    if ( alpha_ptr->display_changed ) {
      if ( alpha_ptr->flash )
        Form1->bfmalpha->SetDisplay2(alpha_ptr->flashchs);
      else
        Form1->bfmalpha->SetDisplay2(alpha_ptr->chs);
    }
  }
  alpha_ptr->flash_changed = 0;
  alpha_ptr->display_changed = 0;
 }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateDebug(void)
{
  MainDebug->Debug_Display();
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateMatrix(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateMatrixs(Byte *map)
{
  if ( Form1->dotmatrix != NULL )
    Form1->dotmatrix->SetMap(map);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateACEMatrix(Byte *map)
{
  if ( Form1->aceled != NULL )
    Form1->aceled->SetMap(map);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdatePROCONNMatrix(Byte *map)
{
  if ( Form1->proconnled != NULL )
    Form1->proconnled->SetMap(map);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateReel(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateReels(jpmreels *reel_ptr)
{
Byte nbr;

  for ( nbr = 0; nbr < 6; nbr++ ) {
    if ( reel_ptr->changed[nbr] && Form1->Reels[nbr] != NULL ) {
      switch ( Form1->ReelType[nbr] ) {
        case RT_REEL:
          ((TReel *)Form1->Reels[nbr])->Position = reel_ptr->reels[nbr].reelpos;
          break;
        case RT_FANCY:
          ((TFancyReel *)Form1->Reels[nbr])->Position = reel_ptr->reels[nbr].reelpos;
          break;
        case RT_BAND:
          ((TBandReel *)Form1->Reels[nbr])->Position = reel_ptr->reels[nbr].reelpos;
          break;
        case RT_DISC:
          ((TDiscReel *)Form1->Reels[nbr])->Position = reel_ptr->reels[nbr].reelpos;
          break;
      }
    }
    reel_ptr->changed[nbr] = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateReels(reels *reel_ptr)
{
Byte nbr;

  for ( nbr = 0; nbr < 10; nbr++ ) {
    if ( reel_ptr->changed[nbr] && Form1->Reels[nbr] != NULL ) {
      switch ( Form1->ReelType[nbr] ) {
        case RT_REEL:
          ((TReel *)Form1->Reels[nbr])->Position = reel_ptr->reelpos[nbr];
          break;
        case RT_FANCY:
          ((TFancyReel *)Form1->Reels[nbr])->Position = reel_ptr->reelpos[nbr];
          break;
        case RT_BAND:
          ((TBandReel *)Form1->Reels[nbr])->Position = reel_ptr->reelpos[nbr];
          break;
        case RT_DISC:
          ((TDiscReel *)Form1->Reels[nbr])->Position = reel_ptr->reelpos[nbr];
          break;
      }
    }
    reel_ptr->changed[nbr] = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateReelLamp(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateReelLamps(jpmreels *ptr)
{
  for ( int i = 0; i < 9; i++ ) {
    if ( Form1->Reels[i] != NULL && ptr->lamp_changed[i] ) {
      switch ( Form1->ReelType[i] ) {
        case RT_REEL:
          ((TReel *)Form1->Reels[i])->SetLamps(ptr->lamps[i]);
          break;
        case RT_FANCY:
          ((TFancyReel *)Form1->Reels[i])->SetLamps(ptr->lamps[i] << 1);
          break;
      }
      ptr->lamp_changed[i] = false;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateLamp(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateLamps(lamps *lamps_ptr, int nbr )
{
TControl *ptr;
struct lamp *lamp_ptr;
int i;

  for ( i = 0; i < nbr; i++ ) {
    if ( lamps_ptr->updated[i] ) {
      for ( lamp_ptr = Form1->Lamps[i]; lamp_ptr; lamp_ptr = lamp_ptr->next) {
        ptr = lamp_ptr->ptr;
        if ( lamps_ptr->lamp[i] ) {
          switch ( lamp_ptr->type ) {
            case LT_LAMP:
              ((TLamp2 *)ptr)->State |= lamp_ptr->nbr;
              break;
            case LT_BUTTON:
              ((TzcShapeBitColBtn *)ptr)->State = true;
              if ( ptr->Tag & _LockMask )
                ((TzcShapeBitColBtn *)ptr)->Enabled = true;
              break;
            case LT_LED:
              ((TzLed *)ptr)->Enabled = true;
              break;
            case LT_SEVENSEG:
              ((TSevenSegment *)ptr)->Value |= lamp_ptr->mask;
              break;
            case LT_LABEL:
              ((TLabel *)ptr)->Visible = true;
              break;
            case LT_REEL:
              ((TReel *)ptr)->SetLamp(lamp_ptr->nbr, true);
              break;
            case LT_FANCYREEL:
              ((TFancyReel *)ptr)->SetLamp(lamp_ptr->nbr, true);
              break;
            case LT_DISCREEL:
              ((TDiscReel *)ptr)->State |= lamp_ptr->mask;
              break;
            case LT_BANDREEL:
              ((TBandReel *)ptr)->SetLamp(lamp_ptr->nbr, true);
              break;
          }
        } else {
          switch ( lamp_ptr->type ) {
            case LT_LAMP:
//              if ( ((TLamp2 *)ptr)->State &= ~lamp_ptr->nbr )
                ((TLamp2 *)ptr)->State &= ~lamp_ptr->nbr;
              break;
            case LT_BUTTON:
              ((TzcShapeBitColBtn *)ptr)->State = false;
              if ( ptr->Tag & _LockMask )
                ((TzcShapeBitColBtn *)ptr)->Enabled = false;
              break;
            case LT_LED:
              ((TzLed *)ptr)->Enabled = false;
              break;
            case LT_SEVENSEG:
              ((TSevenSegment *)ptr)->Value &= ~lamp_ptr->mask;
              break;
            case LT_LABEL:
              ((TLabel *)ptr)->Visible = false;
              break;
            case LT_REEL:
              ((TReel *)ptr)->SetLamp(lamp_ptr->nbr, false);
              break;
            case LT_FANCYREEL:
              ((TFancyReel *)ptr)->SetLamp(lamp_ptr->nbr, false);
              break;
            case LT_DISCREEL:
              ((TDiscReel *)ptr)->State &= ~lamp_ptr->mask;
              break;
            case LT_BANDREEL:
              ((TBandReel *)ptr)->SetLamp(lamp_ptr->nbr, false);
              break;
          }
        }
      }
      lamps_ptr->updated[i]--;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateMeter(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::MeterSensed(meters *meter_ptr, Byte state)
{
  if ( meter_ptr->meter_on && MeterSense != -1 && ((meter_ptr->state^0xff) & MeterMask))
    Form1->MatrixChange3(MeterSense, true);
  else
    Form1->MatrixChange3(MeterSense, false);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateMeters(meters *meter_ptr)
{
int loop, mask;

  mask = 1;
  for ( loop = 0; loop < 8; loop++ ) {
    if ( meter_ptr->state & mask ) {
      if ( TotalInMeterMult[loop] ) {
        TotalIn += TotalInMeterMult[loop];
        STotalIn += TotalInMeterMult[loop];
        VTP += TotalInMeterMult[loop];
        Form1->UpdateMeters();
      }
      if ( TotalOutMeterMult[loop] ) {
        TotalOut += TotalOutMeterMult[loop];
        STotalOut += TotalOutMeterMult[loop];
        Form1->UpdateMeters();
      }
    }
    mask *= 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateTriac(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateTriacs(triacs *triac_ptr)
{
int loop, mask;

  mask = 1;
  for ( loop = 0; loop < 8; loop++ ) {
    if ( triac_ptr->changed & mask ) {
      if ( triac_ptr->value & mask ) {
        if ( TotalInTriacMult[loop] ) {
          TotalIn += TotalInTriacMult[loop];
          STotalIn += TotalInTriacMult[loop];
          VTP += TotalInTriacMult[loop];
          Form1->UpdateMeters();
        }
        if ( TotalOutTriacMult[loop] ) {
          TotalOut += TotalOutTriacMult[loop];
          STotalOut += TotalOutTriacMult[loop];
          Form1->UpdateMeters();
        }
      }
    }
    mask *= 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateSenseTriacs(triacs *triac_ptr)
{
int loop, mask;

  mask = 1;
  for ( loop = 0; loop < 8; loop++ ) {
    if ( triac_ptr->changed & mask ) {
      if ( triac_ptr->value & mask ) {
        if ( TriacSense != -1 && (triac_ptr->value & mask & TriacMask) )
          Form1->MatrixChange3( TriacSense, false);
        if ( TotalInTriacMult[loop] ) {
          TotalIn += TotalInTriacMult[loop];
          STotalIn += TotalInTriacMult[loop];
          VTP += TotalInMeterMult[loop];
          Form1->UpdateMeters();
        }
        if ( TotalOutTriacMult[loop] ) {
          TotalOut += TotalOutTriacMult[loop];
          STotalOut += TotalOutTriacMult[loop];
          Form1->UpdateMeters();
        }
      } else {
        if ( TriacSense != -1 && (triac_ptr->value & mask & TriacMask))
          Form1->MatrixChange3( TriacSense, true);
      }
    }
    mask *= 2;
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateTrace(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateLedDisplay(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateLedDisplays(led *led_ptr)
{
int i, loop, mask;

  for ( i = 0; i < 40; i++ ) {
    if ( led_ptr->updated[i] ) {
      if ( Form1->led_display[i].digit != NULL ) {
        Form1->led_display[i].digit->Value = led_ptr->digits[i];
        led_ptr->updated[i] = false;
      }
      mask = 1;
      for ( loop = 0; loop < 8; loop++ ) {
        if ( Form1->led_display[i].led[loop] != NULL ) {
          if ( led_ptr->digits[i] & ( 1 << loop ) )
            Form1->led_display[i].led[loop]->Enabled = true;
          else
            Form1->led_display[i].led[loop]->Enabled = false;
        }
        mask << 1;
      }
//      if ( Form1->bfmled )
//        Form1->bfmled->SetDot(i, (led_ptr->digits[i] << 1) | (led_ptr->digits[i] >> 7));
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateWave(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateWaves(int tune, int channel)
{
  Form1->play2(tune, channel);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateIOMeters(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateIOTriacs(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateIOSinks(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateIOSelects(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateIOReels(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateSEC(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateMouse(void)
{
  Form1->UpdateMouse2();
}
//---------------------------------------------------------------------------
void __fastcall TSystem::UpdateSECs(SEC *sec_ptr)
{
int sec = sec_ptr->meter;
int value = sec_ptr->value;

  if ( TotalInSECMult[sec] ) {
    TotalIn += (TotalInSECMult[sec] * value);
    STotalIn += (TotalInSECMult[sec] * value);
    Form1->UpdateMeters();
  }
  if ( TotalOutSECMult[sec] ) {
    TotalOut += (TotalOutSECMult[sec] * value);
    STotalOut += (TotalOutSECMult[sec] * value);
    Form1->UpdateMeters();
  }
  sec_ptr->updated = false;
}
//---------------------------------------------------------------------------
void __fastcall TSystem::hard_reset(void)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::samplefinished(int Channel)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::SaveToStream(Byte *&ptr)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::LoadFromStream(Byte *&ptr)
{
}
//---------------------------------------------------------------------------
void __fastcall TSystem::IntelHexRead(byte *ptr, String filename)
{
FILE *fp;
bool done = false;
int nbr_of_bytes, address, type, csum;
byte ch = 0;
byte data;

  fp = fopen(filename.c_str(), "rb");
  if ( !fp )
    return;

  while ( !done ) {
    csum = 0;
    while ( ch != ':' && !feof(fp) )
      fread( &ch, 1, 1, fp);
    if ( ch != ':' )
      return;
    nbr_of_bytes = get_next(fp,2,&csum);
    address      = get_next(fp,4,&csum);
    type         = get_next(fp,2,&csum);
    switch ( type ) {
      case 0: // Data Record
        for ( ; nbr_of_bytes; nbr_of_bytes-- ) {
          data = get_next(fp,2,&csum);
          ptr[address++] = data;
        }
        break;
      case 1: // End Record
        done = true;
        break;
      default:
        return;
    }
    get_next(fp,2,&csum);
    if ( csum & 0xff )
      return;
    ch = 0;
  }
  fclose(fp);
}
//---------------------------------------------------------------------------

int __fastcall TSystem::get_next(FILE *fp, int size, int *csum)
{
Byte ch;
int value = 0, data = 0;
int i;

  for ( i = 0; i < size; i++ ) {
    fread( &ch, 1, 1, fp);
    if ( ch >= '0' && ch <= '9' )
      ch -= '0';
    else if ( ch >= 'A' && ch <= 'F' )
      ch = ch - 'A' + 10;
    data = ch + data * 16;
    value = ch + value * 16;
    if ( i & 1 ) {
      *csum += data;
      data = 0;
    }
  }
  return value;
}

//---------------------------------------------------------------------------
int __fastcall TSystem::SetReels(jpmreels *reel_ptr, int nbr, int adjust)
{
int count;
jpmreels::rstruct *reel = &reel_ptr->reels[0];
int nbr_reels = 0;

  for ( count = 0; count < nbr; count++ ) {
    if ( Form1->Reels[count] != NULL ) {
      nbr_reels++;
      switch ( Form1->ReelType[count] ) {
        case RT_REEL:
          reel->stops = ((TReel *)Form1->Reels[count])->Stops;
          reel->steps = ((TReel *)Form1->Reels[count])->TotalSteps;
          reel->offset = ((TReel *)Form1->Reels[count])->Offset;
          reel->inverted = ((TReel *)Form1->Reels[count])->Inverted;
          reel->flag = ((TReel *)Form1->Reels[count])->Flag;
          reel->adjust = adjust;
          break;
        case RT_FANCY:
          reel->stops = ((TFancyReel *)Form1->Reels[count])->Stops;
          reel->steps = ((TFancyReel *)Form1->Reels[count])->TotalSteps;
          reel->stepsperstop = ((TFancyReel *)Form1->Reels[count])->StepsPerStop;
          reel->offset = ((TFancyReel *)Form1->Reels[count])->Offset;
          reel->inverted = ((TFancyReel *)Form1->Reels[count])->Inverted;
          reel->flag = ((TFancyReel *)Form1->Reels[count])->Flag;
          reel->adjust = adjust;
          break;
        case RT_DISC:
          reel->stops = ((TDiscReel *)Form1->Reels[count])->Stops;
          reel->steps = ((TDiscReel *)Form1->Reels[count])->Steps;
          reel->offset = ((TDiscReel *)Form1->Reels[count])->Offset;
          reel->inverted = ((TDiscReel *)Form1->Reels[count])->Inverted;
          reel->flag = 0;
          reel->adjust = adjust;
          break;
        case RT_BAND:
          reel->stops = ((TBandReel *)Form1->Reels[count])->Stops;
          reel->steps = ((TBandReel *)Form1->Reels[count])->TotalSteps;
          reel->offset = ((TBandReel *)Form1->Reels[count])->Offset;
          reel->inverted = false;
          reel->flag = 0;
          reel->adjust = adjust;
          break;
        default:
          reel->steps = 96;
          reel->offset = 0;
          reel->inverted = false;
          reel->flag = 0;
          reel->adjust = adjust;
          break;
      }
      switch ( reel->flag ) {
        case 0:
          reel->startopto = 6;
          reel->endopto = 8;
          break;
        case 1:
          reel->startopto = 4;
          reel->endopto = 6;
          break;
        case 2:
          reel->startopto = 1;
          reel->endopto = 4;
          break;
        case 3:
          reel->startopto = 0;
          reel->endopto = 4;
          break;
        case 4:
          reel->startopto = 6;
          reel->endopto   = 10;
          break;
      }
    }
    reel++;
  }
  return nbr_reels;
}

//---------------------------------------------------------------------------
int __fastcall TSystem::SetReels(reels *reel_ptr, int nbr, int adjust)
{
int count;
int nbr_reels = 0;
//reel5 *reel5;

  for ( count = 0; count < nbr; count++ ) {
    if ( Form1->Reels[count] != NULL ) {
      nbr_reels++;
      switch ( Form1->ReelType[count] ) {
        case RT_REEL:
          {
            TReel *ptr = (TReel *)Form1->Reels[count];
            reel_ptr->step_size[count] = 96 / ptr->Stops;
            reel_ptr->offset[count] = ptr->Offset;
            reel_ptr->flag[count] = ptr->Flag;
//            reel5->reels[count].inverted = ptr->Inverted;
          }
          break;
        case RT_FANCY:
          {
            TFancyReel *ptr = (TFancyReel *)Form1->Reels[count];
            reel_ptr->step_size[count] = 96 / ptr->Stops;
            reel_ptr->offset[count] = ptr->Offset;
            reel_ptr->flag[count] = ptr->Flag;
//            reel5->reels[count].inverted = ptr->Inverted;
          }
          break;
        case RT_BAND:
          {
            TBandReel *ptr = (TBandReel *)Form1->Reels[count];
            reel_ptr->step_size[count] = 96 / ptr->Stops;
            reel_ptr->offset[count] = ptr->Offset;
            reel_ptr->flag[count] = 0;
//            reel5->reels[count].inverted = false;
          }
          break;
      }
    }
  }
  return nbr_reels;
}
//---------------------------------------------------------------------------
int __fastcall TSystem::LatchState()
{
  return Form1->LatchState();
}
//---------------------------------------------------------------------------
bool __fastcall TSystem::AutoLamp(int lamp, int &reel, int &symbol)
{
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TSystem::CreateAutoLamp()
{
int count, reel, symbol;

  for ( count = 0; count < 256; count++ ) {
    if ( AutoLamp( count, reel, symbol ) ) {
      if ( Form1->Reels[reel] != NULL ) {
        switch ( Form1->ReelType[reel] ) {
          case RT_REEL:
            if ( ((TReel *)Form1->Reels[reel])->ReelLamps && !((TReel *)Form1->Reels[reel])->CustomLamps )
              Form1->InsertLamp( count, Form1->Reels[reel], clYellow, symbol);
            break;
          case RT_FANCY:
            if ( ((TFancyReel *)Form1->Reels[reel])->Lamps && !((TFancyReel *)Form1->Reels[reel])->CustomLamps )
              Form1->InsertLamp( count, Form1->Reels[reel], clYellow, symbol + 1);
            break;
          case RT_BAND:
            break;
        }
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TSystem::WriteCharToMPU4(char ch)
{
  Form1->WriteCharToMPU4(ch);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::WriteCharToVideo(char ch)
{
  Form1->WriteCharToVideo(ch);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::MPU4CharReceived()
{
  Form1->MPU4CharReceived();
}
//---------------------------------------------------------------------------
void __fastcall TSystem::VideoCharReceived()
{
  Form1->VideoCharReceived();
}
//---------------------------------------------------------------------------
void __fastcall TSystem::SetMPU4DCD(Byte value)
{
  Form1->SetMPU4DCD(value);
}
//---------------------------------------------------------------------------
void __fastcall TSystem::SetMPU4CTS(Byte value)
{
  Form1->SetMPU4CTS(value);
}
//---------------------------------------------------------------------------
int __fastcall TSystem::getstack()
{
  return 0;
}
//---------------------------------------------------------------------------
int __fastcall TSystem::getstacktop()
{
  return 0;
}






