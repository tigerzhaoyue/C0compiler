#include <iostream>
#include <fstream>
#include <cctype>
#include <string>
#include <vector>
#include "Symbol.h"
#include "typeofsym.h"
#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include"Table.h"
#include "Quater.h"
using namespace std;

extern int lc;
extern Error error;
extern Lexer lexer;	//new a Lexer
extern Parser parser;	//new a parser
extern Table table;
extern Quater quater;
extern fstream fileSrc;
extern Symbol sym;
extern fstream fileErr;
extern fstream fileTab;
extern fstream fileIns;
extern fstream fileAsm;
extern vector<string> Errors;
extern vector<Quater> Qcode;
int main(int argc, char* argv[]){
	string path;
	cout<<"Please input the file path:"<<endl;
	getline(cin, path);
	/**********************************************************open files***********************************************************/
	fileSrc = fstream(path, ios::in);
	fileErr = fstream("Error.txt", ios::out);
	fileTab = fstream("Table.txt",ios::out);
	fileIns = fstream("Instruction.txt", ios::out);
	fileAsm = fstream("Asm.asm", ios::out);
	if (!fileSrc.is_open()){
		cout << "Cannot find source file!" << endl;
	}
	lc=0;
	/***************************************************************´Ê·¨·ÖÎöÊä³ö****************************************************/
	/*
	inittypeofsym();
	while(lexer.ch!=EOF){
	lexer.getsym();
	cout<<typeofsym[int(sym)]<<" "<<lexer.word<<endl;
	}*/
	/*******************************************************************************************************************************/


	/*****************************************************************test area****************************************************/
	/*sym=RPARENT;
	set <Symbol> s;
	s.insert(RPARENT);
	parser.test(parser.programbegsys,s,1);
	if(parser.programbegsys.find(sym)!=parser.programbegsys.end())
		cout<<"PARENT IN NOW"<<endl;
	else
		cout<<"NO"<<endl;
	*/
	parser.parse();
	//quater.outputIns();
	if(error.success()){
		cout<<"\n Compile completed!"<<endl;
		table.Filladdr();
		table.outputTable();
		cout<<"\n Output table completed!"<<endl;
		quater.outputIns();
		cout<<"\n Output quater completed!"<<endl;
#ifdef OPTIMIZE
		optimizer.optimize();
		optimizer.outputOpt();
		cout << "\n Optimize instructions complete!\n" << endl;
#endif
		parser.ASM();
		cout << "\n Output ASM code completed!" << endl;
		cout << endl;
	}
	else{
		cout << "\n Compile failed!"<<endl;
		cout << "\n Find "<<error.num()<<" errors in total"<<endl;
		for(int i = 0 ; i<(int)Errors.size();i++){
			cout<<Errors[i];
		}
	}
	
/*********************************************************************************************************************************/
	fileSrc.close();
	fileErr.close();
	fileIns.close();
	fileTab.close();
	fileAsm.close();
	system("pause");
	return 0;
}