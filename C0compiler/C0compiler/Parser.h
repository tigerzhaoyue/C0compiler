#ifndef __PARSER_H__
#define __PARSER_H__

#include <vector>
#include <set>
#include "Symbol.h"
#include "Table.h"
#include "Quater.h"

using namespace std;

class Parser {

private:

	void test(set<Symbol> s1, set<Symbol> s2, int n);
	void testsemicolon(set<Symbol> fsys);
	void testsemicolon_and_rbrace(set<Symbol> fsys);
	void testrbrace(set<Symbol> fsys);
	void skip(set<Symbol> fsys, int n);

	void program(set<Symbol> fsys);
	void constantdeclaration(set<Symbol> fsys);
	void constantdefine(set<Symbol> fsys);
	int paralist(set<Symbol> fsys);
	void var_varfuncdeclaration(set<Symbol> fsys);
	void var_varfuncdefine(set<Symbol> fsys);
	bool HaveReachfunc;
	void voidfunc_maindeclaration(set <Symbol> fsys);
	void voidfunc_maindefine(set<Symbol> fsys);
	bool HaveReachMain;
	void compstate(set <Symbol> fsys);
	void constantdeclaration_in_func(set<Symbol> fsys);
	void constantdefine_in_func(set<Symbol> fsys);
	void vardeclaration(set<Symbol> fsys);
	void vardefine(set<Symbol> fsys);
	void statement(set<Symbol> fsys);
	void ifstatement(set<Symbol> fsys);
	void condition(set<Symbol> fsys,Operand& opd,bool nob);
	void scanfstatement(set<Symbol> fsys);
	void printfstatement(set<Symbol> fsys);
	void returnstatement(set<Symbol> fsys);
	void whilestatement(set<Symbol> fsys);
	void assign_or_callstatement(set<Symbol> fsys,Item item,Operand& opd);
	void valueparalist(set<Symbol> fsys);
	void switchstatement(set<Symbol> fsys);
	void casestatement(set<Symbol> fsys,Operand& opd,Operand& finallab);
	void defaultstatement(set<Symbol> fsys);

	Type factor(set<Symbol> fsys, Operand& opd);
	Type term(set<Symbol> fsys, Operand& opd);
	Type expr(set<Symbol> fsys,Operand& opd);
	Type expr_beg_have_read(set<Symbol> fsys, Operand& opd);
	Type resulttype(Type tp1,Type tp2);
	Type selector(set<Symbol> fsys, Item item, Operand& opd, bool ifval);
	void call(set<Symbol> fsys, Item item, Operand& opd);
	//void outputOperand(Operand opd);
	void writeCode(string op, string d = "", string s1= "", string s2 = "");
	string loadAddr(Operand opd, int lev);
	void loadOperand(Operand opd, string reg, int lev);
	
public:

	Parser();
	void initbgsys();
	set<Symbol> programbegsys;
	set<Symbol> statbegsys;
	set<Symbol> exprbegsys;
	set<Symbol> facbegsys;
	set<Symbol> compstatbegsys;
	set<Symbol> cmpop;
	void parse();
	bool compilefinish;
	void ASM();
};

#endif