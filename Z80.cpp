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
	FetchOpcode();
	DecodeOpcode();

	if (ShouldDisableInterrupts)
	{
		if (!DeferInterruptToggle)
		{
			InterruptsEnabled = false;
			ShouldDisableInterrupts = false;
			printf("Interrupts disabled\n");
		}
		else
		{
			DeferInterruptToggle = false;
		}
	}

	if (ShouldEnableInterrupts)
	{
		if (!DeferInterruptToggle)
		{
			InterruptsEnabled = true;
			ShouldEnableInterrupts = false;
			printf("Interrupts enabled\n");
		}
		else
		{
			DeferInterruptToggle = false;
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
}

/* Decrements register B
   If register B is 0, set zero flag
   Set AddSub Flag
   Set Half-carry if no borrow from bit 4*/
void Z80::DecrementB()
{
	registers.B--;
	if (registers.B == 0)
	{
		SetZeroFlag();
	}
	SetAddSubFlag();
	// TODO: Half-carry set if no borrow from bit 4. What does this mean?
	// TODO: Half-carry if > 15!
	PC++;
	printf("0x%.2X: Decremented B (0x%.2X)\n", opcode, registers.B);
}

/* Stores register B into memory at the location specified by the next 8-bit value */
void Z80::LoadBIntoImmediate()
{
	MemoryWriteByte(MemoryReadByte(PC + 1), registers.B);
	printf("0x%.2X: Stored the value in register B (0x%.2X) into RAM at location 0x%.2X\n", opcode, registers.B, memory[PC + 1]);
	PC += 2;
}

/* Decrements register C
   If register C is 0, set zero flag
   Set AddSub Flag
   Set Half-carry if no borrow from bit 4*/
void Z80::DecrementC()
{
	registers.C--;
	if (registers.C == 0)
	{
		SetZeroFlag();
	}
	SetAddSubFlag();
	// TODO: Half-carry set if no borrow from bit 4. What does this mean?
	PC++;
	printf("0x%.2X: Decremented C (0x%.2X)\n", opcode, registers.C);
}

/* Stores register C into memory at the location specified by the next 8-bit value */
void Z80::LoadCIntoImmediate()
{
	MemoryWriteByte(MemoryReadByte(PC + 1), registers.C);
	printf("0x%.2X: Stored the value in register C (0x%.2X) into RAM at location 0x%.2X\n", opcode, registers.C, memory[PC + 1]);
	PC += 2;
}

/* If zero flag is reset, add the next 8-bit value to PC*/
void Z80::JumpOffsetIfNZ()
{
	if (!GetZeroFlag())
	{
		PC += memory[PC + 1];
		printf("0x%.2X: Zero Flag was reset, jumped to 0x%.2X\n", opcode, PC);
		return;
	}

	PC += 2;
	printf("0x%.2X: Zero Flag was set, did not jump\n", opcode);
}

/* Store the MSB of register B into the carry, then shift B left
   If register B is 0, set zero flag
   Clear AddSub Flag
   Clear Half-carry Flag */
void Z80::ShiftBLeftIntoCarry()
{
	registers.B >> 7 == 1 ? SetCarryFlag() : ClearCarryFlag();
	registers.B <<= 1;
	if (registers.B == 0)
	{
		SetZeroFlag();
	}
	ClearAddSubFlag();
	ClearHalfCarryFlag();
	PC += 2;
	printf("0x%.2X: Shifted B left into carry (%i)\n", opcode, GetCarryFlag());
}

/* Loads the next 16-bit value into register HL */
void Z80::LoadImmediate16IntoHL()
{
	SetHL(MemoryReadWord(PC + 1));
	PC += 3;
	printf("0x%.2X: Loaded 0x%.2X into register HL\n", opcode, GetHL());
}

/* Stores register A into register HL, then decrements HL */
void Z80::LoadAIntoHLDecrementHL()
{
	SetHL(registers.A);
	SetHL(GetHL() - 1);
	PC++;
	printf("0x%.2X: Loaded A (0x%.2X) into HL and decremented\n", opcode, registers.A);
}

/* Loads the next 8-bit value into HL */
void Z80::LoadImmediateIntoHL()
{
	unsigned char data = MemoryReadByte(PC + 1);
	SetHL(data);
	printf("0x%.2X: Set HL to 0x%.2X\n", opcode, data);
	PC += 2;
}

/* Loads the next 8-bit value into register A */
void Z80::LoadImmediateIntoA()
{
	registers.A = MemoryReadByte(PC + 1);
	PC += 2;
	printf("0x%.2X: Loaded 0x%.2X into A\n", opcode, registers.A);
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
}

/* Jumps to the offset specified by the next 16-bit value */
void Z80::JumpImmediate()
{
	unsigned short dest = MemoryReadWord(PC + 1);
	PC = dest;
	printf("0x%.2X: Jump Immediate to 0x%.2X\n", opcode, dest);
}

/* Encountered opcode CB, so use the CB function pointer table */
void Z80::CBLookup()
{
	PC++;
	FetchOpcode();
	(this->*CBTable[opcode])();
}

/* Stores register A to 0xFF00 plus the next 8-bit value */
void Z80::StoreAToFFImmediate()
{
	unsigned short dest = 0xFF00 + MemoryReadByte(PC + 1);
	MemoryWriteByte(dest, registers.A);
	PC += 2;
	printf("0x%.2X: A (0x%.2X) stored at 0x%.2X\n", opcode, registers.A, dest);
}

/* Stores register A to memory at the next 16-bit value */
void Z80::StoreAToImmediate16()
{
	unsigned short dest = MemoryReadWord(PC + 1);
	MemoryWriteByte(dest, registers.A);
	PC += 3;
	printf("0x%.2X: Stored A (0x%.2X) to 0x%.4X\n", opcode, registers.A, dest);
}

/* Loads the value at 0xFF00 plus the next 8-bit value into register A */
void Z80::LoadFFImmediateIntoA()
{
	unsigned short loc = 0xFF00 + MemoryReadByte(PC + 1);
	registers.A = MemoryReadByte(loc);
	PC += 2;
	printf("0x%.2X: Loaded value at location 0x%.2X (0x%.2X) into register A\n", opcode, loc, registers.A);
}

/* Disables interrupts after the execution of the NEXT instruction */
void Z80::DisableInterrupts()
{
	ShouldDisableInterrupts = true;
	DeferInterruptToggle = true;
	PC++;
	printf("0x%.2X: Interrupts disabled after next instruction\n", opcode);
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
	memory[dest] = data;
}

unsigned short Z80::MemoryReadWord(unsigned short loc)
{
	return memory[loc + 1] << 8 | memory[loc];
}

void Z80::MemoryWriteWord(unsigned short dest, unsigned short data)
{
	memory[dest] = data;
}

Z80::~Z80()
{
}
