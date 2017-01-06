#ifndef __LEXER_H__
#define __LEXER_H__

#define WORD_MAX 1000
#define LINE_MAX 1000

#include <string>
#include <cctype>
#include "Symbol.h"

using namespace std;

class Lexer {
private:
	bool isConstChar;  //flag of const char(initial to false)
	bool isConstString;  //flag of const string(initial to false)
	char line[LINE_MAX];  //line buffer area
	int cc;  //char counter
	int ll;  //line length
	int k;   //present word's length
	//{these four var is private for Lexer class}

public:
	map<char,Symbol> CharMap;
	map<string,Symbol> ReservedMap;
	char ch;  //present char
	char constChar;  //const char
	string constString;  //const string
	string ident;  //present word
	int num;  //value of present number
	char word[WORD_MAX];  //present word
	Lexer();	//construction func
	void initialReserved();
	void getch();//read a char
	void getsym();//read a word
};

#endif