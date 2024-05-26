//
//
//	scn68681.cpp
//
//	(C) C.J.Wren 2000
//

#include "typedefs.h"
#include "scn68681b.h"

extern char buffer[];

char dp_buffer[256];
Byte pos = 0;

void scn68681b::reset()
{
  ivr = 0xf;
  counter = 0;
  chana = 0;
  chanb = 0;
  counter_running = false;
  imr = 0;
  isr = 0;
  acr = 0;
  opr = 0; // 0xff; ??
  mr1a = mr1b = 0;
  mr2a = mr2b = 0;
  csra = csrb = 0;
  sra = srb = 0;
  ip = 0x20;
  clk = 0;
  prescaler = 0;
  op = 0xff;
  txa = txb = 0;
  TxA = TxB = RxA = RxB = false;
  recva = recvb = false;
  buffposA = buffposB = 0;
//  toggle = false;
}

void scn68681b::tick(unsigned int num_clks)
{
int diff = 0;
bool check = true;

  clk = num_clks;

  if ( ( acr & 0x30 ) == 0x30 || ( acr & 0x60 ) == 0x60 )
  {
    while ( num_clks ) {
      if ( acr & 0x10 ) { // Do Prescaler
        prescaler++;
        if ( prescaler > 15 ) {
          prescaler = 0;
          counter--;
        }
      } else
        counter--;
      if ( !counter )
      {
//        if ( acr & 0x40 ) { // Timer Mode
//          toggle = !toggle;
//          if ( !toggle )
//            isr |= 0x8;
//        } else
          isr |= 0x8;
        counter = (ctur << 8) + ctlr;
        counter -= diff;
      }
      num_clks--;
    }
  }
  if ( txa ) {
    txa--;
    if ( !txa && (sra & TxRDY) ) {
      sra |= TxEMT; // TEmp
      recva = true;
//      isr |= RxRDYA;
//      lenA = buffposA;
//      buffposA = 0;
      if ( mr2a & 0x20 ) {
        TxA = false;
        rtsa = 0;
        isr &= ~TxRDYA;
        sra &= ~TxRDY;
      }
//      printf("Timed out\n"); // Timed out
    }
  } else {
    if ( (sra & TxRDY) == 0 && TxA ) {
      bufferA[buffposA++] = tba;
      if ( buffposA++ >= 60 )
        buffposA = 0;
      txa = 100;  //100
      sra |= TxRDY;
      isr |= TxRDYA;
    }
  }
  if ( txb ) {
    txb--;
    if ( !txb && (srb & TxRDY) ) {
      srb |= TxEMT; // TEmp
      recvb = true;
//      isr |= RxRDYB;
//      rbb = tbb;
      lenB = buffposB;
      buffposB = 0;
      if ( mr2b & 0x20 ) {
        TxB = false;
        rtsb = 0;
        isr &= ~TxRDYB;
        srb &= ~TxRDY;
      }
//      printf("Timed out\n"); // Timed out
    }
  } else {
    if ( (srb & TxRDY) == 0 && TxB ) {
      bufferB[buffposB++] = tbb;
      if ( buffposB++ >= 60 )
        buffposB = 0;
      txb = 100;
      srb |= TxRDY;
      isr |= TxRDYB;
    }
  }
}

