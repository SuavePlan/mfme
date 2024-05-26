//---------------------------------------------------------------------------
#include <vcl\vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("interface.cpp", Form1);
USERES("MFME.res");
USEUNIT("Alpha.cpp");
USEUNIT("Lamp.cpp");
USEUNIT("Ls138.cpp");
USEUNIT("Mc6809.cpp");
USEUNIT("Mc6809in.cpp");
USEUNIT("Mc6821.cpp");
USEUNIT("Mc6840.cpp");
USEUNIT("Meter.cpp");
USEUNIT("Reels.cpp");
USEUNIT("Triac.cpp");
USEUNIT("mpu4.cpp");
USEFORM("property.cpp", PropertiesForm);
USEUNIT("dis6809.cpp");
USEUNIT("led.cpp");
USEUNIT("rs232.cpp");
USEFORM("reeleditor.cpp", Editor);
USEUNIT("m6376.cpp");
USEUNIT("scn68681.cpp");
USEFORM("credits.cpp", Credit);
USEUNIT("hopper.cpp");
USEFORM("configuration.cpp", Config);
USEUNIT("mpu3.cpp");
USEUNIT("Mc6800.cpp");
USEUNIT("various.cpp");
USEUNIT("system.cpp");
USEUNIT("6800dasm.cpp");
USEFORM("iomonitor.cpp", IOMonitor);
USEFORM("chredit.cpp", ChrEditor);
USEUNIT("sample.cpp");
USELIB("bcb\ecl\Lib\C++ Builder 5\ECLPackb5.lib");
USEFORM("samples.cpp", Sampler);
USEUNIT("jpmhopper.cpp");
USEUNIT("JPMReels.cpp");
USEUNIT("mc68000\m68kops.cpp");
USEUNIT("mc68000\m68000.cpp");
USEUNIT("mc68000\d68k.cpp");
USEUNIT("mc68000\m68kcpu.cpp");
USEUNIT("mc68000\m68kopac.cpp");
USEUNIT("mc68000\m68kopdm.cpp");
USEUNIT("mc68000\m68kopnz.cpp");
USEUNIT("impact.cpp");
USEUNIT("scn68681b.cpp");
USEFORM("Debug.cpp", DebugForm);
USEFORM("snap.cpp", SnapForm);
USEUNIT("ym2413.cpp");
USEUNIT("Components\Alphadisplay.cpp");
USEUNIT("Components\Reelunit.cpp");
USEFORM("band.cpp", LoadBandForm);
USEUNIT("m1a.cpp");
USEUNIT("I8279.cpp");
USEUNIT("ay8910.cpp");
USEUNIT("m1a6376.cpp");
USEUNIT("CanvasPanel.cpp");
USEUNIT("TMS\tms99xx.cpp");
USEUNIT("ppi8255.cpp");
USEUNIT("TMS\tms9902.cpp");
USEUNIT("sn76496.cpp");
USEUNIT("COMPONENTS\DiscReel.cpp");
USEFORM("createband.cpp", CreateBandForm);
USEUNIT("COMPONENTS\FancyReel.cpp");
USEUNIT("Z80\z80.cpp");
USEUNIT("ace1.cpp");
USEUNIT("scorpion2.cpp");
USEUNIT("dotmatrix.cpp");
USEUNIT("Components\matrixdisplay.cpp");
USEUNIT("BFMAlpha.cpp");
USEUNIT("sys80.cpp");
USEUNIT("bfmhopper.cpp");
USEUNIT("Components\BandReel.cpp");
USEUNIT("shared.cpp");
USEUNIT("Z80\z80support.cpp");
USEUNIT("sys5.cpp");
USEUNIT("saa1099.cpp");
USEUNIT("Mc6850.cpp");
USEUNIT("mpu4video.cpp");
USEUNIT("Components\video.cpp");
USEUNIT("scn2674.cpp");
USEUNIT("mouse.cpp");
USELIB("C:\mssdk\lib\Borland\dsound.lib");
USELIB("C:\mssdk\lib\Borland\dxguid.lib");
USELIB("C:\mssdk\lib\Borland\c_dinput.lib");
USELIB("C:\mssdk\lib\Borland\dinput8.lib");
USEUNIT("mps2.cpp");
USEUNIT("scorpion1.cpp");
USEFORM("scan.cpp", ScanWindow);
USEFORM("notes.cpp", LayoutNotes);
USEFORM("prefs.cpp", GeneralConfig);
USEFORM("managernotes.cpp", GameNotes);
USEUNIT("sec.cpp");
USEFORM("secmeter.cpp", SECForm);
USEFORM("comps.cpp", Form3);
USEUNIT("bwbchr.cpp");
USEUNIT("proconn.cpp");
USEUNIT("space.cpp");
USEUNIT("COMPONENTS\acedisplay.cpp");
USEUNIT("COMPONENTS\procondisplay.cpp");
USEUNIT("COMPONENTS\newalpha.cpp");
USERES("bfmalpha.res");
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    HANDLE mHandle = CreateMutex(NULL,true,"MFME");
    if ( GetLastError() == ERROR_ALREADY_EXISTS )
      return 0;

	try
	{
		Application->Initialize();
        Application->Title = "MFME";
         Application->CreateForm(__classid(TForm1), &Form1);
         Application->CreateForm(__classid(TPropertiesForm), &PropertiesForm);
         Application->CreateForm(__classid(TConfig), &Config);
         Application->CreateForm(__classid(TIOMonitor), &IOMonitor);
         Application->CreateForm(__classid(TSnapForm), &SnapForm);
         Application->CreateForm(__classid(TLoadBandForm), &LoadBandForm);
         Application->CreateForm(__classid(TCreateBandForm), &CreateBandForm);
         Application->CreateForm(__classid(TScanWindow), &ScanWindow);
         Application->CreateForm(__classid(TLayoutNotes), &LayoutNotes);
         Application->CreateForm(__classid(TGeneralConfig), &GeneralConfig);
         Application->CreateForm(__classid(TGameNotes), &GameNotes);
         Application->CreateForm(__classid(TCredit), &Credit);
         Application->CreateForm(__classid(TSECForm), &SECForm);
         Application->CreateForm(__classid(TForm3), &Form3);
         Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
    if ( mHandle )
      CloseHandle(mHandle);
	return 0;
}
//---------------------------------------------------------------------------



