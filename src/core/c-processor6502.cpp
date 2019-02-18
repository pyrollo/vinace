/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * Vinace
 * Copyright (C) P.Y. Rollo 2009 <dev@pyrollo.com>
 * 
 * Vinace is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Vinace is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "c-processor6502.hpp"
#include <iostream>

// http://apple1.chez.com/Apple1project/Docs/m6502/6502-6510-8500-8502%20Opcodes.htm

/* Getting and Setting Individual Bits of Processor Status */
#define SET(mask) ( P |= (mask) )
#define CLR(mask) ( P &= (~(mask)) )
#define SET_IF(mask, cond) ( P= ( (cond)?(P|(mask)):(P &(~(mask))) ))
#define GET(mask) (((P&(mask))>0))
#define SET_P_FOR(value) (P = ( (P&0x7D) | ((value)&0x80) | ((value)?0:2) ))

#define VECTOR_NMI   0xFFFA
#define VECTOR_RESET 0xFFFC
#define VECTOR_IRQ   0xFFFE
#define VECTOR_BRK   0xFFFE

CProcessor6502::CProcessor6502(CMemory *mem):CProcessor(mem, NUMBER_OF_SIGNALS) {
}

bool CProcessor6502::process_signals() {
	// Reset
	if (signals[SIGNAL_RESET]) {
		signals[SIGNAL_RESET] = 0;
		reset();
		return false;
	}
	// Non masquable interrupts
	if (signals[SIGNAL_NMI]) {
		signals[SIGNAL_NMI] = 0;
		cycles += 7;
		push_word(PC);
		push(P);
		PC = read_word(VECTOR_NMI);
		return false;
	}
	// Interrupt request
	if (signals[SIGNAL_IRQ]) {
		if (!GET(I_BIT)) { // I is set, that means an IRQ is already being processed
			cycles += 7;
			push_word(PC);
			CLR(B_BIT);
			push(P);
			SET(I_BIT);
			PC = read_word(VECTOR_IRQ);
			signals[SIGNAL_IRQ]--;
			return false;
		}
	}
	// Break
	if (signals[SIGNAL_BRK]) {
		cycles += 7;
		push_word(PC);
		SET(B_BIT);
		push(P);
		SET(I_BIT);
		PC = read_word(VECTOR_BRK);
		signals[SIGNAL_BRK] = 0;
		return false;
	}
	return true; // OK all signals have been processed
}

void CProcessor6502::reset() {
	A = X = Y = 0;
	P = 0x20;
	S = 0xFF;
	PC = read_word(VECTOR_RESET);
	printf("Processor reset, jumping to $%04X.\n", PC);
}


void CProcessor6502::branch(BYTE operand) {
	word = (operand & 0x80)?operand-256:operand;
	check_page_crossing(PC, word);
	PC+=word;
	cycles++;
}

void CProcessor6502::opadd(BYTE operand) {
	word = operand + A + GET(C_BIT);
	SET_IF(V_BIT, !((operand^A) & 0x80) && ((A^word) & 0x80));
	if (GET(D_BIT)) {
		if ((word & 0x0F) > 0x09) word += 0x06;
		if ((word & 0xF0) > 0x90) word += 0x60;
	}
	A = word;
	SET_IF(C_BIT, word>>8);
	SET_P_FOR(A);
}

void CProcessor6502::opsub(BYTE operand) {
	word = 0xFF - operand + A + GET(C_BIT);
	SET_IF(V_BIT, !((operand^A) & 0x80) && ((A^word) & 0x80));
	if (GET(D_BIT)) {
		if ((word & 0x0F) > 0x09) word -= 0x06;
		if ((word & 0xF0) > 0x90) word -= 0x60;
	}
	A = word;
	SET_IF(C_BIT, word >> 8);
	// TODO : In decimal mode, the Z bit is not set if 0
	SET_P_FOR(A);
}

void CProcessor6502::opcmp(BYTE operand1, BYTE operand2) {
	word =  0x100 - operand2 + operand1;
	SET_IF(C_BIT, operand1>=operand2);
	SET_P_FOR(word & 0xff);
}

