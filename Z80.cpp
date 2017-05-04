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
    
    if (isCbOp)
    {
        cycles -= 4;
        if ((opcode & 0xF) == 0x6 || (opcode & 0xF) == 0xE)
        {
            cycles -= CB6ECycles;
        }
        else
        {
            cycles -= CBCycles;
        }
    }
    else
    {
        cycles -= opCycles[opcode];
    }
}

bool Z80::IsHalfCarry(unsigned char a, unsigned char b, uint8_t result)
{
    return ((a ^ b ^ result) & 0x10) == 0x10;
}

bool Z80::IsHalfCarry(unsigned short a, unsigned short b, uint16_t result)
{
    return ((a ^ b ^ result) & 0x1000) == 0x1000;
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

/* Handles register incrementing with relevant flag setting */
void Z80::Inc()
{
    if ((opcode & 0x0F) == 0x3)
    {
        // 16-bit registers
        std::string regName;

        switch (opcode)
        {
            case 0x03:
                SetBC(GetBC() + 1);
                regName = "BC";
                break;
            case 0x13:
                SetDE(GetDE() + 1);
                regName = "DE";
                break;
            case 0x23:
                SetHL(GetHL() + 1);
                regName = "HL";
                break;
            case 0x33:
                SP--;
                regName = "Stack";
                break;
        }

        PC++;
        printf("0x%.2X: Incremented %s\n", opcode, regName.c_str());
    }
    else if (opcode != 0x34)
    {
        // 8-bit registers
        unsigned char *reg = NULL;
        char regName;

        switch (opcode)
        {
            case 0x04:
                reg = &registers.B;
                regName = 'B';
                break;
            case 0x0C:
                reg = &registers.C;
                regName = 'C';
                break;
            case 0x14:
                reg = &registers.D;
                regName = 'D';
                break;
            case 0x1C:
                reg = &registers.E;
                regName = 'E';
                break;
            case 0x24:
                reg = &registers.H;
                regName = 'H';
                break;
            case 0x2C:
                reg = &registers.L;
                regName = 'L';
                break;
            case 0x3C:
                reg = &registers.A;
                regName = 'A';
                break;
        }

        uint8_t result = *reg + 1;
        IsHalfCarry(*reg, 1, result) ? SetHalfCarryFlag() : ClearHalfCarryFlag();
        ++*reg;
        *reg == 0 ? SetZeroFlag() : ClearZeroFlag();
        ClearAddSubFlag();
        PC++;
        printf("0x%.2X: Incremented %c (0x%.2X)\n", opcode, regName, *reg);
    }
    else
    {
        // (HL)
        unsigned char data = MemoryReadByte(GetHL());
        uint8_t result = data + 1;
        IsHalfCarry(data, 1, result) ? SetHalfCarryFlag() : ClearHalfCarryFlag();
        data++;
        data == 0 ? SetZeroFlag() : ClearZeroFlag();
        ClearAddSubFlag();
        PC++;
        MemoryWriteByte(GetHL(), data);
        printf("0x%.2X: Incremented memory at address 0x%.4X (0x%.2X)\n", opcode, GetHL(), data);
    }
}

/* Handles register decrementing with relevant flag setting */
void Z80::Dec()
{
    if ((opcode & 0x0F) == 0x0B)
    {
        // 16-bit registers
        std::string regName;

        switch (opcode)
        {
            case 0x0B:
                SetBC(GetBC() - 1);
                regName = "BC";
                break;
            case 0x1B:
                SetDE(GetDE() - 1);
                regName = "DE";
                break;
            case 0x2B: // Can't see this number very well. I think I'm Nier-sighted
                SetHL(GetHL() - 1);
                regName = "HL";
                break;
            case 0x3B:
                SP--;
                regName = "Stack";
                break;
        }

        PC++;
        printf("0x%.2X: Decremented %s\n", opcode, regName.c_str());
    }
    else if (opcode != 0x35)
    {
        // 8-bit registers
        char regName;
        unsigned char *reg = nullptr;

        switch (opcode)
        {
            case 0x05:
                reg = &registers.B;
                regName = 'B';
                break;
            case 0x0D:
                reg = &registers.C;
                regName = 'C';
                break;
            case 0x15:
                reg = &registers.D;
                regName = 'D';
                break;
            case 0x1D:
                reg = &registers.E;
                regName = 'E';
                break;
            case 0x25:
                reg = &registers.H;
                regName = 'H';
                break;
            case 0x2D:
                reg = &registers.L;
                regName = 'L';
                break;
            case 0x3D:
                reg = &registers.A;
                regName = 'A';
                break;
        }

        uint8_t result = *reg - 1;
        IsHalfCarry(*reg, -1, result) ? SetHalfCarryFlag() : ClearHalfCarryFlag();
        --*reg;
        *reg == 0 ? SetZeroFlag() : ClearZeroFlag();
        SetAddSubFlag();
        PC++;
        printf("0x%.2X: Decremented %c (0x%.2X)\n", opcode, regName, *reg);
    }
    else
    {
        // (HL)
        unsigned char data = MemoryReadByte(GetHL());
        uint8_t result = data - 1;
        IsHalfCarry(data, -1, result) ? SetHalfCarryFlag() : ClearHalfCarryFlag();
        data--;
        data == 0 ? SetZeroFlag() : ClearZeroFlag();
        SetAddSubFlag();
        PC++;
        MemoryWriteByte(GetHL(), data);
        printf("0x%.2X: Decremented memory at address 0x%.4X (0x%.2X)\n", opcode, GetHL(), data);
    }
}

/* Stores the next 8-bit value into B */
void Z80::LoadImmediateIntoB()
{
	unsigned char data = MemoryReadByte(PC + 1);
	registers.B = data;
	printf("0x%.2X: Loaded 0x%.2X into B\n", opcode, data);
	PC += 2;
}

/* Stores the value at the address in BC into A */
void Z80::LoadBCIndirectIntoA()
{
	registers.A = MemoryReadByte(GetBC());
	PC++;
	printf("0x%.2X: Stored data at 0x%.4X into A (0x%.2X)\n", opcode, GetBC(), registers.A);
}

/* Stores the next 8-bit value into C */
void Z80::LoadImmediateIntoC()
{
	unsigned char data = MemoryReadByte(PC + 1);
	registers.C = data;
	printf("0x%.2X: Loaded 0x%.2X into C\n", opcode, data);
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
	// TODO: I don't think this opcode is right
	// LD (DE), A
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
	// TODO: Verify this is right. Not sure if half-carry and carry can both be set
	result > 1023 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	result > 0xFFFF ? SetCarryFlag() : ClearCarryFlag();
	PC++;
	printf("0x%.2X: Set HL to HL + DE (0x%.4X)\n", opcode, GetHL());
}

/* Rotates A to the right. Carry becomes bit 7, bit 0 stored in carry
	Z set if result is 0
	AddSub reset
	Half-carry reset
	Carry contains old bit 0 */
void Z80::RotateRightA()
{
	unsigned char carry = GetCarryFlag() << 7;
	(registers.A & 1) == 1 ? SetCarryFlag() : ClearCarryFlag();
	registers.A >>= 1;
	registers.A |= carry;
	PC++;
	printf("0x%.2X: Rotated A right through carry (0x%.2X)\n", opcode, registers.A);
}

/* If zero flag is reset, add the next 8-bit value to PC*/
void Z80::JumpOffsetIfNZ()
{
	if (!GetZeroFlag())
	{
		signed char dest = MemoryReadByte(PC + 1) + 1; // +1 here because I don't advance PC to get the next byte
		PC += dest;
		printf("0x%.2X: Zero Flag was reset, jumped to 0x%.2X\n", opcode, PC);
		cycles -= 4;
		return;
	}

	PC += 2;
	printf("0x%.2X: Zero Flag was set, did not jump\n", opcode);
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

/* If Z is set, add the next signed 8-bit value to PC */
void Z80::JumpOffsetIfZ()
{
	if (GetZeroFlag())
	{
		signed char offset = MemoryReadByte(PC + 1) + 1; // +1 here because I don't increment PC to get the next byte
		PC += offset;
		cycles -= 4;
		printf("0x%.2X: Zero flag was set, jumped to 0x%.4X\n", opcode, PC);
		return;
	}

	PC += 2;
	printf("0x%.2X: Zero flag was not set, did not jump\n", opcode);
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

/* Loads the next 8-bit value into L */
void Z80::LoadImmediateIntoL()
{
	unsigned char data = MemoryReadByte(PC + 1);
	registers.L = data;
	PC += 2;
	printf("0x%.2X: Loaded 0x%.2X into L\n", opcode, data);
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
	StackPush(data);
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
	// TODO: Not sure this is right. LD (HL-), A
}

/* Loads the next 8-bit value into HL */
void Z80::LoadImmediateIntoHL()
{
	unsigned char data = MemoryReadByte(PC + 1);
	SetHL(data);
	printf("0x%.2X: Set HL to 0x%.2X\n", opcode, data);
	PC += 2;
	// TODO: Not sure this is right. LD (HL), d8
}

/* If C is set, add the next signed 8-bit value to PC */
void Z80::JumpOffsetIfC()
{
	if (GetCarryFlag())
	{
		cycles -= 4;
		signed char offset = MemoryReadByte(PC + 1);
		PC += offset;
		printf("0x%.2X: Carry flag was set, jumped to 0x%.4X\n", opcode, PC);
	}

	PC += 2;
	printf("0x%.2X: Carry flag not set, did not jump\n", opcode);
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

/* Adds B to A 
   Zero flag set if A is 0 
   AddSub flag reset
   Half-carry flag set if carry
   Carry flag set if carry */
void Z80::AddBToA()
{
	int result = registers.A + registers.B;
	registers.A += registers.B;
	registers.A == 0 ? SetZeroFlag() : ClearZeroFlag();
	registers.A > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	result > 0xFF ? SetCarryFlag() : ClearCarryFlag();
	printf("0x%.2X: Added B (0x%.2X) to A (result: 0x%.2X)\n", opcode, registers.B, registers.A);
	PC++;
}

/* Adds C to A
   Zero flag set if A is 0
   AddSub flag reset
   Half-carry flag set if carry
   Carry flag set if carry */
void Z80::AddCToA()
{
	int result = registers.A + registers.C;
	registers.A += registers.C;
	registers.A == 0 ? SetZeroFlag() : ClearZeroFlag();
	registers.A > 15 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	result > 0xFF ? SetCarryFlag() : ClearCarryFlag();
	printf("0x%.2X: Added C (0x%.2X) to A (result: 0x%.2X)\n", opcode, registers.C, registers.A);
	PC++;
}

/* Subtract the next 8-bit immediate value, plus carry, from A
   Z set if result is 0
   N set
   H set if no borrow from bit 4
   C set if no borrow */
void Z80::SubtractNCarryFromA()
{
	unsigned char n = MemoryReadByte(PC + 1) + GetCarryFlag();
	registers.A -= n;
	registers.A < 16 ? SetHalfCarryFlag() : ClearHalfCarryFlag();
	// registers.A < 128 ? 
	// TODO: Figure carries out correctly
	PC++;
	printf("0x%.2X: Subtracted 0x%.2X plus carry from A (0x%.2X)\n", opcode, n, registers.A);
}

/* Ands A with A
   Zero flag set if A is 0
   AddSub flag cleared
   Half-carry set
   Carry cleared */
void Z80::AndAWithA()
{
	registers.A &= registers.A;
	registers.A == 0 ? SetZeroFlag() : ClearZeroFlag();
	ClearAddSubFlag();
	SetHalfCarryFlag();
	ClearCarryFlag();
	PC++;
	printf("0x%.2X: AND'd A with A (0x%.2X)\n", opcode, registers.A);
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

/* Returns from the subroutine if the zero flag is clear */
void Z80::RetIfNZ()
{
	if (!GetZeroFlag())
	{
		cycles -= 12;
		PC = StackPop();
		printf("0x%.2X: Zero flag not set, returned from subroutine\n", opcode);
		return;
	}
	PC++;
	printf("0x%.2X: Zero flag set, did not return from subroutine\n", opcode);
}


/* Jumps to the offset specified by the next 16-bit value */
void Z80::JumpImmediate()
{
	unsigned short dest = MemoryReadWord(PC + 1);
	PC = dest;
	printf("0x%.2X: Jump Immediate to 0x%.2X\n", opcode, dest);
}

/* Pushes BC to the stack */
void Z80::PushBCToStack()
{
	StackPush(GetBC());
	PC++;
	printf("0x%.2X: Pushed BC to stack\n", opcode);
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
    isCbOp = true;
	(this->*CBTable[opcode])();
}

/* Pushes address of next instruction to the stack, then jumps to the next 16-bit value */
void Z80::CallImmediate16()
{
	StackPush(PC + 3);
	PC = MemoryReadWord(PC + 1);
	printf("0x%.2X: Called method at 0x%.4X\n", opcode, PC);
}

void Z80::PushDEToStack()
{
	StackPush(GetDE());
	PC++;
	printf("0x%.2X: Pushed DE to stack\n", opcode);
}

/* Returns from the subroutine if C is set */
void Z80::ReturnIfC()
{
	if (GetCarryFlag())
	{
		cycles -= 12;
		PC = StackPop();
		printf("0x%.2X: Carry was set, returned from subroutine\n", opcode);
		return;
	}

	PC++;
	printf("0x%.2X: Carry was not set, did not return\n", opcode);
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

/* Pushes HL to the stack */
void Z80::PushHLToStack()
{
	StackPush(GetHL());
	PC++;
	printf("0x%.2X: Pushed HL to stack\n", opcode);
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

/* Pushes AF to the stack */
void Z80::PushAFToStack()
{
	StackPush(GetAF());
	MemoryWriteWord(SP, GetAF());
	PC++;
	printf("0x%2.X: Pushed AF to stack\n", opcode);
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

/* Push PC to stack, then jump to 0x0038 */
void Z80::Restart38()
{
	StackPush(PC);
	PC = 0x0038;
	printf("0x%.2X: Restart 0x38\n", opcode);
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

unsigned short Z80::StackPop()
{
	unsigned short data = memory[SP];
	SP += 2;
	return data;
}

void Z80::StackPush(unsigned short data)
{
	SP -= 2;
	memory[SP] = data;
}

Z80::~Z80()
{
}
