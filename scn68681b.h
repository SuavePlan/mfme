//
//
//	scn68681b.h
//
//	C.J.Wren 2000
//

#ifndef __scn68681b__
#define __scn68681b__

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

// IMR/ISR Register

#define TxRDYA 0x1
#define RxRDYA 0x2
#define DBA    0x4
#define CNTRDY 0x8
#define TxRDYB 0x10
#define RxRDYB 0x20
#define DBB    0x40
#define IPC    0x80

// SR Register

#define RxRDY  0x1
#define FFULL  0x2
#define TxRDY  0x4
#define TxEMT  0x8
#define OVR    0x10
#define PE     0x20
#define FE     0x40
#define RB     0x40

class scn68681b {

// Internal registers

private:
    Byte mr1a, mr1b;
    Byte sra, srb;
    Byte mr2a, mr2b;
    Byte ipcr, opcr;
    Byte csra, csrb;
    Byte cra, crb;
    Byte opr;
    int prescaler;
    bool toggle;
    int clk;
    bool RxA, RxB, TxA, TxB;

public:
    Byte acr;
    Byte ctur, ctlr;
    Byte isr, imr, ivr;
    Byte ip, op;
    uint8 tba, tbb;
    uint8 rba, rbb;
    Byte op_changed;
    Word counter;
    uint8 bufferA[60], lenA;
    uint8 send_buffA[60];
    uint8 bufferB[60], lenB;
    uint8 send_buffB[60];
    bool recva, recvb;
// Initialisation functions

protected:
    Byte chana, chanb;
    bool counter_running;
    bool transmitA, delayA;
    bool transmitB, delayB;
    uint8 rtsa, rtsb;
    uint8 txa, txb;
    uint8 buffposA, send_countA, send_posA, send_delayA;
    uint8 buffposB, send_countB, send_posB, send_delayB;
    uint8 fifoA[40], fifo_countA;
    uint8 fifoB[40], fifo_countB;

// Read and write functions
public:
    void ReceiveCharA(Byte ch);
    void ReceiveCharB(Byte ch);
	void reset(void);
    void tick(unsigned int num_clks);
    void write(Byte offset, unsigned int value);
    unsigned int read(Byte addr);
    void __fastcall SaveToStream(Byte *&ptr);
    void __fastcall LoadFromStream(Byte *&ptr);
};

#endif // __scn68681b_h__
