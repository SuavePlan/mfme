
//#pragma hdrstop
//#include <condefs.h>

#include <stdio.h>
#include "typedefs.h"
#include "machdep.h"
#include "mc6809.h"
#include "init.h"


char	reg[4] = { 'X','Y','U','S' };
char	*reg2[8] = { "CCR","A","B","DPR","X","Y","S/U","PC" };
char	*src[16] = { "D","X","Y","U","S","PC","?","?",
                     "A","B","CCR","DPR","?","?","?","?" };

//---------------------------------------------------------------------------
#pragma argsused
int mc6809::dasm(int addr, int page, char *p)
{
int count,mask,list,postbyte,register_field,indirect,extended;
struct op_code opcode;
unsigned int	address;

  extended = 0;
  opcode = opcodes[read(addr)];

  p += sprintf(p, "%02d:%05X     %02X", page, addr, opcode.value);

  if ( opcode.value == 0x10 || opcode.value == 0x11 )
    extended = 1;
    
  if ( opcode.value == 0x10 ) {
    opcode = opcodes2[read(addr+1)];
    p += sprintf(p,"%02X",read(++addr));
  }
  if ( opcode.value == 0x11 ) {
    opcode = opcodes3[read(addr+1)];
    p+= sprintf(p,"%02X",read(++addr));
  }

  if ( opcode.mode == 3 ) { /* Indexed  */
    postbyte = read(addr+1);
    register_field = ( postbyte & 96 ) >> 5;
    indirect = ( postbyte & 16 ) >> 4;
    if ( ( postbyte & 139 ) == 136  || ( postbyte & 139 ) == 137)
      opcode.length += 1 + ( postbyte & 1 );
  }
  for ( count = 1; count < 5 - extended; count++)
    if ( count < opcode.length - extended )
      p+= sprintf(p,"%02X", read(addr + count));
    else
      p+= sprintf(p,"  ");
  p+= sprintf(p,"%-6s",opcode.opcode);

  switch ( opcode.mode ) {
    case 1:	     /* Immediate 8 */
      p+= sprintf(p,"#%02X",read(addr+1));
      break;
    case 2: /* Relative 8 */
      if ( read(addr+1) < 128 )
        address = addr+2+read(addr+1);
      else
        address = addr-254+read(addr+1);
      p += sprintf(p,"$%04X", address);
      break;
    case 3:  /* Indexed  */
      if ( (postbyte & 128) == 0 )
        p += sprintf(p,"%02X,%c",postbyte & 31,reg[register_field]);
      else {
        if ( indirect )
          p += sprintf(p,"[");
        switch ( postbyte & 15 ) {
          case 0:
            p += sprintf(p,",%c+",reg[register_field]);
            break;
          case 1:
            p += sprintf(p,",%c++",reg[register_field]);
            break;
          case 2:
            p += sprintf(p,",-%c",reg[register_field]);
            break;
          case 3:
            p += sprintf(p,",--%c",reg[register_field]);
            break;
          case 4:
            p += sprintf(p,",%c",reg[register_field]);
            break;
          case 5:
            p += sprintf(p,"B,%c",reg[register_field]);
            break;
          case 6:
            p += sprintf(p,"A,%c",reg[register_field]);
            break;
          case 11:
            p += sprintf(p,"D,%c",reg[register_field]);
            break;
          case 8:
          case 9:
          case 12:
          case 13:
            if ( postbyte & 1 )
              p += sprintf(p,"%02X%02X,",read(addr+2),read(addr+3));
            else
              p += sprintf(p,"%02X,",read(addr+2));
            if ( postbyte & 4 )
              p += sprintf(p,"PCR");
            else
              p += sprintf(p,"%c",reg[register_field]);
            break;
          case 15:
            p+= sprintf(p,"%02X%02X",read(addr+2),read(addr+3));
            opcode.length += 2;
            break;
        }
        if ( indirect )
          p+= sprintf(p,"]");
      }
      break;
    case 4: /* Extended  */
      address = read(addr+2) + 256 * read(addr+1);
      p+= sprintf(p,"$%04X", address);
      break;
    case 5: /* Inherent */
      ;
      break;
    case 6: /* Immediate 16 */
      address = read(addr+2) + 256 * read(addr+1);
      p += sprintf(p,"#$%04X", address);
      break;
    case 7: /* Direct */
      p += sprintf(p,"P+$%02X",read(addr+1));
      break;
    case 8:  /* Relative 16 */
      address = read(addr+2)+256*read(addr+1);
      if ( address > 32768 )
        address = ( addr + address + 3) & 0xffff; // ***** ????? *****
      else
        address = ( addr + address + 3) & 0xffff;
      p += sprintf(p,"$%04X", address);
      break;
    case 9: /* Push/Pull */
      mask = 1;
      list = 0;
      for ( count = 0; count < 8; count++ ) {
        if ( read(addr+1) & mask ) {
          if ( list )
            p += sprintf(p,",%s",reg2[count]);
          else
            p += sprintf(p,"%s",reg2[count]);
          list = 1;
        }
        mask*=2;
      }
      break;
    case 10: /* TFR/EXG */
      p += sprintf(p,"%s,%s", src[read(addr+1) >> 4],
                      src[read(addr+1) & 15]);
      break;
  }
  return opcode.length;
}

