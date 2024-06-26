// Borland C++ Builder
// Copyright (c) 1995, 1999 by Borland International
// All rights reserved

// (DO NOT EDIT: machine generated header) 'MainUnit.pas' rev: 5.00

#ifndef MainUnitHPP
#define MainUnitHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <GR32_RangeBars.hpp>	// Pascal unit
#include <GR32_Image.hpp>	// Pascal unit
#include <GR32.hpp>	// Pascal unit
#include <ExtCtrls.hpp>	// Pascal unit
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

namespace Mainunit
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TForm1;
class PASCALIMPLEMENTATION TForm1 : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
__published:
	Extctrls::TPanel* Panel1;
	Extctrls::TPanel* Panel2;
	Gr32_image::TImage32* Image;
	Extctrls::TRadioGroup* rgScaleMode;
	Extctrls::TRadioGroup* rgStretchFilter;
	Extctrls::TRadioGroup* rgBitmapAlign;
	Stdctrls::TStaticText* StaticText1;
	Gr32_rangebars::TGaugeBar* sbScale;
	void __fastcall rgBitmapAlignClick(System::TObject* Sender);
	void __fastcall sbScaleChange(System::TObject* Sender);
	void __fastcall rgScaleModeClick(System::TObject* Sender);
	void __fastcall rgStretchFilterClick(System::TObject* Sender);
	
public:
	float Time;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TForm1(Classes::TComponent* AOwner) : Forms::TForm(
		AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TForm1(Classes::TComponent* AOwner, int Dummy
		) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TForm1(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TForm1(HWND ParentWindow) : Forms::TForm(ParentWindow
		) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TForm1* Form1;

}	/* namespace Mainunit */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace Mainunit;
#endif
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// MainUnit
