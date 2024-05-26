// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RGBALoaderUnit.pas' rev: 5.00

#ifndef RGBALoaderUnitHPP
#define RGBALoaderUnitHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Buttons.hpp>	// Pascal unit
#include <ExtDlgs.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
#include <GR32_Filters.hpp>	// Pascal unit
#include <GR32_Image.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
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

namespace Rgbaloaderunit
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRGBALoaderForm;
class PASCALIMPLEMENTATION TRGBALoaderForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Extctrls::TPanel* Panel1;
	Extctrls::TBevel* Bevel1;
	Stdctrls::TLabel* Label1;
	Stdctrls::TLabel* Label2;
	Gr32_image::TImgView32* ImgRGB;
	Stdctrls::TButton* Button1;
	Stdctrls::TLabel* Label3;
	Gr32_image::TImgView32* ImgAlpha;
	Stdctrls::TButton* Button2;
	Stdctrls::TLabel* Label4;
	Stdctrls::TButton* Button3;
	Stdctrls::TButton* Button4;
	Extdlgs::TOpenPictureDialog* OpenPictureDialog;
	Buttons::TSpeedButton* SpeedButton1;
	Buttons::TSpeedButton* SpeedButton2;
	Buttons::TSpeedButton* SpeedButton3;
	Buttons::TSpeedButton* SpeedButton4;
	Stdctrls::TButton* Button5;
	void __fastcall Button1Click(System::TObject* Sender);
	void __fastcall Button2Click(System::TObject* Sender);
	void __fastcall SpeedButton1Click(System::TObject* Sender);
	void __fastcall SpeedButton2Click(System::TObject* Sender);
	void __fastcall SpeedButton3Click(System::TObject* Sender);
	void __fastcall SpeedButton4Click(System::TObject* Sender);
	void __fastcall Button5Click(System::TObject* Sender);
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TRGBALoaderForm(Classes::TComponent* AOwner) : Forms::TForm(
		AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TRGBALoaderForm(Classes::TComponent* AOwner, 
		int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TRGBALoaderForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TRGBALoaderForm(HWND ParentWindow) : Forms::TForm(
		ParentWindow) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRGBALoaderForm* RGBALoaderForm;

}	/* namespace Rgbaloaderunit */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Rgbaloaderunit;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RGBALoaderUnit
