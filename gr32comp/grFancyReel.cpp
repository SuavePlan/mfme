//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <math.h>
#include "grFancyReel.h"
#pragma link "GR32_Image"
#pragma link "GR32_Transforms"
#pragma link "GR32_Add_Blendmodes"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TGRFancyReel::TGRFancyReel(int ANumber, int AX, int AY, int AHeight, int AWidth, TImage32 * Adest)
    : TGRBase(ANumber,"TGRFancyReel",AX,AY,AHeight,AWidth,Adest)
{
//  Width = AWidth;
//  Height = AHeight;
  mask = new TBitmap32();
  destination = new TBitmap32();
  sized_original = new TBitmap32();
  mask->DrawMode=dmCustom;
  mask->OnPixelCombine=Darken;
//  mask->MasterAlpha = 100;
  mask->Clear(clBlack32);
  FDarkness = 100;
  FLampMask = new TBitmap32();
  FBand = new TBitmap32();
  FBand->StretchFilter = sfLinear2;
  FOverlay = new TBitmap32();
  FOverlay->DrawMode = dmBlend;
  FBorderWidth = 1;
  FLampMask2 = new Graphics::TBitmap();
  FLampMask2->Height = 40;
  FLampMask2->Width = 40;
  FBand2 = new Graphics::TBitmap();
  FOverlay2 = new Graphics::TBitmap();
  FOffset = 0;
  FStops = 12;
  FSteps = 96;
  FBand2->Width = 80;
  FBand2->Height = FStops * 56;
  FReversed = false;
  State = 0;
  FReelHeight = 100 * FStops/2;
  FBorderColour = clBlack;
  FWinLines = 1;
  FWinLineWidth = 1;
  for ( int i = 0; i < 5; i++ )
    lamps[i] = -2;
  lines = new TBitmap32();
  lines->DrawMode = dmBlend;
  FToggle = false;
  FExpand = false;
  FFlag = 0;
  line_array = NULL;
  pos = 0;
  FPosition = 0;
//  size = Height/2;
  SetSize(AWidth,AHeight);
  Draw();
}
//---------------------------------------------------------------------------
__fastcall TGRFancyReel::~TGRFancyReel(void)
{
  delete mask;
  delete sized_original;
  delete destination;
  delete FBand;
  delete FOverlay;
  delete FLampMask;
  delete FBand2;
  delete FOverlay2;
  delete FLampMask2;
  delete lines;
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::SetSize(int AWidth, int AHeight)
{
    Width = AWidth;
    Height = AHeight;
    destination->SetSize(AWidth-FBorderWidth*2,AHeight-FBorderWidth*2);
    Bitmap->SetSize(AWidth,AHeight);
    FBand->Assign(FBand2);
    FOverlay->Assign(FOverlay2);
    sized_original->SetSize(AWidth-FBorderWidth*2,FBand->Height);
    FBand->DrawTo(sized_original,Rect(0,0,AWidth-FBorderWidth*2,FBand->Height));
    if ( !FOverlay->Empty() )
      SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
    else
      FOverlay->SetSize(Width,Height);
    FOverlay->PenColor = clBlack32;
    size = (((double)(destination->Height)/2) / sin((M_PI/(double)FStops) * ((double)FReelHeight / 100)));
    mask->Height = int((destination->Height) / sin((M_PI/(double)FStops) * ((double)FReelHeight / 100)));
    mask->SetSize(destination->Width,mask->Height);
    windowrect = Rect( 0, (( mask->Height - (destination->Height))/2), destination->Width, (mask->Height + (destination->Height))/2);
    FLampMask->Assign(FLampMask2);
    if ( !FLampMask->Empty() )
      Dark = *FLampMask->PixelPtr[0][0];
    InitArray();
    DrawWinLines();
    ConstructLampMask();
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::Draw(void)
{
int j,k;
TColor32Array *big2, *reel2;
float *array;
int copy = sized_original->Width * sizeof(TColor32Array);

  if ( sized_original->Height ) {
    array = &line_array[0];
    for ( j = 0; j < destination->Height; j++ ) {
      k = *array+pos;
      if ( k < 0 )
        k += sized_original->Height;
      else if ( k >= sized_original->Height )
        k -= sized_original->Height;
      big2 = (TColor32Array *)sized_original->ScanLine[k];
      reel2 = (TColor32Array *)destination->ScanLine[j];
      memcpy( reel2, big2, copy );
      array++;
    }
  }
  image->BeginUpdate();
  destination->DrawTo(image->Bitmap, X+FBorderWidth, Y+FBorderWidth);
  if ( FLamps )
    mask->DrawTo(image->Bitmap, Rect(X+FBorderWidth,Y+FBorderWidth,X+Width-FBorderWidth,Y+Height-FBorderWidth),windowrect);
  if ( FWinLines || FBorderWidth )
    lines->DrawTo(image->Bitmap,X,Y);
  if ( FOverlay->Height )
    FOverlay->DrawTo(image->Bitmap,X,Y);
  image->EndUpdate();
  if ( Flush )
    image->Flush(Dstrect);
//  TGRBase::Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetPos(int Value)
{
  if ( Value != FPosition ) {
    FPosition = (Value + FSteps) % FSteps;
    if ( FPosition < 0 || FPosition > FSteps )
      FPosition = 0;
    Setpos2();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::Setpos2(void)
{
int newpos = FPosition;

  if ( FReversed )
    newpos = (FSteps - newpos) % FSteps;
  newpos += FOffset;
  pos = (symbol / 2) + ((newpos * sized_original->Height) / FSteps);
  Draw();
}

//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetLamp(int nbr, int state)
{
  if ( state )
    State |= (1 << nbr);
  else
    State &= ~(1 << nbr);
  ConstructLampMask();
  image->BeginUpdate();
  destination->DrawTo(image->Bitmap, X+FBorderWidth, Y+FBorderWidth);
  if ( FLamps )
    mask->DrawTo(image->Bitmap, Rect(X+FBorderWidth,Y+FBorderWidth,X+Width-FBorderWidth,Y+Height-FBorderWidth),windowrect);
  if ( FWinLines || FBorderWidth )
    lines->DrawTo(image->Bitmap,X,Y);
  if ( FOverlay->Height )
    FOverlay->DrawTo(image->Bitmap,X,Y);
  image->EndUpdate();
  if ( Flush )
    image->Flush(Dstrect);
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetLamps(int state)
{
  if ( State != state ) {
    if ( FReversed )
      State = (state & 4) + ((state & 2) << 2) + ((state & 8) >> 2);
    else
      State = state;
    ConstructLampMask();
    image->BeginUpdate();
    destination->DrawTo(image->Bitmap, X+FBorderWidth, Y+FBorderWidth);
    if ( FLamps )
      mask->DrawTo(image->Bitmap, Rect(X+FBorderWidth,Y+FBorderWidth,X+Width-FBorderWidth,Y+Height-FBorderWidth),windowrect);
    if ( FWinLines || FBorderWidth )
      lines->DrawTo(image->Bitmap,X,Y);
    if ( FOverlay->Height )
      FOverlay->DrawTo(image->Bitmap,X,Y);
    image->EndUpdate();
    if ( Flush )
      image->Flush(Dstrect);
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::SetOffset(int Value)
{
  if ( Value != FOffset ) {
    FOffset = Value;
    Setpos2();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetRHeight(int Value)
{
float view, angle;
float height;

  view = sized_original->Height / M_PI;
  angle = Value / view;

  height = view * sin(angle);
  SetReelHeight(height*2);
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::SetDarkness(int Value)
{
  if ( Value != FDarkness ) {
    FDarkness = Value;
    mask->MasterAlpha = FDarkness;
    ConstructLampMask();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetReelHeight(int Value)
{
  if ( Value > 0 && ((double)Value / 100) <= (FStops/2) ) {
    FReelHeight = Value;
    SetSize(Width,Height);
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetStops(int Value)
{
  if ( Value != FStops ) {
    FStops = Value;
    SetSize(Width,Height);
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetSteps(int Value)
{
  if ( Value != FSteps ) {
    FSteps = Value;
    SetSize(Width,Height);
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetBorderWidth(int Value)
{
  if ( Value != FBorderWidth ) {
    FBorderWidth = Value;
    SetSize(Width,Height);
//    DrawWinLines();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetBorderColour(TColor FColour)
{
  if ( FColour != FBorderColour ) {
    FBorderColour = FColour;
    DrawWinLines();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetWinLines(int Value)
{
  if ( Value != FWinLines ) {
    FWinLines = Value;
    DrawWinLines();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetWinLineWidth(int Value)
{
  if ( Value != FWinLineWidth ) {
    FWinLineWidth = Value;
    DrawWinLines();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetReversed(bool Value)
{
  if ( Value != FReversed ) {
    FReversed = Value;
    Setpos2();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetLamps(bool Value)
{
  if ( Value != FLamps ) {
    FLamps = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetCustom(bool Value)
{
  if ( Value != FCustom ) {
    FCustom = Value;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetExpand(bool Value)
{
  if ( FToggle && Value != FExpand ) {
    FExpand = Value;
    if ( Value ) {
      Expanded = FReelHeight;
      FReelHeight = BandHeight;
    } else
      FReelHeight = Expanded;
    InitSize();
    DrawWinLines();
    ConstructLampMask();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetToggle(bool Value)
{
  if ( FToggle != Value ) {
    FToggle = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetInverted(bool Value)
{
  if ( FInverted != Value ) {
    FInverted = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetHorizontal(bool Value)
{
  if ( FHorizontal != Value ) {
    FHorizontal = Value;
    Draw();
  }
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::SetBand(Graphics::TBitmap *ptr)
{
  FBand->Assign(ptr);
  FBand2->Assign(ptr);
  SetSize(Width,Height);
  Draw();
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::SetOverlay(Graphics::TBitmap *ptr)
{
  if ( ptr->Width && ptr->Height ) {
    FOverlay->Assign(ptr);
    FOverlay2->Assign(ptr);
    SetAlpha(FOverlay, *FOverlay->PixelPtr[FOverlay->Width/2][FOverlay->Height/2]);
//  DrawWinLines();
    Draw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetMask(Graphics::TBitmap *ptr)
{
  FLampMask->Assign(ptr);
  FLampMask2->Assign(ptr);
  if ( !FLampMask->Empty() )
    Dark = *FLampMask->PixelPtr[0][0];
  ConstructLampMask();
  Draw();
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRFancyReel::GetBand(void)
{
  return FBand2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRFancyReel::GetOverlay(void)
{
  return FOverlay2;
}
//---------------------------------------------------------------------------
Graphics::TBitmap * __fastcall TGRFancyReel::GetMask(void)
{
  return FLampMask2;
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::ConstructLampMask(void)
{
int i, msk = 1;

  mask->Clear(Dark);
  for ( i = 0; i < 6; i++, msk *= 2 )
    if ( (State & msk) || lamps[i] == -1 )
      mask->Draw( rect[i], Rect(0,0, FLampMask->Width, FLampMask->Height), FLampMask);

//  mask->SaveToFile("mask.bmp");
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetAlpha(TBitmap32 *ptr, TColor32 Colour)
{
PColor32 P;
TColor32 C;
int I, J;

  ptr->ResetAlpha();
  Colour = Colour & 0x00FFFFFF;
  for ( I = 0; I < ptr->Height; I++ ) {
    for ( J = 0; J < ptr->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour )
        *P = C;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::SetLampNumber(int index, int value)
{
  lamps[index] = value;
}
//---------------------------------------------------------------------------
int __fastcall TGRFancyReel::GetLampNumber(int index)
{
  return lamps[index];
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::InitArray(void)
{
int j;
float pos2;

  if ( line_array )
    delete[] line_array;
  line_array = new float[destination->Height];

  for ( j = 0; j < 6; j++ )
    lamp_pos[j] = 0;

  symbol = sized_original->Height / FStops;
  int mark = -(((sized_original->Height / FStops) * 2) + ( symbol / 2));
  int pos = 0;

  for ( j = 0; j < destination->Height; j++ ) {
    double angle = acos((((double)destination->Height/2) - j) / size);
    line_array[j] = ((angle / M_PI) - 0.5 ) * ((double)sized_original->Height / 2) + 0.5;
  }
  for ( j = 0; j < mask->Height; j++ ) {
    double angle = acos((((double)mask->Height/2) - j) / ((double)mask->Height/2));
    pos2 = ((angle / M_PI) - 0.5 ) * ((double)sized_original->Height / 2) + 0.5;
    if ( pos2 >= mark && !lamp_pos[pos] && pos < 6 ) {
      mark += symbol;
      lamp_pos[pos++] = j;
    }
  }
  for ( j = 0; j < 6; j++ )
    rect[j] = Rect( 0, lamp_pos[j], mask->Width, lamp_pos[j+1]);

  for ( j = 0; j < 6; j++ )
    lamprect[j] = Rect(0, (destination->Height/2) - ((mask->Height/2 )-lamp_pos[j]), destination->Width, (destination->Height/2) + (lamp_pos[j+1] - (mask->Height/2 )));
//  windowrect = Rect( 0, (( mask->Height - (destination->Height))/2), destination->Width, (mask->Height + (destination->Height))/2);
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::Darken(TColor32 F, TColor32 &B, TColor32 M)
{
int fR, fG, fB, bR, bG, bB;
//  Channel separation
  bR = (B >> 16) & 0xFF;
  bG = (B >> 8)  & 0xFF;
  bB = B         & 0xFF;
  fR = (F >> 16) & 0xFF;
  fG = (F >> 8)  & 0xFF;
  fB = F         & 0xFF;
//  Darken
  bR = (bR * fR) >> 8;
  bG = (bG * fG) >> 8;
  bB = (bB * fB) >> 8;
//  Reset - keeping B alpha
  B = (B & 0xff000000) | (bR << 16) | (bG << 8) | bB;
}
//---------------------------------------------------------------------------
void __fastcall TGRFancyReel::DrawWinLines(void)
{
int i,j;
int height = Height/*-FBorderWidth*2*/;
int width = Width/*-FBorderWidth*2*/;
TCanvas *Canvas = new TCanvas();
int offset, offset2;


  lines->SetSize(width,height);
  Canvas->Handle = lines->Handle;
    lines->Clear(clWhite32);
  
  if ( FBorderWidth )
  {
    offset = FBorderWidth/2;
    offset2 = (FBorderWidth+1)/2;
    Canvas->Pen->Width = FBorderWidth;
    Canvas->Pen->Color = FBorderColour;
    Canvas->MoveTo( offset, offset);
    Canvas->LineTo(Width-offset2,offset);
    Canvas->LineTo(Width-offset2,Height-offset2);
    Canvas->LineTo(offset,Height-offset2);
    Canvas->LineTo(offset,offset);
  }

//    lines->PenColor = Color32(FBorderColour);
    Canvas->Pen->Color = FBorderColour;
    Canvas->Pen->Width = FWinLineWidth;

  if ( FWinLines ) {
    for ( i = 0; i < 3; i++ ) {
      if ( i == 1 || ( i == 0 && FWinLines > 2) || ( i == 2 && FWinLines > 1) ) {
        Canvas->MoveTo(0, (lamprect[i+1].Bottom + lamprect[i+1].Top) / 2);
        Canvas->LineTo(lines->Width,(lamprect[i+1].Bottom + lamprect[i+1].Top) / 2);
      }
      if ( FWinLines > 3 ) {
        if ( (i == 0 && Number == 0) || ( i == 2 && Number == 2) || ( i == 1 && Number == 1) ||
             (i == 0 && Number == 3) || ( i == 2 && Number == 5) || ( i == 1 && Number == 4) ) {
          Canvas->MoveTo(0, lamprect[i+1].Top+2);    //  Top to Bottom
          Canvas->LineTo(lines->Width,lamprect[i+1].Bottom-2);
        }
        if ( FWinLines > 5 ) {
          if ( (i == 0 && Number == 4) || ( i == 1 && Number == 5) || ( i == 2 && Number == 6) ) {
            Canvas->MoveTo(0, lamprect[i+1].Top+2);    //  Top to Bottom
            Canvas->LineTo(lines->Width,lamprect[i+1].Bottom-2);
          }
        }
        if ( (i == 2 && Number == 0) || ( i == 0 && Number == 2) || ( i == 1 && Number == 1) ||
             (i == 2 && Number == 3) || ( i == 0 && Number == 5) || ( i == 1 && Number == 4) ) {
          Canvas->MoveTo(0, lamprect[i+1].Bottom-2); // Bottom to Top
          Canvas->LineTo(lines->Width,lamprect[i+1].Top+2);
        }
        if ( FWinLines > 5 ) {
          if ( (i == 2 && Number == 4) || ( i == 1 && Number == 5) || ( i == 0 && Number == 6) ) {
            Canvas->MoveTo(0, lamprect[i+1].Bottom-2); // Bottom to Top
            Canvas->LineTo(lines->Width,lamprect[i+1].Top+2);
          }
        }
      }
    }
  }
  lines->ResetAlpha();
  SetAlpha(lines,clWhite32);
//    lines->SaveToFile("lines.bmp");
  delete Canvas;
}
//---------------------------------------------------------------------------

void __fastcall TGRFancyReel::SaveToStream(TStream *ptr)
{
//  on->SaveToStream(ptr);
//  off->SaveToStream(ptr);
}

