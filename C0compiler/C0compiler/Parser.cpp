#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <set>
#include "Symbol.h"
#include "Lexer.h"
#include "Parser.h"
#include "Error.h"
#include "Table.h"
#include "Quater.h"
using namespace std;

extern int lev;  //程序层次
extern Symbol sym;  //当前的单词类型
extern Lexer lexer;
extern Error error;
extern Table table;
extern vector<Quater> Qcode;
extern fstream fileAsm;
Parser::Parser() {
		initbgsys();
		HaveReachfunc=false;
		HaveReachMain=false;
		compilefinish=false;
}

int currentTab=-1;
int mainTab=-1;
int addr=0;
int label=0;
int temp=0;
int defaultnumvalue=0;
char defaultcharvalue='^';
int defaultarraysize=-1;
int FuncLabelNum[1000];
bool invoid = false;
bool debug = false;
/*******************************************************init beginsys set******************************************************************************/
void Parser::initbgsys(){
	/*program begin sys set init*/
	programbegsys.insert(CONSTSYM);	//［＜常量说明＞］
	programbegsys.insert(INTSYM);		//［＜变量说明＞］
	programbegsys.insert(CHARSYM);		////{＜有返回值函数定义＞}
	programbegsys.insert(VOIDSYM);	//{＜无返回值函数定义＞}＜主函数＞
	/*statement begin sys init*/
	statbegsys.insert(IFSYM);	//＜条件语句＞
	statbegsys.insert(WHILESYM);	//＜循环语句＞
	statbegsys.insert(FORSYM);	
	statbegsys.insert(LBRACE);	//‘{’＜语句列＞‘}’
	statbegsys.insert(IDENT);	//＜有返回值函数调用语句＞; |＜赋值语句＞;|＜无返回值函数调用语句＞;
	statbegsys.insert(SCANFSYM);	//＜读语句＞;
	statbegsys.insert(PRINTFSYM);	//＜写语句＞;
	statbegsys.insert(SEMICOLON);	//＜空＞;
	statbegsys.insert(SWITCHSYM);	//＜情况语句＞
	statbegsys.insert(RETURNSYM);	//＜返回语句＞;
	/*factor begin sys init*/
	facbegsys.insert(IDENT);
	facbegsys.insert(LPARENT);
	facbegsys.insert(PLUS);
	facbegsys.insert(MINUS);					
	facbegsys.insert(POSNUMBER);	
	facbegsys.insert(ZERONUMBER);
	facbegsys.insert(CONSTCHAR);
	/*expression begin sys init*/
	exprbegsys=facbegsys;
	/*compoud statement begin sys*/
	compstatbegsys.insert(CONSTSYM);	//[常量说明]
	compstatbegsys.insert(INTSYM);	//＜INT变量说明＞
	compstatbegsys.insert(CHARSYM);	//＜CHAR变量说明＞
	compstatbegsys.insert(IFSYM);	//＜条件语句＞
	compstatbegsys.insert(WHILESYM);	//＜循环语句＞
	compstatbegsys.insert(FORSYM);	
	compstatbegsys.insert(LBRACE);	//‘{’＜语句列＞‘}’
	compstatbegsys.insert(IDENT);	//＜有返回值函数调用语句＞; |＜赋值语句＞;|＜无返回值函数调用语句＞;
	compstatbegsys.insert(SCANFSYM);	//＜读语句＞;
	compstatbegsys.insert(PRINTFSYM);	//＜写语句＞;
	compstatbegsys.insert(SEMICOLON);	//＜空＞;
	compstatbegsys.insert(SWITCHSYM);	//＜情况语句＞
	compstatbegsys.insert(RETURNSYM);	//＜返回语句＞;
	/*compare operations*/
	cmpop.insert(NEQ);
	cmpop.insert(EQL);
	cmpop.insert(LSS);
	cmpop.insert(LEQ);
	cmpop.insert(GTR);
	cmpop.insert(GEQ);
	/*set<Symbol>::iterator it;
	it=programbegsys.find(INTSYM);
	if(it!=programbegsys.end())
		if(debug)cout<<*it<<endl;*/
}


/************************************************some useful functions*******************************************************************************/
set<Symbol> merge(set<Symbol> s1, set<Symbol> s2) {			//function merge  to merge two set of beginsym
	set<Symbol> s(s1);
	s.insert(s2.begin(), s2.end());
	return s;
}

bool match(Type t1, Type t2) {

	if (t1 == INT && t2 == INT) {
		return true;
	} 
	else if (t1 == CHAR && t2 == CHAR) {
		return true;
	}
	else if (t1 == CHAR && t2 == CHAR) {
		return true;
	}
	else {
		return false;
	}
}

/**********************************************some test funxtions***********************************************************************************/
void Parser::test(set<Symbol> s1, set<Symbol>s2, int n) {
	if (s1.find(sym) != s1.end()) {
		return;
	}
	error.outputError(n);						
	s1.insert(s2.begin(), s2.end());
	while (s1.find(sym) == s1.end()) {		//until sym in s1 or s2
		lexer.getsym();
	}
}

void Parser::testsemicolon(set<Symbol> fsys) {			//REMEMBER : This func will read the next sym after ;
	if (sym == SEMICOLON) {
		lexer.getsym();
		return;
	}
	else {
		skip(fsys,10);
		}
}

void Parser::testsemicolon_and_rbrace(set<Symbol> fsys) {			//REMEMBER : This func will read the next sym after ; and }
	if (sym == SEMICOLON||sym==RBRACE) {
		lexer.getsym();
		return;
	}
	else {
		skip(fsys,9);
		}
}

void Parser::testrbrace(set<Symbol> fsys) {			//REMEMBER : This func will read the next sym after ;
	if (sym == RBRACE) {
		lexer.getsym();
		return;
	}
	else {
		skip(fsys,5);
		}
}

void Parser::skip(set<Symbol> fsys, int n) {
	error.outputError(n);
	while (fsys.find(sym) == fsys.end()) {			//never find such a sym in fsys   stop at first meet fsys
		lexer.getsym();
	}
}



/************************************************************************parser***********************************************************************/
void Parser::parse(){
	lexer.getsym();
	set<Symbol> s = programbegsys;
	currentTab = table.newTable();
	program(s);
}

void Parser::program(set <Symbol> fsys){
	if(debug)
		if(debug)
			cout<<"【 program.】"<<endl;
	test(programbegsys,set <Symbol>(),0);
	while(fsys.find(sym)!=fsys.end() ){
		if(sym==CONSTSYM){
			constantdeclaration(fsys);
		}
		if (sym ==INTSYM || sym==CHARSYM) {
			var_varfuncdeclaration(fsys);
			}
		if (sym==VOIDSYM){
			voidfunc_maindeclaration(fsys);
		}
	}
	
}

void Parser::constantdeclaration(set<Symbol> fsys) {
	if(debug)cout<<"\t【 const declaration.】"<<endl;	
	while(sym==CONSTSYM){
		constantdefine(fsys);
		testsemicolon(fsys);			//test if end with semicolon,if so read next sym
	}
}

