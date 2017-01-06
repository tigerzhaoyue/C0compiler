#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Table.h"
#include "Quater.h"

using namespace std;

extern Table table;

extern vector<Quater> Qcode;

extern fstream fileIns;


string Opt[] = {
	"ADD", "SUB", "MUL", "DIV", "NEG",
	"BEQ", "BNE", "BGT", "BGE", "BLT", "BLE",
	"JMP", "OFF", "SEL", "ASG", "RAD", "WRT","WRTC",
	"CALVF", "CALF", "RET", "RETM", "PUSH", "SAVE", "LAB"
};

Operand::Operand() {
	isNULL=false;
}

string Operand::getName() {
	stringstream ss;
	Item item;
	switch (type) {
	case oconst:
		ss << value.cont;
		break;
	case oident:
		item = table.find(tabx, value.ident);
		ss << item.name;
		break;
	case otemp:
		item = table.find(tabx, value.temp);
		ss << item.name;
		break;
	case olabel:
		ss << "[label_" << value.label << "]";
		break;
	case ostring:
		ss << "[string_" << value.label << "]";
		break;
	}
	return ss.str();
}

string Operand::toString() {
	stringstream ss;
	switch (type) {
	case oconst:
		ss << value.cont;
		break;

	case oident:
		ss << table.find(tabx, value.ident).name;
		break;

	case otemp:
		ss << table.find(tabx, value.temp).name;
		break;

	case olabel:
		if (value.label == 0) {
			ss << "Main";
		}
		else {
			ss << "Label_" << value.label;
		}
		break;

	case ostring:
		ss << "\"" << table.findString(value.str) << "\"";
		break;
	}
	return ss.str();
}

Quater::Quater() {}

Quater::Quater(int lev, Operation opt, Operand dest, Operand src1, Operand src2) {
	this->opt = opt;
	this->dest = dest;
	this->src1 = src1;
	this->src2 = src2;
	this->lev = lev;
}

string Quater::toString() {
	stringstream ss;
	if(opt == LAB)
		ss << "\n"<<Opt[opt] << "  ";
	else
		ss << Opt[opt] << "  ";
	if (opt == CALVF || opt == CALF || opt == RET){
		ss << dest.getName();
		if (opt == CALF) {
			ss << ",";
			ss << src1.toString();
		}
	}
	else if (opt == JMP || opt == RAD || opt == WRT || opt == LAB || opt == RETM || opt == WRTC) {
		ss << dest.toString();
	}
	else if (opt == NEG || opt == ASG) {
		ss << dest.toString();
		ss << ",";
		ss << src1.toString();
	}
	else if (opt != SAVE){
		ss << dest.toString();
		ss << ",";
		ss << src1.toString();
		ss << ",";
		ss << src2.toString();
	}
	ss << "\n";
	return ss.str();
}

void Quater::outputIns(){
	for (int i = 0; i < (int)Qcode.size(); i++) {
		fileIns << Qcode[i].toString();
	}
}