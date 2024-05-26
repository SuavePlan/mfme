//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <stdlib.h>
#include "reeleditor.h"
#include "interface.h"
#include "band.h"
#include "createband.h"

#define SHEIGHT 56
#define SWIDTH 80

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditor *Editor;

//---------------------------------------------------------------------------
__fastcall TEditor::TEditor(TComponent* Owner)
    : TForm(Owner)
{
int X, Y;
TRect rect;
int stops;

  BigReel->Canvas->Brush->Color = clWhite;
  Overlay->Canvas->Brush->Color = clWhite;
  Reels[0] = Reel1;
  Reels[1] = Reel2;
  Reels[2] = Reel3;
  Reels[3] = Reel4;
  Reels[4] = Reel5;
  Reels[5] = Reel6;
  Reels[6] = Reel7;
  Reels[7] = Reel8;
  Reels[8] = Reel9;
  Reels[9] = Reel10;

  for ( X = 0; X < NBR_REELS; X++ ) {
    Reels[X]->Picture->Bitmap->Width = SWIDTH;
    if ( Form1->Reels[X] ) {
      switch ( Form1->ReelType[X] ) {
        case RT_REEL:
          {
            stops = ((TReel *)Form1->Reels[X])->Stops;
            Reels[X]->Height = stops * 21;
            Reels[X]->Tag = stops;
            Graphics::TBitmap * ptr = ((TReel *)Form1->Reels[X])->GetBitmap(0);
            int height = ptr->Height;
            if ( !height )
              height = 56;
            int width = ptr->Width;
            if ( !width )
              width = 80;
            Reels[X]->Picture->Bitmap->Height = stops * height;
            Reels[X]->Picture->Bitmap->Width = width;
            for ( Y = 0; Y < NBR_STOPS; Y++ ) {
              if ( ((TReel *)Form1->Reels[X])->GetBitmap(Y) != NULL ) {
                rect = Rect( 0, height * Y, width, height + height * Y);
                Reels[X]->Picture->Bitmap->Canvas->StretchDraw(rect,((TReel *)Form1->Reels[X])->GetBitmap(Y));
              }
            }
          }
          break;
        case RT_FANCY:
          stops = ((TFancyReel *)Form1->Reels[X])->Stops;
          Reels[X]->Height = stops * 21;
          Reels[X]->Tag = stops;
          Reels[X]->Picture->Bitmap->Assign(((TFancyReel *)Form1->Reels[X])->GetBand());
          break;
        case RT_BAND:
          stops = ((TBandReel *)Form1->Reels[X])->Stops;
          Reels[X]->Height = stops * 21;
          Reels[X]->Tag = stops;
          Reels[X]->Picture->Bitmap->Assign(((TBandReel *)Form1->Reels[X])->GetBand());
          break;
      }
    } else {
      Reels[X]->Height = 25 * 21;
      Reels[X]->Tag = 25;
      Reels[X]->Picture->Bitmap->Width = SWIDTH;
      Reels[X]->Picture->Bitmap->Height = 25 * SHEIGHT;
      Reels[X]->Picture->Bitmap->Canvas->Brush->Color = clWhite;
      Reels[X]->Picture->Bitmap->Canvas->FillRect(Rect(0,0,SWIDTH,25*SHEIGHT));
    }
  }
//  CreateBandForm->BandHeight->Text = IntToStr(SHEIGHT);
//  CreateBandForm->BandWidth->Text = IntToStr(SWIDTH);
  HasImage = false;
  Apply->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TEditor::ApplyClick(TObject *Sender)
{
int X, Y;
Graphics::TBitmap *ptr;

  Form1->LayoutChanged();
  for ( X = 0; X < NBR_REELS; X++ ) {
    if ( Form1->Reels[X] ) {
      switch ( Form1->ReelType[X] ) {
        case RT_REEL:
          for ( Y = 0; Y < NBR_STOPS; Y++ ) {
            ptr = GetSymbol( Y, Reels[X] );
            ((TReel *)Form1->Reels[X])->SetBitmap( Y, ptr);
            delete ptr;
          }
          ((TReel *)Form1->Reels[X])->DrawReels();
          ((TReel *)Form1->Reels[X])->Draw();
          break;
        case RT_FANCY:
          ((TFancyReel *)Form1->Reels[X])->SetBand(Reels[X]->Picture->Bitmap);
          break;
        case RT_BAND:
          ((TBandReel *)Form1->Reels[X])->SetBand(Reels[X]->Picture->Bitmap);
          break;
      }
    }
  }
  Apply->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TEditor::ImageDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
Graphics::TBitmap *ptr;

  if ( Source != BigReel && Source != Overlay )
    ptr = GetSymbol(drag_symbol, (TImage *)Source);
  else
    ptr = ((TImage *)Source)->Picture->Bitmap;
  SetSymbol( Y / 21, (TImage *)Sender, ptr);
  if ( Source != BigReel && Source != Overlay )
    delete ptr;
    Apply->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TEditor::CopyToBigReel(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    if ( Button == mbLeft ) {
      if ( 1 ) {
        Graphics::TBitmap *ptr = GetSymbol(Y/21,(TImage *)Sender);
        BigReel->Picture->Bitmap->Assign(ptr);
        delete ptr;
        HasImage = true;
        PreviewSize->Caption = IntToStr(BigReel->Picture->Bitmap->Width) + " x " +
                               IntToStr(BigReel->Picture->Bitmap->Height);
      } else {
        BigReel->Canvas->Brush->Color = clWhite;
        BigReel->Canvas->FillRect(BigReel->Canvas->ClipRect);
        HasImage = false;
      }
      if ( !((TImage *)Sender)->Picture->Bitmap->Empty ) {
        ((TImage *)Sender)->BeginDrag(false,0);
        drag_symbol = Y / 21;
      }
    } else {
      int row = Y / 21;
      int width = ((TImage *)Sender)->Picture->Bitmap->Width;
      int height = ((TImage *)Sender)->Picture->Bitmap->Height / ((TImage *)Sender)->Tag;
      TRect rect = Rect(0, row * height, width, height + row * height);
      ((TImage *)Sender)->Picture->Bitmap->Canvas->FillRect(rect);
      Apply->Enabled = true;
    }
}
//---------------------------------------------------------------------------

void __fastcall TEditor::ImageDragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
  if ( drag_symbol == ( Y / 21 ) && Sender == Source )
    Accept = FALSE;
  else
    Accept = TRUE;
}
//---------------------------------------------------------------------------
void __fastcall TEditor::SaveImagesClick(TObject *Sender)
{
int X, Y;
char FileName[8];
String Path;

  SaveDialog1->Filter = "BMP files (*.bmp)|*.bmp";
  SaveDialog1->DefaultExt = "bmp";
  if ( SaveDialog1->Execute() ) {
    Path = ExtractFilePath(SaveDialog1->FileName);
    Graphics::TBitmap *ptr;
    for ( X = 0; X < NBR_REELS; X++ ) {
      for ( Y = 0; Y < NBR_STOPS; Y++ ) {
        ptr = GetSymbol( Y, Reels[X] );
//        if ( !Images[X][Y]->Picture->Bitmap->Empty ) {
          sprintf( FileName, "%d%02d.bmp", X, Y + 1);
        ptr->SaveToFile(Path + AnsiString(FileName));
        delete ptr;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TEditor::LoadImagesClick(TObject *Sender)
{
int count;
String bmp_file;
int image_nbr;
int reel_nbr;
int nbr_images;
int id;
bool updated = false;
  OpenPictureDialog1->Filter = "Reel Images (*.bmp)|*.bmp";
  OpenPictureDialog1->DefaultExt = AnsiString("bmp");
  OpenPictureDialog1->Options << ofFileMustExist;

  if ( OpenPictureDialog1->Execute() ) {
    Graphics::TBitmap *ptr = new Graphics::TBitmap();
    nbr_images = OpenPictureDialog1->Files->Count;
    for ( count = 0; count < nbr_images; count++ ) {
      bmp_file = OpenPictureDialog1->Files->Strings[count];
      if ( bmp_file.Length() > 6 ) {
        id = StrToIntDef(bmp_file.SubString( bmp_file.Pos(".") - 3, 3), 65535);
        if ( id > 0 && id < 925 ) {
          reel_nbr = id/100;
          image_nbr = (id % 100)-1;
          ptr->LoadFromFile(bmp_file);
          SetSymbol( image_nbr, Reels[reel_nbr], ptr);
          updated = true;
        }
      }
    }
    delete ptr;
  }
  if ( updated )
    Apply->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditor::ClearClick(TObject *Sender)
{
int X;

  for ( X = 0; X < NBR_REELS; X++ )
    Reels[X]->Picture->Bitmap->Canvas->FillRect(Rect(0,0,Reels[X]->Picture->Bitmap->Width,Reels[X]->Picture->Bitmap->Height));
  Apply->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditor::DefaultClick(TObject *Sender)
{
int X, Y, width, height;
String Text;
TFont *font;
TColor Colors[5] = { clRed, clYellow, clGreen, clBlue, clTeal };
Graphics::TBitmap *ptr = new Graphics::TBitmap();

  font = new TFont();
  font->Charset = 0;
  font->Height = 60;
  font->Name = "Challenge Extra Bold";
  ptr->Canvas->Font = font;
  ptr->Canvas->Brush->Color = clWhite;
  for ( X = 0; X < NBR_REELS; X++ ) {
    ptr->Width = Reels[X]->Picture->Bitmap->Width;
    ptr->Height = Reels[X]->Picture->Bitmap->Height / Reels[X]->Tag;
    int stops = Reels[X]->Tag;
    for ( Y = 0; Y < stops; Y++ ) {
      ptr->Canvas->FillRect(Rect(0,0,ptr->Width,ptr->Height));
//      ClearImage(Images[X][Y],clWhite);
//      Images[X][Y]->Picture->Bitmap->Width = 30;
//      Images[X][Y]->Picture->Bitmap->Height = 20;
      Text = IntToStr(Y + 1);
//      Images[X][Y]->Canvas->Font = font;
      ptr->Canvas->Font->Color = Colors[Y % 4];
      width = ptr->Canvas->TextWidth(Text);
      height = ptr->Canvas->TextHeight(Text);
      ptr->Canvas->TextOut((ptr->Width - width) / 2 ,( ptr->Height - height) / 2, Text);
      Reels[X]->Picture->Bitmap->Canvas->Draw(0,ptr->Height*Y,ptr);
    }
  }
  delete font;
  delete ptr;
  Apply->Enabled = true;
}
//---------------------------------------------------------------------------


void __fastcall TEditor::TxtFontClick(TObject *Sender)
{
  FontDialog2->Font = TextFont->Font;
  if ( FontDialog2->Execute() ) {
    TextFont->Font = FontDialog2->Font;
  }
}
//---------------------------------------------------------------------------

void __fastcall TEditor::AddNumberClick(TObject *Sender)
{
  int ogx, ogy;
  unsigned int Flags;
  TRect TxtRect;

  // Add Symbol
  // Panel: SymbFontPrev
  // Caption: EditSymbol
  BigReel->Picture->Bitmap->Canvas->Pen->Mode=pmCopy;
  BigReel->Picture->Bitmap->Canvas->Pen->Color=SymFont->Brush->Color;
  BigReel->Picture->Bitmap->Canvas->Brush->Color=SymFont->Brush->Color;
  BigReel->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
  BigReel->Picture->Bitmap->Canvas->Font=SymFont->Font;
  ogy=BigReel->Picture->Bitmap->Canvas->TextHeight("W")+10;
  ogx=ogy;
  BigReel->Picture->Bitmap->Canvas->Ellipse(BigReel->Picture->Bitmap->Width-ogx-5,
  BigReel->Picture->Bitmap->Height-ogy-5,
  BigReel->Picture->Bitmap->Width-5,
  BigReel->Picture->Bitmap->Height-5);
  TxtRect.Top=BigReel->Picture->Bitmap->Height-ogy-1;
  TxtRect.Left=BigReel->Picture->Bitmap->Width-ogx-2;
  TxtRect.Bottom=TxtRect.Top+(ogy-5);
  TxtRect.Right=TxtRect.Left+(ogx-5);
  Flags=DT_EXPANDTABS | DT_VCENTER | DT_CENTER;
  Flags=DrawTextBiDiModeFlags(Flags);
  DrawText(BigReel->Picture->Bitmap->Canvas->Handle, Symbol->Text.c_str(), -1, &TxtRect, Flags);
}
//---------------------------------------------------------------------------


void __fastcall TEditor::BigReelMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ( Button == mbLeft && !Shift.Contains(ssShift) && HasImage )
      BigReel->BeginDrag(false,0);
}
//---------------------------------------------------------------------------

void __fastcall TEditor::AddTextClick(TObject *Sender)
{
  int ogx, ogy;
  unsigned int Flags;
  TRect TxtRect;

  // Add Text
  // Panel: TextFontPrev
  // Caption: EditText
  BigReel->Picture->Bitmap->Canvas->Pen->Mode=pmCopy;
  BigReel->Picture->Bitmap->Canvas->Pen->Color=TextFont->Brush->Color;
  BigReel->Picture->Bitmap->Canvas->Brush->Color=TextFont->Brush->Color;
  BigReel->Picture->Bitmap->Canvas->Brush->Style = bsSolid;
  BigReel->Picture->Bitmap->Canvas->Font=TextFont->Font;
  ogy=BigReel->Picture->Bitmap->Canvas->TextHeight("W")+10;
  ogx=BigReel->Picture->Bitmap->Canvas->TextWidth(Word->Text)+10;
  BigReel->Picture->Bitmap->Canvas->RoundRect(((BigReel->Picture->Bitmap->Width / 2)-(ogx / 2)),
      BigReel->Picture->Bitmap->Height-ogy-5,
      ((BigReel->Picture->Bitmap->Width / 2)+(ogx / 2)),
      BigReel->Picture->Bitmap->Height-5, ogx / 2, ogy / 2);
  TxtRect.Top=BigReel->Picture->Bitmap->Height-ogy;
  TxtRect.Left=((BigReel->Picture->Bitmap->Width / 2)-(ogx / 2)+2);
  TxtRect.Bottom=TxtRect.Top+(ogy-5);
  TxtRect.Right=TxtRect.Left+(ogx-5);
  Flags=DT_EXPANDTABS | DT_VCENTER | DT_CENTER;
  Flags=DrawTextBiDiModeFlags(Flags);
  DrawText(BigReel->Picture->Bitmap->Canvas->Handle, Word->Text.c_str(), -1, &TxtRect, Flags);
}
//---------------------------------------------------------------------------

void __fastcall TEditor::SymbolFontClick(TObject *Sender)
{
  FontDialog1->Font = SymFont->Font;
  if ( FontDialog1->Execute() ) {
    SymFont->Font = FontDialog1->Font;
  }
}
//---------------------------------------------------------------------------
void __fastcall TEditor::ClearImage(TImage *Sender, TColor colour)
{
  Sender->Picture->Bitmap->Assign(NULL);
  Sender->Picture->Bitmap->Width = Sender->Width;
  Sender->Picture->Bitmap->Height = Sender->Height;
  Sender->Canvas->Brush->Color = colour;
  Sender->Canvas->FillRect(((TImage *)Sender)->Canvas->ClipRect);
  Sender->Repaint();
  Sender->Picture->Bitmap->Assign(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TEditor::DeleteRow(TObject *Sender)
{
int i, row;
TRect rect;

  row = ((TControl *)Sender)->Tag;
  for ( i = 0; i < NBR_REELS; i++ ) {
    rect = Rect(0, row * SHEIGHT, SWIDTH, SHEIGHT + row * SHEIGHT);
    Reels[i]->Picture->Bitmap->Canvas->FillRect(rect);
  }
  Apply->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditor::DeleteColumn(TObject *Sender)
{
int row;

  row = ((TControl *)Sender)->Tag;
  Reels[row]->Picture->Bitmap->Canvas->FillRect(Rect(0,0,Reels[row]->Picture->Bitmap->Width,Reels[row]->Picture->Bitmap->Height));
  Apply->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditor::DoneClick(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TEditor::FormCloseQuery(TObject *Sender, bool &CanClose)
{

  if ( Apply->Enabled )
    if ( Application->MessageBox("Symbols changed, Are you sure ?", "Confirm", MB_OKCANCEL | MB_ICONSTOP | MB_DEFBUTTON2) == ID_CANCEL )
      CanClose = false;
}
//---------------------------------------------------------------------------

void __fastcall TEditor::AddBoxClick(TObject *Sender)
{
int thickness, offset;

  thickness = Thickness->Text.ToIntDef(10);
  offset = Offset->Text.ToIntDef(6);
  BigReel->Picture->Bitmap->Canvas->Pen->Color = clBlack;
  BigReel->Picture->Bitmap->Canvas->Pen->Width = thickness;
  BigReel->Picture->Bitmap->Canvas->MoveTo( offset, offset);
  BigReel->Picture->Bitmap->Canvas->LineTo( BigReel->Picture->Bitmap->Width - offset, offset);
  BigReel->Picture->Bitmap->Canvas->LineTo( BigReel->Picture->Bitmap->Width - offset, BigReel->Picture->Bitmap->Height -offset );
  BigReel->Picture->Bitmap->Canvas->LineTo( offset, BigReel->Picture->Bitmap->Height - offset);
  BigReel->Picture->Bitmap->Canvas->LineTo( offset, offset);
  HasImage = true;

}
//---------------------------------------------------------------------------


void __fastcall TEditor::LoadOverlayClick(TObject *Sender)
{
  OpenPictureDialog1->FileName = "";
  if ( OpenPictureDialog1->Execute() ) {
    Overlay->Picture->Bitmap->LoadFromFile(OpenPictureDialog1->FileName);
    Overlay->Transparent = true;
    OverlaySize->Caption = IntToStr(Overlay->Picture->Bitmap->Width) + " x " +
                           IntToStr(Overlay->Picture->Bitmap->Height);
  }
}
//---------------------------------------------------------------------------

void __fastcall TEditor::AddOverlayClick(TObject *Sender)
{
  BigReel->Picture->Bitmap->Canvas->Draw(0,0, Overlay->Picture->Bitmap);
}
//---------------------------------------------------------------------------

void __fastcall TEditor::LoadBand1Click(TObject *Sender)
{
int reel_nbr, stops, loop, width, height;
Graphics::TBitmap *ptr = new Graphics::TBitmap();
Graphics::TBitmap *sym = new Graphics::TBitmap();
TRect symrect;
static int Stops[] = { 8, 12, 16, 20, 24, 25 };

  OpenPictureDialog1->Filter = "Reel Band (*.bmp)|*.bmp";
  OpenPictureDialog1->DefaultExt = AnsiString("bmp");
  OpenPictureDialog1->Options << ofFileMustExist;

  OpenPictureDialog1->FileName = "";
  if ( OpenPictureDialog1->Execute() ) {
    if ( LoadBandForm->ShowModal() == mrOk ) {
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      reel_nbr = LoadBandForm->Reel->ItemIndex;
      stops = Stops[LoadBandForm->Stops->ItemIndex];
      height = ptr->Height / stops;
      width = ptr->Width;
      sym->Width = width;
      sym->Height = height;
      Reels[reel_nbr]->Tag = stops;
      Reels[reel_nbr]->Height = stops * 21;
      Reels[reel_nbr]->Picture->Bitmap->Height = ptr->Height;
      Reels[reel_nbr]->Picture->Bitmap->Width = ptr->Width;
      symrect = Rect( 0, 0, width, height);
      for ( loop = 0; loop < stops; loop++ ) {
        sym->Canvas->CopyRect( symrect, ptr->Canvas,
                               Rect(0, height * loop, width, height + height *loop));
        SetSymbol(loop, Reels[reel_nbr], sym);
      }
      delete sym;
      delete ptr;
      Apply->Enabled = true;
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TEditor::LoadPreviewClick(TObject *Sender)
{
      OpenPictureDialog1->FileName = "";
      if ( OpenPictureDialog1->Execute() ) {
        BigReel->Picture->Bitmap->LoadFromFile(OpenPictureDialog1->FileName);
        HasImage = true;
        PreviewSize->Caption = IntToStr(BigReel->Picture->Bitmap->Width) + " x " +
                               IntToStr(BigReel->Picture->Bitmap->Height);
      }
}
//---------------------------------------------------------------------------

void __fastcall TEditor::FormClose(TObject *Sender, TCloseAction &Action)
{
int X;

  for ( X = 0; X < NBR_REELS; X++ )
    delete Reels[X];
  Release();
  Editor = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TEditor::FontBGClick(TObject *Sender)
{
   // Select new Font BG Color
   // First pull the existing color
   ColorDialog1->Color=((TPanel *)(Sender))->Color;

   // And request another
   if (ColorDialog1->Execute())
      ((TPanel *)(Sender))->Color=ColorDialog1->Color; // And apply if OK
}
//---------------------------------------------------------------------------

void __fastcall TEditor::SaveBandsClick(TObject *Sender)
{
int X, stops, height, width, reel;
String Path;
Graphics::TBitmap *band = new Graphics::TBitmap();
Graphics::TBitmap *ptr;
static int Stops[] = { 8, 12, 16, 20, 24, 25 };

  SaveDialog1->Filter = "BMP files (*.bmp)|*.bmp";
  SaveDialog1->DefaultExt = "bmp";
  if ( CreateBandForm->ShowModal() == mrOk ) {
    band->PixelFormat = pf24bit;
    reel = CreateBandForm->Reel->ItemIndex;
    stops = Stops[CreateBandForm->Stops->ItemIndex];
    height = Reels[reel]->Picture->Bitmap->Height / stops;
    width = Reels[reel]->Picture->Bitmap->Width;
    band->Height = stops * height;
    band->Width = width;
    for ( X = 0; X < stops; X++ ) {
      ptr = GetSymbol(X,Reels[reel]);
      band->Canvas->Draw(0, X*height, ptr);
      delete ptr;
    }
    if ( SaveDialog1->Execute() ) {
      Path = ExtractFilePath(SaveDialog1->FileName);
      band->SaveToFile(SaveDialog1->FileName);
      delete band;
    }
  }
}
//---------------------------------------------------------------------------

Graphics::TBitmap * __fastcall TEditor::GetSymbol(int symbol, TImage *source)
{
Graphics::TBitmap *ptr = new Graphics::TBitmap();
TRect rect;
int width, height;

  height = source->Picture->Bitmap->Height / ( source->Height/21);
  width  =  source->Picture->Bitmap->Width;
  ptr->Height = height;
  ptr->Width = width;
  rect = Rect(0, symbol * height, width, height + symbol * height);
  ptr->Canvas->CopyRect(Rect(0,0,width,height), source->Picture->Bitmap->Canvas, rect);

  return ptr;
}
//---------------------------------------------------------------------------
void __fastcall TEditor::SetSymbol(int symbol, TImage *dest, Graphics::TBitmap *src)
{
TRect rect;
int width = src->Width;
int height = src->Height;

  if ( dest->Picture->Bitmap->Width != width ) {
    dest->Picture->Bitmap->Width = width;
  }
  if ( dest->Picture->Bitmap->Height != dest->Tag * height ) {
    dest->Picture->Bitmap->Height = dest->Tag * height;
  }

  rect = Rect( 0, height * symbol, width, height + height * symbol);
  dest->Picture->Bitmap->Canvas->Draw(0,height * symbol,src);

//  rect = Rect( 0, SHEIGHT * symbol, SWIDTH, SHEIGHT + SHEIGHT * symbol);
//  dest->Picture->Bitmap->Canvas->StretchDraw(rect,src);
}