void Parser::constantdefine(set<Symbol> fsys){
	if(debug)cout<<"\t\t【 const define.】"<<endl;
	lexer.getsym();
	string name;
	char constcharvalue='^';
	int constnumvalue=0;
	set<Symbol> tps1;tps1.insert(INTSYM);tps1.insert(CHARSYM);
	set<Symbol> tps2;tps2.insert(SEMICOLON);
	test(tps1, tps2, 8);					//must start with char or int ,stop at first ；、 int、 char
	if (sym == INTSYM || sym==CHARSYM) {
		Symbol consttype=sym;			//int or char in this defination. prepare to enter the tale
		Type type= sym==INTSYM ? INT : CHAR ;
		set<Symbol> tps3;tps3.insert(SEMICOLON);tps3.insert(COMMA);
		if(HaveReachfunc&&!HaveReachMain)
				error.outputError(31);
		do{
		lexer.getsym();
		if(sym==IDENT){
			name = lexer.ident;		
			lexer.getsym();
			if(sym==BECOMES){
				lexer.getsym();		
				if(sym==POSNUMBER || sym==ZERONUMBER || sym==CONSTCHAR || sym==PLUS || sym==MINUS){
					if(sym==PLUS){
						lexer.getsym();
						if(sym==POSNUMBER || sym==ZERONUMBER ){
							constnumvalue=lexer.num;
						}
						else{
							skip(tps3,3);		//getsym until the next , or ;
							continue;
						}
					}
					else if(sym==MINUS){
						lexer.getsym();
						if(sym==POSNUMBER || sym==ZERONUMBER ){
							constnumvalue=-lexer.num;
						}
						else{
							skip(tps3,3);		//getsym until the next , or ;
							continue;
						}

					}
					else if(sym==ZERONUMBER || sym==POSNUMBER){
						constnumvalue=lexer.num;
					}
					else{
						constcharvalue=lexer.word[0];
					}
					if(table.AssertEnter(currentTab,name)){
						table.enter(currentTab,name,CONST,type,addr,defaultarraysize,constnumvalue,constcharvalue);
					}
					else{
						error.outputError(47);
					}
				}
				else{
					skip(tps3,27);		//getsym until the next , or ;
					continue;
				}
			}
			else{
				skip(tps3,30);		//getsym until the next , or ;
				continue;
			}
		}
		else{
				skip(tps3,20);		//getsym until the next , or ;
				continue;
		}
		lexer.getsym();
		}while(sym==COMMA);
	}
}

int Parser::paralist(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t【 parameter list】"<<endl;
	set<Symbol> rparent_set;
	rparent_set.insert(RPARENT);
	int paranum=0;
	if(sym==RPARENT)
		return paranum;
	if(sym!=INTSYM && sym!=CHARSYM){
		skip(rparent_set,44);
		return paranum;
	}
	Type type = sym==INTSYM? INT:CHAR; 
	lexer.getsym();
	if(sym!=IDENT){
		skip(rparent_set,20);
		return paranum;
	}
	string name=lexer.ident;
	if(table.AssertEnter(currentTab,name)){
		table.enter(currentTab,name,PARA,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);
		paranum++;
	}
	else{
		error.outputError(47);
	}
	lexer.getsym();
	while(sym==COMMA){
		lexer.getsym();
		if(sym!=INTSYM && sym!=CHARSYM){
			skip(rparent_set,44);
			return paranum;
		}
		type = sym==INTSYM? INT:CHAR; 
		lexer.getsym();
		if(sym!=IDENT){
			skip(rparent_set,20);
			return paranum;
		}
		name=lexer.ident;
		if(table.AssertEnter(currentTab,name)){
			table.enter(currentTab,name,PARA,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);
			paranum++;
		}
		else{
			error.outputError(47);
		}
		lexer.getsym();
	}
	return paranum;
}

void Parser::var_varfuncdeclaration(set<Symbol> fsys){
	if(debug)cout<<"\t【 var or varfunction declaration】"<<endl;
	while(sym==INTSYM || sym==CHARSYM){
		var_varfuncdefine(fsys);
		testsemicolon_and_rbrace(fsys);
		currentTab=0;
	}
	
}

void Parser::var_varfuncdefine(set<Symbol> fsys){
	if(debug)cout<<"\t\t【 var or varfunction define】"<<endl;
	string name;				//save the IDENT
	Symbol vartype=sym;			//int or char in this defination. prepare to enter the tale
	set<Symbol> tps; tps.insert(SEMICOLON); tps.insert(COMMA);tps.insert(RBRACE);
	Item item;
	int tabx = -1;
	int index = -1;
	Type type= sym==INTSYM ? INT : CHAR ;
	do{
		lexer.getsym();
		if(sym==IDENT){
			name = lexer.ident;																	//record the name of var\func\array
			lexer.getsym();																			// should be , or ;  or [ or (			
			if(sym==COMMA || sym==SEMICOLON){							//VAR
				if(HaveReachfunc)	error.outputError(31);
				if(table.AssertEnter(currentTab,name)){
					table.enter(currentTab,name,VAR,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);
				}
				else{
					error.outputError(47);
				}
			}
			else if(sym==LBRACKET){														//ARRAY
				if(HaveReachfunc)
					error.outputError(31);
				lexer.getsym();
				if(sym!=POSNUMBER){
					skip(tps,21);																			//getsym until the next , or ; or }
					continue;
				}
				int arraysize=lexer.num;
				lexer.getsym();
				if(sym!=RBRACKET){
					skip(tps,17);																			//getsym until the next , or ; or }
					continue;
				}
				lexer.getsym();
				if(table.AssertEnter(currentTab,name)){
					table.enter(currentTab,name,ARRAY,type,addr,arraysize,defaultnumvalue,defaultcharvalue);
				}
				else{
					error.outputError(47);
				}
				}
			else if(sym==LPARENT){															//FUNCTION with RETURN value
				set<Symbol> func_stop;
				func_stop.insert(RBRACE);
				HaveReachfunc=true;
				lexer.getsym();
				int childTab=table.newTable();
				if(table.AssertEnter(currentTab,name)){
					table.enter(currentTab,name,FUNC,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue,childTab);
					item = table.get(currentTab, name, tabx, index);
					
				}
				else{
					error.outputError(47);
				}

				Operand lab;
				lab.type = olabel;
				lab.value.label = ++label;
				Qcode.push_back(Quater(lev, LAB, lab));
				FuncLabelNum[childTab] = lab.value.label;

				currentTab=childTab;
				table.table[tabx].thistable[index].paranum=paralist(fsys);
				if(sym==RPARENT){
					lexer.getsym();
					if(sym==LBRACE){
						lexer.getsym();
						compstate(compstatbegsys);
					}
					else{
						skip(func_stop,7);																			//getsym until the next , or ; or }
						continue;
					}
				}
				else{
					skip(func_stop,23);																			//getsym until the next , or ; or }
					continue;
				}					
			}
			else{
				skip(tps,12);																			//getsym until the next , or ; or }
				continue;
			}
		}
		else{
				skip(tps,20);																			//getsym until the next , or ; or }
				continue;
		}
	}while(sym==COMMA);
	
}

void Parser::voidfunc_maindeclaration(set<Symbol> fsys){
	if(debug)cout<<"\t【 voidfunc or main declaration】"<<endl;
	invoid = true;
	while(sym==VOIDSYM){
		voidfunc_maindefine(fsys);
		if(!HaveReachMain){
			testrbrace(fsys);			//every defination should end with }  
		}
		else{
			if(sym==RBRACE){
				compilefinish=true;
			}
			else{
				error.outputError(5);
			}
		}
		currentTab=0;
	}
	invoid = false;
}

