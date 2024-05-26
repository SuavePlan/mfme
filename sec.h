//
//
//	sec.h
//

#ifndef __secH__
#define __secH__

#define SEC_DATA 0x60
#define SEC_ACK  0x61
#define SEC_NAK  0x62

class SEC {

// Internal registers

private:

	Byte			 clk;
	Byte			 ch;
	Byte			 clks;
    Byte             data1;
    Byte             command[60];
    Byte             response[60];
    Byte             pos;
    Byte             rx_pos;
    Byte             rx_clk;
    Byte             rx_data;
    Byte             rx_length;
    Byte             bytes_left;
    Byte             last_id;
    Byte             status;
    Byte             market_type;
    Byte             last_error;
    Byte             nbr_of_counters;
    int              fingerprint;
//    struct counters {
//      int           value;
//      unsigned char text[7];
//    } counter[31];
    bool             enabled;
    void             Processcommand(void);
    void             Buildresponse(Byte code, Byte id, Byte len, Byte *data);
	void			 do_char(Byte ch);
// Initialisation functions

protected:

// Read and write functions
public:
    Byte             meter;
    int              value;
    bool             updated;
	void			 enable(Byte e);
	void			 reset(void);
    void             setclock(Byte x);
    void             setdata(Byte x);
    Byte             readdata(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);
// Public constructor and destructor

    SEC();
};

#endif // __secH__
