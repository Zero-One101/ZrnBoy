#include "Z80.h"

#include <QMessageBox>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <string.h>

Z80::Z80()
{
}

void Z80::Init()
{
	PC = 0x0100;
	SP = 0xFFFE;
	opcode = 0;
	memset(memory, 0, memorySize);
	ClearZeroFlag();
	ClearAddSubFlag();
	ClearHalfCarryFlag();
	ClearCarryFlag();

	SetAF(0x01B0);
	SetBC(0x0013);
	SetDE(0x00D8);
	SetHL(0x014D);

	// Set stack
	MemoryWriteByte(0xFF10, 0x80);
	MemoryWriteByte(0xFF11, 0xBF);
	MemoryWriteByte(0xFF12, 0xF3);
	MemoryWriteByte(0xFF14, 0xBF);
	MemoryWriteByte(0xFF16, 0x3F);
	MemoryWriteByte(0xFF19, 0xBF);
	MemoryWriteByte(0xFF1A, 0x7F);
	MemoryWriteByte(0xFF1B, 0xFF);
	MemoryWriteByte(0xFF1C, 0x9F);
	MemoryWriteByte(0xFF1E, 0xBF);
	MemoryWriteByte(0xFF20, 0xFF);
	MemoryWriteByte(0xFF23, 0xBF);
	MemoryWriteByte(0xFF24, 0x77);
	MemoryWriteByte(0xFF25, 0xF3);
	MemoryWriteByte(0xFF26, 0xF1);
	MemoryWriteByte(0xFF40, 0x91);
	MemoryWriteByte(0xFF47, 0xFC);
	MemoryWriteByte(0xFF48, 0xFF);
	MemoryWriteByte(0xFF49, 0xFF);

	cycles = 455;
	memory[LY] = 144;
}

bool Z80::LoadGame(std::string const &gameName)
{
	std::ifstream input(gameName, std::ios::binary);
	input >> std::noskipws;
	std::istream_iterator<char> start(input), end;
	std::vector<char> buffer(start, end);

	memcpy(memory, buffer.data(), 0x7FFF);

	return true;
}

void Z80::AdvanceCycle()
{
	if (!stopped)
	{
		FetchOpcode();
		DecodeOpcode();

		if (shouldDisableInterrupts)
		{
			if (!deferInterruptToggle)
			{
				interruptsEnabled = false;
				shouldDisableInterrupts = false;
				printf("Interrupts disabled\n");
			}
			else
			{
				deferInterruptToggle = false;
			}
		}

		if (shouldEnableInterrupts)
		{
			if (!deferInterruptToggle)
			{
				interruptsEnabled = true;
				shouldEnableInterrupts = false;
				printf("Interrupts enabled\n");
			}
			else
			{
				deferInterruptToggle = false;
			}
		}
	}

	if (cycles <= 0)
	{
		cycles += 455;
		memory[LY]++;
		if (memory[LY] == 144)
		{
			PC = VBlankAddr;
		}
		else if (memory[LY] > 153)
		{
			memory[LY] = 0;
		}
	}
}

void Z80::FetchOpcode()
{
	opcode = memory[PC];
}

void Z80::DecodeOpcode()
{
	(this->*OpcodeTable[opcode])();
}

/* Performs no operation */
void Z80::NoOp()
{
	PC++;
	printf("0x%.2X: NOP\n", opcode);
	cycles -= 4;
}

/* Loads the next 16-bit value into BC */
void Z80::LoadImmediate16IntoBC()
{
	SetBC(MemoryReadWord(PC + 1));
	PC += 3;
	printf("0x%.2X: Set BC to 0x%.4X\n", opcode, GetBC());
	cycles -= 12;
}

/* Increments BC */
void Z80::IncBC()
{
	SetBC(GetBC() + 1);
	PC++;
	printf("0x%.2X: Incremented BC (0x%.4X)\n", opcode, GetBC());
	cycles -= 8;
}

