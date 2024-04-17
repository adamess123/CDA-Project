#include "spimcore.h"

// Naziat C.
/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{
    switch(ALUControl) {
        // A + B case
        case '0':
            *ALUresult = A + B;
            break;

        // A - B case
        case '1':
            *ALUresult = A - B;
            break;

        // signed
        case '2':
            if ((signed) A < (signed) B) {
                *ALUresult = 1;
            }
            else {
                *ALUresult = 0;
            }
            break;

        // Z = 1 if A less than B, otherwise z = 0 - A and B unsigned int
        case '3':
            if (A < B) {
                *ALUresult = 1;
            }
            else {
                *ALUresult = 0;
            }
            break;

        // Z and B case
        case '4':
            *ALUresult = A & B;
            break;

        // Z or B case
        case '5':
            *ALUresult = A | B;
            break;

        // Z is shifted left 16 bits
        case '6':
            *ALUresult = B << 16;
            break;

        // Z = not a
        case '7':
            *ALUresult = -A;
            break;
    }
    // if alu result = 0 set to 1, otherwise 0
    if (*ALUresult == 0) {
        *Zero = 1;
    } 
    else {
        *Zero = 0;
    }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{
    if (PC % 4 == 0) {
        *instruction = Mem[ PC >> 2];
        return 0;
    }

    return 1;

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{
    *op = instruction >> 26;
    // r1 - instruction 31 - 26
    // r2 instruction 20-16
    // r3 instruction 15-11
    *r1 = instruction >> 21 & 0x1F;
    *r2 = instruction >> 16 & 0x1F;
    *r3 = instruction >> 11 & 0x1F;

    *funct = instruction & 0x3F;
    *jsec = instruction & 0x3FFFFFF;
    *offset = instruction & 0xFFFF;


}



/* instruction decode */
/* 15 Points */
//Grace Rudie
int instruction_decode(unsigned op,struct_controls *controls)
{
    //use controls struct to initalize the controls to 2 for the "dont care" situtation that will be left if needed
    controls->RegDst = 2;
    controls->Jump = 2;
    controls->Branch = 2;
    controls->MemRead = 2;
    controls->MemtoReg = 2;
    controls->MemWrite = 2;
    controls->ALUSrc = 2;
    controls->RegWrite = 2;
    //not set to 2 because 2 has a different value that does not represent "don't care"
    controls->ALUOp = 0;

    //check the opcode to see which type of instruction
    //R-Type Instruction (for add, sub, and, or, slt) 0000 0000
    if(op == 0x0){
        //enable or indicate selected path with 1 
        controls->RegDst = 1;
        controls->RegWrite = 1;
        //Disable following for R-Type
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        //7 is binary value of 111 which is the instruction for R-type
        controls->ALUOp = 7;
    }

    //Next conditions are checking for I-type instructions
    //if the op value is addi 0000 1000
    else if(op == 0x8){
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
        //disable following for addi
        controls->MemRead = 0;
        controls->MemWrite = 0;
    }
    //if the op value is lw 0010 0011
    else if(op == 0x23){
        controls->RegWrite = 1;
        controls->MemRead = 1;
        controls->MemtoReg = 1;
        controls->ALUSrc = 1;
        //disabel following for lw
        controls->MemWrite = 0;
    }
    //if the op value is sw 101011
    else if(op == 0x2b){
        controls->MemWrite = 1;
        controls->ALUSrc = 1;
        //disable for sw
        controls->MemRead = 0;
        controls->RegWrite = 0;
    }
    //if the op value is for lui 0000 1111
    else if(op == 0xf){
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
        //disable following for lui
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->ALUSrc = 0;
        //6 binary value is 110 used for shift left 
        controls->ALUOp = 6;
    }
    //if op value is beq 0000 0100
    else if(op == 0x4){
        controls->Branch = 1;
        //disable following for beq
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->RegWrite = 0;
        controls->ALUSrc = 0;
        //1 binary value is 001 used for subtractoin in the branching
        controls->ALUOp = 1;
    }
    //if op value is slti 0000 1010
    else if(op == 0xa){
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
        //disable for slti
        controls->MemRead = 0;
        controls->MemWrite = 0;
        //2 binary value is 010 used for set less than 
        controls->ALUOp = 2;
    }
    //if op value is sltiu 0000 1011
    else if(op == 0xb){
        controls->RegWrite = 1;
        controls->ALUSrc = 1;
        //disable for sltiu
        controls->MemRead = 0;
        controls->MemWrite = 0;
        //3 binary value is 011 used for set less than unsighed
        controls->ALUOp = 3;
    }

    //J-Type Instructions
    //if op value is j 0000 0010
    else if(op == 0x2){
        controls->Jump = 1;
        //disable for j
        controls->MemRead = 0;
        controls->MemWrite = 0;
        controls->RegWrite = 0;
        controls->ALUSrc = 1;
    }

    //if an illegal instruction is encountered halt
    else{
        return 1;
    }

    //if one of the conditions was read then no halt needed 
    return 0;
}

/* Read Register */
/* 5 Points */
// Adam Essaydi
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2)
{
    // Read the registers r1 and r2 from Reg, then writes the read values to data1 and data2
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}


/* Sign Extend */
/* 10 Points */
//Grace Rudie
void sign_extend(unsigned offset,unsigned *extended_value)
{
    //offest is a 16-bits therefore to check most significant bit use right shift to see if it is one(meaning negative)
    if(offset >> 15 == 1){
        *extended_value = offset | 0xfffff0000;
    }
    //if MSB is not 1, then sign extension not needed(positive)
    else{
        *extended_value = offset & 0x0000ffff;
    }
}

/* ALU operations */
/* 10 Points */
// Grace Rudie
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero){
    
    unsigned temp;              //stores the results so that the pointer does not change until the end of the function
    unsigned ALUControl;        //used to store what will be based to the ALU function call and will be changed based on the values of ALUOp and funct

    //determine based on ALUSrc if data2 or extend_value should be used
    if(ALUSrc == 0){
        temp = data2;
    }else{
        temp = extended_value;
    }

    //now set operations based on ALUOp
    //for R-Type
    if(ALUOp == 7){
        //for add instruction
        if(funct == 0x20){
            ALUControl = 0;
        }
        //for sub instruction
        else if(funct == 0x22){
            ALUControl = 1;
        }
        //for slt(signed)
        else if(funct == 0x2a){
            ALUControl = 2;
        }
        //for slt(unsigned)
        else if(funct == 0x2b){
            ALUControl = 3;
        }
        //for AND instruction
        else if(funct == 0x24){
            ALUControl = 4;
        }
        //for OR instruction
        else if(funct == 0x25){
            ALUControl = 5;
        }
        //for invalid funct value so therefore halt condition
        else{
            return 1;
        }
    }
    //for non R-Type instructions 
    else{
        ALUControl = ALUOp;
    }

    //Now to perform the ALU operations 
    ALU(data1, temp, ALUControl, ALUresult, Zero);

    return 0;           //because no halt condition occcured 
}

/* Read / Write Memory */
/* 10 Points */
// Adam Essaydi
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


/* Write Register */
/* 10 Points */
// Adam Essaydi
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg)
{
    if(RegWrite == 1){
        // If info from memory and regdst of 0, then set reg of r2 to memdata
        if (MemtoReg == 1 && RegDst == 0)
            Reg[r2] = memdata;

        // if info from memory and regdst of 1, set reg of r3 to memdata
        else if(MemtoReg == 1 && RegDst == 1)
            Reg[r3] = memdata;

        // if info from reg and regdst of 1, set r3 to aluresult (r-type)
        else if (MemtoReg == 0 && RegDst == 1)
            Reg[r3] = ALUresult;

        // if info from reg and regdst of 0, set r2 to aluresult (i-type)
        else if (MemtoReg == 0 && RegDst == 0)
            Reg[r2] = ALUresult;
    }
}

/* PC update */
/* 10 Points */
// Adam Essaydi
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

