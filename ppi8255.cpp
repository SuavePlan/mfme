//
//
//	ppi8255.cpp
//
//	(C) C.J.Wren 2000
//

#include "typedefs.h"
#include "ppi8255.h"

PPI8255::PPI8255()
{
  reset();
}

PPI8255::~PPI8255()
{
}

void PPI8255::reset()
{
  groupA_mode = 0;
  groupB_mode = 0;
  for ( int i = 0; i < 3; i++ ) {
    io[i] = 0xff;
    latch[i] = 0;
  }
  porta = portb = portc = 0;
}

Byte PPI8255::read(Word offset)
{
   Byte result=0;

   if (offset>3)
   {
      return 0xff;
   }
   switch (offset)
   {
      case 0: /* Port A read */
         if ( !io[0] )
           result = latch[0];
         else
           result = porta_in;
         break;
      case 1: /* Port B read */
         if ( !io[1] )
           result = latch[1];
         else
           result = portb_in;
         break;
      case 2: /* Port C read */
         result = portc_in;
         result = ( latch[2] & ~io[2] ) | ( result & io[2]);
         break;
      case 3: /* Control Word */
         result=0xff;
         break;
   }
   return result;
}

void PPI8255::writea(void)
{
Byte temp;

  temp = porta;
  porta = (latch[0] & ~io[0]) | ( 0xff & io[0]);
  porta_changed = temp ^ porta;
  porta_updated = true;
}
void PPI8255::writeb(void)
{
Byte temp;

  temp = portb;
  portb = (latch[1] & ~io[1]) | ( 0xff & io[1]);
  portb_changed = temp ^ portb;
  portb_updated = true;
}
void PPI8255::writec(void)
{
Byte temp;

  temp = portc;
  portc = (latch[2] & ~io[2]) | ( 0xff & io[2]);
  portc_changed = temp ^ portc;
  portc_updated = true;
}

void PPI8255::write(Word offset, Byte data)
{
   if (offset>3)
   {
      return;
   }
   porta_changed = portb_changed = portc_changed = 0;
   porta_updated = portb_updated = portc_updated = false;
   switch( offset )
   {
      case 0: /* Port A write */
         latch[0]=data;
         writea();
         break;
      case 1: /* Port B write */
         latch[1]=data;
         writeb();
         break;
      case 2: /* Port C write */
         latch[2]=data;
         writec();
         break;
      case 3: /* Control Word */
         if (data & 0x80)
         {
           groupA_mode=(data>>5) & 3;
           groupB_mode=(data>>2) & 1;
           if ( groupA_mode || groupB_mode )
           {
             return;
           }
           if ( data & 0x10 )
             io[0] = 0xff;
           else
             io[0] = 0x00;

			/* Port B direction */
           if ( data & 0x02 )
             io[1] = 0xff;
           else
             io[1] = 0x00;

			/* Port C upper direction */
           if ( data & 0x08 )
             io[2] |= 0xf0;
           else
             io[2] &= 0x0f;

			/* Port C lower direction */
		   if ( data & 0x01 )
             io[2] |= 0x0f;
           else
             io[2] &= 0xf0;
           writea();
           writeb();
           writec();

           latch[0]=latch[1]=latch[2]=0;
           writea();
           writeb();
           writec();
         } else
         {
            /* bit set/reset */
            int bit;

            bit=(data>>1)&0x07;
            if (data&1)
               latch[2] |= (1<<bit);    /* set bit */
            else
               latch[2] &= ~(1<<bit);  /* reset bit */
            writec();
         }
   }

}