void Parser::voidfunc_maindefine(set <Symbol> fsys){
	if(debug)cout<<"\t\t【 void Func define】"<<endl;
	Item item;
	int tabx = -1;
	int index = -1;
	HaveReachfunc=true;
	string name;					//save the IDENT
	lexer.getsym();
	set<Symbol> tps1,tps2;
	tps1.insert(IDENT);tps1.insert(MAINSYM);
	tps2.insert(RBRACE);
	test(tps1,tps2,38);			//stop at ident\main\}
	if(sym==IDENT){
		name = lexer.ident;																	//record the name of void func
		lexer.getsym();																			// should be(
		if(sym==LPARENT){
			lexer.getsym();
			int childTab=table.newTable();
			if(table.AssertEnter(currentTab,name)){
				table.enter(currentTab,name,VOIDFUNC,VOID,addr,defaultarraysize,defaultnumvalue,defaultcharvalue,childTab);
				item = table.get(currentTab, name, tabx, index);
			}
			else{
				skip(tps2,77);
				return;
			}

			Operand lab;
			lab.type = olabel;
			lab.value.label = ++label;
			Qcode.push_back(Quater(lev, LAB, lab));
			FuncLabelNum[childTab] = lab.value.label;

			currentTab=childTab;
			table.table[tabx].thistable[index].paranum=paralist(fsys);
			if(sym==RPARENT){
				lexer.getsym();
				if(sym==LBRACE){
					lexer.getsym();
					compstate(compstatbegsys);				/////////复合语句
					Operand opd;
					opd.isNULL = true;
					Qcode.push_back(Quater(lev, RET, opd));
				}
				else{
					skip(tps2,7);
					return;
				}
			}
			else{
				skip(tps2,23);
				return;
			}			
		}
		else{
			skip(tps2,32);
			return;
		}
	}
	else if(sym==MAINSYM){
		HaveReachMain=true;
		name = lexer.ident;																	//record the name of void func
		lexer.getsym();																			// should be(
		if(sym==LPARENT){
			lexer.getsym();	
			if(sym==RPARENT){
				lexer.getsym();
				if(sym==LBRACE){
					lexer.getsym();
					int childTab=table.newTable();
					table.enter(currentTab,name,VOIDFUNC,VOID,addr,defaultarraysize,defaultnumvalue,defaultcharvalue,childTab);		//	Nothing can occupy main ident
					item = table.get(currentTab, name, tabx, index);

					Operand lab;
					lab.type = olabel;
					lab.value.label = 0;
					Qcode.push_back(Quater(lev, LAB, lab));
					FuncLabelNum[childTab] = lab.value.label;

					currentTab = childTab;
					mainTab = childTab;
					table.table[tabx].thistable[index].paranum=0;
					compstate(compstatbegsys);
					if(sym==RBRACE){
						compilefinish=true;
					}
					else{	//DO NOTHING
					}
				}
				else{
					skip(tps2,7);
					return;
				}
			}
			else{
				skip(tps2,23);
				return;
			}
		}
		else{
			skip(tps2,32);
			return;
		}
		if(debug)cout<<"\t\tMAIN function "<<" -> table"<<endl;		
	}
	else if(sym==RBRACE){		//somthing wrong in test
	}	
}


/*******************************************************************main begin***********************************************************************/
void Parser::compstate(set <Symbol> fsys){			//传入语句和声明的头符号集
	if(debug)cout<<"\t\t\t【 compound state.】"<<endl;
	//test(fsys, set <Symbol>(), 14);
	set<Symbol> rbrace_set;
	rbrace_set.insert(RBRACE);
	if(fsys.find(sym)==fsys.end() && sym!=RBRACE){
		skip(rbrace_set,14);
		return;
	}
	if(sym==CONSTSYM){
		constantdeclaration_in_func(fsys);
	}
	if(sym==INTSYM || sym==CHARSYM){
		vardeclaration(fsys);
	}
	while(fsys.find(sym) != fsys.end()){				
		if(statbegsys.find(sym)!=statbegsys.end()){		//a statementt
			statement(statbegsys);		//when finish   sym shuold =} or statbegsys
		}
		else{								//a  delaration error!
			set<Symbol> end_state;end_state.insert(RBRACE);end_state.insert(SEMICOLON);
			skip(end_state,19);
			lexer.getsym();	//next 合法句子开头
		}
	}
}	 

void Parser::constantdeclaration_in_func(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t【 const declaration.】"<<endl;	
	while(sym==CONSTSYM){
		constantdefine_in_func(fsys);
		testsemicolon(fsys);			//test if end with semicolon,if so read next sym
	}
}

void Parser::constantdefine_in_func(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 const define.】"<<endl;
	lexer.getsym();
	string name;
	char constcharvalue='^';
	int constnumvalue=0;
	set<Symbol> tps1;tps1.insert(INTSYM);tps1.insert(CHARSYM);
	set<Symbol> tps2;tps2.insert(SEMICOLON);
	test(tps1, tps2, 8);					//must start with char or int ,stop at first ；、 int、 char
	if (sym == INTSYM || sym==CHARSYM) {
		Symbol consttype=sym;			//int or char in this defination. prepare to enter the tale
		Type type= sym==INTSYM ? INT : CHAR ;
		set<Symbol> tps3;tps3.insert(SEMICOLON);tps3.insert(COMMA);
		/*if(HaveReachfunc&&!HaveReachMain)
				error.outputError(31);*/
		do{
		lexer.getsym();
		if(sym==IDENT){
			name = lexer.ident;		
			lexer.getsym();
			if(sym==BECOMES){
				lexer.getsym();		
				if(sym==POSNUMBER || sym==ZERONUMBER || sym==CONSTCHAR || sym==PLUS || sym==MINUS){
					if(sym==PLUS){
						lexer.getsym();
						if(sym==POSNUMBER || sym==ZERONUMBER ){
							constnumvalue=lexer.num;
						}
						else{
							skip(tps3,3);		//getsym until the next , or ;
							continue;
						}
					}
					else if(sym==MINUS){
						lexer.getsym();
						if(sym==POSNUMBER || sym==ZERONUMBER ){
							constnumvalue=-lexer.num;
						}
						else{
							skip(tps3,3);		//getsym until the next , or ;
							continue;
						}

					}
					else if(sym==ZERONUMBER || sym==POSNUMBER){
						constnumvalue=lexer.num;
					}
					else{
						constcharvalue=lexer.word[0];
					}
					if(table.AssertEnter(currentTab,name)){
						table.enter(currentTab,name,CONST,type,addr,defaultarraysize,constnumvalue,constcharvalue);
					}
					else{
						error.outputError(47);
					}
					
				}
				else{
					skip(tps3,27);		//getsym until the next , or ;
					continue;
				}
			}
			else{
				skip(tps3,30);		//getsym until the next , or ;
				continue;
			}
		}
		else{
				skip(tps3,20);		//getsym until the next , or ;
				continue;
		}
		lexer.getsym();
		}while(sym==COMMA);
	}
}

void Parser::vardeclaration(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t【 var declaration】"<<endl;
	while(sym==INTSYM || sym==CHARSYM){
		vardefine(fsys);
		testsemicolon(fsys);
	}
	
}

void Parser::vardefine(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 var define】"<<endl;
	string name;				//save the IDENT
	Symbol vartype=sym;			//int or char in this defination. prepare to enter the tale
	set<Symbol> tps; tps.insert(SEMICOLON); tps.insert(COMMA);tps.insert(RBRACE);
	Type type = sym==INTSYM?INT:CHAR;
	do{
		lexer.getsym();
		if(sym==IDENT){
			name = lexer.ident;																	//record the name of var\func\array
			lexer.getsym();																			// should be , or ;  or [ or (			
			if(sym==COMMA || sym==SEMICOLON){							//VAR
				if(table.AssertEnter(currentTab,name)){
					table.enter(currentTab,name,VAR,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);
				}
				else{
					error.outputError(47);
				}
			}
			else if(sym==LBRACKET){														//ARRAY
				lexer.getsym();
				if(sym!=POSNUMBER){
					skip(tps,21);																			//getsym until the next , or ; or }
					continue;
				}
				int arraysize=lexer.num;
				lexer.getsym();
				if(sym!=RBRACKET){
					skip(tps,17);																			//getsym until the next , or ; or }
					continue;
				}
				lexer.getsym();
				if(table.AssertEnter(currentTab,name)){
					table.enter(currentTab,name,ARRAY,type,addr,arraysize,defaultnumvalue,defaultcharvalue);
				}
				else{
					error.outputError(47);
				}
				}
			else{
				skip(tps,18);																			//getsym until the next , or ; or }
				continue;
			}
		}
		else{
				skip(tps,20);																			//getsym until the next , or ; or }
				continue;
		}
	}while(sym==COMMA);
}


