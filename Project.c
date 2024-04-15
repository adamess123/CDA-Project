#include "spimcore.h"


/* ALU */
/* 10 Points */
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero)
{

}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction)
{

}


/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec)
{

}



/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op,struct_controls *controls)
{

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
void sign_extend(unsigned offset,unsigned *extended_value)
{

}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero)
{

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

