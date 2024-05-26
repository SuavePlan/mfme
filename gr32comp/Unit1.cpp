//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>

#include "math.h"
#include "base.h"
#include "Unit1.h"
#include "FancyReel.h"
#include "FancyLamp.h"
#include "AlphaDisplay.h"
#include "DiscReel.h"
#include "BandReel.h"
#include "SevenSeg.h"
#include "bfmleddisplay.h"
#include "matrixalpha.h"
#include "matrixdisplay.h"
#include "video.h"
#include "background.h"
#include "label.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GR32_Image"
#pragma link "Std2"
#pragma resource "*.dfm"
TForm1 *Form1;
TCompList TBase::List;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
TPaintStage * stage;

//  Image->Bitmap->SetSize(1018, 691);
//  Image->Bitmap->Clear(Color32(clBtnFace));
//  ptr = new TBitmapLayer(Image->Layers);
  Image->Bitmap->SetSize(Image->Width,Image->Height);
  Image->Bitmap->Clear(Color32(clBtnFace));

  background = new TBackground(0, 0, 0, Image->Height, Image->Width, Image);
//  ptr->Location = FloatRect(0,0,ptr->Bitmap->Width,ptr->Bitmap->Height);
//  ptr->Scaled = false;
//  Image->Bitmap = background;
//  Image->ScaleMode = smResize;
//  background->DrawTo(Image->Buffer);
//  ptr->Location = FloatRect(0,0,ptr->Bitmap->Width,ptr->Bitmap->Height);
//  TrackBar1->Max = ptr->Bitmap->Width;
//  TrackBar2->Max = ptr->Bitmap->Height;
//  ptr = new TBitmapLayer(Image->Layers);
//  ptr->Bitmap->LoadFromFile("c:\\release\\apache gold\\apache3.bmp");
//  ptr->Location = FloatRect(100,100,150,150);
//  ptr->Bitmap->DrawMode = dmBlend;
//  ptr->Scaled = Gr32_image::smScale;
//  inlay = ptr;
//  inlay = NULL;
/*  Image->OnPaintStage = OnPaintStage;
  Image->ScaleMode = smStretch;
  stage = Image->PaintStages->Add();
  stage->DsgnTime = false;
  stage->RunTime = true;
  stage->Stage = PST_CUSTOM;
  stage->Parameter = 0;

  stage = Image->PaintStages->Add();
  stage->DsgnTime = false;
  stage->RunTime = true;
  stage->Stage = PST_DRAW_BITMAP;
  stage->Parameter = 0;   */

  dir = false;
  for ( int i = 0; i < 320; i++ )
    lamps[i] = NULL;
  reel[0] = 0;
  reel[1] = 0;
  reel[2] = 0;
  move = false;
  pos = 0;
  pos2 = 0;
