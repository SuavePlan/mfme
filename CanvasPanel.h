//---------------------------------------------------------------------------
#ifndef CanvasPanelH
#define CanvasPanelH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TCanvasPanel : public TPanel
{
private:
  Graphics::TBitmap * __fastcall GetBackground(void);
  Graphics::TBitmap *FBackground;
  bool FFlicker;
  TNotifyEvent FOnPaint;
protected:
  virtual void __fastcall TCanvasPanel::Paint(void);
public:
  void __fastcall LoadBackground(Graphics::TBitmap *ptr);
    __fastcall TCanvasPanel(TComponent* Owner);
    __fastcall ~TCanvasPanel();
   void __fastcall SetFlicker(bool AFlicker);
//   int Rectl, Rectr, Rectt, Rectb;
__published:
   __property Canvas;
   __property Graphics::TBitmap * Background = {read=GetBackground, write=LoadBackground, nodefault};
   __property bool Flicker = {read=FFlicker, write=SetFlicker, default = false};
   __property TNotifyEvent OnPaint = { read=FOnPaint, write=FOnPaint };
};
//---------------------------------------------------------------------------
#endif