/************************************************************************* fundamental func**********************************************************/
Type Parser::factor(set<Symbol> fsys, Operand& opd){
	if(debug)cout<<"\t\t【 factor】"<<endl;
	Type type = UNKNOWN;
	set<Symbol> semicolon_rbrace_set;
	semicolon_rbrace_set.insert(SEMICOLON);
	semicolon_rbrace_set.insert(RBRACE);
	//semicolon_rbrace_set.insert(RPARENT);
	int num;
	char ch;
	if(fsys.find(sym)==fsys.end()){		//if sym NOT in factor beginsys
		skip(semicolon_rbrace_set,37);	 //stop at ;}
	}
	switch(sym){
	case LPARENT:
		type = expr(exprbegsys,opd);
		if(sym!=RPARENT)	{			//here should be )
			skip(semicolon_rbrace_set,23);	 //stop at ;}
		}
		lexer.getsym();		//* or ;
		break;

	case PLUS:
		lexer.getsym();
		if(sym==POSNUMBER || sym==ZERONUMBER)	{		
			if(sym==POSNUMBER){
				num = lexer.num;
				type=INT;
				lexer.getsym();
				opd.type = oconst;
				opd.value.cont = num;
			}
			else{
				skip(semicolon_rbrace_set,2);	 //stop at ;}
			}
		}
		else{
			skip(semicolon_rbrace_set,3);	 //stop at ;}
		}
		break;

	case MINUS:
		lexer.getsym();
		if(sym==POSNUMBER || sym==ZERONUMBER)	{		
			if(sym==POSNUMBER){
				num = -lexer.num;
				type=INT;
				lexer.getsym();
				opd.type = oconst;
				opd.value.cont = num;
			}
			else{
				skip(semicolon_rbrace_set,2);	 //stop at ;}
			}
		}
		else{
			skip(semicolon_rbrace_set,3);	 //stop at ;}
		}
		break;

	case POSNUMBER:
		num = lexer.num;
		type=INT;
		lexer.getsym();
		opd.type = oconst;
		opd.value.cont = num;
		break;

	case ZERONUMBER:
		num = lexer.num;
		type=INT;
		lexer.getsym();
		opd.type = oconst;
		opd.value.cont = num;
		break;

	case CONSTCHAR:
		ch = lexer.word[0];
		type=CHAR;
		lexer.getsym();
		opd.type = oconst;
		opd.value.cont = (int)ch;
		break;

	case IDENT:
		int tabx=-1;
		int index=-1;
		Item item = table.get(currentTab,lexer.ident,tabx,index);
		lexer.getsym();
		if(index==-1){
			error.outputError(48);
		}
		else{
			opd.type = oident;
			opd.tabx = tabx;
			opd.value.ident = index;
			
			if(sym==LBRACKET){
				type = selector(exprbegsys, item, opd, true);
			}
			else if(sym==LPARENT){
				type = item.type;		//	函数类型
				if(type != INT && type != CHAR){
					skip(semicolon_rbrace_set,51);
					return type;
				}
				call(semicolon_rbrace_set,item,opd);
				/*valueparalist(exprbegsys);
				if(sym==RPARENT){
					lexer.getsym();
				}
				else{
					skip(semicolon_rbrace_set,23);
				}*/
			}		
			else{
				type=item.type;	//单个标识符
			}
		}
		break;
	}
	return type;
}

Type Parser::term(set<Symbol> fsys,Operand& opd){
	if(debug)cout<<"\t【 term】"<<endl;
	Operand opd1,opd2;
	set<Symbol> semicolon_rbrace_set;
	semicolon_rbrace_set.insert(SEMICOLON);
	semicolon_rbrace_set.insert(RBRACE);
	Type type = factor(facbegsys,opd1);
	opd=opd1;
	while(sym==MULTI || sym==DIVID){
		Symbol op=sym;
		lexer.getsym();
		Type typex = factor(facbegsys, opd2);
		type =resulttype(type,typex);
		if(opd1.type == oconst && opd2.type == oconst){	//都是常数  直接运算
			opd.type = oconst;
			if(op == MULTI){
				opd.value.cont = opd1.value.cont*opd2.value.cont;
			}
			else{
				opd.value.cont = opd1.value.cont/opd2.value.cont;
			}
		}
		else{
			stringstream ss;
			ss << "(" << temp++ << ")";
			table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

			opd.type = otemp;
			opd.tabx = currentTab;
			opd.value.temp = table.index(currentTab);
			if (op == MULTI) {
				Qcode.push_back(Quater(lev,MUL, opd, opd1, opd2));
			}
			else {
				Qcode.push_back(Quater(lev,DIV, opd, opd1, opd2));
			}
		}
		opd1=opd;
	}
	return type;
}

Type Parser::expr(set<Symbol> fsys, Operand& opd){			//fsys=exprbegsys
	if(debug)cout<<"【  Expression】"<<endl;
	Operand opd1,opd2;
	Type type=UNKNOWN;
	set<Symbol> semicolon_rbrace_set;
	semicolon_rbrace_set.insert(SEMICOLON);
	semicolon_rbrace_set.insert(RBRACE);
	lexer.getsym();				//get first sym of a expr 					
	if(fsys.find(sym)==fsys.end()){		//if sym NOT in  exprbegsys
		skip(semicolon_rbrace_set,36);	 //stop at ;}
		lexer.getsym();
		return type;		//avoid the following 
	}
	if(sym==PLUS || sym==MINUS){
		Symbol op = sym;
		lexer.getsym();
		type = term(facbegsys,opd1);
		if(op==MINUS){
			if(opd1.type == oconst){
				opd2.type=oconst;
				opd2.value.cont=-opd1.value.cont;
			}
			else{
				stringstream ss;
				ss<<"("<<temp++<<")";
				table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

				opd2.type=otemp;
				opd2.tabx=currentTab;
				opd2.value.temp = table.index(currentTab);
				Qcode.push_back(Quater(lev,NEG,opd2,opd1));
			}
			opd1=opd2;
		}
	}
	else{
		type=term(facbegsys,opd1);
	}
	opd=opd1;
	while(sym==PLUS || sym==MINUS){
		Symbol op = sym;
		lexer.getsym();
		Type typex = term(facbegsys,opd2);				//change begsys to factor begsys,,though they are the same
		type = resulttype(type,typex);

		if(opd1.type == oconst && opd2.type == oconst){
			opd.type = oconst;
			if(op==PLUS){
				opd.value.cont = opd1.value.cont+opd2.value.cont;
			}
			else{
				opd.value.cont = opd1.value.cont-opd2.value.cont;
			}
		}
		else{
			stringstream ss;
			ss << "(" << temp++ << ")";
			table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

			opd.type = otemp;
			opd.tabx = currentTab;
			opd.value.temp = table.index(currentTab);

			if (op == PLUS) {
				Qcode.push_back(Quater(lev,ADD, opd, opd1, opd2));
			}
			else {
				Qcode.push_back(Quater(lev,SUB, opd, opd1, opd2));
			}
		}
		opd1 = opd;
	}
	return type;
}

