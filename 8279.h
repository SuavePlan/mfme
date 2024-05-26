//---------------------------------------------------------------------------
#ifndef 8279H
#define 8279H
//---------------------------------------------------------------------------
#endif

#undef int8
#undef int16
#undef int32
#undef uint
#undef uint8
#undef uint16
#undef uint

#define int8   signed char			/* ASG: changed from char to signed char */
#define uint8  unsigned char
#define int16  short
#define uint16 unsigned short
#define int32  long
#define uint   unsigned int

class I8279 {
public:
  void write(uint8 offset, uint8 value);
  void tick(int cycles);
  uint8 read(uint8 offset);
  void reset(void);
private:
  uint16 total;
};

