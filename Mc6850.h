//
//
//	mc6850.h
//
//	(C) C.J.Wren 2004
//

#ifndef __mc6850_h__
#define __mc6850_h__

#define ACIA_IRQ  0x80
#define ACIA_PE   0x40
#define ACIA_OVRN 0x20
#define ACIA_FE   0x10
#define ACIA_CTS  0x08
#define ACIA_DCD  0x04
#define ACIA_TDRE 0x02
#define ACIA_RDRF 0x01

class mc6850 {

// Internal registers

public:
	Byte			 tdr, rdr;
	Byte			 control, status, status2;
    Byte             word_sel;
    Byte             cnt_divide;
    Byte             tx_control;
    Byte             rx_int;
    bool             tx_empty;
    int              timer;
    int              timer2;
    Byte             rts;
    Byte             rts_changed;
    bool             status_read;
    bool             tx_char;
    bool             rx_char;
    int              tx_speed;
// Initialisation functions

protected:
    void             check_irq();

// Read and write functions
public:
    void             reset();
	Byte			 read(int rs);
	void			 write(int rs, Byte data);
    void             tick(int cycles);
    void             set_cts(Byte value);
    void             set_dcd(Byte value);
    void             receive_char(Byte value);
    void             char_received();

// Public constructor and destructor

    mc6850();

};

#endif // __mc6850_h__
