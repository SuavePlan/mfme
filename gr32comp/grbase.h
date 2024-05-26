//---------------------------------------------------------------------------

#ifndef grbaseH
#define grbaseH

#include "GR32_Image.hpp"
//#include "GR32_Transforms.hpp"
//---------------------------------------------------------------------------

class TGRBase;

class TCompEntry
{
  public:
    TCompEntry *next;
    TCompEntry *previous;
    TGRBase * ptr;
};

class TCompList
{
  public:
    int Count;
    TCompEntry head;
  public:
    TCompList::TCompList();
    void Add(TGRBase *ptr);
    void Remove(TGRBase *ptr);
    TGRBase * FindXY(int X, int Y);
    TGRBase * TCompList::FindShortcut(int key);
    TGRBase * operator[](int i);
    void __fastcall Redraw();
    void __fastcall SaveToStream(TStream *stream);
};

class TGRBase //: public TCompEntry
{
  friend TCompList;
  private:
    TBitmap32 * Src;
    TBitmap32 * Dst;
    TBitmap32 * dest;
    bool Rotatable;
    TRect DstRrect;
  public:
    static TCompList List;
    TImage32 * image;
    TRect Dstrect;
    String Name;
    int X;
    int Y;
    bool Flush;
    bool Moveable;
    bool Sizeable;
    int Height;
    int Width;
    TBitmap32 * Bitmap;
    Single Angle;
    int Number;
    int Shortcut;
    bool HasShortcut;
    int Button;
    bool HasButton;
    bool HasCoin;
    int Coin;
    bool Visible;
    int State;
  public:
    __fastcall TGRBase(int ANumber, String AName, int aX, int aY, int AHeight, int AWidth, TImage32 * Adest);
    __fastcall ~TGRBase();
    void __fastcall SetAngle(int Value);
    void __fastcall SetBounds(int AX, int AY, int AWidth, int AHeight);
//    void __fastcall SetSize(int AHeight, int AWidth);
//    void __fastcall SetPosition(int aX, int aY);
    void __fastcall DoAlpha(TBitmap32 *ptr, TColor32 Colour, bool Reset = true);
    void __fastcall DoAlpha(TBitmap32 *ptr);
    void __fastcall SendToBack(void);
    void __fastcall BringToFront(void);
    void _fastcall WriteToStream(TStream *ptr);
    virtual __fastcall void SetPos(int pos);
    virtual __fastcall void SetDown(bool value);
    virtual __fastcall void SetState(int state);
    virtual __fastcall void SetLamp(int nbr, int state);
    virtual __fastcall void SetLamps(int state);
    virtual _fastcall void SaveToStream(TStream *ptr);
    virtual _fastcall void LoadFromStream(TStream *ptr);
    virtual __fastcall void Draw();
};
#endif
 