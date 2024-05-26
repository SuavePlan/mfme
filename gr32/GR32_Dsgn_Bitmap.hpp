// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'GR32_Dsgn_Bitmap.pas' rev: 5.00

#ifndef GR32_Dsgn_BitmapHPP
#define GR32_Dsgn_BitmapHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <DsgnIntf.hpp>	// Pascal unit
#include <GR32_Filters.hpp>	// Pascal unit
#include <GR32_Layers.hpp>	// Pascal unit
#include <GR32_Image.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <ToolWin.hpp>	// Pascal unit
#include <Menus.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <ImgList.hpp>	// Pascal unit
#include <Clipbrd.hpp>	// Pascal unit
#include <ExtDlgs.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <Registry.hpp>	// Pascal unit
#include <Consts.hpp>	// Pascal unit
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

namespace Gr32_dsgn_bitmap
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TPictureEditorForm;
class PASCALIMPLEMENTATION TPictureEditorForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Comctrls::TToolBar* ToolBar1;
	Comctrls::TToolButton* Load;
	Comctrls::TToolButton* Save;
	Controls::TImageList* ImageList;
	Comctrls::TToolButton* Clear;
	Comctrls::TToolButton* ToolButton2;
	Comctrls::TToolButton* Copy;
	Comctrls::TToolButton* Paste;
	Extctrls::TTimer* Timer;
	Comctrls::TPageControl* PageControl;
	Comctrls::TTabSheet* ImageSheet;
	Comctrls::TTabSheet* AlphaSheet;
	Extdlgs::TOpenPictureDialog* OpenDialog;
	Extdlgs::TSavePictureDialog* SaveDialog;
	Menus::TPopupMenu* PopupMenu;
	Menus::TMenuItem* mnSave;
	Menus::TMenuItem* mnSeparator;
	Menus::TMenuItem* mnCopy;
	Menus::TMenuItem* mnPaste;
	Menus::TMenuItem* mnClear;
	Menus::TMenuItem* Load1;
	Menus::TMenuItem* mnSeparator2;
	Menus::TMenuItem* mnInvert;
	Extctrls::TPanel* Panel1;
	Stdctrls::TButton* OKButton;
	Stdctrls::TButton* Cancel;
	Stdctrls::TStaticText* StaticText1;
	Stdctrls::TComboBox* MagnCombo;
	Extctrls::TPanel* Panel2;
	Extctrls::TBevel* Bevel1;
	void __fastcall LoadClick(System::TObject* Sender);
	void __fastcall SaveClick(System::TObject* Sender);
	void __fastcall ClearClick(System::TObject* Sender);
	void __fastcall CopyClick(System::TObject* Sender);
	void __fastcall PasteClick(System::TObject* Sender);
	void __fastcall TimerTimer(System::TObject* Sender);
	void __fastcall PopupMenuPopup(System::TObject* Sender);
	void __fastcall mnInvertClick(System::TObject* Sender);
	void __fastcall FormCreate(System::TObject* Sender);
	void __fastcall MagnComboChange(System::TObject* Sender);
	
protected:
	Gr32_image::TImage32* AlphaChannel;
	Gr32_image::TImage32* RGBChannels;
	void __fastcall AlphaChannelMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int 
		Y, Gr32_layers::TCustomLayer* Layer);
	void __fastcall RGBChannelsMouseMove(System::TObject* Sender, Classes::TShiftState Shift, int X, int 
		Y, Gr32_layers::TCustomLayer* Layer);
	Gr32_image::TImage32* __fastcall CurrentImage(void);
	
public:
	__fastcall virtual TPictureEditorForm(Classes::TComponent* AOwner);
public:
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TPictureEditorForm(Classes::TComponent* AOwner
		, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TPictureEditorForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TPictureEditorForm(HWND ParentWindow) : Forms::TForm(
		ParentWindow) { }
	#pragma option pop
	
};


class DELPHICLASS TBitmap32Editor;
class PASCALIMPLEMENTATION TBitmap32Editor : public Classes::TComponent 
{
	typedef Classes::TComponent inherited;
	
private:
	Gr32::TBitmap32* FBitmap32;
	TPictureEditorForm* FPicDlg;
	void __fastcall SetBitmap32(Gr32::TBitmap32* Value);
	
public:
	__fastcall virtual TBitmap32Editor(Classes::TComponent* AOwner);
	__fastcall virtual ~TBitmap32Editor(void);
	bool __fastcall Execute(void);
	__property Gr32::TBitmap32* Bitmap32 = {read=FBitmap32, write=SetBitmap32};
};


class DELPHICLASS TBitmap32Property;
class PASCALIMPLEMENTATION TBitmap32Property : public Dsgnintf::TClassProperty 
{
	typedef Dsgnintf::TClassProperty inherited;
	
public:
	virtual void __fastcall Edit(void);
	virtual Dsgnintf::TPropertyAttributes __fastcall GetAttributes(void);
	virtual AnsiString __fastcall GetValue();
	virtual void __fastcall SetValue(const AnsiString Value);
protected:
	#pragma option push -w-inl
	/* TPropertyEditor.Create */ inline __fastcall virtual TBitmap32Property(const Dsgnintf::_di_IFormDesigner 
		ADesigner, int APropCount) : Dsgnintf::TClassProperty(ADesigner, APropCount) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TPropertyEditor.Destroy */ inline __fastcall virtual ~TBitmap32Property(void) { }
	#pragma option pop
	
};


class DELPHICLASS TImage32Editor;
class PASCALIMPLEMENTATION TImage32Editor : public Dsgnintf::TComponentEditor 
{
	typedef Dsgnintf::TComponentEditor inherited;
	
public:
	virtual void __fastcall ExecuteVerb(int Index);
	virtual AnsiString __fastcall GetVerb(int Index);
	virtual int __fastcall GetVerbCount(void);
public:
	#pragma option push -w-inl
	/* TComponentEditor.Create */ inline __fastcall virtual TImage32Editor(Classes::TComponent* AComponent
		, Dsgnintf::_di_IFormDesigner ADesigner) : Dsgnintf::TComponentEditor(AComponent, ADesigner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TImage32Editor(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Gr32_dsgn_bitmap */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Gr32_dsgn_bitmap;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// GR32_Dsgn_Bitmap