Type Parser::expr_beg_have_read(set<Symbol> fsys,Operand& opd){//NOT read the first of expr   [differ from expr();]
	if(debug)cout<<"【  Expression】"<<endl;
	Operand opd1,opd2;
	Type type=UNKNOWN;
	set<Symbol> semicolon_rbrace_set;
	semicolon_rbrace_set.insert(SEMICOLON);
	semicolon_rbrace_set.insert(RBRACE);
	//lexer.getsym();				//get first sym of a expr 					
	if(fsys.find(sym)==fsys.end()){		//if sym NOT in  exprbegsys
		skip(semicolon_rbrace_set,36);	 //stop at ;}
		lexer.getsym();
		return type;		//avoid the following 
	}
	if(sym==PLUS || sym==MINUS){
		Symbol op = sym;
		lexer.getsym();
		type = term(facbegsys,opd1);
		if(op==MINUS){
			if(opd1.type == oconst){
				opd2.type=oconst;
				opd2.value.cont=-opd1.value.cont;
			}
			else{
				stringstream ss;
				ss<<"("<<temp++<<")";
				table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

				opd2.type=otemp;
				opd2.tabx=currentTab;
				opd2.value.temp = table.index(currentTab);
				Qcode.push_back(Quater(lev,NEG,opd2,opd1));
			}
			opd1=opd2;
		}
	}
	else{
		type=term(facbegsys,opd1);
	}
	opd=opd1;
	while(sym==PLUS || sym==MINUS){
		Symbol op = sym;
		lexer.getsym();
		Type typex = term(facbegsys,opd2);				//change begsys to factor begsys,,though they are the same
		type = resulttype(type,typex);

		if(opd1.type == oconst && opd2.type == oconst){
			opd.type = oconst;
			if(op==PLUS){
				opd.value.cont = opd1.value.cont+opd2.value.cont;
			}
			else{
				opd.value.cont = opd1.value.cont-opd2.value.cont;
			}
		}
		else{
			stringstream ss;
			ss << "(" << temp++ << ")";
			table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

			opd.type = otemp;
			opd.tabx = currentTab;
			opd.value.temp = table.index(currentTab);

			if (op == PLUS) {
				Qcode.push_back(Quater(lev,ADD, opd, opd1, opd2));
			}
			else {
				Qcode.push_back(Quater(lev,SUB, opd, opd1, opd2));
			}
		}
		opd1 = opd;
	}
	return type;
}

Type Parser::resulttype(Type tp1,Type tp2){
	if(tp1==tp2)
		return tp1;
	else if(tp1 == INT&&tp2==CHAR)
		return CHAR;
	else if(tp1 == CHAR&&tp2==INT)
		return CHAR;
	else
		return INT;
}

Type Parser::selector(set<Symbol> fsys, Item item, Operand& opd, bool ifval) {
	
	Type type = UNKNOWN;
	Operand opd1, opd2;
	set<Symbol> rbracket_set;
	rbracket_set.insert(RBRACKET);
	opd1 = opd;
	type = expr(exprbegsys, opd2);
	if (type != INT) {
		error.outputError(49);
	}

	type = item.type;
	stringstream ss;
	ss << "(" << temp++ << ")";
	table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

	opd.type = otemp;
	opd.tabx = currentTab;
	opd.value.temp = table.index(currentTab);

	if (ifval) {
		Qcode.push_back(Quater(lev, SEL, opd, opd1, opd2));
	}
	else {
		Qcode.push_back(Quater(lev, OFF, opd, opd1, opd2));
	}

	if (sym == RBRACKET) {
		lexer.getsym();
	}
	else {
		error.outputError(17);
		if (sym == RPARENT) {	//...(1+a[2)*5...
			lexer.getsym();
		}
	}
	return type;
}

void Parser::call(set<Symbol> fsys, Item item, Operand& opd){
	vector<Quater> calQcode;
	calQcode.push_back(Quater(lev,SAVE));
	int i = 1;
	int paranum=0;
	do{
		lexer.getsym();
		if(sym == RPARENT)
			break;
		if(exprbegsys.find(sym) == exprbegsys.end()){
			skip(fsys,36);
			return;
		}
		Operand opd1,opd2;
		Type type = expr_beg_have_read(exprbegsys,opd1);
		opd2.type = oconst;
		opd2.value.cont = i++;
		calQcode.push_back(Quater(lev,PUSH,opd,opd1,opd2));	
		paranum++;
		if(exprbegsys.find(sym) != exprbegsys.end()){
			skip(fsys,11);
			return;
		}
	}while(sym==COMMA);
	if(item.paranum!=paranum){
		error.outputError(54);
	}
	for (int i = 0; i < (int)calQcode.size(); i++) {
		Qcode.push_back(calQcode[i]);
	}
	Operand cal = opd;
	if (item.object == VOIDFUNC) {
		Qcode.push_back(Quater(lev, CALVF, cal));
	}
	else {
		stringstream ss;
		ss << "(" << temp++ << ")";
		Type type = item.type;
		table.enter(currentTab, ss.str(), TEMP,type,addr,defaultarraysize,defaultnumvalue,defaultcharvalue);

		opd.type = otemp;
		opd.tabx = currentTab;
		opd.value.temp = table.index(currentTab);

		Qcode.push_back(Quater(lev, CALF, cal, opd));
	}
	if(sym == RPARENT){
		lexer.getsym();
	}
	else{
		skip(fsys,23);
	}
}
/************************************************************************statement*******************************************************************/

void Parser::statement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t【 statement.】"<<endl;
	set<Symbol> stop;stop.insert(RBRACE);
	set<Symbol> semicolon_set;semicolon_set.insert(SEMICOLON);
	set<Symbol> semicolon_rbrace_set = merge(semicolon_set,stop);
	stop=merge(stop,fsys);		//STOP AT stat begin or }
	Item item;
	int tabx = -1;
	int index = -1;
	Operand opd;
	switch(sym){
	case IFSYM:
		ifstatement(stop);
		break;
	case WHILESYM:
		whilestatement(stop);
		break;
	case LBRACE:
		if(debug)cout<<"\t\t\t\t\t 【 series statement.】"<<endl;
		lexer.getsym();
		while(statbegsys.find(sym)!=statbegsys.end()){			//a statementt
			statement(statbegsys);		//when finish   sym shuold =} or statbegsys
		}//end at }
		testrbrace(stop);
		break;
	case IDENT:
		item = table.get(currentTab, lexer.ident, tabx, index);
		opd.type = oident;
		opd.tabx = tabx;
		opd.value.ident = index;
		if(index==-1){
			skip(semicolon_set,48);	 //stop at ;
			testsemicolon(stop);
			break;
		}
		assign_or_callstatement(stop,item,opd);
		testsemicolon(stop);
		break;
	case SCANFSYM:
		scanfstatement(stop);
		testsemicolon(stop);
		break;
	case PRINTFSYM:
		printfstatement(stop);
		testsemicolon(stop);
		break;
	case SEMICOLON:
		if(debug)cout<<"\t\t\t\t\t 【 null statement.】"<<endl;
		testsemicolon(stop);
		break;
	case SWITCHSYM:
		switchstatement(stop);
		break;
	case RETURNSYM:
		returnstatement(stop);
		testsemicolon(stop);
		break;
	case FORSYM:
		error.outputError(34);
		do{
			lexer.getsym();
		}while(sym!=RBRACE);
		lexer.getsym();
		break;
	}
}

void Parser::ifstatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 if statement.】"<<endl;
	set<Symbol> semicolon_set;semicolon_set.insert(SEMICOLON);
	Operand lab1;
	lab1.type = olabel;
	lab1.value.label = ++label;
	lexer.getsym();
	if(sym==LPARENT){
		condition(exprbegsys,lab1,false);		//condition begin with expression
		if(sym==RPARENT){
			lexer.getsym();
			if(statbegsys.find(sym)!=statbegsys.end()){		//a statementt
				statement(statbegsys);		//when finish   sym shuold =} or statbegsys
			}
			else{
				skip(semicolon_set,26);
				return;
			}
		}
		else{
			skip(semicolon_set,23);
			return;
		}
		Qcode.push_back(Quater(lev, LAB, lab1));
	}
	else{
		skip(semicolon_set,32);
		return;
	}
}

