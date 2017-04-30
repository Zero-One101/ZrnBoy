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

/* Loads the next 16-bit value into BC */
void Z80::LoadImmediate16IntoBC()
{
	SetBC(MemoryReadWord(PC + 1));
	PC += 3;
	printf("0x%.2X: Set BC to 0x%.4X\n", opcode, GetBC());
}

/* Increments BC */
void Z80::IncBC()
{
	SetBC(GetBC() + 1);
	PC++;
	printf("0x%.2X: Incremented BC (0x%.4X)\n", opcode, GetBC());
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

/* Stores the value at the address in BC into A */
void Z80::LoadBCIndirectIntoA()
{
	registers.A = MemoryReadByte(GetBC());
	PC++;
	printf("0x%.2X: Stored data at 0x%.4X into A (0x%.2X)\n", opcode, GetBC(), registers.A);
}

/* Decrements register BC*/
void Z80::DecBC()
{
	SetBC(GetBC() - 1);
	PC++;
	printf("0x%.2X: Decremented BC (0x%.4X)\n", opcode, GetBC());
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
	// TODO: Half-carry
	PC++;
	printf("0x%.2X: Incremented C (0x%.2X)\n", opcode, registers.C);
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

/* Halts CPU and LCD until button pressed */
void Z80::Stop()
{
	PC += 2;
	stopped = true;
	printf("0x%.2X: CPU STOP\n", opcode);
}

/* Loads the next 16-bit immediate value into DE */
void Z80::LoadImmediate16IntoDE()
{
	unsigned short data = MemoryReadWord(PC + 1);
	SetDE(data);
	PC += 3;
	printf("0x%.2X: Set DE equal to (0x%.4X)\n", opcode, data);
}

/* Stores A into DE */
void Z80::LoadAIntoDE()
{
	SetDE(registers.A);
	PC++;
	printf("0x%.2X: Set DE equal to A (0x%.4X)\n", opcode, registers.A);
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
	// TODO: Half-carry
	PC++;
	printf("0x%.2X: Decremented D (0x%.2X)\n", opcode, registers.D);
}

/* Add the next signed 8-bit value to the PC */
void Z80::JumpSignedImmediate()
{
	signed char offset = MemoryReadByte(PC + 1);
	PC += offset;
	printf("0x%.2X: Signed immediate jump to 0x%.4X\n", opcode, PC);
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
	result > 0xFFFF ? SetCarryFlag() : ClearCarryFlag();
	PC++;
	// TODO: Half-carry
	printf("0x%.2X: Set HL to HL + DE (0x%.4X)\n", opcode, GetHL());
}

/* Decrements DE */
void Z80::DecDE()
{
	SetDE(GetDE() - 1);
	PC++;
	printf("0x%.2X: Decremented DE (0x%.4X)\n", opcode, GetDE());
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
	// TODO: Half-carry
	PC++;
	printf("0x%.2X: Incremented E (0x%.2X)\n", opcode, registers.E);
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
	// TODO: Half-carry
	PC++;
	printf("0x%.2X: Decremented E (0x%.2X)\n", opcode, registers.E);
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
	registers.B == 0 ? SetCarryFlag() : ClearCarryFlag();
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

/* Loads the next 8-bit value into register H */
void Z80::LoadImmediateIntoH()
{
	unsigned char data = MemoryReadByte(PC + 1);
	registers.H = data;
	PC += 2;
	printf("0x%.2X: Loaded 0x%.2X into H\n", opcode, data);
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
	// TODO: Half-carry
	PC++;
	printf("0x%.2X: Added HL to itself (0x%.4X)\n", opcode, GetHL());
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
}

/* Pushes the next 16-bit value onto the stack */
void Z80::PushImmediate16()
{
	unsigned short data = MemoryReadWord(PC + 1);
	MemoryWriteWord(SP, data);
	SP -= 2;
	printf("0x%.2X: Pushed 0x%.2X onto stack\n", opcode, data);
	PC += 3;
}

/* Stores register A into register HL, then decrements HL */
void Z80::LoadAIntoHLDecHL()
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

/* Puts the value of B into A */
void Z80::LoadBIntoA()
{
	registers.B = registers.A;
	PC++;
	printf("0x%.2X: B set to A (0x%.2X)\n", opcode, registers.B);
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
}

/* Jumps to the offset specified by the next 16-bit value */
void Z80::JumpImmediate()
{
	unsigned short dest = MemoryReadWord(PC + 1);
	PC = dest;
	printf("0x%.2X: Jump Immediate to 0x%.2X\n", opcode, dest);
}

/* Pops two bytes from the stack and jumps to that address */
void Z80::Return()
{
	PC = MemoryReadWord(SP);
	SP += 2;
	printf("0x%.2X: Returned from subroutine\n", opcode);
}

/* Encountered opcode CB, so use the CB function pointer table */
void Z80::CBLookup()
{
	PC++;
	FetchOpcode();
	(this->*CBTable[opcode])();
}

/* Pushes address of next instruction to the stack, then jumps to the next 16-bit value */
void Z80::CallImmediate16()
{
	MemoryWriteWord(SP, PC + 3);
	SP -= 2;
	PC = MemoryReadWord(PC + 1);
	printf("0x%.2X: Called method at 0x%.4X\n", opcode, PC);
}

/* Stores register A to 0xFF00 plus the next 8-bit value */
void Z80::StoreAToFFImmediate()
{
	unsigned short dest = 0xFF00 + MemoryReadByte(PC + 1);
	MemoryWriteByte(dest, registers.A);
	PC += 2;
	printf("0x%.2X: A (0x%.2X) stored at 0x%.2X\n", opcode, registers.A, dest);
}

/* Stores register A to 0xFF00 plus register C */
void Z80::StoreAToFFPlusC()
{
	unsigned short loc = 0xFF00 + registers.C;
	MemoryWriteByte(loc, registers.A);
	PC++;
	printf("0x%.2X: Wrote A (0x%.2X) to 0x%.2X\n", opcode, registers.A, loc);
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
	shouldDisableInterrupts = true;
	deferInterruptToggle = true;
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
