unit GR32_Filters;

{*********************************************}
{  This unit is a part of Graphics32 library  }
{  Copyright � 2000-2002 Alex Denisov         }
{  See License.txt for licence information    }
{*********************************************}
// $Id: GR32_Filters.pas,v 1.2 2002/02/08 06:00:07 alex Exp $

interface

{$I GR32.INC}

uses
  Windows, Classes, SysUtils, GR32, GR32_Blend, GR32_ByteMaps;

{ Basic processing }
type
  TLUT8 = array [Byte] of Byte;

procedure AlphaToGrayscale(Dst, Src: TBitmap32);
procedure IntensityToAlpha(Dst, Src: TBitmap32);
procedure Invert(Dst, Src: TBitmap32);
procedure InvertRGB(Dst, Src: TBitmap32);
procedure ColorToGrayscale(Dst, Src: TBitmap32);
procedure ApplyLUT(Dst, Src: TBitmap32; const LUT: TLUT8);

implementation

const
  SEmptySource = 'The source is nil';
  SEmptyDestination = 'Destination is nil';
  SNoInPlace = 'In-place operation is unsupported';

procedure CheckParams(Dst, Src: TBitmap32);
begin
  if Src = nil then raise Exception.Create(SEmptySource);
  if Dst = nil then raise Exception.Create(SEmptyDestination);
  Dst.SetSize(Src.Width, Src.Height);
end;

{rocedure CheckParamsNoInPlace(Dst, Src: TBitmap32);
begin
  if (Src = nil) then
    raise Exception.Create(SEmptySource);
  if Dst = nil then
    raise Exception.Create(SEmptyDestination);
  if Dst = Src then
    raise Exception.Create(SNoInPlace);
  Dst.SetSize(Src);
end;       }

procedure AlphaToGrayscale(Dst, Src: TBitmap32);
var
  I: Integer;
  D, S: PColor32;
begin
  CheckParams(Dst, Src);
  Dst.SetSize(Src.Width, Src.Height);
  D := @Dst.Bits[0];
  S := @Src.Bits[0];
  for I := 0 to Src.Width * Src.Height - 1 do
  begin
    D^ := Gray32(AlphaComponent(S^));
    Inc(S); Inc(D);
  end;
  Dst.Changed;
end;

procedure IntensityToAlpha(Dst, Src: TBitmap32);
var
  I: Integer;
  D, S: PColor32;
begin
  CheckParams(Dst, Src);
  Dst.SetSize(Src.Width, Src.Height);
  D := @Dst.Bits[0];
  S := @Src.Bits[0];
  for I := 0 to Src.Width * Src.Height - 1 do
  begin
    D^ := SetAlpha(D^, Intensity(S^));
    Inc(S); Inc(D);
  end;
  Dst.Changed;
end;

procedure Invert(Dst, Src: TBitmap32);
var
  I: Integer;
  D, S: PColor32;
begin
  CheckParams(Dst, Src);
  Dst.SetSize(Src.Width, Src.Height);
  D := @Dst.Bits[0];
  S := @Src.Bits[0];
  for I := 0 to Src.Width * Src.Height - 1 do
  begin
    D^ := S^ xor $FFFFFFFF;
    Inc(S); Inc(D);
  end;
  Dst.Changed;
end;

procedure InvertRGB(Dst, Src: TBitmap32);
var
  I: Integer;
  D, S: PColor32;
begin
  CheckParams(Dst, Src);
  Dst.SetSize(Src.Width, Src.Height);
  D := @Dst.Bits[0];
  S := @Src.Bits[0];
  for I := 0 to Src.Width * Src.Height - 1 do
  begin
    D^ := S^ xor $00FFFFFF;
    Inc(S); Inc(D);
  end;
  Dst.Changed;
end;

procedure ColorToGrayscale(Dst, Src: TBitmap32);
var
  I: Integer;
  D, S: PColor32;
begin
  CheckParams(Dst, Src);
  Dst.SetSize(Src.Width, Src.Height);
  D := @Dst.Bits[0];
  S := @Src.Bits[0];
  for I := 0 to Src.Width * Src.Height - 1 do
  begin
    D^ := Gray32(Intensity(S^));
    Inc(S); Inc(D);
  end;
  Dst.Changed;
end;

procedure ApplyLUT(Dst, Src: TBitmap32; const LUT: TLUT8);
var
  I: Integer;
  D, S: PColor32;
  r, g, b: TColor32;
  C: TColor32;
begin
  CheckParams(Src, Dst);
  Dst.SetSize(Src.Width, Src.Height);
  D := @Dst.Bits[0];
  S := @Src.Bits[0];

  for I := 0 to Src.Width * Src.Height - 1 do
  begin
    C := S^;
    r := C and $00FF0000;
    g := C and $0000FF00;
    r := r shr 16;
    b := C and $000000FF;
    g := g shr 8;
    r := LUT[r];
    g := LUT[g];
    b := LUT[b];
    D^ := $FF000000 or r shl 16 or g shl 8 or b;
    Inc(S);
    Inc(D);
  end;
  Dst.Changed;
end;


end.