// Shift bits to the left and the most significant goes to C
// if rotate is set then C is inserted at the right instead of 0
void CProcessor6502::oprol(WORD addr, bool rotate) {
	operand = read_byte(addr);
	byte = (operand << 1 | (GET(C_BIT) & rotate));
	SET_IF(C_BIT, operand & 0x80);
	operand = operand << 1;
    SET_P_FOR(byte);
	write_byte(addr, byte);
}

// Shift bits to the right and the less significant goes to C
// if rotate is set then C is inserted at the left instead of 0
void CProcessor6502::opror(WORD addr, bool rotate) {
	operand = read_byte(addr);
	byte = (operand >> 1 | ((GET(C_BIT) & rotate) << 7));
	SET_IF(C_BIT, operand & 0x01);
	operand = operand >> 1;
	SET_P_FOR(byte);
	write_byte(addr, byte);
}


void CProcessor6502::opdec(WORD addr) {
    byte = read_byte(addr) - 1;
	SET_P_FOR(byte);
	write_byte(addr, byte);
}

void CProcessor6502::opinc(WORD addr) {
    byte = read_byte(addr) + 1;
	SET_P_FOR(byte);
	write_byte(addr, byte);
}

void CProcessor6502::process_instruction() {
	
	// Processing
	last_PC = PC;
	opcode = next_byte();
	
	switch (opcode) {
		
		case 0x69:  /* ADC #imm */
			cycles += 2;
			opadd(next_byte());
			break;
			
		case 0x6D:  /* ADC abs */
			cycles += 4;
			opadd(read_byte(eaabs()));
			break;
		
		case 0x65:  /* ADC zp */
			cycles += 3;
			opadd(read_byte(eazp()));
			break;
		
		case 0x61:  /* ADC (zp,X) */
			cycles += 6;
			opadd(read_byte(eazpxind()));
			break;
		
		case 0x71:  /* ADC (zp),Y */
			cycles += 5;
			opadd(read_byte(eazpindy()));
			break;
		
		case 0x75:  /* ADC zp,X */
			cycles += 4;
			opadd(read_byte(eazp(X)));
			break;
		
		case 0x7D:  /* ADC abs,X */
			cycles += 4;
			opadd(read_byte(eaabs(X)));
			break;
		
		case 0x79:  /* ADC abs,Y */
			cycles += 4;
			opadd(read_byte(eaabs(Y)));
			break;
		
		case 0x29:	/* AND #imm */
			cycles += 2;
			A &= next_byte();
			SET_P_FOR(A);
			break;
		
		case 0x2D:	/* AND abs */
			cycles += 4;
			A &= read_byte(eaabs());
			SET_P_FOR(A);
			break;
		
		case 0x25:	/* AND zp */
			cycles += 3;
			A &= read_byte(eazp());
			SET_P_FOR(A);
			break;
		
		case 0x21:	/* AND (zp,X) */
			cycles += 6;
			A &= read_byte(eazpxind());
			SET_P_FOR(A);
			break;
		
		case 0x31:	/* AND (zp),Y */
			cycles += 5;
			A &= read_byte(eazpindy());
			SET_P_FOR(A);
			break;
		
		case 0x35:	/* AND zp,X */
			cycles += 4;
			A &= read_byte(eazp(X));
			SET_P_FOR(A);
			break;
		
		case 0x3D:	/* AND abs,X */
			cycles += 4;
			A &= read_byte(eaabs(X));
			SET_P_FOR(A);
			break;
		
		case 0x39:	/* AND abs,Y */
			cycles += 4;
			A &= read_byte(eaabs(Y));
			SET_P_FOR(A);
			break;
		
		case 0x0E:	/* ASL abs */
			cycles += 6;
			oprol(eaabs(), false);
			break;
		
		case 0x06:	/* ASL zp */
			cycles += 6;
			oprol(eazp(), false);
			break;
		
		case 0x0A:	/* ASL acc */
			cycles += 2;
		    SET_IF(C_BIT, A & 0x80);
			byte = A << 1;
			A = byte;
			SET_P_FOR(A);
			break;
		
		case 0x16:	/* ASL zp,X */
			cycles += 6;
			oprol(eazp(X), false);
			break;
		
		case 0x1E:	/* ASL abs,X */
			cycles = 7;
			oprol(eaabs(X), false);
			break;
		
		case 0x90:	/* BCC rr */
			cycles += 2;
			operand = next_byte();
			if ( !GET(C_BIT) ) 
				branch( operand );
			break;
		
		case 0xB0:	/* BCS rr */
			cycles += 2;
			operand = next_byte();
			if ( GET(C_BIT) ) 
				branch( operand );
			break;

		case 0xF0:	/* BEQ rr */
			cycles += 2;
			operand = next_byte();
			if ( GET(Z_BIT) ) 
				branch( operand );
			break;
		
		case 0x2C:	/* BIT abs */
			cycles += 4;
			operand = read_byte(eaabs());
			SET_IF(V_BIT, operand & 0x40 ); 
			SET_IF(N_BIT, operand & 0x80 ); 
			SET_IF(Z_BIT, not (A & operand));
			break;

		case 0x24:	/* BIT zp */
			cycles += 3;
			operand = read_byte(eazp());
			SET_IF(V_BIT, operand & 0x40 ); 
			SET_IF(N_BIT, operand & 0x80 ); 
			SET_IF(Z_BIT, not (A & operand));
			break;

		case 0x30:	/* BMI rr */
			cycles += 2;
			operand = next_byte();
			if ( GET(N_BIT) ) 
				branch( operand );
			break;
		
		case 0xD0:	/* BNE rr */
			cycles += 2;
			operand = next_byte();
			if ( !GET(Z_BIT) ) 
				branch( operand );
			break;
		
		case 0x10:	/* BPL rr */
			cycles += 2;
			operand = next_byte();
			if ( !GET(N_BIT) ) 
				branch( operand );
			break;
		
		case 0x00:	/* BRK */
			signal(SIGNAL_BRK);
			break;
		
		case 0x50:	/* BVC rr */
			cycles += 2;
			operand = next_byte();
			if ( !GET(V_BIT) ) 
				branch( operand );
			break;
		
		case 0x70:	/* BVS rr */
			cycles += 2;
			operand = next_byte();
			if ( GET(V_BIT) ) 
				branch( operand );
			break;
		
		case 0x18:	/* CLC */
			cycles += 2;
			CLR(C_BIT);
			break;
		
		case 0xD8:	/* CLD */
			cycles += 2;
			CLR(D_BIT);
			break;
		
		case 0x58:	/* CLI */
			cycles += 2;
			CLR(I_BIT);
			break;
		
		case 0xB8:	/* CLV */
			cycles += 2;
			CLR(V_BIT);
			break;

		case 0xC9:	/* CMP #imm */
			cycles += 2;
			opcmp(A, next_byte());
			break;
	      
		case 0xCD:	/* CMP abs */
			cycles += 4;
			opcmp(A, read_byte(eaabs()));
			break;
	      
		case 0xC5:	/* CMP zp */
			cycles += 3;
			opcmp(A, read_byte(eazp()));
			break;
	      
		case 0xC1:	/* CMP (zp,X) */
			cycles += 6;
			opcmp(A, read_byte(eazpxind()));
			break;
	      
		case 0xD1:	/* CMP (zp),Y */
			cycles += 5;
			opcmp(A, read_byte(eazpindy()));
			break;
	      
		case 0xD5:	/* CMP zp,X */
			cycles += 4;
			opcmp(A, read_byte(eazp(X)));
			break;
	      
		case 0xDD:	/* CMP abs,X */
			cycles += 4;
			opcmp(A, read_byte(eaabs(X)));
			break;
	      
		case 0xD9:	/* CMP abs,Y */
			cycles += 4;
			opcmp(A, read_byte(eaabs(Y)));
			break;
	      
		case 0xE0:	/* CPX #imm */
			cycles += 2;
			opcmp(X, next_byte());
			break;
	      
		case 0xEC:	/* CPX abs */
			cycles += 4;
			opcmp(X, read_byte(eaabs()));
			break;
	      
		case 0xE4:	/* CPX zp */
			cycles += 3;
			opcmp(X, read_byte(eazp()));
			break;
		
		case 0xC0:	/* CPY #imm */
			cycles += 2;
			opcmp(Y, next_byte());
			break;
	      
		case 0xCC:	/* CPY abs */
			cycles += 4;
			opcmp(Y, read_byte(eaabs()));
			break;
	      
		case 0xC4:	/* CPY zp */
			cycles += 3;
			opcmp(Y, read_byte(eazp()));
			break;
	      
		case 0xCE:	/* DEC abs */
			cycles += 6;
			opdec(eaabs());
			break;
	      
	   case 0xC6:	/* DEC zp */
			cycles += 5;
			opdec(eazp());
			break;
	      
	   case 0xD6:	/* DEC zp,X */
			cycles += 6;
			opdec(eazp(X));
			break;
	      
	   case 0xDE:	/* DEC abs,X */
			cycles += 7;
			opdec(eaabs(X));
			break;
		
		case 0xCA:	/* DEX */
			cycles += 2;
			X--;
			SET_P_FOR(X);
			break;
		
		case 0x88:	/* DEY */
			cycles += 2;
			Y--;
			SET_P_FOR(Y);
			break;
		
		case 0x49:	/* EOR #imm */
			cycles += 2;
			A ^= next_byte();
			SET_P_FOR(A);
			break;
	      
		case 0x4D:	/* EOR abs */
			cycles += 4;
			A ^= read_byte(eaabs());
			SET_P_FOR(A);
			break;
	      
		case 0x45:	/* EOR zp */
			cycles += 3;
			A ^= read_byte(eazp());
			SET_P_FOR(A);
			break;

		case 0x41:	/* EOR (zp,X) */
			cycles += 6;
			A ^= read_byte(eazpxind());
			SET_P_FOR(A);
			break;
		
		case 0x51:	/* EOR (zp),Y */
			cycles += 5;
			A ^= read_byte(eazpindy());
			SET_P_FOR(A);
			break;
	      
		case 0x55:	/* EOR zp,X */
			cycles += 4;
			A ^= read_byte(eazp(X));
			SET_P_FOR(A);
			break;
	      
		case 0x5D:	/* EOR abs,X */
			cycles += 4;
			A ^= read_byte(eaabs(X));
			SET_P_FOR(A);
			break;
	      
		case 0x59:	/* EOR abs,Y */
			cycles += 4;
			A ^= read_byte(eaabs(Y));
			SET_P_FOR(A);
			break;
	      
		case 0xEE:	/* INC abs */
			cycles += 6;
			opinc(eaabs());
			break;
	      
		case 0xE6:	/* INC zp */
			cycles += 5;
			opinc(eazp());
			break;
	      
		case 0xF6:	/* INC zp,X */
			cycles += 6;
			opinc(eazp(X));
			break;
	      
		case 0xFE:	/* INC abs,X */
			cycles += 7;
			opinc(eaabs(X));
			break;
			
		case 0xE8:	/* INX */
			cycles += 2;
			X++;
			SET_P_FOR(X);
			break;

		case 0xC8:	/* INY */
			cycles += 2;
			Y++;
			SET_P_FOR(Y);
			break;
		
		case 0x4C:  /* JMP abs */
			cycles += 3;
			PC = eaabs();
			break;

		case 0x6C:  /* JMP abs */
			cycles += 5;
			PC = read_word(eaabs());
			break;

		case 0x20:	/* JSR abs */
			cycles += 6;
			push_word(PC+1);
			PC = eaabs();
			break;
		
		case 0xA9:	/* LDA #imm */
			cycles += 2;
			A = next_byte();
			SET_P_FOR(A);
			break;
	      
		case 0xAD:	/* LDA abs */
			cycles += 4;
			A = read_byte(eaabs());
			SET_P_FOR(A);
			break;
	      
		case 0xA5:	/* LDA zp */
			cycles += 4;
			A = read_byte(eazp());
			SET_P_FOR(A);
			break;
	      
		case 0xA1:	/* LDA (zp,X) */
			cycles += 6;
			A = read_byte(eazpxind());
			SET_P_FOR(A);
			break;
	      
		case 0xB1:	/* LDA (zp),Y */
			cycles += 5;
			A = read_byte(eazpindy());
			SET_P_FOR(A);
			break;

		case 0xB5:	/* LDA zp,X */
			cycles += 4;
			A = read_byte(eazp(X));
			SET_P_FOR(A);
			break;
	      
		case 0xBD:	/* LDA abs,X */
			cycles += 4;
			A = read_byte(eaabs(X));
			SET_P_FOR(A);
			break;

		case 0xB9:	/* LDA abs,Y */
			cycles += 4;
			A = read_byte(eaabs(Y));
			SET_P_FOR(A);
			break;
	      
		case 0xA2:	/* LDX #imm */
			cycles += 2;
			X = next_byte();
			SET_P_FOR(X);
			break;
		
		case 0xAE:	/* LDX abs */
			cycles += 4;
			X = read_byte(eaabs());
			SET_P_FOR(X);
			break;

		case 0xA6:	/* LDX zp */
			cycles += 3;
			X = read_byte(eazp());
			SET_P_FOR(X);
			break;

		case 0xBE:	/* LDX abs,Y */
			cycles += 4;
			X = read_byte(eaabs(Y));
			SET_P_FOR(X);
			break;

		case 0xB6:	/* LDX zp,Y */
			cycles += 4;
			X = read_byte(eazp(Y));
			SET_P_FOR(X);
			break;

		case 0xA0:	/* LDY #imm */
			cycles += 2;
			Y = next_byte();
			SET_P_FOR(Y);
			break;

		case 0xAC:	/* LDY abs */
			cycles += 4;
			Y = read_byte(eaabs());
			SET_P_FOR(Y);
			break;

		case 0xA4:	/* LDY zp */
			cycles += 3;
			Y = read_byte(eazp());
			SET_P_FOR(Y);
			break;

		case 0xBC:	/* LDY abs,X */
			cycles += 4;
			Y = read_byte(eaabs(X));
			SET_P_FOR(Y);
			break;

		case 0xB4:	/* LDY zp,X */
			cycles += 4;
			Y = read_byte(eazp(X));
			SET_P_FOR(Y);
			break;

		case 0x4E:	/* LSR abs */
			cycles += 6;
			opror(eaabs(), false);
			break;
	      
		case 0x46:	/* LSR zp */
			cycles += 5;
			opror(eazp(), false);
			break;

		case 0x4A:	/* LSR acc */
			cycles += 2;
			SET_IF(C_BIT, A & 0x01);
            A >>= 1;
			SET_P_FOR(A);
			break;
	      
	   case 0x56:	/* LSR zp,X */
			cycles += 6;
			opror(eazp(X), false);
			break;
	      
	   case 0x5E:	/* LSR abs,X */
			cycles += 7;
			opror(eaabs(X), false);
			break;
		
		case 0xEA:  /* NOP */
			cycles += 2;
			break;
		
		case 0x09:	/* ORA #imm */
			cycles += 2;
			A |= next_byte();
	        SET_P_FOR(A);
			break;
	      
		case 0x0D:	/* ORA abs */
			cycles += 4;
			A |= read_byte(eaabs());
	        SET_P_FOR(A);
			break;
	      
		case 0x05:	/* ORA zp */
			cycles += 3;
			A |= read_byte(eazp());
	        SET_P_FOR(A);
			break;
	      
		case 0x01:	/* ORA (zp,X) */
			cycles += 6;
			A |= read_byte(eazpxind());
	        SET_P_FOR(A);
			break;
	      
		case 0x11:	/* ORA (zp),Y */
			cycles += 5;
			A |= read_byte(eazpindy());
	        SET_P_FOR(A);
			break;

		case 0x15:	/* ORA zp,X */
			cycles += 4;
			A |= read_byte(eazp(X));
	        SET_P_FOR(A);
			break;
	      
		case 0x1D:	/* ORA abs,X */
			cycles += 4;
			A |= read_byte(eaabs(X));
	        SET_P_FOR(A);
			break;
	      
		case 0x19:	/* ORA abs,Y */
			cycles += 4;
			A |= read_byte(eaabs(Y));
	        SET_P_FOR(A);
			break;
	      
		case 0x48:	/* PHA */
			cycles += 3;
			push(A);
			break;

		case 0x08:	/* PHP */
			cycles += 3;
			push(P);
			break;

		case 0x68:	/* PLA */
			cycles += 4;
			A = pull();
			SET_P_FOR(A);
			break;

		case 0x28:	/* PLP */
			cycles += 4;
			P = pull() | 0x20; /* fix bug in bit5 of P */
			break;

		case 0x2E:	/* ROL abs */
			cycles += 6;
			oprol(eaabs(), true);
			break;
	      
		case 0x26:	/* ROL zp */
			cycles += 5;
			oprol(eazp(), true);
			break;
	      
		case 0x2A:	/* ROL acc */
			cycles += 2;
			operand = (A << 1) | GET(C_BIT);
			SET_IF(C_BIT, A & 0x80);
			A = operand;
		    SET_P_FOR(A);
			break;
	      
		case 0x36:	/* ROL zp,X */
			cycles += 6;
			oprol(eazp(X), true);
			break;
	      
		case 0x3E:	/* ROL abs,X */
			cycles += 7;
			oprol(eaabs(X), true);
			break;

		case 0x6E:	/* ROR abs */
			cycles += 6;
			opror(eaabs(), true);
			break;
	      
		case 0x66:	/* ROR zp */
			cycles += 5;
			opror(eazp(), true);
			break;
	      
		case 0x6A:	/* ROR acc */
			cycles += 2;
			operand = (A >> 1) | (GET(C_BIT) << 7);
			SET_IF(C_BIT, A & 0x01);
			A = operand;
		    SET_P_FOR(A);
			break;
	      
		case 0x76:	/* ROR zp,X */
			cycles += 6;
			opror(eazp(X), true);
			break;
	      
		case 0x7E:	/* ROR abs,X */
			cycles += 7;
			opror(eaabs(X), true);
			break;

		case 0x40:	/* RTI */
			cycles += 6;
			P = pull() | 0x20; /* bit 5 bug of 6502 */
			PC = pull_word();
			break;

		case 0x60:	/* RTS */
			cycles += 6;
			PC = pull_word();
			PC++;
			break;
		
		case 0xE9:	/* SBC #imm */
			cycles += 2;
			opsub(next_byte());
			break;
		
		case 0xED:	/* SBC abs */
			cycles += 4;
			opsub(read_byte(eaabs()));
			break;
		
		case 0xE5:	/* SBC zp */
			cycles += 3;
			opsub(read_byte(eazp()));
			break;
		
		case 0xE1:	/* SBC (zp,X) */
			cycles += 6;
			opsub(read_byte(eazpxind()));
			break;
		
		case 0xF1:	/* SBC (zp),Y */
			cycles += 5;
			opsub(read_byte(eazpindy()));
			break;
		
		case 0xF5:	/* SBC zp,X */
			cycles += 4;
			opsub(read_byte(eazp(X)));
			break;
		
		case 0xFD:	/* SBC abs,X */
			cycles += 4;
			opsub(read_byte(eaabs(X)));
			break;
		
		case 0xF9:	/* SBC abs,Y */
			cycles += 4;
			opsub(read_byte(eaabs(Y)));
			break;
		
		case 0x38:	/* SEC */
			cycles += 2;
			SET(C_BIT);
			break;
		
		case 0xF8:	/* SED */
			cycles += 2;
			SET(D_BIT);
			break;
		
		case 0x78:	/* SEI */
			cycles += 2;
			SET(I_BIT);
			break;
		
		case 0x8D:	/* STA abs */
			cycles += 4;
			write_byte(eaabs(), A);
			break;
		
		case 0x85:	/* STA zp */
			cycles += 3;
			write_byte(eazp(), A);
			break;
		
		case 0x81:	/* STA (zp,X) */
			cycles += 6;
			write_byte(eazpxind(), A);
			break;
		
		case 0x91:	/* STA (zp),Y */
			cycles += 6;
			write_byte(eazpindy(), A);
			break;
		
		case 0x95:	/* STA zp,X */
			cycles += 4;
			write_byte(eazp(X), A);
			break;
		
		case 0x9D:	/* STA abs,X */
			cycles += 5;
			write_byte(eaabs(X), A);
			break;
		
		case 0x99:	/* STA abs,Y */
			cycles += 5;
			write_byte(eaabs(Y), A);
			break;
		
		case 0x8E:	/* STX abs */
			cycles += 4;
			write_byte(eaabs(), X);
			break;
		
		case 0x86:	/* STX zp */
			cycles += 3;
			write_byte(eazp(), X);
			break;
		
		case 0x96:	/* STX zp,Y */
			cycles += 4;
			write_byte(eazp(Y), X);
			break;
		
		case 0x8C:	/* STY abs */
			cycles += 4;
			write_byte(eaabs(), Y);
			break;
		
		case 0x84:	/* STY zp */
			cycles += 3;
			write_byte(eazp(), Y);
			break;
		
		case 0x94:	/* STY zp,X */
			cycles += 4;
			write_byte(eazp(X), Y);
			break;
		
		case 0xAA:	/* TAX */
			cycles += 2;
			X = A;
			SET_P_FOR(X);
			break;
		
		case 0xA8:	/* TAY */
			cycles += 2;
			Y = A;
			SET_P_FOR(Y);
			break;
		
		case 0xBA:	/* TSX */
			cycles += 2;
			X = S;
			SET_P_FOR(X);
			break;
		
		case 0x8A:	/* TXA */
			cycles += 2;
			A = X;
			SET_P_FOR(A);
			break;
		
		case 0x9A:	/* TXS */
			cycles += 2;
			S = X;
			break;
		
		case 0x98:	/* TYA */
			cycles += 2;
			A = Y;
			SET_P_FOR(A);
			break;

		default:
			cycles += 2; // An instruction MUST consume some time (or the proc may stuck)
			printf("Unknown opcode $%02X at $%04X\n", opcode, PC);
	}
}

