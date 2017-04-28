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
}

void Z80::FetchOpcode()
{
	opcode = memory[PC];
}

void Z80::DecodeOpcode()
{
	(this->*OpcodeTable[opcode])();
}

void Z80::NoOp()
{
	PC++;
	printf("0x%.2X: NOP\n", opcode);
}

void Z80::DecrementB()
{
	registers.B--;
	if (registers.B == 0)
	{
		SetZeroFlag();
	}
	SetAddSubFlag();
	// TODO: Half-carry set if no borrow from bit 4. What does this mean?
	PC++;
	printf("0x%.2X: Decremented B (0x%.2X)\n", opcode, registers.B);
}

void Z80::LoadBIntoImmediate()
{
	memory[memory[PC + 1]] = registers.B;
	printf("0x%.2X: Stored the value in register B (0x%.2X) into RAM at location 0x%.2X\n", opcode, registers.B, memory[PC + 1]);
	PC += 2;
}

void Z80::LoadCIntoImmediate()
{
	memory[memory[PC + 1]] = registers.C;
	printf("0x%.2X: Stored the value in register C (0x%.2X) into RAM at location 0x%.2X\n", opcode, registers.C, memory[PC + 1]);
	PC += 2;
}

void Z80::JumpOffsetIfNZ()
{

}

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

void Z80::LoadImmediateIntoHL()
{
	SetHL(memory[PC + 2] << 8 | memory[PC + 1]);
	PC += 3;
	printf("0x%.2X: Loaded 0x%.2X into register HL\n", opcode, GetHL());
}

void Z80::LoadAIntoHLDecrementHL()
{
	SetHL(registers.A);
	SetHL(GetHL() - 1);
	PC++;
	printf("0x%.2X: Loaded A (0x%.2X) into HL and decremented\n", opcode, registers.A);
}

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

void Z80::JumpImmediate()
{
	unsigned short dest = memory[PC + 2] << 8 | memory[PC + 1];
	PC = dest;
	printf("0x%.2X: Jump Immediate to 0x%.2X\n", opcode, dest);
}

void Z80::CBLookup()
{

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

Z80::~Z80()
{
}
