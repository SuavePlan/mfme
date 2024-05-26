//---------------------------------------------------------------------------
#ifndef TMS9902H
#define TMS9902H
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


class TMS9902 {
public:
  void write(int offset, int value);
  void tick(int cycles);
  int read(int offset);
  void reset(int Anbr);
  void TxReply(char *ptr);
  uint8 pending;
  uint8 RTS;
  uint8 CTS;
  uint8 DSR;
  int prescaler;
  uint8 timer;
  bool RxChar;
  uint8 tx_register;
private:
  int nbr;
  int control;
  uint8 parameter;
  uint16 tx_rate;
  uint16 rx_rate;
  uint8 divider;
  int tx_speed, rx_speed;
  uint8 tx_char;
  uint8 timer_interval;
  uint16 value;
  uint8 test_mode;
  uint8 tx_break;
  uint8 mask;
  int event;
  uint8 RxD;
  int clk;
  uint8 receive_buffer;
  int tx_delay, rx_delay;
  uint8 rx_buffer[40];
  uint8 *rx_ptr;
  uint8 rx_count;
  void rx_char(uint8 chr);
};
#endif

