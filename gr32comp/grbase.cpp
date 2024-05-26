//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "math.h"
#include "grbase.h"
#include "comps.h"
#include "GR32_Transforms.hpp"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TCompList TGRBase::List;

//---------------------------------------------------------------------------

__fastcall TGRBase::TGRBase(int ANumber, String AName, int AX, int AY, int AHeight, int AWidth, TImage32 *Adest)
{
  X = AX;
  Y = AY;
  Height = AHeight;
  Width = AWidth;
  image = Adest;
  dest = Adest->Bitmap;
  Number = ANumber;
  Name = AName;
  Angle = 0;
  Button = 0;
  Shortcut = 0;
//  Bitmap = Adest->Bitmap;
  Bitmap = new TBitmap32();
  Bitmap->SetSize(Width,Height);
  Bitmap->DrawMode = dmBlend;  // dmBlend Before
  Src = new TBitmap32();
  Dst = new TBitmap32();
  int hyp = sqrt( ((Width/2) * (Width/2)) + ((Height/2) * (Height/2)) );
  Src->SetSize(hyp*2,hyp*2);
  Dst->SetSize(hyp*2,hyp*2);
  Dst->DrawMode = dmBlend;
  Dstrect = Rect(X,Y,X+Width,Y+Height);
  DstRrect = Rect(X - ((Src->Width - Width) / 2),
               Y - ((Src->Height - Height) / 2),
              (X - ((Src->Width - Width) / 2)) + Src->Width,
              (Y - ((Src->Height - Height) / 2)) + Src->Height);
  List.Add(this);
  Flush = true;
  Moveable = true;
  HasButton = false;
  HasShortcut = false;
  HasCoin = false;
  State = 0;
}
//---------------------------------------------------------------------------

void __fastcall TGRBase::SetBounds(int AX, int AY, int AWidth, int AHeight)
{
TCompEntry * ptr;
TRect IRect,JRect;

  if ( AX < 0 )
    AX = 0;
  if ( AY < 0 )
    AY = 0;
  if ( AX + AWidth > image->Bitmap->Width )
    AX = image->Bitmap->Width - AWidth;
  if ( AY + AHeight > image->Bitmap->Height )
    AY = image->Bitmap->Height - AHeight;
  if ( X != AX || Y != AY || AWidth != Width || AHeight != Height ) {
    bool doall = false;
    ptr = List.head.next;
//    if ( image->Width != image->Bitmap->Width ||
//         image->Height != image->Bitmap->Height )
//      doall = true;
    image->BeginUpdate();
    while ( ptr->ptr ) {
      if ( ptr->ptr != this ) {
        if ( IntersectRect( IRect, Dstrect, ptr->ptr->Dstrect ) ) {
          JRect = IRect;
          OffsetRect(JRect,-ptr->ptr->X,-ptr->ptr->Y);
          ptr->ptr->Bitmap->DrawTo(dest, IRect, JRect);
        }
      }
      ptr = ptr->next;
    }
    image->EndUpdate();
    if ( Flush ) {
      image->Flush(Dstrect);
    }
    X = AX;
    Y = AY;
    Height = AHeight;
    Width = AWidth;
    Bitmap->SetSize(Width,Height);
    int hyp = sqrt( ((Width/2) * (Width/2)) + ((Height/2) * (Height/2)) );
    Src->SetSize(hyp*2,hyp*2);
    Dst->SetSize(hyp*2,hyp*2);
    Dstrect = Rect(X,Y,X+Width,Y+Height);
    DstRrect = Rect(X - ((Src->Width - Width) / 2),
               Y - ((Src->Height - Height) / 2),
              (X - ((Src->Width - Width) / 2)) + Src->Width,
              (Y - ((Src->Height - Height) / 2)) + Src->Height);
    ptr = List.head.next;
    image->BeginUpdate();
    while ( ptr->ptr ) {
      if ( IntersectRect( IRect, Dstrect, ptr->ptr->Dstrect ) ) {
        JRect = IRect;
        OffsetRect(JRect,-ptr->ptr->X,-ptr->ptr->Y);
        ptr->ptr->Bitmap->DrawTo(dest, IRect, JRect);
      }
      ptr = ptr->next;
    }
    image->EndUpdate();
    if ( Flush ) {
      image->Flush(Dstrect);
    }
  }
}
//---------------------------------------------------------------------------