//  PaintBox->OnPaintBuffer = DoPaintBuffer;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBar1Change(TObject *Sender)
{
  Image->Width = TrackBar1->Position;
  width->Text = IntToStr(Image->Bitmap->Width);
  height->Text = IntToStr(Image->Bitmap->Height);

  width2->Text = IntToStr(((Image->Width * 65536) / Image->Bitmap->Width));
  height2->Text = IntToStr(((Image->Height * 65536) / Image->Bitmap->Height));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBar2Change(TObject *Sender)
{
  Image->Height = TrackBar2->Position;
  width2->Text = IntToStr(((Image->Width * 65536) / Image->Bitmap->Width));
  height2->Text = IntToStr(((Image->Height * 65536) / Image->Bitmap->Height));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OnPaintStage(TObject *Sender, TBitmap32 *Dest, unsigned int StageNum)
{
//  width->Text = Image->Bitmap->Width;
//  height->Text = Image->Bitmap->Height;
//  width2->Text = Image->Buffer->Width;
//  height2->Text = Image->Buffer->Height;
//  if ( StageNum == 5 )
//    Image->Buffer->DrawTo(Image->Bitmap);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrackBar3Change(TObject *Sender)
{
  Edit2->Text = IntToStr(TrackBar3->Position);
//  disc->SetDarkness(TrackBar3->Position);

//Image->BeginUpdate();
//  background->DrawTo(Image->Bitmap);
  if ( disc )
    disc->SetState(TrackBar3->Position);
  if ( seg )
    seg->SetAngle(TrackBar3->Position);
  if ( reel[0] )
    reel[0]->SetAngle(TrackBar3->Position);
  if ( reel[1] )
    reel[1]->SetAngle(TrackBar3->Position);
  if ( reel[2] )
    reel[2]->SetAngle(TrackBar3->Position);
  if ( alpha )
    alpha->SetAngle(TrackBar3->Position);
//  Image->Bitmap = background;
    TBase::List.Redraw();
//Image->EndUpdate();
Image->Changed();
//  disc->Draw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
Image->BeginUpdate();
//  if ( dir ) {
//    inlay->Bitmap->MasterAlpha++;
//    if ( inlay->Bitmap->MasterAlpha == 255 )
//      dir = false;
//  } else {
//    inlay->Bitmap->MasterAlpha--;
//    if ( inlay->Bitmap->MasterAlpha == 0 )
//      dir = true;
//  }
//Image->EndUpdate();
  count++;
  if ( count >= 1 ) {
    count = 0;
    on = !on;
  for ( int i = 0; i < 320; i++ ) {
    if ( lamps[i] )
      if ( on )
        lamps[i]->SetState(1);
      else
        lamps[i]->SetState(0);
  }
  }
  pos--;
  if ( pos < 0 )
    pos = 95;
  if ( reel[0] )
    reel[0]->SetPosition(pos);
  if ( reel[1] )
    reel[1]->SetPosition(pos);
  if ( reel[2] )
    reel[2]->SetPosition(pos);
  if ( disc )
    disc->SetPosition(pos);
  pos2--;
  if ( pos2 < 0 )
    pos2 = 319;
  if ( band )
    band->SetPosition(pos2);

Image->EndUpdate();
//Image->Changed();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BitBtn1Click(TObject *Sender)
{
Graphics::TBitmap *ptr;

  ptr = new Graphics::TBitmap();
  ptr->LoadFromFile("c:\\release\\Andys Big Time\\band1.bmp");
  for ( int i = 0; i < 3; i++) {
    reel[i] = new TFancyReel(i, 140 + i * 100, 80, 140, 80, Image);
    reel[i]->SetBand(ptr);
//  ((TFancyReel *)reel)->SetBorderWidth(1);
//  ((TFancyReel *)reel)->SetWinLines(1);
  }
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Change(TObject *Sender)
{
//  ((TFancyReel *)reel)->SetBorderWidth(Edit1->Text.ToIntDef(1));
  if ( seg )
    seg->SetSpace(Edit1->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn2Click(TObject *Sender)
{
Graphics::TBitmap * ptr = new Graphics::TBitmap();
String name;
int X, Y, nbr;
char type[10];

  if ( OpenPictureDialog1->Execute() ) {
    for ( int i = 0; i < OpenPictureDialog1->Files->Count; i++ ) {
      name = OpenPictureDialog1->Files->Strings[i];
      ptr->LoadFromFile( name );
      name = ExtractFileName(name);
      sscanf( name.c_str(), "%d_%d_%d_%s", &X, &Y, &nbr, type);
      if ( !lamps[nbr] )
        lamps[nbr] = new TFancyLamp( nbr, X, Y, ptr->Height, ptr->Width, Image);
      if ( ptr->Height && ptr->Width ) {
        if ( !strncmp( type, "on", 2) )
          lamps[nbr]->SetOn(ptr);
        else
          lamps[nbr]->SetOff(ptr);
      }
    }
    delete ptr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn3Click(TObject *Sender)
{
Graphics::TBitmap * ptr = new Graphics::TBitmap();

  ptr->LoadFromFile("c:\\fruitpublic\\alpha.bmp");
  alpha = new TAlpha( 0, 200, 200, 20, 200, Image);
  alpha->SetBitmap(ptr);
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer)
{
int  X2 = ((( Image->Bitmap->Width * 65536 ) / Image->Width ) * X ) / 65536;
int  Y2 = ((( Image->Bitmap->Height * 65536 ) / Image->Height ) * Y ) / 65536;

  sel = TBase::List.FindXY(X2,Y2);
  if ( sel ) {
    move = true;
    biasX = X2 - sel->X;
    biasY = Y2 - sel->Y;
//    Timer1->Enabled = false;
    oldX = X;
    oldY = Y;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y, TCustomLayer *Layer)
{
int  X2 = ((( Image->Bitmap->Width * 65536 ) / Image->Width ) * X ) / 65536;
int  Y2 = ((( Image->Bitmap->Height * 65536 ) / Image->Height ) * Y ) / 65536;

  if ( move /*&& ( X != oldX || Y != oldY ) */) {
    sel->SetPosition(X2 - biasX, Y2 - biasY);
//    sel->X = X - biasX;
//    sel->Y = Y - biasY;
//    Image->Bitmap = background;
    TBase::List.Redraw();
    Image->Changed();
    oldX = X;
    oldY = Y;
  }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ImageMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y, TCustomLayer *Layer)
{
  move = false;
//  Timer1->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Save()
{
TMemoryStream *ptr = new TMemoryStream();

  for ( int i = 0; i < TBase::List.Count; i++ ) {
    TBase::List[i]->SaveToStream(ptr);
  }
  ptr->SaveToFile("stream.dat");
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn4Click(TObject *Sender)
{
  Save();
}
//---------------------------------------------------------------------------


void __fastcall TForm1::BitBtn5Click(TObject *Sender)
{
Graphics::TBitmap * ptr = new Graphics::TBitmap();

  disc = new TDiscReel(0, 300, 300, 160, 160, Image);
  ptr->LoadFromFile("c:\\release\\5 Alive\\untitled5.bmp");
  disc->SetSrc(ptr);
  ptr->LoadFromFile("c:\\release\\5 Alive\\overlay.bmp");
  disc->SetOverlay(ptr);
  disc->SetOuterH(63);
  disc->SetOuterL(30);
  ptr->LoadFromFile("c:\\release\\reel lamp.bmp");
  disc->SetMask(ptr);
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn6Click(TObject *Sender)
{
Graphics::TBitmap * ptr = new Graphics::TBitmap();

  ptr->LoadFromFile("c:\\gr32 fruit\\new folder\\vvg.bmp");
  band = new TBandReel(0, 400, 400, 70, 300, Image);
  band->SetBand(ptr);
  delete ptr;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn7Click(TObject *Sender)
{
  seg = new TSevenSeg( 0, 200, 400, 32, 24, Image);
  seg->SetValue(255);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn8Click(TObject *Sender)
{
  bfmled = new TBFMLED( 0, 300, 300, 5, 5, Image);
  for ( int i = 0; i < 20; i++ )
    bfmled->SetDot(i, 255);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn9Click(TObject *Sender)
{
  dotalpha = new TDotAlpha(0, 350, 400, 5, 5, Image);    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn10Click(TObject *Sender)
{
  matrix = new TDotmatrix(0, 370, 400, 5, 5, Image);
  for ( int i = 0; i < 9*21; i++ )
    matrix->SetPixel(i, 255);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn11Click(TObject *Sender)
{
  video = new TVideo(0, 400, 400, 0, 0, Image);    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TrackBar4Change(TObject *Sender)
{
/*  TFloatRect Rect;
  Image->Scale = pow( 10, (double)TrackBar4->Position/100);
  Rect = ((TBitmapLayer *)Image->Layers->Items[0])->GetAdjustedLocation();
  Edit3->Text = IntToStr((int)Rect.Right);
  Edit4->Text = IntToStr((int)Rect.Bottom); */
  background->SetDarkness(TrackBar4->Position);
//  Image->Width = Rect.Right;
//  Image->Height = Rect.Bottom;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::DoPaintBuffer(TObject *Sender)
{
  ((TPaintBox32 *)Sender)->Buffer->Clear(Color32(clBtnFace));
}

//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn12Click(TObject *Sender)
{
String txt = "£1";

  label = new TTextLabel(0, 50, 50, 20, 50, Image);
  label->SetText(txt);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BitBtn13Click(TObject *Sender)
{
Graphics::TBitmap *ptr = new Graphics::TBitmap();

  if ( OpenPictureDialog1->Execute() ) {
    ptr->LoadFromFile(OpenPictureDialog1->FileName);
    background->SetBitmap(ptr);
  }
  delete ptr;
}
//---------------------------------------------------------------------------