void CProcessor6502::write_byte(WORD addr, BYTE byte) {
	memory->write(addr, byte);
}

BYTE CProcessor6502::read_byte(WORD addr) {
	return memory->read(addr);
}

WORD CProcessor6502::read_word(WORD addr) {
	return memory->read(addr)|(memory->read(addr+1)<<8);
}

BYTE CProcessor6502::next_byte() {
	return read_byte(PC++);
}

WORD CProcessor6502::next_word() {
	return next_byte()+(next_byte()<<8);
}

void CProcessor6502::push(BYTE byte) {
	write_byte( (S--) + 0x0100, byte);
}

BYTE CProcessor6502::pull() {
	return read_byte( (++S) + 0x0100);
}

void CProcessor6502::push_word(WORD word) {
	push( word >> 8 );
	push( word & 0xFF );
}

WORD CProcessor6502::pull_word() {
	return pull() + ((WORD)pull()<<8);
}

void CProcessor6502::check_page_crossing(WORD addr, int offset) {
	if ( ((addr+offset)^addr) & 0xff00) cycles++;
}

// Effective addressing calculation

WORD CProcessor6502::eazp(BYTE offset) {
	return (next_byte()+offset)&0xff;
}

WORD CProcessor6502::eaabs(BYTE offset, bool extracycle) {
	addr = next_word()+offset;
	// In most cases, changing page costs one extra cycle
	if (extracycle) check_page_crossing(addr,offset);
	return addr;
}

/* ($00,X) */
WORD CProcessor6502::eazpxind() {
	return read_word(eazp(X));
}

/* ($00),Y */
WORD CProcessor6502::eazpindy(bool extracycle) {
	addr = read_word(eazp());
	if (extracycle) check_page_crossing(addr,Y);
	return addr+Y;	
}


