//
//
//	mc6821.cpp
//
//	(C) C.J.Wren 2000
//

#include "typedefs.h"
#include "mc6821.h"

mc6821::mc6821()
{
	reset();
}

mc6821::~mc6821()
{
}

void mc6821::reset()
{
	cra = 0;		// Clear Control Reg A
	crb = 0;		// Clear Control Reg B
	Dda = 0;		// Port A set to input
	Ddb = 0;		// Port B set to input
	ora = 0;		// Clear output reg A
	orb = 0;		// Clear output reg B
	ca1 = ca2 = cb1 = cb2 = 0;
	ca2_changed = cb2_changed = false;
	ca2_strobed = cb2_strobed = false;
	ora_changed = orb_changed = false;
	ora_in = 0;
	orb_in = 0;
    ca2_in = 1;
    cb2_in = 1;
    ca1_transition = ca2_transition = cb1_transition = cb2_transition = 0;
}

Byte mc6821::read(Word offset)
{
	Byte ret;
    ca2_strobed = false;

	switch ( offset ) {
   	  case 0:
		if ( cra & 4 ) {
		  ret = ( ora & Dda ) | ora_in;
          cra &= 0x3f;
          if ( (cra & 0x38) == 0x28 )
            ca2_strobed = true;
        } else
          ret = Dda;
		break;
	  case 1:
		ret = cra;
		break;
	  case 2:
		if ( crb & 4 ) {
          ret = ( orb & Ddb ) | ( orb_in & ~Ddb);
          crb &= 0x3f;
		} else
          ret = Ddb;
		break;
	  case 3:
		ret = crb;
		break;
	}
	return ret;
}

void mc6821::set_ca1(Byte val)
{
  val = val ? 1 : 0;

  if ( val != ca1 ) {
    ca1 = val;
    if ( (cra & 2) && ca1 )
      ca1_transition = true;
    if ( !(cra & 2) && !ca1 )
      ca1_transition = true;
    if ( ca1_transition ) {
      ca1_transition = false;
      cra |= 0x80;
    }
  }

}

void mc6821::set_ca2(Byte val)
{
  val = val ? 1 : 0;
  ca2_in = val;
  if ( val != ca2 ) {
    ca2 = val;
    if ( (cra & 0x10) && ca2 )
      ca2_transition = true;
    if ( !(cra & 0x10) && !ca2 )
      ca2_transition = true;
    if ( ca2_transition ) {
      ca2_transition = false;
      cra |= 0x40;
    }
  }

}

void mc6821::set_cb1(Byte val)
{
  val = val ? 1 : 0;
  if ( val != cb1 ) {
    cb1 = val;
    if ( (crb & 2) && cb1 )
      cb1_transition = true;
    if ( !(crb & 2) && !cb1 )
      cb1_transition = true;
    if ( cb1_transition ) {
      cb1_transition = false;
      crb |= 0x80;
    }
  }
  
}

void mc6821::set_cb2(Byte val)
{
  val = val ? 1 : 0;
  cb2_in = val;
  if ( val != cb2 ) {
    cb2 = val;
    if ( (crb & 0x10) && cb2 )
      cb2_transition = true;
    if ( !(crb & 0x10) && !cb2 )
      cb2_transition = true;
    if ( cb2_transition ) {
      cb2_transition = false;
      crb |= 0x40;
    }
  }
}


void mc6821::write(Word offset, Byte val)
{
	ora_changed = orb_changed = 0;
    ca2_changed = cb2_changed = false;
    ca2_updated = cb2_updated = ora_updated = orb_updated = false;
    cb2_strobed = false;

	switch ( offset ) {
	case 0:
      if ( cra & 4 ) {
        ora_updated = true;
        ora_changed = ora;
        ora = val;
        ora_changed ^= ora;
      } else
        Dda = val;
      break;
	case 1:
      cra = ( cra & 0xc0 ) | (val & 0x3f);
//      if ( (cra & 1) || (( cra & 0x38) == 0x8) )
//        printf("Interrupt Enabled on ca1 or ca2");
      if ( cra & 0x20 ) {  // Output
        if ( cra & 0x10 ) {
          ca2_updated = true;
          if ( cra & 0x08 ) {
            if ( !ca2 )
              ca2_changed = true;
            ca2 = 1;
          } else {
            if ( ca2 )
              ca2_changed = true;
            ca2 = 0;
          }
        } else
          ;
      } else {      // Input
        if ( ca2 ^ ca2_in ) {
          ca2_updated = true;
          ca2_changed = true;
        }
        ca2 = ca2_in;
      }
      break;
	case 2:
      if ( crb & 4 ) {
        orb_updated = true;
        orb_changed = orb;
        orb = val;
        orb_changed ^= orb;
        if ( (crb & 0x38) == 0x28 )
          cb2_strobed = true;
      } else
        Ddb = val;
      break;
	case 3:
//      if ( (( val & 1 ) == 0 ) )
//        printf("Interrupt on cb1 dis-abled");
      crb = (crb & 0xc0 ) | (val & 0x3f);
//      if ( (crb & 1) || (( crb & 0x28) == 0x8) )
//        printf("Interrupt Enabled on cb1 or cb2");
      if ( crb & 0x20 ) {  // Output
        if ( crb & 0x10 ) {
          cb2_updated = true;
          if ( crb & 0x08 ) {
            if ( !cb2 )
              cb2_changed = true;
            cb2 = 1;
          } else {
            if ( cb2 )
              cb2_changed = true;
            cb2 = 0;
          }
        } else
          ;
      } else {     // Input
        if ( cb2 ^ cb2_in ) {
          cb2_updated = true;
          cb2_changed = true;
        }
        cb2 = cb2_in;
      }
      break;
	}
}

void __fastcall mc6821::SaveToStream(Byte *&ptr)
{
  savestate(ptr,cra);
  savestate(ptr,crb);
  savestate(ptr,Dda);
  savestate(ptr,Ddb);
  savestate(ptr,ora);
  savestate(ptr,orb);
  savestate(ptr,ca1);
  savestate(ptr,ca2);
  savestate(ptr,cb1);
  savestate(ptr,cb2);
  savestate(ptr,ora_in);
  savestate(ptr,orb_in);
  savestate(ptr,ca1_transition);
  savestate(ptr,cb1_transition);
  savestate(ptr,ca2_transition);
  savestate(ptr,cb2_transition);
}

void __fastcall mc6821::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,cra);
  readstate(ptr,crb);
  readstate(ptr,Dda);
  readstate(ptr,Ddb);
  readstate(ptr,ora);
  readstate(ptr,orb);
  readstate(ptr,ca1);
  readstate(ptr,ca2);
  readstate(ptr,cb1);
  readstate(ptr,cb2);
  readstate(ptr,ora_in);
  readstate(ptr,orb_in);
  readstate(ptr,ca1_transition);
  readstate(ptr,cb1_transition);
  readstate(ptr,ca2_transition);
  readstate(ptr,cb2_transition);
}