/* Decrements register B
   If register B is 0, set zero flag
   Set AddSub Flag
   Set Half-carry if no borrow from bit 4*/
void Z80::DecB()
{
	registers.B--;
	registers.B == 0 ? SetCarryFlag() : ClearCarryFlag();
	SetAddSubFlag();
	registers.B > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Decremented B (0x%.2X)\n", opcode, registers.B);
	cycles -= 4;
}

/* Stores register B into memory at the location specified by the next 8-bit value */
void Z80::LoadBIntoImmediate()
{
	MemoryWriteByte(MemoryReadByte(PC + 1), registers.B);
	printf("0x%.2X: Stored the value in register B (0x%.2X) into RAM at location 0x%.2X\n", opcode, registers.B, memory[PC + 1]);
	PC += 2;
	cycles -= 8;
}

/* Stores the value at the address in BC into A */
void Z80::LoadBCIndirectIntoA()
{
	registers.A = MemoryReadByte(GetBC());
	PC++;
	printf("0x%.2X: Stored data at 0x%.4X into A (0x%.2X)\n", opcode, GetBC(), registers.A);
	cycles -= 8;
}

/* Decrements register BC*/
void Z80::DecBC()
{
	SetBC(GetBC() - 1);
	PC++;
	printf("0x%.2X: Decremented BC (0x%.4X)\n", opcode, GetBC());
	cycles -= 8;
}

/* Increments register C
   If C is 0, set zero flag
   Reset AddSubFlag
   Set Half-carry if carry from bit 3 */
void Z80::IncC()
{
	registers.C++;
	registers.C == 0 ? SetCarryFlag() : ClearCarryFlag();
	ClearAddSubFlag();
	registers.C > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Incremented C (0x%.2X)\n", opcode, registers.C);
	cycles -= 4;
}

/* Decrements register C
   If register C is 0, set zero flag
   Set AddSub Flag
   Set Half-carry if no borrow from bit 4*/
void Z80::DecC()
{
	registers.C--;
	registers.C == 0 ? SetCarryFlag() : ClearCarryFlag();
	SetAddSubFlag();
	registers.C > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Decremented C (0x%.2X)\n", opcode, registers.C);
	cycles -= 4;
}

/* Stores the next 8-bit value into C */
void Z80::LoadImmediateIntoC()
{
	unsigned char data = MemoryReadByte(PC + 1);
	registers.C = data;
	printf("0x%.2X: Loaded 0x%.2X into C\n", opcode, data);
	PC += 2;
	cycles -= 8;
}

/* Halts CPU and LCD until button pressed */
void Z80::Stop()
{
	PC += 2;
	stopped = true;
	printf("0x%.2X: CPU STOP\n", opcode);
	cycles -= 4;
}

/* Loads the next 16-bit immediate value into DE */
void Z80::LoadImmediate16IntoDE()
{
	unsigned short data = MemoryReadWord(PC + 1);
	SetDE(data);
	PC += 3;
	printf("0x%.2X: Set DE equal to (0x%.4X)\n", opcode, data);
	cycles -= 12;
}

/* Stores A into DE */
void Z80::LoadAIntoDE()
{
	SetDE(registers.A);
	PC++;
	printf("0x%.2X: Set DE equal to A (0x%.4X)\n", opcode, registers.A);
	cycles -= 8;
	// TODO: I don't think this opcode is right
	// LD (DE), A
}

/* Decrements D
   Zero flag set if D is 0
   AddSub flag set
   Half-carry set if no borrow from bit 4 */
void Z80::DecD()
{
	registers.D--;
	registers.D == 0 ? SetZeroFlag() : ClearZeroFlag();
	SetAddSubFlag();
	registers.D > 0 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Decremented D (0x%.2X)\n", opcode, registers.D);
	cycles -= 4;
}