/*void __fastcall TGRBase::SetSize(int AWidth, int AHeight)
{
  Height = AHeight;
  Width = AWidth;
  Bitmap->SetSize(Width,Height);
  int hyp = sqrt( ((Width/2) * (Width/2)) + ((Height/2) * (Height/2)) );
  Src->SetSize(hyp*2,hyp*2);
  Dst->SetSize(hyp*2,hyp*2);
  Dstrect = Rect(X,Y,X+Width,Y+Height);
  DstRrect = Rect(X - ((Src->Width - Width) / 2),
               Y - ((Src->Height - Height) / 2),
              (X - ((Src->Width - Width) / 2)) + Src->Width,
              (Y - ((Src->Height - Height) / 2)) + Src->Height);
}
//---------------------------------------------------------------------------

void __fastcall TGRBase::SetPosition(int AX, int AY)
{
  if ( AX < 0 )
    AX = 0;
  if ( AY < 0 )
    AY = 0;
  X = AX;
  Y = AY;
  Dstrect = Rect(X,Y,X+Width,Y+Height);
  DstRrect = Rect(X - ((Src->Width - Width) / 2),
               Y - ((Src->Height - Height) / 2),
              (X - ((Src->Width - Width) / 2)) + Src->Width,
              (Y - ((Src->Height - Height) / 2)) + Src->Height);
}   */

//---------------------------------------------------------------------------

