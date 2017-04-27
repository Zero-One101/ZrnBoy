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
	void UnknownOpcode();

	// Function pointer table
	void(Z80::*OpcodeTable[256])() =
	{
		&Z80::NoOp, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,			// 0x07
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x0F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x17
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x1F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x27
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x2F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x37
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x3F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x47
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x4F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x57
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x5F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x67
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x6F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x77
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x7F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x87
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x8F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x97
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0x9F
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xA7
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xAF
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xB7
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xBF
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::JumpImmediate, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xC7
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xCF
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xD7
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0XDF
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xE7
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xEF
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode,	// 0xF7
		&Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode, &Z80::UnknownOpcode	// 0xFF
	};

	void NoOp();
	void JumpImmediate();

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

	// Program counter
	unsigned short PC;

	// Stack pointer
	unsigned short SP;

	unsigned char opcode;
};

