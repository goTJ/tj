// vim:ft=c:foldmethod=marker
#include<stdio.h>

int n;	// the number of instructions
long int dm[4000], im[4000];	// dm: data memory, im: instructions memory
long int reg[32];
long int pc;
long int inst_count;
long int cycle_count;
char *ALUOpName[6] = {"", "add", "sub", "and", "or", "slt"};
enum DecodeStatus{NORMAL, STALL, BRANCH};

// {{{ struct IFID{
struct IFID{
	long int ir;
	long int pc;
}ifid; // }}}
// {{{ struct IDEX{
struct IDEX{
	enum DecodeStatus status;
	long int a;
	long int b;
	long int rt;
	long int rs;
	long int rd;
	long int imm;
	long int pc;
	long int RegDst;
	long int ALUSrc;
	long int ALUOp;
	long int PCSrc;
	long int MemRead;
	long int MemWrite;
	long int RegWrite;
	long int MemtoReg;
}idex; // }}}
// {{{ struct EXMEM{
struct EXMEM{
	long int b;
	long int rd;
	long int aluout;
	long int MemRead;
	long int MemWrite;
	long int RegWrite;
	long int MemtoReg;
}exmem, before_em; // }}}
// {{{ struct MEMWB{
struct MEMWB{
	long int rd;
	long int mdr;
	long int aluout;
	long int RegWrite;
	long int MemtoReg;
}memwb, before_mw; // }}}
// {{{ struct Hazard{
struct Hazard{
	long int pcw;
	long int ifidw;
}hazard; // }}}
struct IDEX result;

