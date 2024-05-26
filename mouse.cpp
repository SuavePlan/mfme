//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "c:\mssdk\include\dinput.h"
#include "interface.h"
#include "shared.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define SAMPLE_BUFFER_SIZE 16  // arbitrary number of buffer elements

LPDIRECTINPUT8       g_pDI    = NULL;
LPDIRECTINPUTDEVICE8 g_pMouse = NULL;

void __fastcall TForm1::OpenMouse(void)
{
    HRESULT hr;
    BOOL    bExclusive;
    BOOL    bForeground;
    BOOL    bImmediate;
    DWORD   dwCoopFlags;

    // Cleanup any previous call first
//    KillTimer( hwd, 0 );
    FreeDirectInput();

    // Detrimine where the buffer would like to be allocated
    bExclusive         = False;
    bForeground        = True;
    bImmediate         = False;

    if( bExclusive )
        dwCoopFlags = DISCL_EXCLUSIVE;
    else
        dwCoopFlags = DISCL_NONEXCLUSIVE;

    if( bForeground )
        dwCoopFlags |= DISCL_FOREGROUND;
    else
        dwCoopFlags |= DISCL_BACKGROUND;

    // Create a DInput object
    if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                                         IID_IDirectInput8, (VOID**)&g_pDI, NULL ) ) )
        exit(0);

    // Obtain an interface to the system mouse device.
    if( FAILED( hr = g_pDI->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) )
        exit(0);

    // Set the data format to "mouse format" - a predefined data format 
    //
    // A data format specifies which controls on a device we
    // are interested in, and how they should be reported.
    //
    // This tells DirectInput that we will be passing a
    // DIMOUSESTATE2 structure to IDirectInputDevice::GetDeviceState.
    if( FAILED( hr = g_pMouse->SetDataFormat( &c_dfDIMouse2 ) ) )
        return;

    // Set the cooperativity level to let DirectInput know how
    // this device should interact with the system and with other
    // DirectInput applications.
    hr = g_pMouse->SetCooperativeLevel( Handle, dwCoopFlags );
    if( hr == DIERR_UNSUPPORTED && !bForeground && bExclusive )
    {
        FreeDirectInput();
        return;
    }

    if( FAILED(hr) )
        return;

    if( !bImmediate )
    {
        // IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
        //
        // DirectInput uses unbuffered I/O (buffer size = 0) by default.
        // If you want to read buffered data, you need to set a nonzero
        // buffer size.
        //
        // Set the buffer size to SAMPLE_BUFFER_SIZE (defined above) elements.
        //
        // The buffer size is a DWORD property associated with the device.
        DIPROPDWORD dipdw;
        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = SAMPLE_BUFFER_SIZE; // Arbitary buffer size

        if( FAILED( hr = g_pMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ) )
            return;
    }

    // Acquire the newly created device
    g_pMouse->Acquire();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateMouse(void)
{
    char              strNewText[256*5 + 1] = TEXT("");
    char              strElement[10];
    DIDEVICEOBJECTDATA didod[ SAMPLE_BUFFER_SIZE ];  // Receives buffered data 
    DWORD              dwElements;
    DWORD              i;
    HRESULT            hr;

    if( NULL == g_pMouse ) 
        return;

    dwElements = SAMPLE_BUFFER_SIZE;
    hr = g_pMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
                                     didod, &dwElements, 0 );
    if( hr != DI_OK )
    {
        // We got an error or we got DI_BUFFEROVERFLOW.
        //
        // Either way, it means that continuous contact with the
        // device has been lost, either due to an external
        // interruption, or because the buffer overflowed
        // and some events were lost.
        //
        // Consequently, if a button was pressed at the time
        // the buffer overflowed or the connection was broken,
        // the corresponding "up" message might have been lost.
        //
        // But since our simple sample doesn't actually have
        // any state associated with button up or down events,
        // there is no state to reset.  (In a real game, ignoring
        // the buffer overflow would result in the game thinking
        // a key was held down when in fact it isn't; it's just
        // that the "up" event got lost because the buffer
        // overflowed.)
        //
        // If we want to be cleverer, we could do a
        // GetDeviceState() and compare the current state
        // against the state we think the device is in,
        // and process all the states that are currently
        // different from our private state.
        hr = g_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pMouse->Acquire();

        // Update the dialog text
        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
            ;//SetDlgItemText( Handle, IDC_DATA, "Unacquired" );

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of
        // switching, so just try again later
        return;
    }

    if( FAILED(hr) )  
        return;

    // Study each of the buffer elements and process them.
    //
    // Since we really don't do anything, our "processing"
    // consists merely of squirting the name into our
    // local buffer.
    for( i = 0; i < dwElements; i++ ) 
    {
        // this will display then scan code of the key
        // plus a 'D' - meaning the key was pressed 
        //   or a 'U' - meaning the key was released
        switch( didod[ i ].dwOfs )
        {
            case DIMOFS_BUTTON0:
                strcpy( strElement, "B0" );
                break;

            case DIMOFS_BUTTON1:
                strcpy( strElement, "B1" );
                break;

            case DIMOFS_BUTTON2:
                strcpy( strElement, "B2" );
                break;

            case DIMOFS_BUTTON3:
                strcpy( strElement, "B3" );
                break;

            case DIMOFS_X:
                strcpy( strElement, "X" );
                break;

            case DIMOFS_Y:
                strcpy( strElement, "Y" );
                break;

            case DIMOFS_Z:
                strcpy( strElement, "Z" );
                break;

            default:
                strcpy( strElement, "" );
        }

        switch( didod[ i ].dwOfs )
        {
            case DIMOFS_BUTTON0:
            case DIMOFS_BUTTON1:
            case DIMOFS_BUTTON2:
            case DIMOFS_BUTTON3:
                if( didod[ i ].dwData & 0x80 )
                    strcat( strElement, "U " );
                else
                    strcat( strElement, "D " );
                break;

            case DIMOFS_X:
            case DIMOFS_Y:
            case DIMOFS_Z:
            {
                TCHAR strCoordValue[255];
                wsprintf( strCoordValue, "%d ", didod[ i ].dwData );
                strcat( strElement, strCoordValue );
                break;
            }
        }

        strcat( strNewText, strElement );
    }

    // Get the old text in the text box
    TCHAR *strOldText;