void Parser::condition(set<Symbol> fsys,Operand& opd,bool nob){
	if(debug)cout<<"\t\t\t\t\t\t【 condition.】"<<endl;
	Operand opd1, opd2;
	Type typex = expr(exprbegsys, opd1);
	if(cmpop.find(sym)!=cmpop.end()){
		Symbol op = sym;
		Type typey = expr(exprbegsys,opd2);
		if(nob){
			switch (op) {
			case EQL:
				Qcode.push_back(Quater(lev, BEQ, opd, opd1, opd2));
				break;
			case NEQ:
				Qcode.push_back(Quater(lev, BNE, opd, opd1, opd2));
				break;
			case LSS:
				Qcode.push_back(Quater(lev, BLT, opd, opd1, opd2));
				break;
			case LEQ:
				Qcode.push_back(Quater(lev, BLE, opd, opd1, opd2));
				break;
			case GTR:
				Qcode.push_back(Quater(lev, BGT, opd, opd1, opd2));
				break;
			case GEQ:
				Qcode.push_back(Quater(lev, BGE, opd, opd1, opd2));
				break;
			}
		}
		else{
			switch (op) {
			case EQL:
				Qcode.push_back(Quater(lev, BNE, opd, opd1, opd2));
				break;
			case NEQ:
				Qcode.push_back(Quater(lev, BEQ, opd, opd1, opd2));
				break;
			case LSS:
				Qcode.push_back(Quater(lev, BGE, opd, opd1, opd2));
				break;
			case LEQ:
				Qcode.push_back(Quater(lev, BGT, opd, opd1, opd2));
				break;
			case GTR:
				Qcode.push_back(Quater(lev, BLE, opd, opd1, opd2));
				break;
			case GEQ:
				Qcode.push_back(Quater(lev, BLT, opd, opd1, opd2));
				break;
			}
		}
	}
	else{
		if(nob){
			Type typey=INT;
			opd2.type = oconst;
			opd2.value.cont = 0;
			Qcode.push_back(Quater(lev, BNE, opd, opd1, opd2));
		}
		else{
			Type typey=INT;
			opd2.type = oconst;
			opd2.value.cont = 0;
			Qcode.push_back(Quater(lev, BEQ, opd, opd1, opd2));
		}
	}
}

void Parser::whilestatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 while statement.】"<<endl;
	set<Symbol> semicolon_set;semicolon_set.insert(SEMICOLON);
	Operand lab1,lab2;
	lab2.type = olabel;		//before while
	lab2.value.label = ++label;
	Qcode.push_back(Quater(lev, LAB, lab2));
	lab1.type = olabel;	//after while
	lab1.value.label = ++label;
	lexer.getsym();
	if(sym==LPARENT){
		condition(exprbegsys,lab1,false);		//condition begin with expression
		if(sym==RPARENT){
			lexer.getsym();
			if(statbegsys.find(sym)!=statbegsys.end()){		//a statementt
				statement(statbegsys);		//when finish   sym shuold =} or statbegsys
			}
			else{
				skip(semicolon_set,26);
				return;
			}
		}
		else{
			skip(semicolon_set,23);
			return;
		}
		Qcode.push_back(Quater(lev, JMP, lab2));
		Qcode.push_back(Quater(lev, LAB, lab1));
	}
	else{
		skip(semicolon_set,32);
		return;
	}
}

void Parser::assign_or_callstatement(set<Symbol> fsys,Item item,Operand& opd){
	set<Symbol> semicolon_set;semicolon_set.insert(SEMICOLON);
	Operand src;
	Type typex = item.type;
	lexer.getsym();


	if(sym==LPARENT){	
		if(debug)cout<<"\t\t\t\t\t【 func_call statement.】"<<endl;
		/*valueparalist(exprbegsys);
		if(sym==RPARENT){
			lexer.getsym();
		}
		else{
			skip(semicolon_set,23);
			return;
		}*/
		call(semicolon_set, item, opd);
	}


	else if(sym==BECOMES){
		if(debug)cout<<"\t\t\t\t\t【 assign statement.】"<<endl;
		Type typey = expr(exprbegsys, src);
		if(match(typex,typey)){
			Qcode.push_back(Quater(lev,ASG, opd, src));
		}
		else{
			//skip(semicolon_set,50);
			Qcode.push_back(Quater(lev,ASG, opd, src));
		}
	}
	else if(sym==LBRACKET){
		if(debug)cout<<"\t\t\t\t\t【 assign statement.】"<<endl;
		typex = selector(exprbegsys,item,opd,false);			//			left side ==> false
		if(sym==BECOMES){
			Type typey = expr(exprbegsys,src);
			if(match(typex,typey)){
				Qcode.push_back(Quater(lev,ASG, opd, src));
			}
			else{
				//skip(semicolon_set,50);
				Qcode.push_back(Quater(lev,ASG, opd, src));
			}
		}
		else{
			skip(semicolon_set,29);
			return;
		}
	}
	else{
		skip(semicolon_set,45);
		return;
	}
}

void Parser::valueparalist(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t\t【 value_parameter list】"<<endl;
	Operand opd;
	if(sym==RPARENT)
		return;
	expr(exprbegsys,opd);
	while(sym==COMMA){
		expr(exprbegsys,opd);
	}
}//No need 

void Parser::switchstatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 switch statement.】"<<endl;
	set<Symbol> rbrace_set;
	rbrace_set.insert(RBRACE);
	Operand opd,finallab;
	finallab.type = olabel;
	finallab.value.label = ++label;
	lexer.getsym();
	if(sym==LPARENT){
		expr(exprbegsys,opd);
		if(sym==RPARENT){
			lexer.getsym();
			if(sym==LBRACE){
				lexer.getsym();
				if(sym==CASESYM){
					casestatement(fsys,opd,finallab);
					if(sym == DEFAULTSYM)
						defaultstatement(fsys);
					Qcode.push_back(Quater(lev, LAB, finallab));
					testrbrace(fsys);
				}
				else{
					skip(rbrace_set,41);
					return;
				}
			}
			else{
				skip(rbrace_set,7);
				return;
			}
		}
		else{
			skip(rbrace_set,23);
			return;
		}
	}
	else{
		skip(rbrace_set,32);				//bug if case statement has a }
		return;
	}
}

void Parser::casestatement(set<Symbol> fsys,Operand& opd, Operand& finallab){
	if(debug)cout<<"\t\t\t\t\t\t【 case list.】"<<endl;
	set<Symbol> rbrace_set;
	rbrace_set.insert(RBRACE);
	set<Symbol> case_default;
	case_default.insert(CASESYM);
	case_default.insert(DEFAULTSYM);
	do{
		Operand nextlab,opd2;
		int num;
		char ch;
		Type type;
		nextlab.type = olabel;
		nextlab.value.label = ++label;
		lexer.getsym();
		if(sym==PLUS || sym==MINUS || sym==CONSTCHAR || sym==POSNUMBER || sym==ZERONUMBER){
			if(sym == PLUS){
				lexer.getsym();
				num = lexer.num;
				type=INT;
				opd2.type = oconst;
				opd2.value.cont = num;	
			}
			else if (sym == MINUS){
				lexer.getsym();
				num = -lexer.num;
				type=INT;
				opd2.type = oconst;
				opd2.value.cont = num;	
			}
			else if(sym == CONSTCHAR){
				ch = lexer.word[0];
				type=CHAR;
				opd2.type = oconst;
				opd2.value.cont = (int)ch;	
			}
			else{
				num = lexer.num;
				type=INT;
				opd2.type = oconst;
				opd2.value.cont = num;	
			}
			Qcode.push_back(Quater(lev, BNE, nextlab, opd, opd2));
			lexer.getsym();
			if(sym!=COLON){
				skip(rbrace_set,43);
				return;
			}
			lexer.getsym();
			statement(merge(statbegsys,case_default));
			Qcode.push_back(Quater(lev,JMP,finallab));
			Qcode.push_back(Quater(lev, LAB, nextlab));
		}
		else{
			skip(rbrace_set,42);
			return;
		}
	}while(sym==CASESYM);
}

