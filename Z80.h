#pragma once
#include <string>

class Z80
{
public:
	Z80();
	~Z80();

	// Initialise registers and memory
	void Init();

	// Load the ROM into memory
	bool LoadGame(std::string const &gameName);

	// Fetch, decode and execute opcode
	void AdvanceCycle();

private:
	void FetchOpcode();
	void DecodeOpcode();
	void UnknownOp();
	void UnknownCB();

	// Function pointer table
	void(Z80::*OpcodeTable[256])() =
	{
		&Z80::NoOp, &Z80::LoadImmediate16IntoBC, &Z80::UnknownOp, &Z80::IncBC, &Z80::IncB, &Z80::DecB, &Z80::LoadImmediateIntoB, &Z80::UnknownOp,				// 0x07
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadBCIndirectIntoA, &Z80::DecBC, &Z80::IncC, &Z80::DecC, &Z80::LoadImmediateIntoC, &Z80::UnknownOp,			// 0x0F
		&Z80::Stop, &Z80::LoadImmediate16IntoDE, &Z80::LoadAIntoDE, &Z80::UnknownOp, &Z80::IncD, &Z80::DecD, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x17
		&Z80::JumpSignedImmediate, &Z80::AddDEToHL, &Z80::UnknownOp, &Z80::DecDE, &Z80::IncE, &Z80::DecE, &Z80::UnknownOp, &Z80::RotateRightA,						// 0x1F
		&Z80::JumpOffsetIfNZ, &Z80::LoadImmediate16IntoHL, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoH, &Z80::UnknownOp,		// 0x27
		&Z80::JumpOffsetIfZ, &Z80::AddHLToHL, &Z80::LoadHLIndirectIntoAIncHL, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoL, &Z80::ComplementA,						// 0x2F
		&Z80::UnknownOp, &Z80::PushImmediate16, &Z80::LoadAIntoHLDecHL, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoHL, &Z80::UnknownOp,		// 0x37
		&Z80::JumpOffsetIfC, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoA, &Z80::UnknownOp,			// 0x3F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x47
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x4F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x57
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x5F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x67
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x6F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x77
		&Z80::LoadBIntoA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x7F
		&Z80::AddBToA, &Z80::AddCToA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x87
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x8F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x97
		&Z80::SubtractNCarryFromA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x9F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::AndAWithA,						// 0xA7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::XorWithA,						// 0xAF
		&Z80::UnknownOp, &Z80::OrCWithA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xB7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xBF
		&Z80::RetIfNZ, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::JumpImmediate, &Z80::UnknownOp, &Z80::PushBCToStack, &Z80::UnknownOp, &Z80::UnknownOp,					// 0xC7
		&Z80::UnknownOp, &Z80::Return, &Z80::UnknownOp, &Z80::CBLookup, &Z80::UnknownOp, &Z80::CallImmediate16, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xCF
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::PushDEToStack, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xD7
		&Z80::ReturnIfC, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0XDF
		&Z80::StoreAToFFImmediate, &Z80::UnknownOp, &Z80::StoreAToFFPlusC, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::PushHLToStack, &Z80::UnknownOp, &Z80::UnknownOp,			// 0xE7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::StoreAToImmediate16, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,			// 0xEF
		&Z80::LoadFFImmediateIntoA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::DisableInterrupts, &Z80::UnknownOp, &Z80::PushAFToStack, &Z80::UnknownOp, &Z80::UnknownOp,	// 0xF7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::CompareAWithImmediate, &Z80::Restart38			// 0xFF
	};

	// Function pointer table for CB prefix
	void(Z80::*CBTable[256])() =
	{
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x07
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x0F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x17
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x1F
		&Z80::ShiftBLeftIntoCarry, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,			// 0x27
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x2F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x37
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x3F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x47
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x4F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x57
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x5F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x67
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x6F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x77
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x7F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x87
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x8F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x97
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0x9F
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xA7
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xAF
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xB7
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xBF
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xC7
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xCF
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xD7
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xDF
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xE7
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xEF
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB,						// 0xF7
		&Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB, &Z80::UnknownCB						// 0xFF
	};

	void NoOp();						// 0x00
	void LoadImmediate16IntoBC();		// 0x01
	void IncBC();						// 0x03
	void IncB();						// 0x04
	void DecB();						// 0x05
	void LoadImmediateIntoB();			// 0x06
	void LoadBCIndirectIntoA();			// 0x0A
	void DecBC();						// 0x0B
	void IncC();						// 0x0C
	void DecC();						// 0x0D
	void LoadImmediateIntoC();			// 0x0E