/* Add the next signed 8-bit value to the PC */
void Z80::JumpSignedImmediate()
{
	signed char offset = MemoryReadByte(PC + 1);
	PC += offset;
	printf("0x%.2X: Signed immediate jump to 0x%.4X\n", opcode, PC);
	cycles -= 12;
}

/* Adds register DE to register HL
   AddSub flag reset
   Half-carry flag set if carry from bit 11
   Carry flag set if carry from bit 15 */
void Z80::AddDEToHL()
{
	int result = GetHL() + GetDE();
	SetHL(GetHL() + GetDE());
	ClearAddSubFlag();
	// TODO: Verify this is right. Not sure if half-carry and carry can both be set
	result > 1023 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	result > 0xFFFF ? SetCarryFlag() : ClearCarryFlag();
	PC++;
	printf("0x%.2X: Set HL to HL + DE (0x%.4X)\n", opcode, GetHL());
	cycles -= 8;
}

/* Decrements DE */
void Z80::DecDE()
{
	SetDE(GetDE() - 1);
	PC++;
	printf("0x%.2X: Decremented DE (0x%.4X)\n", opcode, GetDE());
	cycles -= 8;
}

/* Increments E
   Zero flag set or cleared
   AddSub flag set
   Half-carry set if no borrow from bit 4 */
void Z80::IncE()
{
	registers.E++;
	registers.E == 0 ? SetZeroFlag() : ClearZeroFlag();
	ClearAddSubFlag();
	registers.E > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Incremented E (0x%.2X)\n", opcode, registers.E);
	cycles -= 4;
}

/* Decrements E 
   Zero flag set or cleared
   AddSub flag set
   Half-carry set if no borrow from bit 4 */
void Z80::DecE()
{
	registers.E--;
	registers.E == 0 ? SetZeroFlag() : ClearZeroFlag();
	SetAddSubFlag();
	registers.E > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Decremented E (0x%.2X)\n", opcode, registers.E);
	cycles -= 4;
}

/* If zero flag is reset, add the next 8-bit value to PC*/
void Z80::JumpOffsetIfNZ()
{
	if (!GetZeroFlag())
	{
		PC += memory[PC + 1];
		printf("0x%.2X: Zero Flag was reset, jumped to 0x%.2X\n", opcode, PC);
		cycles -= 12;
		return;
	}

	PC += 2;
	printf("0x%.2X: Zero Flag was set, did not jump\n", opcode);
	cycles -= 8;
}

/* Loads the next 16-bit value into register HL */
void Z80::LoadImmediate16IntoHL()
{
	SetHL(MemoryReadWord(PC + 1));
	PC += 3;
	printf("0x%.2X: Loaded 0x%.2X into register HL\n", opcode, GetHL());
	cycles -= 12;
}

/* Loads the next 8-bit value into register H */
void Z80::LoadImmediateIntoH()
{
	unsigned char data = MemoryReadByte(PC + 1);
	registers.H = data;
	PC += 2;
	printf("0x%.2X: Loaded 0x%.2X into H\n", opcode, data);
	cycles -= 8;
}

/* Adds HL to itself
   AddSub flag reset
   Half-carry set if carry from bit 11
   Carry set if carry from bit 15 */
void Z80::AddHLToHL()
{
	int result = GetHL() + GetHL();
	SetHL(GetHL() + GetHL());
	result > 0xFFFF ? SetCarryFlag() : ClearCarryFlag();
	ClearAddSubFlag();
	result > 1023 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	PC++;
	printf("0x%.2X: Added HL to itself (0x%.4X)\n", opcode, GetHL());
	cycles -= 8;
}

/* Takes the value of the address stored in HL and stores it in A
   HL is then decremented */
void Z80::LoadHLIndirectIntoAIncHL()
{
	unsigned char data = MemoryReadByte(GetHL());
	registers.A = data;
	SetHL(GetHL() - 1);
	PC++;
	printf("0x%.2X: Loaded HL indirect value 0x%.2X into A\n", opcode, data);
	cycles -= 8;
}