void Parser::defaultstatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t\t【 default statement.】"<<endl;
	set<Symbol> rbrace_set;
	rbrace_set.insert(RBRACE);
	lexer.getsym();
	if(sym!=COLON){
		skip(rbrace_set,43);
		return;
	}
	lexer.getsym();
	if(statbegsys.find(sym)!=statbegsys.end()){		//a statementt
		statement(statbegsys);		//when finish   sym shuold =} or statbegsys
	}
	else{
		////////////////DO NOTHING 
	}
}

void Parser::scanfstatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 scanf statement.】"<<endl;
	set<Symbol> semicolon_set;semicolon_set.insert(SEMICOLON);
	lexer.getsym();
	if(sym==LPARENT){
		do{
			lexer.getsym();
			if(sym!=IDENT){
				skip(semicolon_set,20);
				return;	// 否则会继续判断下面的右括号
			}
			int tabx = -1;
			int index = -1;
			Item item = table.get(currentTab, lexer.ident, tabx, index);
			if (index == -1) {
				error.outputError(48);
			}
			else if(item.object == FUNC || item.object == VOIDFUNC){
				error.outputError(53);
			}
			else {
				Operand opd;
				opd.type = oident;
				opd.tabx = tabx;
				opd.value.ident = index;
				Qcode.push_back(Quater(lev, RAD, opd));
			}
			lexer.getsym();
			if(sym==IDENT){		//缺少逗号
				skip(semicolon_set,11);
				return;	// 否则会继续判断下面的右括号
			}
		}while(sym==COMMA);
		if(sym==RPARENT){
			lexer.getsym();
		}
		else{
			skip(semicolon_set,23);
			return;
		}
	}
	else{
		skip(semicolon_set,32);
		return;
	}
}

void Parser::printfstatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 printf statement.】"<<endl;
	set<Symbol> semicolon_set;
	semicolon_set.insert(SEMICOLON);
	Operand opd;
	lexer.getsym();
	if(sym==LPARENT){																//CONST STRING
		lexer.getsym();																	//const string or expr
		if(sym==CONSTSTRING){
			int index = table.enterString(lexer.constString);
			opd.type = ostring;
			opd.value.str = index;
			Qcode.push_back(Quater(lev, WRT, opd));
			lexer.getsym();
			if(sym==COMMA){
				Type type=expr(exprbegsys,opd);
				type == INT? Qcode.push_back(Quater(lev, WRT, opd)):Qcode.push_back(Quater(lev, WRTC, opd));
				if(sym==RPARENT){
					lexer.getsym();
					return;																	//OK printf finish
				}
				else{
					skip(semicolon_set,23);
					return;
				}
			}
			else if(sym==RPARENT){
				lexer.getsym();
				return;																		//OK printf finish
			}
			else{
				skip(semicolon_set,23);
				return;
			}
		}		
		else if(exprbegsys.find(sym) != exprbegsys.end()){	//exprsion
			Type type =  expr_beg_have_read(exprbegsys,opd);
			type == INT? Qcode.push_back(Quater(lev, WRT, opd)):Qcode.push_back(Quater(lev, WRTC, opd));
			if(sym==RPARENT){
				lexer.getsym();
				return;																		//OK printf finish
			}
			else{
				skip(semicolon_set,23);
				return;
			}
		}	
		else{
			skip(semicolon_set,35);
			return;
		}
	}
	else{																							//error
		skip(semicolon_set,32);
		return;
	}
}

void Parser::returnstatement(set<Symbol> fsys){
	if(debug)cout<<"\t\t\t\t\t【 return statement.】"<<endl;
	set<Symbol> semicolon_set;semicolon_set.insert(SEMICOLON);
	Operand opd;
	lexer.getsym();
	if(sym==LPARENT){
		if(invoid){
			error.outputError(52);
		}
		expr(exprbegsys,opd);
		Qcode.push_back(Quater(lev, RET, opd));
		if(sym==RPARENT){
			lexer.getsym();
		}
		else{
			skip(semicolon_set,23);
			return;
		}
	}
	else{																															//return void
		opd.isNULL  = true;
		if(currentTab == mainTab){
			Qcode.push_back(Quater(lev,RETM,opd));
		}
		else{
			Qcode.push_back(Quater(lev, RET, opd));
		}
		
	}
}

/********************************************************************************ASM******************************************************************/


void Parser::writeCode(string op, string d, string s1, string s2) {

	fileAsm << op;
	if (d == "" && s1 == "" && s2 == ""&&op!="nop") {
		fileAsm << ":";
	}
	else {
		if (d != "") {
			fileAsm << "\t" << d;
		}
		if (s1 != "") {
			fileAsm << "," << s1;
		}
		if (s2 != "") {
			fileAsm << "," << s2;
		}
	}
	fileAsm << endl;
}

string Parser::loadAddr(Operand opd, int lev) {
	
	stringstream ss;
	if (opd.type == oident) {
		Item item = table.find(opd.tabx, opd.value.ident);
		if(opd.tabx == 0)
			ss << item.addr << "($gp)";
		else
			ss << item.addr << "($fp)";
	}
	else {
		Item item = table.find(opd.tabx, opd.value.temp);
		ss << item.addr << "($fp)";
	}
	return ss.str();
}

void Parser::loadOperand(Operand opd, string reg, int lev) {

	stringstream ss;
	if (opd.type == oconst) {
		ss << opd.value.cont;
		writeCode("li", reg, ss.str());
	}
	else if (opd.type == otemp) {
		writeCode("lw", reg, loadAddr(opd, lev));
	}
	else {
		Item item = table.find(opd.tabx, opd.value.ident);
		if (item.object == CONST) {
			ss.str("");
			if(item.type == INT)
				ss<<item.numvalue;
			else
				ss<<(int)item.charvalue;
			writeCode("li", reg, ss.str());
		}
		else {
			writeCode("lw", reg, loadAddr(opd, lev));
		}
	}
}

