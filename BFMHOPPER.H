//
//
//	bfmhopper.h
//
//	C.J.Wren 2003
//

#ifndef __bfmhopper__
#define __bfmhopper__

class bfmhopper {

// Internal registers

protected:
    System::Word timer1;
    System::Word timer2;
    System::Byte coin1;
    System::Byte coin2;
    System::Byte opto1;
    System::Byte opto2;
    System::Byte motor1;
    System::Byte motor2;
    System::Byte p03;
    System::Byte state1;
    System::Byte state2;
    bool SW24;
    bool TREN;

public:
    int coin_on_period;
    int coin_off_period;
    int start_delay;
    bool opto_drive;

// Initialisation functions

protected:
	void statechange(void);

// Read and write functions
public:
	void reset(void);
    void writeP03(System::Byte x);
    void writepayen1(System::Byte x);
    void writepayen2(System::Byte x);
    System::Byte readpay(void);
    void update(void);
    void SaveToStream(System::Byte *&ptr);
    void LoadFromStream(System::Byte *&ptr);

// Public constructor and destructor

    bfmhopper(void);

};

#endif // __hopper_hpp__
