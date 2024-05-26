//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include "property.h"
#include "lamps.h"
#include "interface.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"

TPropertiesForm *PropertiesForm;

int coinmasks[] = { 0x88, 0x9c, 0x8b, 0x8e, 0x8d, 0x9a, 0x9f, 0x9a, 0xf8, 0x1, 0x2, 0x4, 0x8, 0x10, 0x20, 0x40, 0x80};

int DiscLamp[24];

//---------------------------------------------------------------------------
__fastcall TPropertiesForm::TPropertiesForm(TComponent* Owner)
    : TForm(Owner)
{
  for ( int i = 0; i < PageControl->PageCount; i++ )
    PageControl->Pages[i]->TabVisible = false;
  last_tab = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OKClick(TObject *Sender)
{
int X, Y;
String type;
int shortcut, old_shortcut;
TzLed *segs[8] = {
  PropertiesForm->SegA,
  PropertiesForm->SegB,
  PropertiesForm->SegC,
  PropertiesForm->SegD,
  PropertiesForm->SegE,
  PropertiesForm->SegF,
  PropertiesForm->SegG,
  PropertiesForm->SegH
};
TzLed *old_ptr;
int Lamp, old_lamp, Button, old_button, Coin;

  Form1->LayoutChanged();
  type = String(sel_ptr->ClassName());
  if ( type == "TLamp2" ) {
    TLamp2 *ptr = (TLamp2 *)sel_ptr;
    old_shortcut = _ShortcutFromTag(ptr->Tag);
    Lamp = Glamp1->Text.ToIntDef(NBR_LAMPS);
    if ( Lamp < NBR_LAMPS ) {
      old_lamp = _LampFromTag(ptr->Tag);
      Form1->RemoveLamp(old_lamp, ptr);
      ptr->Tag = _LampTag(Lamp) | _LampMask;
      Form1->InsertLamp( Lamp, ptr, GColour1->Brush->Color, 1);
      ptr->Colour1 = GColour1->Brush->Color;
      sel_ptr->Hint = IntToStr(Lamp);
      ptr->Caption = Caption->Text;
      ptr->Font = FontDialog1->Font;
    }
    if ( Glamp2->Text != "" ) {
      Lamp = Glamp2->Text.ToIntDef(NBR_LAMPS);
      if ( Lamp < NBR_LAMPS ) {
        ptr->Tag += _LampTag2(Lamp);
        ptr->Tag |= _LampMask2;
//        ptr->Tag &= ~_ButtonMask;
        Form1->InsertLamp(Lamp, ptr, GColour2->Brush->Color, 2);
        ptr->Colour2 = GColour2->Brush->Color;
      }
    }
    Coin = ComboBox1->ItemIndex;
    if ( LButton->Text != "" ) {
      Button = LButton->Text.ToIntDef(NBR_BUTTONS);
      if ( Button < NBR_BUTTONS ) {
        ptr->Button = Button;
        ptr->Tag |= _ButtonMask;
        ptr->Tag &= ~_CoinMask;
        ptr->Cursor = crHandPoint;
//        ptr->Tag &= ~_LampMask2;
      }
    } else if ( Coin > 0 ) {
      ptr->Button = Coin - 1;
      ptr->Tag |= _CoinMask;
      ptr->Tag &= ~_ButtonMask;
      ptr->Hint = "Coin " + IntToStr(coinmasks[Coin-1]);
      ptr->Cursor = crHandPoint;
    } else {
      ptr->Tag &= ~(_CoinMask + _ButtonMask);
      ptr->Cursor = crDefault;
    }
    if ( NoOutLine->Checked )
      ptr->Tag |= _NoOutLineMask;
    else
      ptr->Tag &= ~_NoOutLineMask;
    ptr->Trans = Transparent->Checked;
    ptr->StartingAngle = Angle->Text.ToIntDef(90);
    ptr->BackColor = GColourOff->Brush->Color;
    ptr->Colour1 = GColour1->Brush->Color;
    ptr->Colour2 = GColour2->Brush->Color;
    Form1->shortcuts[old_shortcut] = NULL;
    shortcut = StrToIntDef(Edit4->Text,0);
    if ( shortcut && (ptr->Tag & (_CoinMask + _ButtonMask))) {
      Form1->shortcuts[shortcut] = (TComponent *)ptr;
      ptr->Tag |= (shortcut << 20);
    }
  } else if ( type == "TLabel" ) {
    TLabel *ptr = (TLabel *)sel_ptr;
//    Caption->Font->Color = TextColour->Brush->Color;
    ptr->Caption = Caption->Text;
    ptr->Font = FontDialog1->Font;
    ptr->Transparent = TransLabel->Checked;
    ptr->Color = BackColour->Brush->Color;
//    ptr->Font->Color = TextColour->Brush->Color;
    if ( _HasLamp(ptr->Tag) ) {
      Lamp = _LampFromTag(ptr->Tag);
      Form1->RemoveLamp(Lamp, ptr);
      ptr->Tag = 0;
      ptr->Visible = true;
    }
    if ( LabelLamp->Text != "" ) {
      Lamp = LabelLamp->Text.ToIntDef(NBR_LAMPS);
      if ( Lamp < NBR_LAMPS ) {
        Form1->InsertLamp(Lamp, ptr, ptr->Font->Color, 1);
        ptr->Tag = _LampMask + _LampTag(Lamp);
      }
    }
  } else if ( type == "TCheckBox" ) {
    TCheckBox *ptr = (TCheckBox *)sel_ptr;
    ptr->Caption = Caption->Text;
    ptr->Font = FontDialog1->Font;
    Button = CheckBox->Text.ToIntDef(256);
    if ( Button < NBR_BUTTONS ) {
      ptr->Caption = Caption->Text;
      ptr->Tag = Button << 8;
      ptr->Hint = IntToStr(Button);
    }
  } else if ( type == "TImage" ) {
    ((TImage *)sel_ptr)->Transparent = TransBitmap->Checked;
  } else if ( type == "TSevenSegment" ) {
    TSevenSegment *ptr = (TSevenSegment *)sel_ptr;
    TEdit *seg[16] = { Seg0, Seg1, Seg2, Seg3, Seg4, Seg5, Seg6, Seg7,
                       Seg8, Seg9, Seg10, Seg11, Seg12, Seg13, Seg14, Seg15 };
    int i, nbr;
    ptr->DPRight = !DPLeft->Checked;
    ptr->OnColor = OnColour->Brush->Color;
    ptr->OffColor = OffColour->Brush->Color;
    ptr->Color = BGColour->Brush->Color;
    ptr->Thickness = LEDThickness->Text.ToIntDef(5);
    ptr->HorzSpace = HSpacing->Text.ToIntDef(20);
    ptr->VertSpace = VSpacing->Text.ToIntDef(6);
    ptr->Space = Spacing->Text.ToIntDef(6);
    ptr->Seg14 = Seg16->Checked;
    ptr->DPOn = DPOn->Checked;
    ptr->Programmable = Programmable->Checked;
    if ( ptr->Programmable ) {
      for ( i = 0; i < 16; i++ ) {
        if ( ptr->GetMasks(i) < 256 && ptr->GetMasks(i) >= 0 ) {
          Form1->RemoveLamp( ptr->GetMasks(i), ptr);
          ptr->Value &= ~(1 << i);
        }
        if ( seg[i]->Text != "" ) {
          nbr = seg[i]->Text.ToIntDef(256);
          if ( nbr == -1 ) {
            ptr->Value |= ( 1 << i );
            ptr->SetMasks(i, nbr);
          } else if ( nbr >= 0 && nbr < NBR_LAMPS ) {
            Form1->InsertLamp( nbr, ptr, clYellow, 0, seg[i]->Tag);
            ptr->SetMasks(i, nbr);
          }
        } else {
          ptr->SetMasks(i, 256);
          ptr->Value &= ~( 1 << i );
        }
      }
    }
  } else if ( type == "TzLed" ) {
    TzLed *ptr = (TzLed *)sel_ptr;
    if ( ptr->Tag & _LEDLampMask ) {
      Lamp = LED1->Text.ToIntDef(NBR_LAMPS);
      ptr->Tag = _LEDLampMask + _LampTag(Lamp); //  *********
      ptr->LEDColor = LEDColour->LEDColor;
      if ( Lamp < NBR_LAMPS ) {
        old_lamp = _LampFromTag(ptr->Tag);
        Form1->RemoveLamp(old_lamp, ptr);
        Form1->InsertLamp(Lamp, ptr, clRed, 1);
      }
    } else {
      if ( LEDdigit->Tag < 0xff ) {
        ptr->LEDColor = segs[LEDdigit->Tag]->LEDColor;
        X = _DigitMask(ptr->Tag);
        Y = _SegmentMask(ptr->Tag);
        if ( X < 16 && Y < 8 )
          Form1->led_display[X].led[Y] = NULL; // Remove old
      }
      X = LEDdigit->Text.ToIntDef(255);
      Y = LEDdigit->Tag;
      if ( X < 16 && Y < 8 ) {
        if ( Form1->led_display[X].led[Y] != NULL ) {
          old_ptr = Form1->led_display[X].led[Y];
          Form1->led_display[X].led[Y] = NULL;
          delete old_ptr;
        }
        Form1->led_display[X].led[Y] = ptr;
        ptr->Tag = (X << 4) + Y;
        ptr->Hint = IntToStr(X) + " , " + IntToStr(Y);
      }
    }
  } else if ( type == "TAlpha" ) {
    TAlpha *ptr = (TAlpha *)sel_ptr;
    ptr->Colour = AlphaColour->Brush->Color;
    ptr->Reversed = Reversed->Checked;
    ptr->DigitWidth = DigitWidth->Text.ToIntDef(17);
  } else if ( type == "TBFMAlpha" ) {
    TBFMAlpha *ptr = (TBFMAlpha *)sel_ptr;
    ptr->Colour = AlphaColour->Brush->Color;
    ptr->Reversed = Reversed->Checked;
    ptr->DigitWidth = DigitWidth->Text.ToIntDef(17);
  } else if ( type == "TReel" ) {
    TReel *ptr = (TReel *)sel_ptr;
    ptr->Tag = _ReelTag(Edit1->Text.ToIntDef(9));
//    ptr->RHeight = Height->Text.ToIntDef(3);
    ptr->RHeight = HeightAdjust->Position;
    ptr->Offset = Edit3->Text.ToIntDef(0);
    ptr->Toggle = CheckBox1->Checked;
    ptr->WinLine = Edit7->Text.ToIntDef(0) > 0;
    ptr->WinLines = Edit7->Text.ToIntDef(0);
    ptr->WinLineWidth = Edit8->Text.ToIntDef(0);
    ptr->Coverage = Edit9->Text.ToIntDef(100);
    ptr->BorderWidth = BorderSize->Text.ToIntDef(1);
    ptr->BorderColor = ReelBorderColour->Brush->Color;
    ptr->Inverted = OptoInv->Checked;
    ptr->Stops = Edit5->Text.ToIntDef(16);
    ptr->Flag = Edit6->Text.ToIntDef(0);
    ptr->SetMask(LampMask->Picture->Bitmap);
    ptr->Hint = IntToStr(_ReelFromTag(ptr->Tag)) + " " + IntToStr(ptr->Stops);
    ptr->ReelLamps = ReelLamps->Checked;
    ptr->Horizontal = Horizontal->Checked;
    ptr->Smooth = Smooth->Checked;
    ptr->Reversed = Reverse->Checked;
    ptr->TotalSteps = Steps->Text.ToIntDef(96);
    ptr->CustomLamps = Custom->Checked;
    Form1->RemoveLamp(ptr->Lamp1, ptr);
    Form1->RemoveLamp(ptr->Lamp2, ptr);
    Form1->RemoveLamp(ptr->Lamp3, ptr);
    if ( Custom->Checked ) {
      if ( Lamp1->Text != "" ) {
        ptr->Lamp1 = Lamp1->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp1, ptr, clYellow, 0);
      } else
        ptr->Lamp1 = -2;
      if ( Lamp2->Text != "" ) {
        ptr->Lamp2 = Lamp2->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp2, ptr, clYellow, 1);
      } else
        ptr->Lamp2 = -2;
      if ( Lamp3->Text != "" ) {
        ptr->Lamp3 = Lamp3->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp3, ptr, clYellow, 2);
      } else
        ptr->Lamp3 = -2;
    }
  } else if ( type == "TFancyReel" ) {
    TFancyReel *ptr = (TFancyReel *)sel_ptr;
    ptr->Stops = Edit5->Text.ToIntDef(16);
    ptr->Tag = _ReelTag(Edit1->Text.ToIntDef(9));
//    ptr->RHeight = Height->Text.ToIntDef(3);
    ptr->RHeight = HeightAdjust->Position;
    ptr->Offset = Edit3->Text.ToIntDef(0);
    ptr->Toggle = CheckBox1->Checked;
//    ptr->WinLine = Edit7->Text.ToIntDef(0) > 0;
    ptr->WinLines = Edit7->Text.ToIntDef(0);
    ptr->WinLineWidth = Edit8->Text.ToIntDef(0);
//    ptr->Coverage = Edit9->Text.ToIntDef(100);
    ptr->BorderWidth = BorderSize->Text.ToIntDef(1);
    ptr->BorderColour = ReelBorderColour->Brush->Color;
    ptr->Inverted = OptoInv->Checked;
    ptr->Horizontal = Horizontal->Checked;
    ptr->Stops = Edit5->Text.ToIntDef(16);
    ptr->StepsPerStop = StepsPerStop->Text.ToIntDef(0);
    ptr->Flag = Edit6->Text.ToIntDef(0);
    ptr->SetMask(LampMask->Picture->Bitmap);
//    ptr->Hint = IntToStr(_ReelFromTag(ptr->Tag)) + " " + IntToStr(ptr->Stops);
    ptr->Lamps = ReelLamps->Checked;
//    ptr->Horizontal = Horizontal->Checked;
//    ptr->Smooth = Smooth->Checked;
    ptr->Reversed = Reverse->Checked;
    ptr->TotalSteps = Steps->Text.ToIntDef(96);
    ptr->CustomLamps = Custom->Checked;
    Form1->RemoveLamp(ptr->Lamp2, ptr);
    Form1->RemoveLamp(ptr->Lamp3, ptr);
    Form1->RemoveLamp(ptr->Lamp4, ptr);
//    if ( Custom->Checked ) {
    if ( Lamp1->Text != "" ) {
      ptr->Lamp2 = Lamp1->Text.ToIntDef(0);
      Form1->InsertLamp(ptr->Lamp2, ptr, clYellow, 1);
    } else
      ptr->Lamp2 = -2;
    if ( Lamp2->Text != "" ) {
      ptr->Lamp3 = Lamp2->Text.ToIntDef(0);
      Form1->InsertLamp(ptr->Lamp3, ptr, clYellow, 2);
    } else
      ptr->Lamp3 = -2;
    if ( Lamp3->Text != "" ) {
      ptr->Lamp4 = Lamp3->Text.ToIntDef(0);
      Form1->InsertLamp(ptr->Lamp4, ptr, clYellow, 3);
    } else
      ptr->Lamp4 = -2;
  } else if ( type == "TBandReel" ) {
    TBandReel *ptr = (TBandReel *)sel_ptr;
    ptr->Stops = BRstops->Text.ToIntDef(16);
    ptr->Tag = _ReelTag(BRnbr->Text.ToIntDef(9));
    ptr->Offset = BRoffset->Text.ToIntDef(0);
    ptr->BorderWidth = BRbordersize->Text.ToIntDef(1);
    ptr->BorderColour = BRbordercolour->Brush->Color;
    ptr->Reversed = BRoptoinvert->Checked;
    ptr->Vertical = Vertical->Checked;
    ptr->Stops = BRstops->Text.ToIntDef(16);
    ptr->SetMask(BRlampmask->Picture->Bitmap);
    ptr->SetOverlay(BROverlay->Picture->Bitmap);
    ptr->Lamps = BRlamps->Checked;
    ptr->Reversed = BRreversespin->Checked;
    ptr->TotalSteps = BRmotorsteps->Text.ToIntDef(96);
    Form1->RemoveLamp(ptr->Lamp1, ptr);
    Form1->RemoveLamp(ptr->Lamp2, ptr);
    Form1->RemoveLamp(ptr->Lamp3, ptr);
    Form1->RemoveLamp(ptr->Lamp4, ptr);
    Form1->RemoveLamp(ptr->Lamp5, ptr);
    if ( BRlamps->Checked ) {
      if ( BRlamp1->Text != "" ) {
        ptr->Lamp1 = BRlamp1->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp1, ptr, clYellow, 0);
      } else
        ptr->Lamp1 = -2;
      if ( BRlamp2->Text != "" ) {
        ptr->Lamp2 = BRlamp2->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp2, ptr, clYellow, 1);
      } else
        ptr->Lamp2 = -2;
      if ( BRlamp3->Text != "" ) {
        ptr->Lamp3 = BRlamp3->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp3, ptr, clYellow, 2);
      } else
        ptr->Lamp3 = -2;
      if ( BRlamp4->Text != "" ) {
        ptr->Lamp4 = BRlamp4->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp4, ptr, clYellow, 3);
      } else
        ptr->Lamp4 = -2;
      if ( BRlamp5->Text != "" ) {
        ptr->Lamp5 = BRlamp5->Text.ToIntDef(0);
        Form1->InsertLamp(ptr->Lamp5, ptr, clYellow, 4);
      } else
        ptr->Lamp5 = -2;
    }
  } else if ( type == "TFrame2" ) {
    TFrame2 *ptr = (TFrame2 *)sel_ptr;
    ptr->Style = ( FrameRaised->Checked == true ? Extctrls::bsRaised : Extctrls::bsLowered);
    ptr->Shape = (TBevelShape)FrameShape->ItemIndex;
  } else if ( type == "TCanvasPanel" ) {
    Form1->MainPanel->Background = BackgroundImage->OnImage1;
    Form1->MainPanel->Color = BackgroundColour->Brush->Color;
    if ( AutoSize->Checked && BackgroundImage->OnImage1->Width > 0 && BackgroundImage->OnImage1->Height ) {
      Form1->ClientWidth = BackgroundImage->OnImage1->Width;
      Form1->ClientHeight = BackgroundImage->OnImage1->Height + Form1->MeterPanel->Height;
    } else {
      Form1->Width = PWidth->Text.ToInt();
      Form1->Height = PHeight->Text.ToInt();
    }
  } else if ( type == "TzcShapeBitColBtn" ) {
    TzcShapeBitColBtn *ptr = (TzcShapeBitColBtn *)sel_ptr;
    old_shortcut = _ShortcutFromTag(ptr->Tag);
    if ( _HasLamp(ptr->Tag) ) {
      Form1->RemoveLamp(_LampFromTag(ptr->Tag), ptr);
      ptr->Tag &= ~_LampMask;
    }
    if ( ptr->Lamp2 ) {
      Form1->RemoveLamp(ptr->Lamp2-1, ptr);
      ptr->Lamp2 = 0;
    }
    if ( (ptr->Tag & _CoinMask) == 0 ) {
      old_button = _ButtonFromTag(ptr->Tag);
      Form1->Buttons[old_button] = NULL;
    }
    ptr->Caption = Caption->Text;
    ptr->Font = FontDialog1->Font;
//    ptr->Frame = ButFrame->Checked;
    ptr->Tag = 0;
    if ( Caption->Lines->Count > 1 )
      ptr->MultipleLines = true;
    else
      ptr->MultipleLines = false;
    Button = ButtonNumber->Text.ToIntDef(128);
    Coin = CoinNumber->ItemIndex;
    if ( Button < 128 ) {
      ptr->Tag = _ButtonTag(Button);
      Form1->Buttons[Button] = ptr;
      ptr->Hint = IntToStr(Button);
    }
    if ( Coin > 0 ) {
      ptr->Tag = _ButtonTag(Coin-1) | _CoinMask;
      ptr->Hint = "Coin " + IntToStr(coinmasks[Coin-1]);
    }
    if ( ButtonLamp1->Text != "" ) {
      Lamp = ButtonLamp1->Text.ToIntDef(NBR_LAMPS);
      if ( Lamp < NBR_LAMPS ) {
        Form1->InsertLamp(Lamp, ptr, ButtonOnColour->Brush->Color, 1);
        ptr->Tag |= _LampMask + _LampTag(Lamp);
        if ( CheckBox2->Checked )
          ptr->Tag |= _LockMask;
      }
    }
    if ( ButtonLamp2->Text != "" ) {
      Lamp = ButtonLamp2->Text.ToIntDef(NBR_LAMPS);
      if ( Lamp < NBR_LAMPS ) {
        Form1->InsertLamp(Lamp, ptr, ButtonOnColour->Brush->Color, 2);
        ptr->Lamp2 = Lamp + 1;
      }
    }
    ptr->OnColor = ButtonOnColour->Brush->Color;
    ptr->OffColor = ButtonOffColour->Brush->Color;
    ptr->Tag &= ~0x0ff00000;
    Form1->shortcuts[old_shortcut] = NULL;
    shortcut = StrToIntDef(Edit12->Text,0);
    if ( shortcut ) {
//      Form1->shortcuts[shortcut] = (TComponent *)ptr;
      ptr->Tag |= (shortcut << 20);
    }
  } else if ( type == "TDiscReel" ) {
    TDiscReel *ptr = (TDiscReel *)sel_ptr;
    int i, nbr;
    ptr->OuterH = OuterH->Text.ToIntDef(0);
    ptr->OuterL = OuterL->Text.ToIntDef(0);
    ptr->InnerH = InnerH->Text.ToIntDef(0);
    ptr->InnerL = InnerL->Text.ToIntDef(0);
    ptr->Lampsize = LampSize->Text.ToIntDef(0);
    ptr->Outline = Outline->Text.ToIntDef(0);
    ptr->SetSrc(Image->Picture->Bitmap);
    ptr->SetOverlay(Overlay->Picture->Bitmap);
    ptr->SetMask(Mask->Picture->Bitmap);
    ptr->Offset = Offset->Text.ToIntDef(0);
    ptr->LampOffset = LampOffset->Text.ToIntDef(0);
    ptr->Darkness = Darkness->Text.ToIntDef(0);
    ptr->Stops = DRStops->Text.ToIntDef(12);
    ptr->Steps = DRSteps->Text.ToIntDef(96);
    ptr->Inverted = DInvert->Checked;
    ptr->Reversed = DReversed->Checked;
    Form1->Reels[ptr->Tag] = NULL;
    ptr->Tag = Nbr->Text.ToInt();
    Form1->Reels[ptr->Tag] = ptr;
    for ( i = 0; i < 24; i++ ) {
      if ( ptr->GetLamp(i) < NBR_LAMPS )
        Form1->RemoveLamp( ptr->GetLamp(i), ptr);
      nbr = DiscLamp[i];
      if ( nbr < NBR_LAMPS ) {
        Form1->InsertLamp( nbr, ptr, clYellow, 0, 1 << i);
        ptr->SetLamp(i, nbr);
      }
    }
  } else if ( type == "TDotmatrix" ) {
    TDotmatrix *ptr = (TDotmatrix *)sel_ptr;
    ptr->Size = DotMatrixSize->Text.ToIntDef(7);
  } else if ( type == "TACELED" ) {
    TACELED *ptr = (TACELED *)sel_ptr;
    ptr->Size = DotMatrixSize->Text.ToIntDef(7);
    ptr->Rotated = Rotated->Checked;
  } else if ( type == "TVideo" ) {
    TVideo *ptr = (TVideo *)sel_ptr;
    Button = LeftMouse->Text.ToIntDef(128);
    if ( Button < 128 ) {
      ptr->Tag = _ButtonTag(Button);
      Form1->Buttons[Button] = ptr;
    }
  }

}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::CancelClick(TObject *Sender)
{
  PropertiesForm->Visible = FALSE;
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::LampColourMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ( Button == mbLeft ) {
    Form1->ColorDialog1->Color = ((TShape *)Sender)->Brush->Color;
    if ( Form1->ColorDialog1->Execute() )
     ((TShape *)Sender)->Brush->Color = Form1->ColorDialog1->Color;
  } else if ( Button == mbRight /*&& Sender == BackgroundColour */)
     ((TShape *)Sender)->Brush->Color = clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::Updateposition(TRect pos)
{
  if ( sel_ptr != Form1->MainPanel ) {
    Px->Text = IntToStr(pos.left);
    Py->Text = IntToStr(pos.top);
    PHeight->Text = IntToStr(pos.bottom-pos.top);
    PWidth->Text = IntToStr(pos.right-pos.left);
  } else {
    Px->Text = "";
    Py->Text = "";
    PHeight->Text = IntToStr(Form1->Height);
    PWidth->Text = IntToStr(Form1->Width);
  }
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::Update(void)
{
String type;
int X;
char buffer[5];
int key, tag, Button;
TzLed *segs[8] = {
  PropertiesForm->SegA,
  PropertiesForm->SegB,
  PropertiesForm->SegC,
  PropertiesForm->SegD,
  PropertiesForm->SegE,
  PropertiesForm->SegF,
  PropertiesForm->SegG,
  PropertiesForm->SegH
};
TTabSheet *current_tab;

  sel_ptr = Form1->menu_ptr;
  type = sel_ptr->ClassName();
  Caption->Text = "";
  Updateposition(sel_ptr->BoundsRect);

  if ( type == "TLamp2" ) {
    int Lamp;
    current_tab = GraphicLamps;
    TLamp2 *ptr = (TLamp2 *)sel_ptr;
    GShape->ItemIndex = (int)ptr->Shape;
    GColourOff->Brush->Color = ptr->BackColor;
    GColour1->Brush->Color = ptr->Colour1;
    GColour2->Brush->Color = ptr->Colour2;
    Graphic->Checked = ( ptr->SurfaceType == stBrushImage );
    OffLabel->Font->Color = clBlack;
    OnLabel1->Font->Color = clBlack;
    OnLabel2->Font->Color = clBlack;
    Points->Text = IntToStr(ptr->Vectors);
    Roundness->Text = IntToStr(ptr->RoundPerc);
    switch ( ptr->State ) {
      case 0:
        OffLabel->Font->Color = clRed;
        break;
      case 1:
        OnLabel1->Font->Color = clRed;
        break;
      case 2:
        OnLabel2->Font->Color = clRed;
        break;
    }
    if ( ptr->Tag & _NoOutLineMask )
      NoOutLine->Checked = true;
    else
      NoOutLine->Checked = false;
    Transparent->Checked = ptr->Trans;
    Angle->Text = IntToStr(ptr->StartingAngle);
    Lamp = _LampFromTag(ptr->Tag);
    Glamp1->Text = IntToStr(Lamp);
    GColour1->Brush->Color = ptr->Colour1;
    if ( _HasLamp2(ptr->Tag) ) {
      Lamp = _Lamp2FromTag(ptr->Tag);
      Glamp2->Text = IntToStr(Lamp);
      GColour2->Brush->Color = ptr->Colour2;
    } else {
      Glamp2->Text = "";
      GColour2->Brush->Color = clWhite;
    }
    if ( _HasCoin(ptr->Tag) ) {
      ComboBox1->ItemIndex = ptr->Button+1;
      LButton->Text = "";
      LButton->Enabled = false;
      ComboBox1->Enabled = true;
    } else if ( _HasButton(ptr->Tag) ) {
      Button = ptr->Button;
      LButton->Text = IntToStr(Button);
      LButton->Enabled = true;
      ComboBox1->Enabled = false;
      ComboBox1->ItemIndex = 0;
    } else {
      LButton->Enabled = true;
      ComboBox1->Enabled = true;
      LButton->Text = "";
      ComboBox1->ItemIndex = 0;
    }
    key = _ShortcutFromTag(ptr->Tag);
    if ( key ) {
      sprintf(buffer,"0x%02x", key);
      Edit4->Text = AnsiString(buffer);
    } else
      Edit4->Text = "";
    Caption->Text = ptr->Caption;
    Caption->Font = ptr->Font;
    Caption->Font->Color = clBlack;
    FontDialog1->Font = ptr->Font;
    OnImage1->OffImage->Assign(ptr->OnImage1);
    OnImage2->OffImage->Assign(ptr->OnImage2);
    OffImage->OffImage->Assign(ptr->OffImage);
  } else if ( type == "TLabel" ) {
    TLabel *ptr = (TLabel *)sel_ptr;
    int Lamp;
    current_tab = Labels;
    Caption->Text = ptr->Caption;
    Caption->Font = ptr->Font;
//    Caption->Font->Color = clBlack;
    FontDialog1->Font = ptr->Font;
    TransLabel->Checked = ptr->Transparent;
    BackColour->Brush->Color = ptr->Color;
//    TextColour->Brush->Color = ptr->Font->Color;
    Caption->Enabled = true;
    if ( _HasLamp(ptr->Tag) ) {
      Lamp = _LampFromTag(ptr->Tag);
      LabelLamp->Text = IntToStr(Lamp);
    } else
      LabelLamp->Text = "";
  } else if ( type == "TCheckBox" ) {
    TCheckBox *ptr = (TCheckBox *)sel_ptr;
    current_tab = CheckBoxes;
    CheckBox->Text = IntToStr(_ButtonFromTag(ptr->Tag));
    Caption->Text = ptr->Caption;
    Caption->Font = ptr->Font;
    Caption->Font->Color = clBlack;
    FontDialog1->Font = ptr->Font;
  } else if ( type == "TImage" ) {
    TImage *ptr = (TImage *)sel_ptr;
      if ( ptr->Tag == 0x10000 ) {
        current_tab = Bitmap;
        BitmapImage->Picture->Bitmap->Assign(ptr->Picture->Bitmap);
        TransBitmap->Checked = ptr->Picture->Bitmap->Transparent;
      }
  } else if ( type == "TSevenSegment" ) {
    TSevenSegment *ptr = (TSevenSegment *)sel_ptr;
    TEdit *seg[16] = { Seg0, Seg1, Seg2, Seg3, Seg4, Seg5, Seg6, Seg7,
                       Seg8, Seg9, Seg10, Seg11, Seg12, Seg13, Seg14, Seg15 };
    int i;
    current_tab = LEDs;
    DPLeft->Checked = !ptr->DPRight;
    Seg16->Checked = ptr->Seg14;
    DPOn->Checked = ptr->DPOn;
    Programmable->Checked = ptr->Programmable;
    OnColour->Brush->Color = ptr->OnColor;
    OffColour->Brush->Color = ptr->OffColor;
    BGColour->Brush->Color = ptr->Color;
    if ( !Programmable->Checked )
      DigitNumber->Text = IntToStr(ptr->Tag);
    else
      DigitNumber->Text = "";
    LEDThickness->Text = IntToStr(ptr->Thickness);
    HSpacing->Text = IntToStr(ptr->HorzSpace);
    VSpacing->Text = IntToStr(ptr->VertSpace);
    Spacing->Text = IntToStr(ptr->Space);
    for ( i = 0; i < 16; i++ )
      if ( Programmable->Checked && ptr->GetMasks(i) < 256 )
        seg[i]->Text = IntToStr(ptr->GetMasks(i));
      else
        seg[i]->Text = "";
  } else if ( type == "TzLed" ) {
    TzLed *ptr = (TzLed *)sel_ptr;
    if ( ptr->Tag & _LEDLampMask ) {
      current_tab = LEDLamp;
      LED1->Text = IntToStr(_LampFromTag(ptr->Tag));
      LEDColour->LEDColor = ptr->LEDColor;
    } else {
      current_tab = LED;
      for ( X = 0; X < 8; X++ ) {
        segs[X]->Enabled = false;
        segs[X]->LEDColor = lctRed;
      }
      LEDdigit->Text = "";
      tag = _DigitMask(ptr->Tag);
      if ( tag < 16 ) {
        LEDdigit->Text = IntToStr(tag);
        tag = _SegmentMask(ptr->Tag);
        if ( tag < 8 ) {
          LEDdigit->Tag = tag;
          segs[tag]->Enabled = true;
          segs[tag]->LEDColor = ptr->LEDColor;
        }
      } else
        LEDdigit->Tag = 0xff;
    }
  } else if ( type == "TAlpha" ) {
    current_tab = Alpha;
    TAlpha *ptr = (TAlpha *)sel_ptr;
    DrawAlpha(ptr->GetBitmap(),ptr->DigitWidth);
    AlphaNbr->Text = IntToStr(ptr->Tag);
    AlphaColour->Brush->Color = ptr->Colour;
//    AlphaImage->Height = ptr->Height;
//    AlphaImage->Width = ptr->Width/4;
    Reversed->Checked = ptr->Reversed;
    DigitWidth->Text = IntToStr(ptr->DigitWidth);
  } else if ( type == "TBFMAlpha" ) {
    current_tab = Alpha;
    TBFMAlpha *ptr = (TBFMAlpha *)sel_ptr;
    DrawAlpha(ptr->GetBitmap(),ptr->DigitWidth);
    AlphaNbr->Text = IntToStr(ptr->Tag);
    AlphaColour->Brush->Color = ptr->Colour;
//    AlphaImage->Height = ptr->Height;
//    AlphaImage->Width = ptr->Width/4;
    Reversed->Checked = ptr->Reversed;
    DigitWidth->Text = IntToStr(ptr->DigitWidth);
  } else if ( type == "TReel" ) {
    TReel *ptr = (TReel *)sel_ptr;
    current_tab = Reel;
    CheckBox1->Checked = ptr->Toggle;
    OptoInv->Checked = ptr->Inverted;
    Edit7->Text = IntToStr(ptr->WinLines);
    Edit1->Text = IntToStr(_ReelFromTag(ptr->Tag));
//    Height->Text = IntToStr(ptr->RHeight);
    HeightAdjust->Position = ptr->RHeight;
    Edit3->Text = IntToStr(ptr->Offset);
    BorderSize->Text = IntToStr(ptr->BorderWidth);
    Edit5->Text = IntToStr(ptr->Stops);
    Edit6->Text = IntToStr(ptr->Flag);
    Edit8->Text = IntToStr(ptr->WinLineWidth);
    Edit9->Text = IntToStr(ptr->Coverage);
    Steps->Text = IntToStr(ptr->TotalSteps);
    ReelBorderColour->Brush->Color = ptr->BorderColor;
    ReelLamps->Checked = ptr->ReelLamps;
    LampMask->Picture->Bitmap->Assign(ptr->GetMask());
    Horizontal->Checked = ptr->Horizontal;
    Smooth->Checked = ptr->Smooth;
    Reverse->Checked = ptr->Reversed;
    Custom->Checked = ptr->CustomLamps;
    if ( ptr->Lamp1 >= -1 )
      Lamp1->Text = IntToStr(ptr->Lamp1);
    else
      Lamp1->Text = "";
    if ( ptr->Lamp2 >= -1 )
      Lamp2->Text = IntToStr(ptr->Lamp2);
    else
      Lamp2->Text = "";
    if ( ptr->Lamp3 >= -1 )
      Lamp3->Text = IntToStr(ptr->Lamp3);
    else
      Lamp3->Text = "";
    if ( Custom->Checked ) {
      Lamp1->Enabled = true;
      Lamp2->Enabled = true;
      Lamp3->Enabled = true;
    } else {
      Lamp1->Enabled = false;
      Lamp2->Enabled = false;
      Lamp3->Enabled = false;
    }
  } else if ( type == "TFancyReel" ) {
    TFancyReel *ptr = (TFancyReel *)sel_ptr;
    current_tab = Reel;
    CheckBox1->Checked = ptr->Toggle;
    OptoInv->Checked = ptr->Inverted;
    Edit7->Text = IntToStr(ptr->WinLines);
    Edit1->Text = IntToStr(_ReelFromTag(ptr->Tag));
//    Height->Text = IntToStr(ptr->RHeight);
    HeightAdjust->Position = ptr->RHeight;
    Edit3->Text = IntToStr(ptr->Offset);
    BorderSize->Text = IntToStr(ptr->BorderWidth);
    Edit5->Text = IntToStr(ptr->Stops);
    StepsPerStop->Text = IntToStr(ptr->StepsPerStop);
    Edit6->Text = IntToStr(ptr->Flag);
    Edit8->Text = IntToStr(ptr->WinLineWidth);
//    Edit9->Text = IntToStr(ptr->Coverage);
    Steps->Text = IntToStr(ptr->TotalSteps);
    ReelBorderColour->Brush->Color = ptr->BorderColour;
    ReelLamps->Checked = ptr->Lamps;
    LampMask->Picture->Bitmap->Assign(ptr->GetMask());
//    Horizontal->Checked = ptr->Horizontal;
//    Smooth->Checked = ptr->Smooth;
    Reverse->Checked = ptr->Reversed;
    Custom->Checked = ptr->CustomLamps;
    Horizontal->Checked = ptr->Horizontal;
    Smooth->Checked = true;
    if ( ptr->Lamp2 >= -1 )
      Lamp1->Text = IntToStr(ptr->Lamp2);
    else
      Lamp1->Text = "";
    if ( ptr->Lamp3 >= -1 )
      Lamp2->Text = IntToStr(ptr->Lamp3);
    else
      Lamp2->Text = "";
    if ( ptr->Lamp4 >= -1 )
      Lamp3->Text = IntToStr(ptr->Lamp4);
    else
      Lamp3->Text = "";
    if ( Custom->Checked ) {
      Lamp1->Enabled = true;
      Lamp2->Enabled = true;
      Lamp3->Enabled = true;
    } else {
      Lamp1->Enabled = false;
      Lamp2->Enabled = false;
      Lamp3->Enabled = false;
    }
  } else if ( type == "TBandReel" ) {
    TBandReel *ptr = (TBandReel *)sel_ptr;
    current_tab = BandReel;
    BRnbr->Text = IntToStr(_ReelFromTag(ptr->Tag));
    BRoffset->Text = IntToStr(ptr->Offset);
    BRbordersize->Text = IntToStr(ptr->BorderWidth);
    BRstops->Text = IntToStr(ptr->Stops);
    BRmotorsteps->Text = IntToStr(ptr->TotalSteps);
    BRbordercolour->Brush->Color = ptr->BorderColour;
    BRlamps->Checked = ptr->Lamps;
    Vertical->Checked = ptr->Vertical;
    BRlampmask->Picture->Bitmap->Assign(ptr->GetMask());
    BROverlay->Picture->Bitmap->Assign(ptr->GetOverlay());
    BRreversespin->Checked = ptr->Reversed;
    BRlamps->Checked = ptr->Lamps;
    if ( ptr->Lamp1 >= -1 )
      BRlamp1->Text = IntToStr(ptr->Lamp1);
    else
      BRlamp1->Text = "";
    if ( ptr->Lamp2 >= -1 )
      BRlamp2->Text = IntToStr(ptr->Lamp2);
    else
      BRlamp2->Text = "";
    if ( ptr->Lamp3 >= -1 )
      BRlamp3->Text = IntToStr(ptr->Lamp3);
    else
      BRlamp3->Text = "";
    if ( ptr->Lamp4 >= -1 )
      BRlamp4->Text = IntToStr(ptr->Lamp4);
    else
      BRlamp4->Text = "";
    if ( ptr->Lamp5 >= -1 )
      BRlamp5->Text = IntToStr(ptr->Lamp5);
    else
      BRlamp5->Text = "";
    if ( BRlamps->Checked ) {
      BRlamp1->Enabled = true;
      BRlamp2->Enabled = true;
      BRlamp3->Enabled = true;
      BRlamp4->Enabled = true;
      BRlamp5->Enabled = true;
    } else {
      BRlamp1->Enabled = false;
      BRlamp2->Enabled = false;
      BRlamp3->Enabled = false;
      BRlamp4->Enabled = false;
      BRlamp5->Enabled = false;
    }
  } else if ( type == "TFrame2" ) {
    TFrame2 *ptr = (TFrame2 *)sel_ptr;
    current_tab = Frame;
    FrameRaised->Checked = ptr->Style == Extctrls::bsRaised;
    FrameShape->ItemIndex = (int)ptr->Shape;
  } else if ( type == "TCanvasPanel" ) {
    current_tab = Background;
    BackgroundImage->OnImage1->Assign(Form1->MainPanel->Background);
    BackgroundColour->Brush->Color = Form1->MainPanel->Color;
  } else if ( type == "TzcShapeBitColBtn" ) {
    TzcShapeBitColBtn *ptr = (TzcShapeBitColBtn *)sel_ptr;
    current_tab = BitmapButton;
    ButtonOnColour->Brush->Color = ptr->OnColor;
    if ( _HasLamp(ptr->Tag) ) {
      ButtonLamp1->Text = IntToStr(_LampFromTag(ptr->Tag));
    } else {
      ButtonLamp1->Text = "";
      ButtonOnColour->Brush->Color = clWhite;
    }
    if ( ptr->Lamp2 ) {
      ButtonLamp2->Text = IntToStr(ptr->Lamp2-1);
    } else {
      ButtonLamp2->Text = "";
    }
    TestButton->Shape = ptr->Shape;
    ButtonOffColour->Brush->Color = ptr->OffColor;
    if ( ptr->Tag & _CoinMask ) {
      CoinNumber->ItemIndex = _ButtonFromTag(ptr->Tag)+1;
      ButtonNumber->Text = "";
      ButtonNumber->Enabled = false;
      CoinNumber->Enabled = true;
    } else {
      ButtonNumber->Text = IntToStr(_ButtonFromTag(ptr->Tag));
      ButtonNumber->Enabled = true;
      CoinNumber->Enabled = false;
      CoinNumber->ItemIndex = 0;
    }
    Caption->Text = ptr->Caption;
    Caption->Font = ptr->Font;
    Caption->Font->Color = clBlack;
    FontDialog1->Font = ptr->Font;
    ButtonOffBMP->OffImage->Assign(ptr->OffBitmap);
    ButtonOn1BMP->OffImage->Assign(ptr->OnBitmap);
    ButtonOn2BMP->OffImage->Assign(ptr->OnBitmap2);
    key = _ShortcutFromTag(ptr->Tag);
    if ( key ) {
      sprintf(buffer,"0x%02x", key);
      Edit12->Text = AnsiString(buffer);
    } else
      Edit12->Text = "";
    if ( ptr->Tag & _LockMask )
      CheckBox2->Checked = true;
    else
      CheckBox2->Checked = false;
    CheckBox3->Checked = ptr->Graphic;
  } else if ( type == "TDiscReel" ) {
    current_tab = DiscReel;
    TDiscReel *ptr = (TDiscReel *)sel_ptr;
    UpDown1->Position = ptr->OuterH;
    UpDown2->Position = ptr->OuterL;
//    OuterH->Text = IntToStr(ptr->OuterH);
//    OuterL->Text = IntToStr(ptr->OuterL);
    UpDown3->Position = ptr->InnerH;
    UpDown4->Position = ptr->InnerL;
    InnerH->Text = IntToStr(ptr->InnerH);
    InnerL->Text = IntToStr(ptr->InnerL);
    LampSize->Text = IntToStr(ptr->Lampsize);
    Outline->Text = IntToStr(ptr->Outline);
    DRSteps->Text = IntToStr(ptr->Steps);
    DRStops->Text = IntToStr(ptr->Stops);
    DInvert->Checked = ptr->Inverted;
    DReversed->Checked = ptr->Reversed;
    Image->Picture->Bitmap->Assign(ptr->Source);
    Overlay->Picture->Bitmap->Assign(ptr->Overlay);
    Mask->Picture->Bitmap->Assign(ptr->LampMask);
    Nbr->Text = IntToStr(_ReelFromTag(ptr->Tag));
    Offset->Text = IntToStr(ptr->Offset);
    LampOffset->Text = IntToStr(ptr->LampOffset);
    Darkness->Text = IntToStr(ptr->Darkness);
    for ( int i = 0; i < 24; i++ )
      DiscLamp[i] = ptr->GetLamp(i);
    UpDown->Position = 1;
    if ( DiscLamp[0] < 256 )
      Lamp->Text = IntToStr(DiscLamp[0]);
    else
      Lamp->Text = "";
  } else if ( type == "TDotmatrix" ) {
    current_tab = DotMatrix;
    TDotmatrix *ptr = (TDotmatrix *)sel_ptr;
    DotMatrixSize->Text = IntToStr(ptr->Size);
  } else if ( type == "TACELED" ) {
    current_tab = DotMatrix;
    TACELED *ptr = (TACELED *)sel_ptr;
    DotMatrixSize->Text = IntToStr(ptr->Size);
    Rotated->Checked = ptr->Rotated;
  } else if ( type == "TVideo" ) {
    current_tab = Video;
    TVideo *ptr = (TVideo *)sel_ptr;
    LeftMouse->Text = IntToStr(_ButtonFromTag(ptr->Tag));
  } else
    current_tab = None;

  if ( last_tab && last_tab != current_tab )
    last_tab->TabVisible = false;

  PageControl->ActivePage = current_tab;
  current_tab->TabVisible = true;
  last_tab = current_tab;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::PHeightChange(TObject *Sender)
{
  if ( PHeight->Text != "" && !Form1->FResizing && !Form1->FMove )
    if ( PHeight->Text.ToIntDef(0) > 5 )
      sel_ptr->Height = PHeight->Text.ToInt();
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::PWidthChange(TObject *Sender)
{
  if ( PWidth->Text != "" && !Form1->FResizing && !Form1->FMove )
    if ( PWidth->Text.ToIntDef(0) > 5 )
      sel_ptr->Width = PWidth->Text.ToInt();
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::PxChange(TObject *Sender)
{
  if ( Px->Text != "" && !Form1->FResizing && !Form1->FMove )
    sel_ptr->Left = Px->Text.ToIntDef(5);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::PyChange(TObject *Sender)
{
  if ( Py->Text != "" && !Form1->FResizing && !Form1->FMove )
    sel_ptr->Top = Py->Text.ToIntDef(5);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::FontClick(TObject *Sender)
{
  if ( FontDialog1->Execute() ) {
    Caption->Font = FontDialog1->Font;
    Caption->Font->Color = clBlack;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::ShapeChange(TObject *Sender)
{
  if ( sel_ptr->ClassNameIs("TLamp2") )
    ((TLamp2 *)sel_ptr)->Shape = (TGPShapeType)GShape->ItemIndex;
  else
    ((TFrame2 *)sel_ptr)->Shape = (TBevelShape)FrameShape->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OffImageClick(TObject *Sender)
{
Graphics::TBitmap *ptr;

  if ( sel_ptr->ClassNameIs("TLamp2") )
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      OffImage->OffImage->Assign(ptr);
      delete ptr;
      ((TLamp2 *)sel_ptr)->OffImage->Assign(OffImage->OffImage);
      ((TLamp2 *)sel_ptr)->SurfaceType = stBrushImage;
    }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::SegClick(TObject *Sender)
{
TzLed *segs[8] = {
  PropertiesForm->SegA,
  PropertiesForm->SegB,
  PropertiesForm->SegC,
  PropertiesForm->SegD,
  PropertiesForm->SegE,
  PropertiesForm->SegF,
  PropertiesForm->SegG,
  PropertiesForm->SegH
};
int loop;

  for ( loop = 0; loop < 8; loop++ ) {
    segs[loop]->Enabled = false;
    segs[loop]->LEDColor = lctRed;
  }
  ((TzLed *)Sender)->Enabled = true;
  LEDdigit->Tag = ((TzLed *)Sender)->Tag;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::SegDblClick(TObject *Sender)
{
  if ( ((TzLed *)Sender)->LEDColor == lctRed )
    ((TzLed *)Sender)->LEDColor = lctGreen;
  else
    ((TzLed *)Sender)->LEDColor = lctRed;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OnImage1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;
Graphics::TBitmap *image_ptr;

  if ( Button == mbLeft )
    if ( !Shift.Contains(ssShift) ) {
      OpenPictureDialog1->FileName = "";
      if ( OpenPictureDialog1->Execute() ) {
        ptr = new Graphics::TBitmap();
        ptr->LoadFromFile(OpenPictureDialog1->FileName);
        ((TLamp2 *)Sender)->OffImage->Assign(ptr);
        switch ( ((TLamp2 *)Sender)->Tag ) {
          case 0:
            image_ptr = ((TLamp2 *)sel_ptr)->OffImage;
            break;
          case 1:
            image_ptr = ((TLamp2 *)sel_ptr)->OnImage1;
            break;
          case 2:
            image_ptr = ((TLamp2 *)sel_ptr)->OnImage2;
            break;
          case 3:
            image_ptr = ((TzcShapeBitColBtn *)sel_ptr)->OffBitmap;
            break;
          case 4:
            image_ptr = ((TzcShapeBitColBtn *)sel_ptr)->OnBitmap;
            break;
          case 5:
            image_ptr = ((TzcShapeBitColBtn *)sel_ptr)->OnBitmap2;
            break;
        }
        if ( ((TLamp2 *)Sender)->Tag < 3 ) {
          if ( GLAutoSize->Checked ) {
            ((TLamp2 *)sel_ptr)->Height = ptr->Height;
            ((TLamp2 *)sel_ptr)->Width = ptr->Width;
            PHeight->Text = IntToStr(ptr->Height);
            PWidth->Text = IntToStr(ptr->Width);
          }
          ((TLamp2 *)sel_ptr)->SurfaceType = stBrushImage;
        }
        image_ptr->Assign(ptr);
        delete ptr;
      }
    } else {
      SaveDialog1->DefaultExt = AnsiString("bmp");
      if ( SaveDialog1->Execute() ) {
//        SaveDialog1->InitialDir = ExtractFilePath(SaveDialog1->FileName);
        ((TLamp2 *)Sender)->OffImage->SaveToFile(SaveDialog1->FileName);
      }
    }
  else if ( Button == mbRight ) {
    ptr = new Graphics::TBitmap();
    ((TLamp2 *)Sender)->OffImage->Assign(ptr);
    switch ( ((TLamp2 *)Sender)->Tag ) {
      case 0:
        image_ptr = ((TLamp2 *)sel_ptr)->OffImage;
        break;
      case 1:
        image_ptr = ((TLamp2 *)sel_ptr)->OnImage1;
        break;
      case 2:
        image_ptr = ((TLamp2 *)sel_ptr)->OnImage2;
        break;
      case 3:
        image_ptr = ((TzcShapeBitColBtn *)sel_ptr)->OffBitmap;
        break;
      case 4:
        image_ptr = ((TzcShapeBitColBtn *)sel_ptr)->OnBitmap;
        break;
      case 5:
        image_ptr = ((TzcShapeBitColBtn *)sel_ptr)->OnBitmap2;
        break;
    }
    image_ptr->Assign(ptr);
    delete ptr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::BitmapImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;

  if ( Button == mbLeft ) {
    OpenPictureDialog1->FileName = "";
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      BitmapImage->Picture->Bitmap->Assign(ptr);
      if ( BitmapAutoSize->Checked ) {
        ((TImage *)sel_ptr)->Height = ptr->Height;
        ((TImage *)sel_ptr)->Width = ptr->Width;
        PHeight->Text = IntToStr(ptr->Height);
        PWidth->Text = IntToStr(ptr->Width);
      }
      ((TImage *)sel_ptr)->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
  } else {
    SaveDialog1->DefaultExt = AnsiString("bmp");
    if ( SaveDialog1->Execute() ) {
      ((TImage *)sel_ptr)->Picture->Bitmap->SaveToFile(SaveDialog1->FileName);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OnLabel1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if ( ((TLabel *)Sender)->Font->Color == clBlack ) {
     OnLabel1->Font->Color = clBlack;
     OnLabel2->Font->Color = clBlack;
     OffLabel->Font->Color = clBlack;
     ((TLabel *)Sender)->Font->Color = clRed;
     ((TLamp2 *)sel_ptr)->State = ((TLabel *)Sender)->Tag;
     if ( ((TLabel *)Sender)->Tag > 0 && NoOutLine->Checked )
       ((TLamp2 *)sel_ptr)->OutlineStyle = psClear;
     else
       ((TLamp2 *)sel_ptr)->OutlineStyle = psSolid;
   }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::ButColourMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if ( Form1->ColorDialog1->Execute() )
   ((TShape *)Sender)->Brush->Color = Form1->ColorDialog1->Color;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::TransparentClick(TObject *Sender)
{
  if ( Transparent->Checked ) {
    ((TLamp2 *)sel_ptr)->Trans = true;
  } else
    ((TLamp2 *)sel_ptr)->Trans = false;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::FrameRaisedClick(TObject *Sender)
{
  ((TFrame2 *)sel_ptr)->Style = ( FrameRaised->Checked == true ? Extctrls::bsRaised : Extctrls::bsLowered);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::PointsChange(TObject *Sender)
{
  ((TLamp2 *)sel_ptr)->Vectors = Points->Text.ToIntDef(3);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::GraphicClick(TObject *Sender)
{
  if ( Graphic->Checked )
    ((TLamp2 *)sel_ptr)->SurfaceType = stBrushImage;
  else
    ((TLamp2 *)sel_ptr)->SurfaceType = stBrushOpaque;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::BackgroundImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;

  if ( Button == mbLeft ) {
    if ( !Shift.Contains(ssShift) ) {
      OpenPictureDialog1->FileName = "";
      if ( OpenPictureDialog1->Execute() ) {
        ptr = new Graphics::TBitmap();
        ptr->LoadFromFile(OpenPictureDialog1->FileName);
        BackgroundImage->OnImage1->Assign(ptr);
        delete ptr;
      }
    } else {
      SaveDialog1->DefaultExt = AnsiString("bmp");
      if ( SaveDialog1->Execute() ) {
        BackgroundImage->OnImage1->SaveToFile(SaveDialog1->FileName);
      }
    }
  } else if ( Button == mbRight ) {
    ptr = new Graphics::TBitmap();
    BackgroundImage->OnImage1->Assign(ptr);
    delete ptr;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::CheckBox3Click(TObject *Sender)
{
  if ( CheckBox3->Checked )
    ((TzcShapeBitColBtn *)sel_ptr)->Graphic = true;
  else
    ((TzcShapeBitColBtn *)sel_ptr)->Graphic = false;
}
//---------------------------------------------------------------------------


void __fastcall TPropertiesForm::testbuttonClick(TObject *Sender)
{
  switch ( TestButton->Shape ) {
    case sbRectangle:
      TestButton->Shape = sbUpTriangle;
      break;
    case sbUpTriangle:
      TestButton->Shape = sbDnTriangle;
      break;
    case sbDnTriangle:
      TestButton->Shape = sbLTriangle;
      break;
    case sbLTriangle:
      TestButton->Shape = sbRTriangle;
      break;
    case sbRTriangle:
      TestButton->Shape = sbCircle;
      break;
    default:
      TestButton->Shape = sbRectangle;
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::TestButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  switch ( TestButton->Shape ) {
    case sbRectangle:
      TestButton->Shape = sbUpTriangle;
      break;
    case sbUpTriangle:
      TestButton->Shape = sbDnTriangle;
      break;
    case sbDnTriangle:
      TestButton->Shape = sbLTriangle;
      break;
    case sbLTriangle:
      TestButton->Shape = sbRTriangle;
      break;
    case sbRTriangle:
      TestButton->Shape = sbCircle;
      break;
    default:
      TestButton->Shape = sbRectangle;
      break;
  }
  ((TzcShapeBitColBtn *)sel_ptr)->Shape = TestButton->Shape;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::LEDColourMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  switch ( ((TzLed *)Sender)->LEDColor ) {
    case lctRed:
      ((TzLed *)Sender)->LEDColor = lctGreen;
      break;
    case lctGreen:
      ((TzLed *)Sender)->LEDColor = lctyellow;
      break;
    case lctyellow:
      ((TzLed *)Sender)->LEDColor = lctBlue;
      break;
    default:
      ((TzLed *)Sender)->LEDColor = lctRed;
      break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::AlphaImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;
int nbr = sel_ptr->Tag;

  if ( Button == mbLeft ) {
    if ( !Shift.Contains(ssShift) ) {
      OpenPictureDialog1->FileName = "";
      if ( OpenPictureDialog1->Execute() ) {
        ptr = new Graphics::TBitmap();
        ptr->LoadFromFile(OpenPictureDialog1->FileName);
//        if ( ((ptr->Width/64) * 64) == ptr->Width ) {
          if ( Form1->display[nbr] ) {
            Form1->display[nbr]->SetBitmap(ptr);
            DrawAlpha(Form1->display[nbr]->GetBitmap(),Form1->display[nbr]->DigitWidth);
          }
//          AlphaImage->Picture->Bitmap->Assign(ptr);
//          AlphaImage->Height = ptr->Height;
//          AlphaImage->Width = (ptr->Width/64) * 4;
//        }
        delete ptr;
      }
    } else {
      if ( SaveDialog1->Execute() ) {
        ptr = new Graphics::TBitmap();
        ptr->Assign(Form1->display[nbr]->GetBitmap());
        ptr->SaveToFile(SaveDialog1->FileName);
        delete ptr;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::LampMaskClick(TObject *Sender)
{
Graphics::TBitmap *ptr;

//    OpenPictureDialog1->FileName = "";
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      ((TImage *)Sender)->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
}
//---------------------------------------------------------------------------



void __fastcall TPropertiesForm::BitmapButtonMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if ( ((TLabel *)Sender)->Font->Color == clBlack ) {
     Label40->Font->Color = clBlack;
     Label37->Font->Color = clBlack;
     Label41->Font->Color = clBlack;
     ((TLabel *)Sender)->Font->Color = clRed;
     switch ( ((TLabel *)Sender)->Tag ) {
       case 0:
         ((TzcShapeBitColBtn *)sel_ptr)->State = false;
         ((TzcShapeBitColBtn *)sel_ptr)->State2 = false;
         break;
       case 1:
         ((TzcShapeBitColBtn *)sel_ptr)->State = true;
         ((TzcShapeBitColBtn *)sel_ptr)->State2 = false;
         break;
       case 2:
         ((TzcShapeBitColBtn *)sel_ptr)->State = false;
         ((TzcShapeBitColBtn *)sel_ptr)->State2 = true;
         break;
     }
   }

}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::RoundnessChange(TObject *Sender)
{
  ((TLamp2 *)sel_ptr)->RoundPerc = Roundness->Text.ToIntDef(5);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::Edit12KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
char buffer[6];

  if ( Key < VK_F7 || Key > VK_F12 ) {
    sprintf(buffer,"0x%02x",Key);
    ((TEdit *)Sender)->Text = AnsiString(buffer);
  }
  Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::Edit12KeyPress(TObject *Sender, char &Key)
{
  Key = 0;    
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::ClearClick(TObject *Sender)
{
  Edit12->Text = "";
}
//---------------------------------------------------------------------------


void __fastcall TPropertiesForm::Seg16Click(TObject *Sender)
{
TEdit *seg[16] = { Seg0, Seg1, Seg2, Seg3, Seg4, Seg5, Seg6, Seg7,
                   Seg8, Seg9, Seg10, Seg11, Seg12, Seg13, Seg14, Seg15 };

  for ( int i = 0; i < 16; i++ ) {
    if ( Programmable->Checked ) {
      if ( (Seg16->Checked && i > 7) || i < 8 )
        seg[i]->Enabled = true;
      else
        seg[i]->Enabled = false;
    } else
      seg[i]->Enabled = false;
  }
  if ( !Programmable->Checked )
    DigitNumber->Text = IntToStr(sel_ptr->Tag);
  else
    DigitNumber->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::NumberChange(TObject *Sender)
{
  if ( CoinNumber->ItemIndex <= 0 )
    ButtonNumber->Enabled = true;
  else
    ButtonNumber->Enabled = false;
  if ( ButtonNumber->Text == "" )
    CoinNumber->Enabled = true;
  else
    CoinNumber->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::CustomClick(TObject *Sender)
{
  if ( Custom->Checked ) {
    Lamp1->Enabled = true;
    Lamp2->Enabled = true;
    Lamp3->Enabled = true;
  } else {
    Lamp1->Enabled = false;
    Lamp2->Enabled = false;
    Lamp3->Enabled = false;
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::ImageClick(TObject *Sender)
{
Graphics::TBitmap *ptr;

    OpenPictureDialog1->FileName = "";
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      Image->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OverlayClick(TObject *Sender)
{
Graphics::TBitmap *ptr;

    OpenPictureDialog1->FileName = "";
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      Overlay->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::MaskClick(TObject *Sender)
{
Graphics::TBitmap *ptr;

    OpenPictureDialog1->FileName = "";
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      Mask->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::UpDownClick(TObject *Sender,
      TUDBtnType Button)
{
   int value = DiscLamp[UpDown->Position-1];
   if ( value < 256 )
     Lamp->Text = IntToStr(value);
   else
     Lamp->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::LampChange(TObject *Sender)
{
   DiscLamp[UpDown->Position-1] = Lamp->Text.ToIntDef(256);
}
//---------------------------------------------------------------------------


void __fastcall TPropertiesForm::SpeedButton1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int mask = 1 << (UpDown->Position - 1);
  ((TDiscReel *)sel_ptr)->State |= mask;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::SpeedButton1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  int mask = 1 << (UpDown->Position - 1);
  ((TDiscReel *)sel_ptr)->State &= ~mask;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::DrawAlpha(Graphics::TBitmap *ptr, int DigitWidth)
{
//int Digits = bmp->Width / DigitWidth;

  Graphics::TBitmap *bmp = new Graphics::TBitmap();
  bmp->Assign(ptr);
//  AlphaImage->Width = ((bmp->Width / ptr->DigitWidth) / 8) * ptr->DigitWidth;
//  AlphaImage->Height = bmp->Height * 8;
  AlphaImage->Width = DigitWidth * 8;
  AlphaImage->Height = bmp->Height * ((bmp->Width / DigitWidth) / 8);
  AlphaImage->Picture->Bitmap->Width = AlphaImage->Width;
  AlphaImage->Picture->Bitmap->Height = AlphaImage->Height;
  for ( int i = 0; i < bmp->Width / DigitWidth; i++ ) {
    TRect src = Rect( DigitWidth * i, 0, DigitWidth + DigitWidth * i, bmp->Height);
    int X = i % 8;
    int Y = i / 8;
    TRect dst = Rect( DigitWidth * X,
                      bmp->Height * Y,
                      DigitWidth + DigitWidth * X,
                      bmp->Height + bmp->Height * Y);
    AlphaImage->Picture->Bitmap->Canvas->CopyRect( dst, bmp->Canvas, src);
  }
  delete bmp;
}
//---------------------------------------------------------------------------
void __fastcall TPropertiesForm::BRlampsClick(TObject *Sender)
{
  if ( BRlamps->Checked ) {
    BRlamp1->Enabled = true;
    BRlamp2->Enabled = true;
    BRlamp3->Enabled = true;
    BRlamp4->Enabled = true;
    BRlamp5->Enabled = true;
  } else {
    BRlamp1->Enabled = false;
    BRlamp2->Enabled = false;
    BRlamp3->Enabled = false;
    BRlamp4->Enabled = false;
    BRlamp5->Enabled = false;
  }

}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::LampMaskMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;

  if ( Button == mbLeft ) {
    if ( Shift.Contains(ssShift) ) {
      if ( SaveDialog1->Execute() )
        ((TImage *)Sender)->Picture->Bitmap->SaveToFile(SaveDialog1->FileName);
    } else {
      if ( OpenPictureDialog1->Execute() ) {
        ptr = new Graphics::TBitmap();
        ptr->LoadFromFile(OpenPictureDialog1->FileName);
        ((TImage *)Sender)->Picture->Bitmap->Assign(ptr);
        delete ptr;
      }
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::HeightAdjustClick(TObject *Sender,
      TUDBtnType Button)
{
  if ( sel_ptr->ClassNameIs("TFancyReel") )
    ((TFancyReel *)sel_ptr)->RHeight = HeightAdjust->Position;
  else if ( sel_ptr->ClassNameIs("TReel") )
    ((TReel *)sel_ptr)->RHeight = HeightAdjust->Position;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::LButtonChange(TObject *Sender)
{
  if ( ComboBox1->ItemIndex <= 0 )
    LButton->Enabled = true;
  else
    LButton->Enabled = false;
  if ( LButton->Text == "" )
    ComboBox1->Enabled = true;
  else
    ComboBox1->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OuterHClick(TObject *Sender,
      TUDBtnType Button)
{
    TDiscReel *ptr = (TDiscReel *)sel_ptr;
    ptr->OuterH = OuterH->Text.ToIntDef(0);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OuterLClick(TObject *Sender,
      TUDBtnType Button)
{
    TDiscReel *ptr = (TDiscReel *)sel_ptr;
    ptr->OuterL = OuterL->Text.ToIntDef(0);
}
//---------------------------------------------------------------------------


void __fastcall TPropertiesForm::InnerHClick(TObject *Sender,
      TUDBtnType Button)
{
    TDiscReel *ptr = (TDiscReel *)sel_ptr;
    ptr->InnerH = InnerH->Text.ToIntDef(0);
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::InnerLClick(TObject *Sender,
      TUDBtnType Button)
{
    TDiscReel *ptr = (TDiscReel *)sel_ptr;
    ptr->InnerL = InnerL->Text.ToIntDef(0);
}
//---------------------------------------------------------------------------



void __fastcall TPropertiesForm::Clear2Click(TObject *Sender)
{
  Edit4->Text = "";    
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;

    OpenPictureDialog1->FileName = "";
    if ( !Shift.Contains(ssShift) ) {
      if ( OpenPictureDialog1->Execute() ) {
        ptr = new Graphics::TBitmap();
        ptr->LoadFromFile(OpenPictureDialog1->FileName);
        Image->Picture->Bitmap->Assign(ptr);
        delete ptr;
      }
    } else {
      if ( SaveDialog1->Execute() )
        Image->Picture->Bitmap->SaveToFile(SaveDialog1->FileName);
    }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::OverlayMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
Graphics::TBitmap *ptr;

  OpenPictureDialog1->FileName = "";
  if ( !Shift.Contains(ssShift) ) {
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      Overlay->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
  } else {
      if ( SaveDialog1->Execute() )
        Overlay->Picture->Bitmap->SaveToFile(SaveDialog1->FileName);
  }
}
//---------------------------------------------------------------------------

void __fastcall TPropertiesForm::MaskMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  Graphics::TBitmap *ptr;

    OpenPictureDialog1->FileName = "";
  if ( !Shift.Contains(ssShift) ) {
    if ( OpenPictureDialog1->Execute() ) {
      ptr = new Graphics::TBitmap();
      ptr->LoadFromFile(OpenPictureDialog1->FileName);
      Mask->Picture->Bitmap->Assign(ptr);
      delete ptr;
    }
  } else {
      if ( SaveDialog1->Execute() )
       Mask->Picture->Bitmap->SaveToFile(SaveDialog1->FileName);
  }
}
//---------------------------------------------------------------------------