	void Stop();						// 0x10
	void LoadImmediate16IntoDE();		// 0x11
	void LoadAIntoDE();					// 0x12
	void IncD();						// 0x14
	void DecD();						// 0x15
	void JumpSignedImmediate();			// 0x18
	void AddDEToHL();					// 0x19
	void DecDE();						// 0x1B
	void IncE();						// 0x1C
	void DecE();						// 0x1D
	void RotateRightA();				// 0x1F

	void JumpOffsetIfNZ();				// 0x20
	void LoadImmediate16IntoHL();		// 0x21
	void LoadImmediateIntoH();			// 0x26
	void JumpOffsetIfZ();				// 0x28
	void AddHLToHL();					// 0x29
	void LoadHLIndirectIntoAIncHL();	// 0x2A
	void LoadImmediateIntoL();			// 0x2E
	void ComplementA();					// 0x2F

	void PushImmediate16();				// 0x31
	void LoadAIntoHLDecHL();			// 0x32
	void LoadImmediateIntoHL();			// 0x36
	void JumpOffsetIfC();				// 0x38
	void LoadImmediateIntoA();			// 0x3E

	void LoadBIntoA();					// 0x78

	void AddBToA();						// 0x80
	void AddCToA();						// 0x81

	void SubtractNCarryFromA();			// 0x98

	void AndAWithA();					// 0xA7
	void XorWithA();					// 0xAF

	void OrCWithA();					// 0xB1

	void RetIfNZ();						// 0xC0
	void JumpImmediate();				// 0xC3
	void PushBCToStack();				// 0xC5
	void Return();						// 0xC9
	void CBLookup();					// 0xCB
	void CallImmediate16();				// 0xCD

	void PushDEToStack();				// 0xD5
	void ReturnIfC();					// 0xD8

	void StoreAToFFImmediate();			// 0xE0
	void StoreAToFFPlusC();				// 0xE2
	void PushHLToStack();				// 0xE5
	void StoreAToImmediate16();			// 0xEA

	void LoadFFImmediateIntoA();		// 0xF0
	void DisableInterrupts();			// 0xF3
	void PushAFToStack();				// 0xF5
	void CompareAWithImmediate();		// 0xFE
	void Restart38();					// 0xFF

	void ShiftBLeftIntoCarry();			// 0x20

	static const int memorySize = 0xFFFF;
	
	/*
	0x0000-0x3FFF - Bank 0, fixed
	0x4000-0x7FFF - Bank 1, MBC switchable
	0x8000-0x9FFF - VRAM, switchable bank 0/1 in CGB
	0xA000-0xBFFF - 8KB external RAM, in cart
	0xC000-0xCFFF - WRAM bank 0
	0xD000-0xDFFF - WRAM bank 1~N, switchable bank 1~7 in CGB mode
	0xE000-0xFDFF - Mirror of C000~DDFF
	0xFE00-0xFE9F - Object Attribute Memory (OAM)
	0xFEA0-0xFEFF - Not usable
	0xFF00-0xFF7F - I/O Registers
	0xFF80-0xFFFE - HRAM
	0xFFFF-0xFFFF - Interrupts Enable Register
	*/
	unsigned char memory[memorySize];

	// This still looks awful
	struct Registers
	{
		unsigned char A; // Accumulator
		unsigned char B;
		unsigned char C;
		unsigned char D;
		unsigned char E;
		unsigned char F; // Flags
		unsigned char H;
		unsigned char L;
	};

	Registers registers;

	// 16-bit register handling until I think of a better way
	unsigned short GetAF();
	unsigned short GetBC();
	unsigned short GetDE();
	unsigned short GetHL();
	void SetAF(unsigned short word);
	void SetBC(unsigned short word);
	void SetDE(unsigned short word);
	void SetHL(unsigned short word);

	// Flag handling
	bool GetZeroFlag();
	bool GetAddSubFlag();
	bool GetHalfCarryFlag();
	bool GetCarryFlag();
	void SetZeroFlag();
	void SetAddSubFlag();
	void SetHalfCarryFlag();
	void SetCarryFlag();
	void ClearZeroFlag();
	void ClearAddSubFlag();
	void ClearHalfCarryFlag();
	void ClearCarryFlag();