/* A looks wonderful today! Flip all the bits 
   AddSub flag is set
   Half-carry flag is set */
void Z80::ComplementA()
{
	registers.A = ~registers.A;
	SetAddSubFlag();
	SetHalfCarryFlag();
	PC++;
	printf("0x%.2X: Flipped A bits (0x%.2X)\n", opcode, registers.A);
	cycles -= 4;
}

/* Pushes the next 16-bit value onto the stack */
void Z80::PushImmediate16()
{
	SP -= 2;
	unsigned short data = MemoryReadWord(PC + 1);
	MemoryWriteWord(SP, data);
	printf("0x%.2X: Pushed 0x%.2X onto stack\n", opcode, data);
	PC += 3;
	cycles -= 12;
}

/* Stores register A into register HL, then decrements HL */
void Z80::LoadAIntoHLDecHL()
{
	SetHL(registers.A);
	SetHL(GetHL() - 1);
	PC++;
	printf("0x%.2X: Loaded A (0x%.2X) into HL and decremented\n", opcode, registers.A);
	cycles -= 8;
	// TODO: Not sure this is right. LD (HL-), A
}

/* Loads the next 8-bit value into HL */
void Z80::LoadImmediateIntoHL()
{
	unsigned char data = MemoryReadByte(PC + 1);
	SetHL(data);
	printf("0x%.2X: Set HL to 0x%.2X\n", opcode, data);
	PC += 2;
	cycles -= 12;
	// TODO: Not sure this is right. LD (HL), d8
}

/* Loads the next 8-bit value into register A */
void Z80::LoadImmediateIntoA()
{
	registers.A = MemoryReadByte(PC + 1);
	PC += 2;
	printf("0x%.2X: Loaded 0x%.2X into A\n", opcode, registers.A);
	cycles -= 8;
}

/* Puts the value of B into A */
void Z80::LoadBIntoA()
{
	registers.B = registers.A;
	PC++;
	printf("0x%.2X: B set to A (0x%.2X)\n", opcode, registers.B);
	cycles -= 4;
}

/* Xors A with A. Sets zero flag, clears all others */
void Z80::XorWithA()
{
	registers.A ^= registers.A;
	SetZeroFlag();
	ClearAddSubFlag();
	ClearHalfCarryFlag();
	ClearCarryFlag();
	PC++;
	printf("0x%.2X: XOR register A with register A. Result is 0x%.2X\n", opcode, registers.A);
	cycles -= 4;
}

/* Ors C with A and stores the result in A
   If A is zero, zero flag is set
   Other flags are cleared */
void Z80::OrCWithA()
{
	registers.A = registers.C | registers.A;
	registers.A == 0 ? SetZeroFlag() : ClearZeroFlag();
	ClearAddSubFlag();
	ClearHalfCarryFlag();
	ClearCarryFlag();
	PC++;
	printf("0x%.2X: Or'd C with A (0x%.2X)\n", opcode, registers.A);
	cycles -= 4;
}

/* Jumps to the offset specified by the next 16-bit value */
void Z80::JumpImmediate()
{
	unsigned short dest = MemoryReadWord(PC + 1);
	PC = dest;
	printf("0x%.2X: Jump Immediate to 0x%.2X\n", opcode, dest);
	cycles -= 16;
}

/* Pops two bytes from the stack and jumps to that address */
void Z80::Return()
{
	PC = MemoryReadWord(SP);
	SP += 2;
	printf("0x%.2X: Returned from subroutine\n", opcode);
	cycles -= 16;
}

/* Encountered opcode CB, so use the CB function pointer table */
void Z80::CBLookup()
{
	PC++;
	FetchOpcode();
	cycles -= 4;
	(this->*CBTable[opcode])();
}

