unit GR32_Reg;

{*********************************************}
{  This unit is a part of Graphics32 library  }
{  Copyright © 2000-2002 Alex Denisov         }
{  See License.txt for licence information    }
{*********************************************}
// $Id: GR32_Reg.pas,v 1.1 2002/01/15 20:13:22 alex Exp $

interface

{$I GR32.INC}

uses
  Classes, TypInfo,
  {$IFDEF DELPHI6}DesignIntf{$ELSE}DsgnIntf{$ENDIF};

procedure Register;

implementation

uses
  GR32,
  GR32_Dsgn_Color,
  GR32_Dsgn_Bitmap,
  GR32_Image,
  GR32_Layers,
  GR32_RangeBars;

{ Registration }
procedure Register;
begin
  RegisterComponents('Graphics32', [TPaintBox32, TImage32, TBitmap32List,
    TRangeBar, TGaugeBar, TImgView32]);
  RegisterPropertyEditor(TypeInfo(TColor32), nil, '', TColor32Property);
  RegisterPropertyEditor(TypeInfo(TBitmap32), nil, '', TBitmap32Property);
  RegisterComponentEditor(TCustomImage32, TImage32Editor);
end;

end.