__fastcall TGRBase::~TGRBase()
{
  List.Remove(this);
  delete Bitmap;
  delete Src;
  delete Dst;
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::Draw()
{
TCompEntry * ptr = List.head.next;
TRect IRect,JRect,DRect;

  if ( Angle == 0 ) {
    DRect = Dstrect;
    image->BeginUpdate();
    while ( ptr->ptr ) {
      if ( IntersectRect( IRect, Dstrect, ptr->ptr->Dstrect ) ) {
        JRect = IRect;
        OffsetRect(JRect,-ptr->ptr->X,-ptr->ptr->Y);
        ptr->ptr->Bitmap->DrawTo(dest, IRect, JRect);
/*        if ( ptr->ptr->Moveable ) {
        if ( ptr->ptr->Dstrect.Left < DRect.Left )
          DRect.Left = ptr->ptr->Dstrect.Left;
        if ( ptr->ptr->Dstrect.Right > DRect.Right )
          DRect.Right = ptr->ptr->Dstrect.Right;
        if ( ptr->ptr->Dstrect.Top < DRect.Top )
          DRect.Top = ptr->ptr->Dstrect.Top;
        if ( ptr->ptr->Dstrect.Bottom > DRect.Bottom )
          DRect.Bottom = ptr->ptr->Dstrect.Bottom;
        }     */
      }
      ptr = ptr->next;
    }
//    Bitmap->DrawTo(dest,X,Y);
    image->EndUpdate();
    if ( Flush )
      image->Flush(DRect/*Dstrect*/);
  } else {
    int SrcR;
    int SrcB;
    TAffineTransformation *T;
    Src->Clear(0x0);
    Bitmap->DrawTo(Src, (Src->Width - Width) / 2 , (Src->Height - Height) / 2);

    SrcR = Src->Width - 1;
    SrcB = Src->Height - 1;
    T = new TAffineTransformation;
    T->SrcRect = FloatRect(0, 0, SrcR + 1, SrcB + 1);

    // shift the origin
    T->Clear();

    // move the origin to a center for scaling and rotation
    T->Translate(-SrcR / 2, -SrcB / 2);
    T->Rotate(0, 0, Angle);

    // move the origin back
    T->Translate(SrcR / 2, SrcB / 2);

    // transform the bitmap
    Transform(Dst, Src, T);
    image->BeginUpdate();
    Dst->DrawTo(dest,X - ((Src->Width - Width) / 2) ,Y - ((Src->Height - Height) / 2));
    image->EndUpdate();
    if ( Flush )
      image->Flush(DstRrect);

    delete T;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SetAngle(int value)
{
  Angle = value;
  Draw();
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SendToBack(void)
{
TCompEntry * ptr = List.head.next;

  while ( ptr->ptr ) {
    if ( ptr->ptr == this ) {
      ptr->previous->next = ptr->next;
      ptr->next->previous = ptr->previous;
      ptr->next = List.head.next;
      ptr->previous = &List.head;
      List.head.next->previous = ptr;
      List.head.next = ptr;
      Draw();
      return;
    }
    ptr = ptr->next;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::BringToFront(void)
{
TCompEntry * ptr = List.head.next;

  while ( ptr->ptr ) {
    if ( ptr->ptr == this ) {
      ptr->previous->next = ptr->next;
      ptr->next->previous = ptr->previous;
      ptr->previous = List.head.previous;
      ptr->next = &List.head;
      List.head.previous->next = ptr;
      List.head.previous = ptr;
      Draw();
      return;
    }
    ptr = ptr->next;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::WriteToStream(TStream *ptr)
{
int length;

  length = Name.Length();
  ptr->WriteBuffer(&length,4);
  ptr->WriteBuffer(Name.c_str(),length);
  ptr->WriteBuffer(&X, 4);
  ptr->WriteBuffer(&Y, 4);
  ptr->WriteBuffer(&Height,4);
  ptr->WriteBuffer(&Width,4);
  ptr->WriteBuffer(&Number,4);
  ptr->WriteBuffer(&Angle,sizeof(Angle));
  ptr->WriteBuffer(&Moveable,sizeof(Moveable));
  ptr->WriteBuffer(&Sizeable,sizeof(Sizeable));
  ptr->WriteBuffer(&Rotatable,sizeof(Rotatable));
  ptr->WriteBuffer(&HasShortcut,sizeof(HasShortcut));
  ptr->WriteBuffer(&Shortcut,sizeof(Shortcut));
  ptr->WriteBuffer(&HasButton,sizeof(HasButton));
  ptr->WriteBuffer(&Button,sizeof(Button));
  ptr->WriteBuffer(&HasCoin,sizeof(HasCoin));
  ptr->WriteBuffer(&Coin,sizeof(Coin));
  SaveToStream(ptr);
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SaveToStream(TStream *ptr)
{
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::LoadFromStream(TStream *ptr)
{
}
//---------------------------------------------------------------------------

TCompList::TCompList()
{
  head.next = &head;
  head.previous = &head;
  head.ptr = NULL;
}
//---------------------------------------------------------------------------

void TCompList::Add(TGRBase *ptr)
{
TCompEntry * newptr = new TCompEntry();

  newptr->ptr = ptr;
  newptr->next = &head;
  newptr->previous = head.previous;
  head.previous->next = newptr;
  head.previous = newptr;
  Count++;
}
//---------------------------------------------------------------------------

void TCompList::Remove(TGRBase *bptr)
{
TCompEntry * ptr = head.next;

  while ( ptr->ptr ) {
    if ( ptr->ptr == bptr ) {
      ptr->previous->next = ptr->next;
      ptr->next->previous = ptr->previous;
//      delete ptr->ptr;
      delete ptr;
      Count--;
      return;
    }
    ptr = ptr->next;
  }
}
//---------------------------------------------------------------------------

TGRBase * TCompList::FindXY(int X, int Y)
{
TCompEntry * ptr = head.previous;

  while ( ptr->ptr ) {
    if ( X >= ptr->ptr->X && X <= ptr->ptr->X + ptr->ptr->Width &&
         Y >= ptr->ptr->Y && Y <= ptr->ptr->Y + ptr->ptr->Height &&
         ptr->ptr->Moveable )
      return ptr->ptr;
    ptr = ptr->previous;
  }
  return NULL;
}
//---------------------------------------------------------------------------

TGRBase * TCompList::FindShortcut(int key)
{
TCompEntry * ptr = head.next;

  while ( ptr->ptr ) {
    if ( ptr->ptr->Shortcut == key &&
         ptr->ptr->HasShortcut &&
         ptr->ptr->HasButton )
      return ptr->ptr;
    ptr = ptr->next;
  }
  return NULL;
}
//---------------------------------------------------------------------------

TGRBase * TCompList:: operator[](int i )
{
TCompEntry * ptr = head.next;
int count = 0;

  while ( ptr->ptr ) {
    if ( count == i )
      return ptr->ptr;
    ptr = ptr->next;
    count++;
  }
  return NULL;
}
//---------------------------------------------------------------------------

void __fastcall TCompList::Redraw()
{
TCompEntry * ptr = head.next;

  while ( ptr->ptr ) {
    ptr->ptr->Flush = false;
    ptr->ptr->TGRBase::Draw();
    ptr->ptr->Flush = true;
    ptr = ptr->next;
  }
}
//---------------------------------------------------------------------------
void __fastcall TCompList::SaveToStream(TStream *stream)
{
TCompEntry * ptr = head.next;

  while ( ptr->ptr ) {
    ptr->ptr->WriteToStream(stream);
    ptr = ptr->next;
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::DoAlpha(TBitmap32 *ptr, TColor32 Colour, bool Reset)
{
PColor32 P;
TColor32 C;
int I, J;

  if ( Reset )
    ptr->ResetAlpha();

  Colour = Colour & 0x00ffffff;

  for ( I = 0; I < ptr->Height; I++ ) {
    for ( J = 0; J < ptr->Width; J++ ) {
      P = ptr->PixelPtr[J][I];
      C = *P & 0x00FFFFFF;
      if ( C == Colour ) {
        *P = C;
      }
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::DoAlpha(TBitmap32 *ptr)
{
  DoAlpha(ptr,*ptr->PixelPtr[0][0], true);
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SetPos(int pos)
{
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SetState(int state)
{
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SetLamp(int nbr, int state)
{
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SetLamps(int state)
{
}
//---------------------------------------------------------------------------
void __fastcall TGRBase::SetDown(bool value)
{
}