/* Pushes address of next instruction to the stack, then jumps to the next 16-bit value */
void Z80::CallImmediate16()
{
	SP -= 2;
	MemoryWriteWord(SP, PC + 3);
	PC = MemoryReadWord(PC + 1);
	printf("0x%.2X: Called method at 0x%.4X\n", opcode, PC);
	cycles -= 24;
}

/* Stores register A to 0xFF00 plus the next 8-bit value */
void Z80::StoreAToFFImmediate()
{
	unsigned short dest = 0xFF00 + MemoryReadByte(PC + 1);
	MemoryWriteByte(dest, registers.A);
	PC += 2;
	printf("0x%.2X: A (0x%.2X) stored at 0x%.2X\n", opcode, registers.A, dest);
	cycles -= 12;
}

/* Stores register A to 0xFF00 plus register C */
void Z80::StoreAToFFPlusC()
{
	unsigned short loc = 0xFF00 + registers.C;
	MemoryWriteByte(loc, registers.A);
	PC++;
	printf("0x%.2X: Wrote A (0x%.2X) to 0x%.2X\n", opcode, registers.A, loc);
	cycles -= 8;
}

/* Stores register A to memory at the next 16-bit value */
void Z80::StoreAToImmediate16()
{
	unsigned short dest = MemoryReadWord(PC + 1);
	MemoryWriteByte(dest, registers.A);
	PC += 3;
	printf("0x%.2X: Stored A (0x%.2X) to 0x%.4X\n", opcode, registers.A, dest);
	cycles -= 16;
}

/* Loads the value at 0xFF00 plus the next 8-bit value into register A */
void Z80::LoadFFImmediateIntoA()
{
	unsigned short loc = 0xFF00 + MemoryReadByte(PC + 1);
	registers.A = MemoryReadByte(loc);
	PC += 2;
	printf("0x%.2X: Loaded value at location 0x%.2X (0x%.2X) into register A\n", opcode, loc, registers.A);
	cycles -= 12;
}

/* Disables interrupts after the execution of the NEXT instruction */
void Z80::DisableInterrupts()
{
	shouldDisableInterrupts = true;
	deferInterruptToggle = true;
	PC++;
	printf("0x%.2X: Interrupts disabled after next instruction\n", opcode);
	cycles -= 4;
}

/* Compares the value in register A with the next 8-bit value
   Zero flag is set if A = n
   AddSub Flag is set
   Half-carry flag is set if no borrow from bit 4
   Carry set if A < n */
void Z80::CompareAWithImmediate()
{
	SetAddSubFlag();
	// TODO: Half-carry borrow thing
	unsigned char n = MemoryReadByte(PC + 1);
	if (registers.A == n)
	{
		SetZeroFlag();
		printf("0x%.2X: Register A is equal to 0x%.2X\n", opcode, n);
	}
	else
	{
		if (registers.A < n)
		{
			SetCarryFlag();
		}
		printf("0x%.2X: Register A is not equal to 0x%.2X\n", opcode, n);
	}
	cycles -= 8;
	PC += 2;
}

void Z80::UnknownOp()
{
	char buffer[256];
	sprintf_s(buffer, "Unknown opcode: 0x%.2X", opcode);
	std::string message(buffer);
	QMessageBox::critical(NULL, "Unknown opcode", message.c_str());
	exit(EXIT_FAILURE);
}

/* Store the MSB of register B into the carry, then shift B left
If register B is 0, set zero flag
Clear AddSub Flag
Clear Half-carry Flag */
void Z80::ShiftBLeftIntoCarry()
{
	registers.B >> 7 == 1 ? SetCarryFlag() : ClearCarryFlag();
	registers.B <<= 1;
	registers.B == 0 ? SetCarryFlag() : ClearCarryFlag();
	ClearAddSubFlag();
	ClearHalfCarryFlag();
	PC += 2;
	printf("0x%.2X: Shifted B left into carry (%i)\n", opcode, GetCarryFlag());
}

