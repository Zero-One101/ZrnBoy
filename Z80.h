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

	bool interruptsEnabled = false;
	bool shouldDisableInterrupts = false;
	bool shouldEnableInterrupts = false;
	bool deferInterruptToggle = false;

	bool stopped = false;

	// Interrupt jumps
	const unsigned short VBlankAddr = 0x0040; // Priority 1 -- HIGHEST
	const unsigned short LcdcAddr = 0x0048; // Priority 2
	const unsigned short TimerAddr = 0x0050; // Priority 3
	const unsigned short SerialAddr = 0x0058; // Priority 4
	const unsigned short P10 = 0x0060; // Priority 5 -- LOWEST

	// I/O Registers
	const unsigned short P1 = 0xFF00; // Joypad info
	const unsigned short SB = 0xFF01; // Serial transfer data
	const unsigned short SC = 0xFF02; // Serial I/O control
	const unsigned short Div = 0xFF04; // Divider
	const unsigned short Tima = 0xFF05; // Timer counter
	const unsigned short Tma = 0xFF06; // Timer modulo
	const unsigned short Tac = 0xFF07; // Timer control
	const unsigned short IF = 0xFF0F; // Interrupt flag
	const unsigned short NR10 = 0xFF10; // Sound Mode 1 sweep register
	const unsigned short NR11 = 0xFF11; // Sound mode 1 wave pattern duty register
	const unsigned short NR12 = 0xFF12; // Sound mode 1 envelope register
	const unsigned short NR13 = 0xFF13; // Sound mode 1 freq. lo register
	const unsigned short NR14 = 0xFF14; // Sound mode 1 freq. hi register
	const unsigned short NR21 = 0xFF16; // Sound mode 2 wave pattern duty register
	const unsigned short NR22 = 0xFF17; // Sound mode 2 envelope register
	const unsigned short NR23 = 0xFF18; // Sound mode 2 freq. lo register
	const unsigned short NR24 = 0xFF19; // Sound mode 2 freq. hi register
	const unsigned short NR30 = 0xFF1A; // Sound mode 3 on/off register
	const unsigned short NR31 = 0xFF1B; // Sound mode 3 sound length register
	const unsigned short NR32 = 0xFF1C; // Sound mode 3 output level register
	const unsigned short NR33 = 0xFF1D; // Sound mode 3 freq. lo register
	const unsigned short NR34 = 0xFF1E; // Sound mode 3 freq. hi register
	const unsigned short NR41 = 0xFF20; // Sound mode 4 sound length register
	const unsigned short NR42 = 0xFF21; // Sound mode 4 envelope register
	const unsigned short NR43 = 0xFF22; // Sound mode 4 polynomial counter register
	const unsigned short NR44 = 0xFF23; // Sound mode 4 counter/consecutive register
	const unsigned short NR50 = 0xFF24; // Channel control
	const unsigned short NR51 = 0xFF25; // Selection of sound output terminal
	const unsigned short NR52 = 0xFF26; // Sound on/off
	const unsigned short WaveRAM = 0xFF30; // Wave pattern RAM (samples)
	const unsigned short Lcdc = 0xFF40; // LCD Control
	const unsigned short Stat = 0xFF41; // LCD status
	const unsigned short SCY = 0xFF42; // Scroll Y
	const unsigned short SCX = 0xFF43; // Scroll X
	const unsigned short LY = 0xFF44; // LCDC Y-Coordinate
	const unsigned short LYC = 0xFF45; // LY Compare
	const unsigned short DMA = 0xFF46; // DMA transfer and start address
	const unsigned short BGP = 0xFF47; // BG & window palette data
	const unsigned short OBP0 = 0xFF48; // Object palette 0 data
	const unsigned short OBP1 = 0xFF49; // Object palette 1 data
	const unsigned short WY = 0xFF4A; // Window Y position
	const unsigned short WX = 0xFF4B; // Window X position
	const unsigned short IE = 0xFFFF; // Interrupt enable
};

