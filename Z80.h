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
		&Z80::NoOp, &Z80::LoadImmediate16IntoBC, &Z80::UnknownOp, &Z80::IncBC, &Z80::UnknownOp, &Z80::DecB, &Z80::LoadBIntoImmediate, &Z80::UnknownOp,				// 0x07
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadBCIndirectIntoA, &Z80::DecBC, &Z80::IncC, &Z80::DecC, &Z80::LoadCIntoImmediate, &Z80::UnknownOp,			// 0x0F
		&Z80::Stop, &Z80::LoadImmediate16IntoDE, &Z80::LoadAIntoDE, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::DecD, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x17
		&Z80::JumpSignedImmediate, &Z80::AddDEToHL, &Z80::UnknownOp, &Z80::DecDE, &Z80::IncE, &Z80::DecE, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x1F
		&Z80::JumpOffsetIfNZ, &Z80::LoadImmediate16IntoHL, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoH, &Z80::UnknownOp,		// 0x27
		&Z80::UnknownOp, &Z80::AddHLToHL, &Z80::LoadHLIndirectIntoAIncHL, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::ComplementA,						// 0x2F
		&Z80::UnknownOp, &Z80::PushImmediate16, &Z80::LoadAIntoHLDecHL, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoHL, &Z80::UnknownOp,		// 0x37
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::LoadImmediateIntoA, &Z80::UnknownOp,			// 0x3F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x47
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x4F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x57
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x5F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x67
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x6F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x77
		&Z80::LoadBIntoA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x7F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x87
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x8F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x97
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0x9F
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xA7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::XorWithA,						// 0xAF
		&Z80::UnknownOp, &Z80::OrCWithA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xB7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xBF
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::JumpImmediate, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,					// 0xC7
		&Z80::UnknownOp, &Z80::Return, &Z80::UnknownOp, &Z80::CBLookup, &Z80::UnknownOp, &Z80::CallImmediate16, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xCF
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0xD7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,						// 0XDF
		&Z80::StoreAToFFImmediate, &Z80::UnknownOp, &Z80::StoreAToFFPlusC, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,			// 0xE7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::StoreAToImmediate16, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,			// 0xEF
		&Z80::LoadFFImmediateIntoA, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::DisableInterrupts, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp,	// 0xF7
		&Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::UnknownOp, &Z80::CompareAWithImmediate, &Z80::UnknownOp			// 0xFF
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

	void NoOp();
	void LoadImmediate16IntoBC();
	void IncBC();
	void DecB();
	void LoadBIntoImmediate();
	void LoadBCIndirectIntoA();
	void DecBC();
	void IncC();
	void DecC();
	void LoadCIntoImmediate();
	void Stop();
	void LoadImmediate16IntoDE();
	void LoadAIntoDE();
	void DecD();
	void JumpSignedImmediate();
	void AddDEToHL();
	void DecDE();
	void IncE();
	void DecE();
	void JumpOffsetIfNZ();
	void LoadImmediate16IntoHL();
	void LoadImmediateIntoH();
	void AddHLToHL();
	void LoadHLIndirectIntoAIncHL();
	void ComplementA();
	void PushImmediate16();
	void LoadAIntoHLDecHL();
	void LoadImmediateIntoHL();
	void LoadImmediateIntoA();
	void LoadBIntoA();
	void XorWithA();
	void OrCWithA();
	void JumpImmediate();
	void Return();
	void CBLookup();
	void CallImmediate16();
	void StoreAToFFImmediate();
	void StoreAToFFPlusC();
	void StoreAToImmediate16();
	void LoadFFImmediateIntoA();
	void DisableInterrupts();
	void CompareAWithImmediate();

	void ShiftBLeftIntoCarry();

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

	// Program counter
	unsigned short PC;

	// Stack pointer
	unsigned short SP;

	unsigned char opcode;

	bool interruptsEnabled = true;
	bool shouldDisableInterrupts = false;
	bool shouldEnableInterrupts = false;
	bool deferInterruptToggle = false;

	bool stopped = false;
};

