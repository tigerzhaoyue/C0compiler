#include <fstream>
#include <vector>
#include <string>
#include "Symbol.h"
#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "Table.h"
#include "Quater.h"

using namespace std;

int lc;			//line counter
int lev;			//level of program
Symbol sym;		//type of present vacabulary
Lexer lexer;	//new a Lexer
Error error;
Parser parser;	//new a parser
Quater quater;
Table table;

vector<string> Errors;
vector<Quater> Qcode;
vector<vector<Quater>> Block;

fstream fileSrc;//source code file
fstream fileErr;
fstream fileTab;
fstream fileIns;
fstream fileAsm;