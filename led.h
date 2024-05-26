//
//
//	led.h
//

#ifndef __led__
#define __led__

class led {

private:

// Internal registers

protected:

// Initialisation functions

protected:
    Byte             old_latch;

// Read and write functions
public:
    Byte             time[40];
    Byte             segcount[64];
    Byte             digits[40];
    bool             updated[40];
//    Byte             changed;
    bool             write_data(Byte d, Byte position, Byte nbr);
    void             write_data_mpu3(Byte d, Byte position);
    void             write_data_bwb(Byte d, Byte position);
    void             write_data2(Byte nbr, Byte d, Byte position);
    void             write_latch(Byte d, Byte value, Byte pos);
    int              writebuf(Byte *ptr, int base, int mux);
    bool             writejpm(Byte d, Byte position);
    bool             writebfm(Byte d, Byte column, Byte bank);
    bool             writeca(Byte digits, Byte segment, Byte segnum, Byte nbr);
    bool             writebcd(Byte d, Byte position);
    bool             write_static(Byte d, Byte position);
    bool             update(void);
    void	         reset(void);
    void __fastcall  SaveToStream(Byte *&ptr);
    void __fastcall  LoadFromStream(Byte *&ptr);
// Public constructor and destructor

				 led();
				~led();

};

#endif // __led_h__
