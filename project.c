/* Group members:
    Naziat Chowdhury
   Grace Rudie
   Adam Essaydi
   
   CDA3103 Project */ 

#include "spimcore.h"

// Naziat Chowdhury
/* ALU - simulates the operations of an ALU 
    * implements operations based on A and B parameters in accordance to ALU control in the given table
    * outputs result to ALUresult */
void ALU(unsigned A,unsigned B,char ALUOp,unsigned *ALUresult,char *Zero)
{   // switch-case handles operations based on ALUOp
    switch(ALUOp) {
        case 0: // A + B
            *ALUresult = A + B;
            break;

        case 1: // A - B 
            *ALUresult = A - B;
            break;

        case 2: // Compares signed A and B return 1 if A < B, else 0
            if ((signed) A < (signed) B) {
                *ALUresult = 1;
            }
            else {
                *ALUresult = 0;
            }
            break;

        case 3: // Compares unsigned A and B return 1 if A < B, else 0
            if (A < B) {
                *ALUresult = 1;
            }
            else {
                *ALUresult = 0;
            }
            break;

        case 4: // A and B
            *ALUresult = A & B; 
            break;

        case 5: // A or B
            *ALUresult = A | B;
            break;

        case 6: // B is shifted left 16 bits
            *ALUresult = B << 16;
            break;

        case 7: // Not A
            *ALUresult = ~A;
            break;
    }
    // Checks if ALU result = 0 set to 1, otherwise 0
    if (*ALUresult == 0) {
        *Zero = 1;
    } 
    else {
        *Zero = 0;
    }
}

/* Instruction fetch - Fetches the instruction addresses by PC from mem, writes to instruction
    * halt condition: returns 1  */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if (PC % 4 == 0) {
        *instruction = Mem[PC >> 2]; // Right shift PC 2 bits, divides by 4 and use as index
        return 0;
    }

    return 1;

}


/* Instruction partition -  Parititons instruction into parts - each field extracted based on bit positions */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = instruction >> 26;
    *r1 = (instruction >> 21) & 0x1F;
    *r2 = (instruction >> 16) & 0x1F;
    *r3 = (instruction >> 11 )& 0x1F;

    *funct = instruction & 0x3F;
    *jsec = instruction & 0x3FFFFFF;
    *offset = instruction & 0xFFFF;
}

// Grace Rudie
/* Instruction decode - decodes the instruction using the opcode
   * Assigns values of the control signals to the variables in controls struct */
int instruction_decode(unsigned op, struct_controls *controls) {
    if (op == 0) {
        //enable or indicate selected path with 1
        controls->RegDst = 1;
        controls->RegWrite = 1;
        //Disable following for R-Type
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->MemtoReg = 0;
        controls->ALUSrc = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        //7 is binary value of 111 which is the instruction for R-type
        controls->ALUOp = 7;
    }
    //Next conditions are checking for I-type instructions
    //if the op value is addi 0000 1000
    else if (op == 8) {
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
        //disable following for addi
        controls->MemRead = 0;
        controls->MemtoReg = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        //0 is binary of 000 which is instruction for andi
        controls->ALUOp = 0;
    }
    //if the op value is lw 0010 0011
    else if (op == 35) {
        controls->RegWrite = 1;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUSrc = 1;
        //disable following for lw
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        //0 for addition
        controls->ALUOp = 0;
    }
    //if the op value is sw 101011
    else if (op == 43) {
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        //disable for sw
        controls->MemRead = 0;
        controls->RegWrite = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        //0 in binary for addition
        controls->ALUOp = 0;
        // Don't care values
        controls->RegDst = 2;
        controls->MemtoReg = 2;
    }
    //if the op value is for lui 0000 1111
    else if (op == 15) {
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
        //disable following for lui
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        controls->Branch = 0;
        controls->Jump = 0;
        controls->MemtoReg = 0;
        //6 binary value is 110 used for shift left
        controls->ALUOp = 6;
    }
    //if op value is beq 0000 0100
    else if (op == 4) {
        controls->Branch = 1;
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->RegWrite = 0;
        controls->ALUSrc = 0;
        controls->Jump = 0;
        //1 in binary for subtraction
        controls->ALUOp = 1;
        // Don't care values
        controls->RegDst = 0;
        controls->MemtoReg = 0;
    }
     //if op value is slti 0000 1010
    else if (op == 10) {
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
         //disable for slti
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemtoReg = 0;
        controls->RegDst = 0;
         //2 binary value is 010 used for set less than
        controls->ALUOp = 2;
    }
    //if op value is sltiu 0000 1011
    else if (op == 11) {
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
         //disable for sltiu
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->RegDst = 0;
        controls->Jump = 0;
        controls->Branch = 0;
        controls->MemtoReg = 0;
        //3 binary value is 011 used for set less than unsighed
        controls->ALUOp = 3;
    }
    // J-Type Instructions
    else if (op == 2) {
        controls->Jump = 1;
        controls->ALUSrc = 1;
         //disable for j
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->RegWrite = 0;
    }
    //if an illegal instruction is encountered halt
    else {
        return 1;
    }
    //if one of the conditions was read then no halt needed
    return 0;
}

