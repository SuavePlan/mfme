//
//
//	hopper.h
//
//	C.J.Wren 2000
//

#ifndef __hopper__
#define __hopper__

class hopper {

// Internal registers

protected:
    Word timer;
    Byte coin;
    Byte coin2;
    Byte motor;
    Byte state;

public:
    int coin_on_period;
    int coin_off_period;
    int start_delay;
    bool opto_drive;

// Initialisation functions

protected:

// Read and write functions
public:
	void reset(void);
	void optodrive(Byte x);
    Byte optoread(Byte x);
    Byte optoread(Byte x, Byte opto);
    Byte optoread2(Byte x, Byte opto);
    Byte motorread(Byte x);
    void motordrive(Byte x);
    void update(void);
    void stopcoin(void);
    void SaveToStream(Byte *&ptr);
    void LoadFromStream(Byte *&ptr);

// Public constructor and destructor

    hopper(int Astart_delay = 10, int Acoin_on_period = 2, int Acoin_off_period = 15);
    ~hopper();

};

#endif // __hopper_hpp__
