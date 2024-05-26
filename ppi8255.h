//
//
//	ppp8255.h
//
//	(C) C.J.Wren 2000
//

#ifndef __ppi8255_h__
#define __ppi8255_h__

class PPI8255 {

// Internal registers
private:
   void writea(void);
   void writeb(void);
   void writec(void);
public:
   Byte groupA_mode;
   Byte groupB_mode;
   Byte io[3];
   Byte latch[3];
   Byte __fastcall portAread(int offset);
   Byte __fastcall portBread(int offset);
   Byte __fastcall portCread(int offset);
   void __fastcall portAwrite(void);
   void __fastcall portBwrite(void);
   void __fastcall portCwrite(void);
   Byte	porta, portb, portc;
   Byte	porta_in, portb_in, portc_in;
   Byte	porta_changed, portb_changed, portc_changed;
   bool	porta_updated, portb_updated, portc_updated;

// Initialisation functions

protected:


// Read and write functions
public:

	Byte			 read(Word offset);
	void			 write(Word offset, Byte val);
	void			 reset(void);

// Public constructor and destructor

				 PPI8255();
				~PPI8255();

};

#endif // __ppi8255_h__