// Adam Essaydi
/* Read Register - reads the registers addressed by r1 and r2
   * writes the read values to data1 and data2  */
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Read the registers r1 and r2 from Reg, then writes the read values to data1 and data2
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

// Grace Rudie
/* Sign Extend - Assigns the sign-extended value of offset to extended_value */
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //offest is a 16-bits therefore to check most significant bit use right shift to see if it is one(meaning negative)
    if(offset >> 15 == 1){
        *extended_value = offset | 0xFFFFF0000;
    }
    //if MSB is not 1, then sign extension not needed(positive)
    else{
        *extended_value = offset & 0x0000FFFF;
    }
}

// Grace Rudie
/* ALU operations - Applies ALU operations on data1, data2, extend_value */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero){
    //determine based on ALUSrc if data2 or extend_value should be used
    if(ALUSrc == 1)
        data2 = extended_value;

    //now set operations based on ALUOp
    //for R-Type
    if(ALUOp == 7){
        //for add instruction
        if(funct == 0x20)
            ALUOp = 0;

        //for sub instruction
        else if(funct == 0x22)
            ALUOp = 1;

        //for slt(signed)
        else if(funct == 0x2a)
            ALUOp = 2;

        //for slt(unsigned)
        else if(funct == 0x2b)
            ALUOp = 3;

        //for AND instruction
        else if(funct == 0x24)
            ALUOp = 4;

        //for OR instruction
        else if(funct == 0x25)
            ALUOp = 5;

        //shift by 16
        else if(funct == 0x4)
            ALUOp = 6;

        //Z = ~A
        else if(funct == 0x0)
            ALUOp = 7;

        //for invalid funct value so therefore halt condition
        else
            return 1;

    }
    //for non R-Type instructions

    //Now to perform the ALU operations
    ALU(data1, data2, ALUOp, ALUresult, Zero);

    return 0;
}

// Adam Essaydi
/* Read / Write Memory - Uses memwrite/memread value to know if a memory write, memory read or neither is happening  */
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem)
{
    // Bad address will stop the function
    if ((MemWrite == 1 || MemRead == 1) && ALUresult % 4 != 0)
        return 1;

    // If MemRead == 1, set memdata to ALUresult shifted by 2 bits
    if (MemRead == 1)
        *memdata = Mem[ALUresult >> 2];

    // If MemRead == 1, set memory of aluresult to data2
    if (MemWrite == 1)
        Mem[ALUresult >> 2] = data2;

    return 0;
}

// Adam Essaydi
/* Write Register - Writes the data to a register addressed by r2 or r3 */
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if (RegWrite == 1) {
        // handle register writes
        if (MemtoReg == 1) {
            if (RegDst == 0)
                Reg[r2] = memdata;  // write memory data to reg r2 if RegDst is 0

            else if (RegDst == 1)
                Reg[r3] = memdata;  // write memory data to reg r3 if RegDst is 1
        }

        else if (MemtoReg == 0) {
            if (RegDst == 0)
                Reg[r2] = ALUresult;  // write ALUresult to reg r2 if RegDst is 0 (I-type)

            else if (RegDst == 1)
                Reg[r3] = ALUresult;  // write ALUresult to reg r3 if RegDst is 1 (R-type)
        }
    }

}

// Adam Essaydi
/* PC update - Function to update program counter */
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC)
{
    // increment the PC by 4 each time update is ran
    *PC += 4;

    // If jumping, then we shift the instructions and combine that into the pc
    if(Jump == 1){
        *PC = (jsec << 2) | (*PC & 0xf0000000);
    }

    // If we are branching and we have a zero then we add the extended value
    if(Branch == 1 && Zero == 1){
        *PC += extended_value << 2;
    }
}