void scn68681b::write(Byte offset, unsigned int value)
{
Byte old_op;

  op_changed = 0;
  
  switch ( offset ) {
    case 0:
      if ( !chana )
        mr1a = value;
      else
        mr2a = value;
      chana = 1;
      break;
    case 1:
      csra = value;
      break;
    case 2:
      cra = value;
      if ( value & 1 ) {
        RxA = true;
      }
      if ( value & 2 ) {
        RxA = false;
      }
      if ( value & 4 ) {
        TxA = true;
        isr |= TxRDYA;
        sra |= (TxRDY + TxEMT);
      }
      if ( value & 8 ) {
        TxA = false;
        isr &= ~TxRDYA;
        sra &= ~(TxRDY + TxEMT);
      }
      switch ( (value & 0x70) >> 4 ) {
        case 0:
          break;
        case 1:
          chana = 0;
          break;
        case 2:
          RxA = false;
          break;
        case 3:
          TxA = false;
          isr &= ~TxRDYA;
          sra &= ~(TxRDY + TxEMT);
          break;
        case 4:
          sra &= 0x0f; // Reset Error Status
          break;
        case 5:
          isr &= ~DBA;
          break;
        case 6:
          // Start Break;
          break;
        case 7:
          // Stop Break
          break;
      }
      break;
    case 3:
      tba = value;
      if ( (mr2a & 0x80) == 0x80 ) { // Local Loopback
        rba = tba;
        sra |= (RxRDY + FFULL);
        isr |= RxRDYA;
      } else {
        sra &= ~(TxRDY + TxEMT);
        isr &= ~TxRDYA;
//        sprintf(buffer, "uart char a %02x", tba);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//        printf("Char transmitted %d\n", thra);
//        dp_buffer[pos++] = thra;
      }
      break;
    case 4:
      acr = value;
      break;
    case 5:
      imr = value;
      break;
    case 6:
      ctur = value;
      break;
    case 7:
      ctlr = value;
      break;
    case 8:
      if ( !chanb )
        mr1b = value;
      else
        mr2b = value;
      chanb = 1;
      break;
    case 9:
      csrb = value;
      break;
    case 0x0a:
      crb = value;
      if ( value & 1 ) {
        RxB = true;
      }
      if ( value & 2 ) {
        RxB = false;
      }
      if ( value & 4 ) {
        TxB = true;
        isr |= TxRDYB;
        srb |= (TxRDY + TxEMT);
      }
      if ( value & 8 ) {
        TxB = false;
        isr &= ~TxRDYB;
        srb &= ~(TxRDY + TxEMT);
      }
      switch ( (value & 0x70) >> 4 ) {
        case 0:
          break;
        case 1:
          chanb = 0;
          break;
        case 2:
          RxB = false;
          break;
        case 3:
          TxB = false;
          isr &= ~TxRDYB;
          srb &= ~(TxRDY + TxEMT);
          break;
        case 4:
          srb &= 0x0f; // Reset Error Status
          break;
        case 5:
          isr &= ~DBB;
          break;
        case 6:
          // Start Break;
          break;
        case 7:
          // Stop Break
          break;
      }
      break;
    case 0x0b:
      tbb = value;
      if ( (mr2b & 0x80) == 0x80 ) { // Local Loopback
        rbb = tbb;
        srb |= (RxRDY + FFULL); // RxRDY FFULL
        isr |= RxRDYB;
      } else {
        srb &= ~(TxRDY + TxEMT);
        isr &= ~TxRDYB;
//        sprintf(buffer, "uart char b %02x", tbb);
//        DataWindow->AddMessage((AnsiString)buffer, 0xb0);
//        printf("Char transmitted %d\n", thra);
//        dp_buffer[pos++] = thrb;
      }
      break;
    case 0x0c:
      ivr = value;
      break;
    case 0x0d:
      opcr = value;
      break;
    case 0x0e:
      old_op = opr;
      opr = opr | value;
      op = ~opr;
      op_changed = old_op ^ opr;
      break;
    case 0x0f:
      old_op = opr;
      opr = opr & ~value;
      op = ~opr;
      op_changed = old_op ^ opr;
      break;
  }
}

unsigned int scn68681b::read(Byte addr)
{
unsigned int value = 0;

  switch ( addr ) {
    case 0:
      if ( !chana )
        value = mr1a;
      else
        value = mr2a;
      chana = 1;
      break;
    case 1:
      value = sra;
      break;
    case 2:
      value = 0;
      break;
    case 3:
      value = rba;
      isr &= ~RxRDYA;
      sra &= ~(FFULL + RxRDY);
      break;
    case 4:
      value = ipcr;
      isr &= ~IPC;
      break;
    case 5:
      value = isr;
      break;
    case 6:
      value = counter >> 8;
      break;
    case 7:
      value = counter & 0xff;
      break;
    case 8:
      if ( !chanb )
        value = mr1b;
      else
        value = mr2b;
      chanb = 1;
      break;
    case 9:
      value = srb;
      break;
    case 0x0a:
      value = 0;
      break;
    case 0x0b:
      value = rbb;
      isr &= ~RxRDYB;
      sra &= ~(FFULL + RxRDY);
//      sprintf(buffer, "uart char read");
//      DataWindow->AddMessage((AnsiString)buffer, 0xb0);
      break;
    case 0x0c:
      value = ivr;
      break;
    case 0x0d:
      value = ip;
      break;
    case 0x0e:
      counter_running = true;
      counter = (ctur << 8) + ctlr;
      break;
    case 0x0f:
      isr &= 0xf7;
      break;
  }

  return value;
}

