#ifndef __QUATER_H__
#define __QUATER_H__

#include <string>

using namespace std;

enum Operation {
	ADD, SUB, MUL, DIV, NEG,
	BEQ, BNE, BGT, BGE, BLT, BLE,
	JMP, OFF, SEL, ASG, RAD, WRT,WRTC,
	CALVF, CALF, RET, RETM, PUSH, SAVE, LAB
};

enum Opdtype {
	oconst, oident, otemp, olabel, ostring
};


class Operand {
public:
	Opdtype type;
	int tabx;
	union value {
		int cont;
		int ident;
		int temp;
		int label;
		int str;
		char ch;
	} value;

	Operand();
	string getName();
	string toString();
	bool isNULL;
};

class Quater {
public:
	Operation opt;
	Operand src1;
	Operand src2;
	Operand dest;
	int lev;
	Quater();
	Quater(int lev, Operation opt, Operand dest = Operand(), Operand src1 = Operand(), Operand src2 = Operand());
	string toString();
	void outputIns();
};

#endif