void Parser::ASM(){

// alloc glaobal space
	stringstream ss;
	//Initial const string
	fileAsm << "\t.data\n";
	int saddr = 0;
	for (int i = 0; i < table.getStabSize(); i++) {
		string s = table.findString(i);
		ss.str("");
		ss << "s" << i << ":\t.asciiz \"";
		fileAsm << ss.str() << s << "\"" << endl;
		saddr += s.length() + 1;
	}
	saddr = (saddr / 4 + 1) * 4;
	//start code
	fileAsm << "\n\t.text" << endl;
	//initialize stack and main frame
	if (table.getStabSize() > 0) {
		ss.str("");
		ss << saddr;
		//alloc for string
		writeCode("la", "$t0", "s0");
		//move $sp pointer
		writeCode("addi", "$sp", "$t0", ss.str());
		//initial $gp
		writeCode("move","$gp","$sp");
		ss.str("");
		ss<<table.howlong(0);
		//alloc for global var and func
		writeCode("addi", "$sp", "$sp", ss.str());
	}
	else {
		//move $sp pointer
		writeCode("li", "$sp", "0x10010000");
		//initial $gp
		writeCode("move","$gp","$sp");
		ss.str("");
		ss<<table.howlong(0);
		//alloc for global var and func
		writeCode("addi", "$sp", "$sp", ss.str());
	}

//Prepare call main
	//move $fp pointer
	writeCode("move", "$fp", "$sp");
	ss.str("");
	ss << table.howlong(mainTab);
	//alloc local and para
	writeCode("addi", "$sp", "$sp", ss.str());
	//alloc for 3 stores
	writeCode("addi", "$sp", "$sp", "12");
	//jump to Main
	writeCode("j", "Main");
	writeCode("nop","","");

//Begin Qcode analysis
	string cop[] = {
		"add", "sub", "mul", "div"
	};
	string bop[] = {
		"beq", "bne", "bgt", "bge", "blt", "ble"
	};
	Item item;
	for(int i = 0;i<(int)Qcode.size();i++){
		Quater q = Qcode[i];
		switch(q.opt){
		case ADD:
		case SUB:
		case MUL:
		case DIV:
			//load 1st operand
			loadOperand(q.src1, "$t1", q.lev);
			//load 2nd operand
			loadOperand(q.src2, "$t2", q.lev);
			//calculate
			writeCode(cop[q.opt], "$t1", "$t1", "$t2");
			//save result
			writeCode("sw", "$t1", loadAddr(q.dest, q.lev));
			break;
		case NEG:
			//load operand
			loadOperand(q.src1, "$t1", q.lev);
			//neg calculate
			writeCode("neg", "$t1", "$t1");
			//save result
			writeCode("sw", "$t1", loadAddr(q.dest, q.lev));
			break;
			break;
		case BEQ:
		case BNE:
		case BGT:
		case BGE:
		case BLT:
		case BLE:
			ss.str("");
			ss << "Label_" << q.dest.value.label;
			//load 1st operand
			loadOperand(q.src1, "$t1", q.lev);
			//load 2nd operand
			loadOperand(q.src2, "$t2", q.lev);
			//compare and jump label
			writeCode(bop[q.opt - 5], "$t1", "$t2", ss.str());
			writeCode("nop","","");
			break;
		case JMP:
			//jump label
			ss.str("");
			ss << "Label_" << q.dest.value.label;
			writeCode("j", ss.str());
			writeCode("nop","","");
			break;
		case OFF:
		case SEL:
			//load array start address
			writeCode("la", "$t1", loadAddr(q.src1, q.lev));
			//load element index
			loadOperand(q.src2, "$t2", q.lev);
			//calculate address offset
			writeCode("mul", "$t2", "$t2", "4");
			//calculate element address
			writeCode("add", "$t1", "$t1", "$t2");
			if (q.opt == SEL) {
				//load element
				writeCode("lw", "$t1", "0($t1)");
			}
			//save result
			writeCode("sw", "$t1", loadAddr(q.dest, q.lev));
			break;
		case ASG:
			loadOperand(q.src1, "$t1", q.lev);
			if (q.dest.type == oident) {
				writeCode("sw", "$t1", loadAddr(q.dest, q.lev));
			}
			else {  //loadAddress
				item = table.find(q.dest.tabx, q.dest.value.temp);
				ss.str("");
				ss << item.addr << "($fp)";
				writeCode("lw", "$t0", ss.str());
				writeCode("sw", "$t1", "0($t0)");
			}
			break;
		case RAD:
			item = table.find(q.dest.tabx, q.dest.value.ident);
			if (item.type == INT) {
				writeCode("li", "$v0", "5");
			}
			else {	//char
				writeCode("li", "$v0", "12");
			}
			fileAsm << "syscall" << endl;
			writeCode("sw", "$v0", loadAddr(q.dest, q.lev));
			break;

		case WRT:
			if (q.dest.type == ostring) {
				writeCode("li", "$v0", "4");
				ss.str("");
				ss << "s" << q.dest.value.str;
				writeCode("la", "$a0", ss.str());
			}
			else if (q.dest.type == oident) {
				item = table.find(q.dest.tabx, q.dest.value.ident);
				if (item.type == INT) {
					writeCode("li", "$v0", "1");
					loadOperand(q.dest, "$a0", q.lev);
				}
				else {
					writeCode("li", "$v0", "11");
					loadOperand(q.dest, "$a0", q.lev);
				}
			}
			else if (q.dest.type == otemp) {
				item = table.find(q.dest.tabx, q.dest.value.temp);
				if (item.type ==INT) {
					writeCode("li", "$v0", "1");
					loadOperand(q.dest, "$a0", q.lev);
				}
				else {
					writeCode("li", "$v0", "11");
					loadOperand(q.dest, "$a0", q.lev);
				}
			}
			else {		//simple number  or char 
				writeCode("li", "$v0", "1");
				ss.str("");
				ss << q.dest.value.str;
				writeCode("li", "$a0", ss.str());
			}
			fileAsm << "syscall" << endl;
			break;
		case WRTC:
			if (q.dest.type == ostring) {
				writeCode("li", "$v0", "4");
				ss.str("");
				ss << "s" << q.dest.value.str;
				writeCode("la", "$a0", ss.str());
			}
			else if (q.dest.type == oident) {
				item = table.find(q.dest.tabx, q.dest.value.ident);
				if (item.type == INT) {
					writeCode("li", "$v0", "1");
					loadOperand(q.dest, "$a0", q.lev);
				}
				else {
					writeCode("li", "$v0", "11");
					loadOperand(q.dest, "$a0", q.lev);
				}
			}
			else if (q.dest.type == otemp) {
				item = table.find(q.dest.tabx, q.dest.value.temp);
				if (item.type ==INT) {
					writeCode("li", "$v0", "1");
					loadOperand(q.dest, "$a0", q.lev);
				}
				else {
					writeCode("li", "$v0", "11");
					loadOperand(q.dest, "$a0", q.lev);
				}
			}
			else {		//simple number  or char 
				writeCode("li", "$v0", "11");
				ss.str("");
				ss << q.dest.value.str;
				writeCode("li", "$a0", ss.str());
			}
			fileAsm << "syscall" << endl;
			break;

		case CALVF:
		case CALF:
			item = table.find(q.dest.tabx, q.dest.value.ident);
			//record previous $fp
			writeCode("move","$k0","$fp");
			//move $fp pointer
			writeCode("move", "$fp", "$sp");
			//move $sp pointer
			ss.str("");
			ss << table.howlong(item.ref);
			//alloc local and para
			writeCode("addi", "$sp", "$sp", ss.str());
			//alloc for retrun value, previous $fp , $ra store
			writeCode("addi", "$sp", "$sp", "12");
			//save dynamic link
			writeCode("sw", "$k0", "-8($sp)");
			//jal : jump and link
			ss.str("");
			ss << "Label_" <<FuncLabelNum[item.ref];
			writeCode("jal", ss.str());
			//recover registers
			writeCode("nop","","");
			writeCode("lw", "$ra", "-4($sp)");
			if (q.opt == CALF) {			//save return value(in $v0) to src1
				writeCode("sw", "$v0", loadAddr(q.src1, q.lev));
			}
			break;
		case RET:
			//load value
			if(q.dest.isNULL == false)
				loadOperand(q.dest, "$v0", q.lev);
			//store temp $sp
			writeCode("move","$k0","$sp");
			//move $sp pointer
			writeCode("move","$sp","$fp");
			//move $fp pointer
			writeCode("lw", "$fp", "-8($k0)");
			//return
			writeCode("jr", "$ra");
			writeCode("nop","","");
			break;
		case RETM:
			fileAsm << "#RETURN MAIN";
			break;
		case PUSH:
			item = table.find(q.dest.tabx,q.dest.value.ident);
			loadOperand(q.src1,"$t1",q.lev);
			ss.str("");
			ss<<(q.src2.value.cont-1)*4<<"($sp)";
			writeCode("sw","$t1",ss.str());
			break;
		case SAVE:
			writeCode("sw", "$ra", "-4($sp)");
			break;
		case LAB:
			//output label
			if (q.dest.value.label == 0) {
				writeCode("\n#Start here\nMain");
			}
			else {
				ss.str("");
				ss << "\nLabel_" << q.dest.value.label;
				writeCode(ss.str());
			}
			break;
		}
	}
//End Qcode analysis
}