void scn68681b::ReceiveCharA(Byte ch)
{
  isr |= RxRDYA;
  rba = ch;
}

void scn68681b::ReceiveCharB(Byte ch)
{
  isr |= RxRDYB;
  rbb = ch;
}

void __fastcall scn68681b::SaveToStream(Byte *&ptr)
{
  savestate(ptr,mr1a);
  savestate(ptr,mr1b);
  savestate(ptr,sra);
  savestate(ptr,srb);
  savestate(ptr,mr2a);
  savestate(ptr,mr2b);
  savestate(ptr,ipcr);
  savestate(ptr,opcr);
  savestate(ptr,csra);
  savestate(ptr,csrb);
  savestate(ptr,cra);
  savestate(ptr,crb);
  savestate(ptr,acr);
  savestate(ptr,opr);
  savestate(ptr,ctur);
  savestate(ptr,ctlr);
  savestate(ptr,prescaler);
  savestate(ptr,clk);
  savestate(ptr,RxA);
  savestate(ptr,RxB);
  savestate(ptr,TxA);
  savestate(ptr,TxB);
  savestate(ptr,isr);
  savestate(ptr,imr);
  savestate(ptr,ivr);
  savestate(ptr,ip);
  savestate(ptr,op);
  savestate(ptr,tba);
  savestate(ptr,tbb);
  savestate(ptr,rba);
  savestate(ptr,rbb);
  savestate(ptr,recva);
  savestate(ptr,recvb);
  savestate(ptr,chana);
  savestate(ptr,chanb);
  savestate(ptr,counter_running);
  savestate(ptr,transmitA);
  savestate(ptr,delayA);
  savestate(ptr,transmitB);
  savestate(ptr,delayB);
  savestate(ptr,rtsa);
  savestate(ptr,rtsb);
  savestate(ptr,txa);
  savestate(ptr,txb);
  savestate(ptr,txa);
  savestate(ptr,txa);
  savestate(ptr,txa);
  savestate(ptr,txa);
  savestate(ptr,txa);
}

void __fastcall scn68681b::LoadFromStream(Byte *&ptr)
{
  readstate(ptr,mr1a);
  readstate(ptr,mr1b);
  readstate(ptr,sra);
  readstate(ptr,srb);
  readstate(ptr,mr2a);
  readstate(ptr,mr2b);
  readstate(ptr,ipcr);
  readstate(ptr,opcr);
  readstate(ptr,csra);
  readstate(ptr,csrb);
  readstate(ptr,cra);
  readstate(ptr,crb);
  readstate(ptr,acr);
  readstate(ptr,opr);
  readstate(ptr,ctur);
  readstate(ptr,ctlr);
  readstate(ptr,prescaler);
  readstate(ptr,clk);
  readstate(ptr,RxA);
  readstate(ptr,RxB);
  readstate(ptr,TxA);
  readstate(ptr,TxB);
  readstate(ptr,isr);
  readstate(ptr,imr);
  readstate(ptr,ivr);
  readstate(ptr,ip);
  readstate(ptr,op);
  readstate(ptr,tba);
  readstate(ptr,tbb);
  readstate(ptr,rba);
  readstate(ptr,rbb);
  readstate(ptr,recva);
  readstate(ptr,recvb);
  readstate(ptr,chana);
  readstate(ptr,chanb);
  readstate(ptr,counter_running);
  readstate(ptr,transmitA);
  readstate(ptr,delayA);
  readstate(ptr,transmitB);
  readstate(ptr,delayB);
  readstate(ptr,rtsa);
  readstate(ptr,rtsb);
  readstate(ptr,txa);
  readstate(ptr,txb);
  readstate(ptr,txa);
  readstate(ptr,txa);
  readstate(ptr,txa);
  readstate(ptr,txa);
  readstate(ptr,txa);
}