//    strOldText = Edit1->Text.c_str();

    // If nothing changed then don't repaint - avoid flicker
    if( 0 != lstrcmp( strOldText, strNewText ) )
        ;//Edit1->Text = strNewText;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FreeDirectInput()
{
    // Unacquire the device one last time just in case
    // the app tried to exit while the device is still acquired.
    if( g_pMouse ) 
        g_pMouse->Unacquire();
    
    // Release any DirectInput objects.
    SAFE_RELEASE( g_pMouse );
    SAFE_RELEASE( g_pDI );
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateMouse2(void)
{
    HRESULT       hr;
    TCHAR         strNewText[256] = TEXT("");
    DIMOUSESTATE2 dims2;      // DirectInput mouse state structure

    if( NULL == g_pMouse ) 
        return ;
    
    // Get the input's device state, and put the state in dims
    ZeroMemory( &dims2, sizeof(dims2) );
    hr = g_pMouse->GetDeviceState( sizeof(DIMOUSESTATE2), &dims2 );
    if( FAILED(hr) ) 
    {
        // DirectInput may be telling us that the input stream has been
        // interrupted.  We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done.
        // We just re-acquire and try again.
        
        // If input is lost then acquire and keep trying 
        hr = g_pMouse->Acquire();
        while( hr == DIERR_INPUTLOST ) 
            hr = g_pMouse->Acquire();

        // Update the dialog text 
        if( hr == DIERR_OTHERAPPHASPRIO || 
            hr == DIERR_NOTACQUIRED ) 
            ;//SetDlgItemText( hDlg, IDC_DATA, "Unacquired" );

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return;
    }

    // The dims structure now has the state of the mouse, so 
    // display mouse coordinates (x, y, z) and buttons.
    CursorX += dims2.lX;
    CursorY += dims2.lY;
/*    sprintf( strNewText, "(X=% 3.3d, Y=% 3.3d, Z=% 3.3d) B0=%c B1=%c B2=%c B3=%c B4=%c B5=%c B6=%c B7=%c",
                         dims2.lX, dims2.lY, dims2.lZ,
                        (dims2.rgbButtons[0] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[1] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[2] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[3] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[4] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[5] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[6] & 0x80) ? '1' : '0',
                        (dims2.rgbButtons[7] & 0x80) ? '1' : '0');

    // Get the old text in the text box
    TCHAR strOldText[128];
//    GetDlgItemText( hDlg, IDC_DATA, strOldText, 127 );
    
    // If nothing changed then don't repaint - avoid flicker
    if( 0 != lstrcmp( strOldText, strNewText ) ) 
        ;//SetDlgItemText( hDlg, IDC_DATA, strNewText );  */
    
    return;
}
//---------------------------------------------------------------------------