	// Memory Read/Write
	unsigned char MemoryReadByte(unsigned short loc);
	void MemoryWriteByte(unsigned short dest, unsigned char data);
	unsigned short MemoryReadWord(unsigned short loc);
	void MemoryWriteWord(unsigned short dest, unsigned short data);

	// Stack R/W
	unsigned short StackPop();
	void StackPush(unsigned short data);

	// Program counter
	unsigned short PC;

	// Stack pointer
	unsigned short SP;

	unsigned char opcode;

	bool interruptsEnabled = false;
	bool shouldDisableInterrupts = false;
	bool shouldEnableInterrupts = false;
	bool deferInterruptToggle = false;

	bool stopped = false;

	// Interrupt jumps
	const unsigned short VBlankAddr = 0x0040;	// Priority 1 -- HIGHEST
	const unsigned short LcdcAddr = 0x0048;		// Priority 2
	const unsigned short TimerAddr = 0x0050;	// Priority 3
	const unsigned short SerialAddr = 0x0058;	// Priority 4
	const unsigned short P10 = 0x0060;			// Priority 5 -- LOWEST

	// I/O Registers
	const unsigned short P1 = 0xFF00;		// Joypad info
	const unsigned short SB = 0xFF01;		// Serial transfer data
	const unsigned short SC = 0xFF02;		// Serial I/O control
	const unsigned short Div = 0xFF04;		// Divider
	const unsigned short Tima = 0xFF05;		// Timer counter
	const unsigned short Tma = 0xFF06;		// Timer modulo
	const unsigned short Tac = 0xFF07;		// Timer control
	const unsigned short IF = 0xFF0F;		// Interrupt flag
	const unsigned short NR10 = 0xFF10;		// Sound Mode 1 sweep register
	const unsigned short NR11 = 0xFF11;		// Sound mode 1 wave pattern duty register
	const unsigned short NR12 = 0xFF12;		// Sound mode 1 envelope register
	const unsigned short NR13 = 0xFF13;		// Sound mode 1 freq. lo register
	const unsigned short NR14 = 0xFF14;		// Sound mode 1 freq. hi register
	const unsigned short NR21 = 0xFF16;		// Sound mode 2 wave pattern duty register
	const unsigned short NR22 = 0xFF17;		// Sound mode 2 envelope register
	const unsigned short NR23 = 0xFF18;		// Sound mode 2 freq. lo register
	const unsigned short NR24 = 0xFF19;		// Sound mode 2 freq. hi register
	const unsigned short NR30 = 0xFF1A;		// Sound mode 3 on/off register
	const unsigned short NR31 = 0xFF1B;		// Sound mode 3 sound length register
	const unsigned short NR32 = 0xFF1C;		// Sound mode 3 output level register
	const unsigned short NR33 = 0xFF1D;		// Sound mode 3 freq. lo register
	const unsigned short NR34 = 0xFF1E;		// Sound mode 3 freq. hi register
	const unsigned short NR41 = 0xFF20;		// Sound mode 4 sound length register
	const unsigned short NR42 = 0xFF21;		// Sound mode 4 envelope register
	const unsigned short NR43 = 0xFF22;		// Sound mode 4 polynomial counter register
	const unsigned short NR44 = 0xFF23;		// Sound mode 4 counter/consecutive register
	const unsigned short NR50 = 0xFF24;		// Channel control
	const unsigned short NR51 = 0xFF25;		// Selection of sound output terminal
	const unsigned short NR52 = 0xFF26;		// Sound on/off
	const unsigned short WaveRAM = 0xFF30;	// Wave pattern RAM (samples)
	const unsigned short Lcdc = 0xFF40;		// LCD Control
	const unsigned short Stat = 0xFF41;		// LCD status
	const unsigned short SCY = 0xFF42;		// Scroll Y
	const unsigned short SCX = 0xFF43;		// Scroll X
	const unsigned short LY = 0xFF44;		// LCDC Y-Coordinate
	const unsigned short LYC = 0xFF45;		// LY Compare
	const unsigned short DMA = 0xFF46;		// DMA transfer and start address
	const unsigned short BGP = 0xFF47;		// BG & window palette data
	const unsigned short OBP0 = 0xFF48;		// Object palette 0 data
	const unsigned short OBP1 = 0xFF49;		// Object palette 1 data
	const unsigned short WY = 0xFF4A;		// Window Y position
	const unsigned short WX = 0xFF4B;		// Window X position
	const unsigned short IE = 0xFFFF;		// Interrupt enable

	int cycles; // Timer counting down in machine cycles
};

