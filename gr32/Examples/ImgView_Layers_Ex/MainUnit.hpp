// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'MainUnit.pas' rev: 5.00

#ifndef MainUnitHPP
#define MainUnitHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32_Transforms.hpp>	// Pascal unit
#include <GR32_Filters.hpp>	// Pascal unit
#include <GR32_RangeBars.hpp>	// Pascal unit
#include <GR32_Layers.hpp>	// Pascal unit
#include <GR32_Image.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <ExtDlgs.hpp>	// Pascal unit
#include <jpeg.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Mainunit
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TMainForm;
class PASCALIMPLEMENTATION TMainForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Gr32_image::TImgView32* ImgView;
	Extctrls::TPanel* SidePanel;
	Menus::TMainMenu* MainMenu;
	Menus::TMenuItem* mnFileNew;
	Menus::TMenuItem* mnFile;
	Menus::TMenuItem* mnFileOpen;
	Menus::TMenuItem* mnLayers;
	Menus::TMenuItem* mnNewBitmapLayer;
	Menus::TMenuItem* mnDelete;
	Extdlgs::TOpenPictureDialog* OpenPictureDialog1;
	Extctrls::TPanel* pnlImage;
	Stdctrls::TComboBox* ScaleCombo;
	Stdctrls::TLabel* Label1;
	Extctrls::TPanel* Panel2;
	Stdctrls::TCheckBox* ImageInterpolate;
	Extctrls::TPanel* pnlBitmapLayer;
	Extctrls::TPanel* Panel3;
	Stdctrls::TLabel* Label2;
	Gr32_rangebars::TGaugeBar* LayerOpacity;
	Stdctrls::TCheckBox* LayerInterpolate;
	Menus::TMenuItem* mnNewBitmapRGBA;
	Stdctrls::TButton* LayerRescale;
	Stdctrls::TButton* LayerResetScale;
	Stdctrls::TCheckBox* Cropped;
	Menus::TMenuItem* mnNewCustomLayer;
	Menus::TMenuItem* mnMagnifier;
	Extctrls::TPanel* PnlMagn;
	Stdctrls::TLabel* Label3;
	Extctrls::TPanel* Panel4;
	Gr32_rangebars::TGaugeBar* MagnOpacity;
	Dialogs::TSaveDialog* SaveDialog1;
	Stdctrls::TLabel* Label4;
	Gr32_rangebars::TGaugeBar* MagnMagnification;
	Stdctrls::TLabel* Label5;
	Gr32_rangebars::TGaugeBar* MagnRotation;
	Stdctrls::TCheckBox* MagnInterpolate;
	Menus::TMenuItem* mnSimpleDrawing;
	Menus::TMenuItem* mnArrange;
	Menus::TMenuItem* mnBringFront;
	Menus::TMenuItem* mnSendBack;
	Menus::TMenuItem* N1;
	Menus::TMenuItem* mnLevelUp;
	Menus::TMenuItem* mnLevelDown;
	Menus::TMenuItem* N2;
	Menus::TMenuItem* mnScaled;
	Menus::TMenuItem* N4;
	Menus::TMenuItem* mnFlatten;
	void __fastcall mnFileNewClick(System::TObject* Sender);
	void __fastcall mnFileOpenClick(System::TObject* Sender);
	void __fastcall mnNewBitmapLayerClick(System::TObject* Sender);
	void __fastcall mnDeleteClick(System::TObject* Sender);
	void __fastcall ScaleComboChange(System::TObject* Sender);
	void __fastcall FormDestroy(System::TObject* Sender);
	void __fastcall mnLayersClick(System::TObject* Sender);
	void __fastcall ImageInterpolateClick(System::TObject* Sender);
	void __fastcall LayerOpacityChange(System::TObject* Sender);
	void __fastcall LayerInterpolateClick(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall mnNewBitmapRGBAClick(System::TObject* Sender);
	void __fastcall LayerRescaleClick(System::TObject* Sender);
	void __fastcall LayerResetScaleClick(System::TObject* Sender);
	void __fastcall CroppedClick(System::TObject* Sender);
	void __fastcall mnMagnifierClick(System::TObject* Sender);
	void __fastcall MagnChange(System::TObject* Sender);
	void __fastcall mnSimpleDrawingClick(System::TObject* Sender);
	void __fastcall mnArrangeClick(System::TObject* Sender);
	void __fastcall mnScaledClick(System::TObject* Sender);
	void __fastcall mnReorder(System::TObject* Sender);
	void __fastcall ImgViewMouseDown(System::TObject* Sender, Controls::TMouseButton Button, Classes::TShiftState 
		Shift, int X, int Y, Gr32_layers::TCustomLayer* Layer);
	void __fastcall ImgViewPaintStage(System::TObject* Sender, Gr32::TBitmap32* Buffer, unsigned StageNum
		);
	void __fastcall mnFlattenClick(System::TObject* Sender);
	void __fastcall ImgViewMouseWheelUp(System::TObject* Sender, Classes::TShiftState Shift, const Windows::TPoint 
		&MousePos, bool &Handled);
	void __fastcall ImgViewMouseWheelDown(System::TObject* Sender, Classes::TShiftState Shift, const Windows::TPoint 
		&MousePos, bool &Handled);
	
private:
	Gr32_layers::TPositionedLayer* FSelection;
	void __fastcall SetSelection(Gr32_layers::TPositionedLayer* Value);
	HIDESBASE MESSAGE void __fastcall WMEraseBkgnd(Messages::TMessage &Msg);
	
protected:
	Gr32_layers::TRubberbandLayer* RBLayer;
	Gr32_layers::TPositionedLayer* __fastcall CreatePositionedLayer(void);
	void __fastcall LayerMouseDown(System::TObject* Sender, Controls::TMouseButton Buttons, Classes::TShiftState 
		Shift, int X, int Y);
	void __fastcall RBResizing(System::TObject* Sender, const Gr32::TFloatRect &OldLocation, Gr32::TFloatRect 
		&NewLocation, Gr32_layers::TDragState DragState, Classes::TShiftState Shift);
	void __fastcall PaintMagnifierHandler(System::TObject* Sender, Gr32::TBitmap32* Buffer);
	void __fastcall PaintSimpleDrawingHandler(System::TObject* Sender, Gr32::TBitmap32* Buffer);
	
public:
	void __fastcall CreateNewImage(int AWidth, int AHeight, Gr32::TColor32 FillColor);
	void __fastcall OpenImage(const AnsiString FileName);
	__property Gr32_layers::TPositionedLayer* Selection = {read=FSelection, write=SetSelection};
public:
		
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TMainForm(Classes::TComponent* AOwner) : Forms::TForm(
		AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TMainForm(Classes::TComponent* AOwner, int Dummy
		) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TMainForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TMainForm(HWND ParentWindow) : Forms::TForm(ParentWindow
		) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMainForm* MainForm;

}	/* namespace Mainunit */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Mainunit;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// MainUnit
