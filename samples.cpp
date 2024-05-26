//---------------------------------------------------------------------------

#include <classes.hpp>
#pragma hdrstop

#include <stdio.h>
#include "samples.h"
#include "shared.h"
#include "sample.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSampler *Sampler;

//---------------------------------------------------------------------------
__fastcall TSampler::TSampler(TComponent* Owner)
    : TForm(Owner)
{
String line;
int Length;
char buffer[30];

  if ( Sound ) {
    for ( int i = 0; i < 200; i++ ) {
      Length = Sound->RealDetails(i);
      if ( Length > 8) {
        sprintf(buffer,"%03d %X", i, Length);
        ListBox1->Items->Add(AnsiString(buffer));
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TSampler::SpeedButton1Click(TObject *Sender)
{
String item;
int nbr;

  if ( ListBox1->ItemIndex != -1 ) {
    item = ListBox1->Items->Strings[ListBox1->ItemIndex];
    nbr = item.SubString(1,3).ToIntDef(0);
    Sound->PlaySample(nbr);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSampler::FormClose(TObject *Sender, TCloseAction &Action)
{
  Release();
  Sampler = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TSampler::SaveClick(TObject *Sender)
{
String item;
int nbr;

  if ( ListBox1->ItemIndex != -1 ) {
    item = ListBox1->Items->Strings[ListBox1->ItemIndex];
    nbr = item.SubString(1,3).ToIntDef(0);
    Sound->SaveToFile(nbr,IntToStr(nbr) + ".wav");
  }
}
//---------------------------------------------------------------------------

void __fastcall TSampler::ApplyClick(TObject *Sender)
{
   Sound->AdjustRate( Rate->Text.ToIntDef(16000) );
   Apply->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TSampler::FormShow(TObject *Sender)
{
  Rate->Text = IntToStr(SampleFreq);
}
//---------------------------------------------------------------------------

void __fastcall TSampler::RateChange(TObject *Sender)
{
  Apply->Enabled = true;    
}
//---------------------------------------------------------------------------