// {{{ void flushIDEX()
void flushIDEX()
{
	idex.a = 0;
	idex.b = 0;
	idex.rs = 0;
	idex.rt = 0;
	idex.rd = 0;
	idex.imm = 0;
	idex.pc = 0;
	idex.RegDst=0;
	idex.ALUSrc=0;
	idex.ALUOp=0;
	idex.PCSrc=0;
	idex.MemRead=0;
	idex.MemWrite=0;
	idex.RegWrite=0;
	idex.MemtoReg=0;
} // }}}
// {{{ void flushIFID()
void flushIFID()
{
	ifid.ir = 0;
	ifid.pc = 0;
} // }}}
// {{{ void writeback()
void writeback()
{
////	printf("writeback: rd(%d) mdr(%d) alo(%d) %s%s\n", memwb.rd, memwb.mdr, memwb.aluout, memwb.RegWrite?"rw ":"", memwb.MemtoReg?"mtr ":"");
	if(memwb.RegWrite)
		reg[memwb.rd] = (memwb.MemtoReg) ? memwb.mdr : memwb.aluout;
	if(memwb.RegWrite || memwb.rd)
		inst_count++;
} // }}}
// {{{ void mem_access()
void mem_access()
{
////	printf("mem_access: b(%d) rd(%d) alo(%d) %s%s%s%s\n",
////		exmem.b, exmem.rd, exmem.aluout, exmem.MemRead?"mr ":"",
////		exmem.MemWrite?"mw ":"", exmem.RegWrite?"rw ":"", exmem.MemtoReg?"mtr ":"");
	before_mw = memwb;
	if(exmem.MemRead){
		memwb.mdr = dm[exmem.aluout/4];	// because one offset represents 4 bytes(by long int).
	}
	if(exmem.MemWrite){
		dm[exmem.aluout/4] =  exmem.b;
		exmem.rd = 1;	// for counting instruction
	}
	memwb.rd = exmem.rd;
	memwb.aluout = exmem.aluout;
	memwb.RegWrite = exmem.RegWrite;
	memwb.MemtoReg = exmem.MemtoReg;
} // }}}
// {{{ void execute()
void execute()
{
////	printf("excute: a(%d) b(%d) rt(%d) rs(%d) rd(%d) imm(%d) pc(%d) RD(%s) AS(%s) AOP(%s) PCSrc(%d) %s%s%s%s\n"
////		, idex.a, idex.b, idex.rt, idex.rs, idex.rd, idex.imm, idex.pc
////		, idex.RegDst?"rd":"rt", idex.ALUSrc?"imm":"rt", ALUOpName[idex.ALUOp], idex.PCSrc,
////		idex.MemRead?"mr ":"", idex.MemWrite?"mw ":"", idex.RegWrite?"rw ":"", idex.MemtoReg?"mtr ":"");
	before_em = exmem;
	int operandA;
	int operandB;
	int tempB;
	int forwardA=0,forwardB=0;
	//{{{Fowarding
	if(exmem.RegWrite && exmem.rd && exmem.rd == idex.rs)
		forwardA = 2;
	if(exmem.RegWrite && exmem.rd && exmem.rd == idex.rt)
		forwardB = 2;

	if(before_mw.RegWrite && before_mw.rd && exmem.rd != idex.rs && before_mw.rd == idex.rs)
		forwardA=1;
	if(before_mw.RegWrite && before_mw.rd && exmem.rd != idex.rt && before_mw.rd == idex.rt)
		forwardB=1;
//	printf("\tforward A: %d  B: %d\n", forwardA, forwardB);
	switch(forwardA)
	{
		case 0:
			operandA = idex.a;
			break;
		case 1:
		//	printf("forward A: %d %d %d\n", before_mw.MemtoReg, before_mw.mdr, before_mw.aluout);
			operandA = before_mw.MemtoReg?before_mw.mdr:before_mw.aluout;
			break;
		case 2:
			operandA = exmem.aluout;
			break;
	}
	switch(forwardB)
	{
		case 0:
			tempB = idex.b;
			break;
		case 1:
		//	printf("forward B: %d %d %d\n", before_mw.MemtoReg, before_mw.mdr, before_mw.aluout);
			tempB = before_mw.MemtoReg?before_mw.mdr:before_mw.aluout;
			break;
		case 2:
			tempB = exmem.aluout;
			break;
	}
	operandB = (idex.ALUSrc)?idex.imm:tempB;
	//}}}
	switch(idex.ALUOp)
	{
		case 1:
			exmem.aluout = operandA + operandB;
			break;
		case 2:
			exmem.aluout = operandA - operandB;
			break;
		case 3:
			exmem.aluout = operandA & operandB;
			break;
		case 4:
			exmem.aluout = operandA | operandB;
			break;
		case 5:
			exmem.aluout = (operandA<operandB)?1:0;
			break;
	}
	exmem.b=tempB;
	//{{{Give to next stage
	exmem.rd = (idex.RegDst)?idex.rd:idex.rt;
	exmem.MemRead = idex.MemRead;
	exmem.MemWrite = idex.MemWrite;
	exmem.RegWrite = idex.RegWrite;
	exmem.MemtoReg = idex.MemtoReg;
	//}}}
} // }}}
// {{{ void decode()
void decode()
{
////	printf("decode: ir(%x) pc(%d)\n", ifid.ir, ifid.pc);
	int opcode, funct;
	int rs, rt, rd, imm, rsvalue, rtvalue;

	if(!ifid.ir){
		idex.status = STALL;
		result = idex;
		flushIDEX();
		return;
	}
	idex.status = NORMAL;
	rs = (ifid.ir&0x03e00000) >> 21;
	rt = (ifid.ir&0x001f0000) >> 16;
	rd = (ifid.ir&0x0000f800) >> 11;
	imm = (ifid.ir&0x00008000)?((ifid.ir&0x0000ffff)|0xffff0000):(ifid.ir&0x0000ffff);

	rsvalue = reg[rs];
	rtvalue = reg[rt];
	
	// {{{ harzard unit (stall)
	if (idex.MemRead && (idex.rt==rs || idex.rt == rt )){
		idex.status = STALL;
		flushIDEX();
		hazard.pcw = 0;
		hazard.ifidw = 0;
		result = idex;
		return ;
	} // }}}

	opcode = (ifid.ir&0xfc000000) >> 26;
	funct = (ifid.ir&0x0000003f);
//	printf("opcode: %d\n", opcode);
	switch ( opcode ){
		case 0 : // {{{ r-format
			idex.RegDst = 1;
			idex.ALUSrc = 0;
			idex.MemRead = 0;
			idex.MemWrite = 0;
			idex.RegWrite = 1;
			idex.MemtoReg = 0;
			idex.PCSrc = 0;

			// aluop 1:+, 2:-, 3:and, 4:or, 5:slt
			if ( funct == 0x20 )
				idex.ALUOp = 1;
			else if (funct == 0x22 )
				idex.ALUOp = 2;
			else if (funct == 0x24)
				idex.ALUOp = 3;
			else if ( funct == 0x25 )
				idex.ALUOp = 4;
			else if ( funct == 0x2a )
				idex.ALUOp = 5;
			break; // }}}
		case 35 : // {{{ lw
			idex.RegDst = 0;
			idex.ALUSrc = 1;
			idex.MemRead = 1;
			idex.MemWrite = 0;
			idex.RegWrite = 1;
			idex.MemtoReg = 1;
			idex.PCSrc = 0;
			idex.ALUOp = 1;
			break; // }}}
		case 43 : // {{{ sw
			idex.ALUSrc = 1;
			idex.MemRead = 0;
			idex.MemWrite = 1;
			idex.RegWrite = 0;
			idex.PCSrc = 0;
			idex.ALUOp = 1;
			break; // }}}
		case 4 : // {{{ beq (case 5: bne)
		case 5: // bne
			// {{{ stall for execute stage
			if ( exmem.RegWrite == 1 ){
				if( exmem.rd == rs || exmem.rd == rt){
					hazard.ifidw = 0;
					hazard.pcw = 0;
					flushIDEX();
					idex.status = STALL;
					result = idex;
					return ;
				}
			} // }}}
			// {{{ stall for mem_access stage
			if ( before_em.RegWrite == 1 && before_em.MemRead == 1){
				if ( before_em.rd == rs || before_em.rd == rt){
					hazard.ifidw = 0;
					hazard.pcw = 0;
					flushIDEX();
					idex.status = STALL;
					result = idex;
					return;
				}
			} // }}}
			// {{{ forward for mem_acess stage
			if ( before_em.RegWrite == 1 && before_em.MemRead == 0){
				if ( before_em.rd == rs )
					rsvalue = before_em.aluout;
				if ( before_em.rd == rt )
					rtvalue = before_em.aluout;
			}
			// }}}}
			
////			printf("gogo bne/beq\n");
			if (opcode == 4 && rsvalue == rtvalue ){
				pc = imm+ifid.pc;
			}else if (opcode == 5 && rsvalue != rtvalue ){
				pc = imm + ifid.pc;
			}
			hazard.pcw = 0;
			hazard.ifidw = 0;
			inst_count++;
			flushIFID();
			flushIDEX();
			idex.status = BRANCH;
			result = idex;
			return;
			break; // }}}
		case 8: // {{{ addi
			idex.RegDst = 0;
			idex.ALUSrc = 1;
			idex.MemRead = 0;
			idex.MemWrite = 0;
			idex.RegWrite = 1;
			idex.MemtoReg = 0;
			idex.PCSrc = 0;
			idex.ALUOp = 1;
			break; // }}}
		case 0xc: // {{{ andi
			idex.RegDst = 0;
			idex.ALUSrc = 1;
			idex.MemRead = 0;
			idex.MemWrite = 0;
			idex.RegWrite = 1;
			idex.MemtoReg = 0;
			idex.PCSrc = 0;
			idex.ALUOp = 3;
			break; // }}}
		case 0xd: // {{{ ori
			idex.RegDst = 0;
			idex.ALUSrc = 1;
			idex.MemRead = 0;
			idex.MemWrite = 0;
			idex.RegWrite = 1;
			idex.MemtoReg = 0;
			idex.PCSrc = 0;
			idex.ALUOp = 4;
			break; // }}}
		case 0xa: // {{{ slti
			idex.RegDst = 0;
			idex.ALUSrc = 1;
			idex.MemRead = 0;
			idex.MemWrite = 0;
			idex.RegWrite = 1;
			idex.MemtoReg = 0;
			idex.PCSrc = 0;
			idex.ALUOp = 5;
			break; // }}}
		case 2: // {{{ j
			pc = (ifid.pc&0xf0000000)+(ifid.ir&0x03ffffff);
////			printf("jump to pc: %d\n", pc);
			hazard.pcw = 0;
			hazard.ifidw = 0;
			inst_count++;
			flushIFID();
			flushIDEX();
			idex.status = BRANCH;
			result = idex;
			return;
			break; // }}}
	}
	idex.rs = rs;
	idex.a = reg[rs];
	idex.rt = rt;
	idex.b = reg[rt];
	idex.rd = rd;
	idex.imm = imm;
	idex.pc = ifid.pc;

	result = idex;
} // }}}
// {{{ void fetch()
int fetch()
{
////	printf("fetch: pc(%d) %s\n", pc, hazard.ifidw?"":"stall");
	if(pc >= n || pc < 0){
		if(hazard.ifidw){
			ifid.ir = 0;
			ifid.pc = 0;
			return 0;
		}else{
			hazard.ifidw = 1;
			hazard.pcw = 1;
			return 1;
		}
	}
	if(hazard.ifidw){
		ifid.ir = im[pc];
		ifid.pc = pc+1;
	}else{
		hazard.ifidw = 1;
	}
	if(hazard.pcw){
		pc += 1;
	}else{
		hazard.pcw = 1;
	}
	return 1;
} // }}}
// {{{ void initialize()
void initialize()
{
	hazard.ifidw = 1;
	hazard.pcw = 1;
} // }}}
// {{{ void display()
void display()
{
	printf("PC = %d\n", pc*4);
	printf("-\n");
	for(int i=0; i<32; i++)
		printf("reg[%d] = %d\n", i, reg[i]);
	printf("-\n");
	switch(result.status){
		case NORMAL:
			printf("RegDst: %d\n", result.RegDst);
			printf("ALUSrc: %d\n", result.ALUSrc);
			printf("MemRead: %d\n", result.MemRead);
			printf("MemtoReg: %d\n", result.MemtoReg);
			printf("ALUOp: %s\n", ALUOpName[result.ALUOp]);
			printf("MemWrite: %d\n", result.MemWrite);
			printf("RegWrite: %d\n", result.RegWrite);
			break;
		case STALL:
			printf("stall.\n");
			break;
		case BRANCH:
			printf("branch.\n");
			break;
	}
	printf("-\n");
////	printf("%d %d\n", inst_count, cycle_count);
	printf("IPC: %.4lf\n", (double)inst_count/cycle_count);
} // }}}
// {{{ void _display()
void _display()
{
	for(int i=0; i<32; i++){
		if(reg[i])
			printf("reg[%d] = %d\n", i, reg[i]);
	}
} // }}}
// {{{ int main()
int main()
{
	int m;
	char buf[40];
	int i;

	initialize();
	scanf("%d%d", &n, &m);
////	printf("n: %d, m: %d\n", n, m);
	for(int i=0; i<n; i++){
		scanf("%s", buf);
		for(int j=0; j<32; j++)
			im[i] = (im[i]<<1)+buf[j]-'0';
	//	printf("%d: %x\n", i, im[i]);
	}
	for(i=0; i<m; i++, cycle_count++){
////		printf("=== %d === pc: %d\n", i, pc);
		writeback();
		mem_access();
		execute();
		decode();
		if(!fetch()){
			i++;
			cycle_count++;
			break;
		}
////		_display();
////		printf("inst num: %d\n", inst_count);
	}
	m = (m-i > 3) ? i+3 : m;
	for(; i<m; i++, cycle_count++){
////		printf("=== %d === flush\n", i);
		writeback();
		mem_access();
		execute();
		flushIDEX();
////		_display();
	}
	display();
	return 1;
} // }}}