void Z80::UnknownCB()
{
	char buffer[256];
	sprintf_s(buffer, "Unknown CB opcode: CB 0x%.2X", opcode);
	std::string message(buffer);
	QMessageBox::critical(NULL, "Unknown CB opcode", message.c_str());
	exit(EXIT_FAILURE);
}

unsigned short Z80::GetAF()
{
	return (unsigned short(registers.A) << 8 | registers.F);
}

unsigned short Z80::GetBC()
{
	return (unsigned short(registers.B) << 8 | registers.C);
}

unsigned short Z80::GetDE()
{
	return (unsigned short(registers.D) << 8 | registers.E);
}

unsigned short Z80::GetHL()
{
	return (unsigned short(registers.H) << 8 | registers.L);
}

void Z80::SetAF(unsigned short word)
{
	registers.A = (word & 0xFF00) >> 8;
	registers.F = word & 0x00FF;
}

void Z80::SetBC(unsigned short word)
{
	registers.B = (word & 0xFF00) >> 8;
	registers.C = word & 0x00FF;
}

void Z80::SetDE(unsigned short word)
{
	registers.D = (word & 0xFF00) >> 8;
	registers.E = word & 0x00FF;
}

void Z80::SetHL(unsigned short word)
{
	registers.H = (word & 0xFF00) >> 8;
	registers.L = word & 0x00FF;
}

bool Z80::GetZeroFlag()
{
	return ((registers.F >> 7) & 1);
}

bool Z80::GetAddSubFlag()
{
	return ((registers.F >> 6) & 1);
}

bool Z80::GetHalfCarryFlag()
{
	return ((registers.F >> 5) & 1);
}

bool Z80::GetCarryFlag()
{
	return ((registers.F >> 4) & 1);
}

void Z80::SetZeroFlag()
{
	registers.F |= 1 << 7;
}

void Z80::SetAddSubFlag()
{
	registers.F |= 1 << 6;
}

void Z80::SetHalfCarryFlag()
{
	registers.F |= 1 << 5;
}

void Z80::SetCarryFlag()
{
	registers.F |= 1 << 4;
}

void Z80::ClearZeroFlag()
{
	registers.F &= ~(1 << 7);
}

void Z80::ClearAddSubFlag()
{
	registers.F &= ~(1 << 6);
}

void Z80::ClearHalfCarryFlag()
{
	registers.F &= ~(1 << 5);
}

void Z80::ClearCarryFlag()
{
	registers.F &= ~(1 << 4);
}

unsigned char Z80::MemoryReadByte(unsigned short loc)
{
	return memory[loc];
}

void Z80::MemoryWriteByte(unsigned short dest, unsigned char data)
{
	if ((dest >= 0xC000 && dest <= 0xDDFF) || (dest >= 0xE000 && dest <= 0xFDFF))
	{
		// Mirroring
		if (dest >= 0xC000 && dest <= 0xDDFF)
		{
			memory[dest] = data;
			memory[dest + 0x2000] = data;
			return;
		}

		memory[dest] = data;
		memory[dest - 0x2000] = data;
		return;
	}
	memory[dest] = data;
}

unsigned short Z80::MemoryReadWord(unsigned short loc)
{
	return memory[loc + 1] << 8 | memory[loc];
}

void Z80::MemoryWriteWord(unsigned short dest, unsigned short data)
{
	if ((dest >= 0xC000 && dest <= 0xDDFE) || (dest >= 0xE000 && dest <= 0xFDFE))
	{
		// Mirroring
		if (dest >= 0xC000 && dest <= 0xDDFE)
		{
			memory[dest] = data;
			memory[dest + 0x2000] = data;
			return;
		}

		memory[dest] = data;
		memory[dest - 0x2000] = data;
		return;
	}
	memory[dest] = data;
}

Z80::~Z80()
{
}
