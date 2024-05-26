//---------------------------------------------------------------------------
#ifndef I8279H
#define I8279H
//---------------------------------------------------------------------------

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

#define _8CH_LEFT_ENTRY

class I8279 {
public:
  uint8 scan;
  uint8 display_ram[16];
  uint8 sensor_ram[8];
  bool lamps_changed;
  uint8 changed;
  void write(uint8 offset, uint8 value);
  void tick(int cycles);
  uint8 read(uint8 offset, bool keys);
  void reset(void);
private:
  uint8 sense_auto_inc;
  uint8 display_auto_inc;
  uint8 display_size;
  bool leftentry;
  uint8 mode;
  uint8 clock;
  uint8 sense_address;
  uint8 display_address;
  bool read_sensor;
  bool write_display;
};
#endif

