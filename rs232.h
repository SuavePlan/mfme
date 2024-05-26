//
//
//	rs232.hpp
//

#ifndef __rs232__
#define __rs232__

class rs232 {

// Internal registers

private:
    Byte            frame;
    Byte            parity;
    Word            timer;
    Byte            state;
    Byte            in_data;

// Initialisation functions

protected:
	void			 reset(void);

// Read and write functions
public:
    Byte             data;
    Byte             in;
    bool             RXComplete;
    void             write(int bit, int changed);
    bool             update(void);
    void             shiftbit(void);

// Public constructor and destructor

				 rs232();
				~rs232();

};

#endif // __rs232_hpp__
