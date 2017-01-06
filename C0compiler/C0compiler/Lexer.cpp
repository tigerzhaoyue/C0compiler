#include <iostream>
#include <fstream>
#include <sstream>
#include "Symbol.h"
#include "Error.h"
#include "Lexer.h"
using namespace std;

extern int lc;  //line counter
extern Symbol sym;  //type of present word
extern Error error;
extern fstream fileSrc;

Lexer::Lexer() {
	isConstChar = false;
	isConstString = false;
	cc = 0;
	ll = 0;
	k = 0;
	ch = ' ';
	initialReserved();
}

void Lexer::initialReserved(){
	CharMap['+']=PLUS;
	CharMap['-']=MINUS;
	CharMap['*']=MULTI;
	CharMap['/']=DIVID;
	CharMap['=']=BECOMES;
	CharMap['<']=LSS;
	CharMap['>']=GTR;
	CharMap['(']=LPARENT;
	CharMap[')']=RPARENT;
	CharMap['[']=LBRACKET;
	CharMap[']']=RBRACKET;
	CharMap['{']=LBRACE;
	CharMap['}']=RBRACE;
	CharMap['\'']=SINGQUO;
	CharMap['\"']=DOUBQUO;
	CharMap[',']=COMMA;
	CharMap[';']=SEMICOLON;
	CharMap[':']=COLON;

	ReservedMap["const"]=CONSTSYM;
	ReservedMap["int"]=INTSYM;
	ReservedMap["char"]=CHARSYM;
	ReservedMap["if"]=IFSYM;
	ReservedMap["else"]=ELSESYM;
	ReservedMap["for"]=FORSYM;
	ReservedMap["while"]=WHILESYM;
	ReservedMap["switch"]=SWITCHSYM;
	ReservedMap["case"]=CASESYM;
	ReservedMap["default"]=DEFAULTSYM;
	ReservedMap["void"]=VOIDSYM;
	ReservedMap["main"]=MAINSYM;
	ReservedMap["return"]=RETURNSYM;
	ReservedMap["printf"]=PRINTFSYM;
	ReservedMap["scanf"]=SCANFSYM;
}

void Lexer::getch() {
	if (cc == ll) {
		if (fileSrc.eof()) {
			error.outputError(1);
			fileSrc.close();
		}
		ll = 0;
		cc = 0;
		lc++;
		while (!fileSrc.eof() && ch != '\n') {
			ch = fileSrc.get();
			line[ll++] = ch;
		}
		if (ch == '\n') {
			line[ll - 1] = ' ';
		}
	}
	ch = line[cc++];
}

void Lexer::getsym(){
	k=0;
	word[0]='\0';
	num=0;	
	while ((!isConstChar && !isConstString) && (ch == ' ' || ch == '\t' || ch == '\n'))		//skip the umimportant word
		getch();

	/*if (isConstChar && ch != '\'') {																				//constchar before the next "\'"
		constChar = ch;
		if (k < WORD_MAX) {
				word[k++] = ch;
			}
		word[k] = '\0';
		if (!(isalpha(ch)||ch=='_') && !isdigit(ch)&&(ch!='+')&&(ch!='-')&&(ch!='*')&&(ch!='/')) {		//	£¼×Ö·û£¾::=  '£¼¼Ó·¨ÔËËã·û£¾'£ü'£¼³Ë·¨ÔËËã·û£¾'£ü'£¼×ÖÄ¸£¾'£ü'£¼Êý×Ö£¾'									//
			//ERROR HERE!;
		}
		sym = CONSTCHAR;
		getch();
		return;
	}
	*/
	/*if (isConstString && ch != '"') {														//£¼×Ö·û´®£¾   ::=  "£ûÊ®½øÖÆ±àÂëÎª32,33,35-126µÄASCII×Ö·û£ý"
		stringstream ss;
		while (ch >= 32 && ch <= 126 && ch != 34) {						//Until it's a illegal char
			ss << ch;
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();
		}
		word[k] = '\0';
		constString = ss.str();
		sym = CONSTSTRING;
		return;
	}
	*/
	if (isalpha(ch) || ch=='_') {															//£¼±êÊ¶·û£¾    ::=  £¼×ÖÄ¸£¾£û£¼×ÖÄ¸£¾£ü£¼Êý×Ö£¾£ý
		k = 0;
		do {
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();
		} while (isalpha(ch) || ch=='_' || isdigit(ch));
		word[k] = '\0';
		ident = string(word);
		map<string, Symbol>::const_iterator itr = ReservedMap.find(ident);
		if (itr == ReservedMap.end()) {												//never find such a reserved word in Reservedword map, meaning this is an available identity
			sym = IDENT;
		}
		else {
			sym = itr->second;															//a type in Reserved word map
		}
	}

	else if (isdigit(ch)) {
		k = 0;
		num = 0;
		do {
			num = num * 10 + ch - '0';
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();	
		} while (isdigit(ch));
		word[k] = '\0';
		if(word[0]=='0'){
			if(word[1]=='\0')
				sym = ZERONUMBER;
			else{
				sym = POSNUMBER;
				error.outputError(2);
			}
		}
		else
			sym = POSNUMBER;
	}

	else if (ch == '<') {																			//    <=	&	<
		if (k < WORD_MAX) {
				word[k++] = ch;
			}
		getch();
		if (ch == '=') {
			sym = LEQ;
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();
		}
		else {
			sym = LSS;
		}
		word[k] = '\0';
	}
	else if (ch == '>') {																		//>=	&	>
		if (k < WORD_MAX) {
				word[k++] = ch;
			}
		getch();
		if (ch == '=') {
			sym = GEQ;
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();
		}
		else {
			sym = GTR;
		}
		word[k]='\0';
	}
	else if (ch == '!') {																		//>=	&	>
		if (k < WORD_MAX) {
				word[k++] = ch;
			}
		getch();
		if (ch == '=') {
			sym = NEQ;
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();
		}
		else {
			sym = NEQ;
			error.outputError(24);
		}
		word[k]='\0';
	}
	else if (ch == '=') {
		if (k < WORD_MAX) {
				word[k++] = ch;
			}
		getch();
		if (ch == '=') {
			sym = EQL;
			if (k < WORD_MAX) {
				word[k++] = ch;
			}
			getch();
		}
		else
			sym = BECOMES;
		word[k]='\0';
	}

	else if (ch == '\'') {
		//	isConstChar = !isConstChar;					//tackle the switch of constchar
		sym = CONSTCHAR;
		getch();
		if(ch == '\'')						/* NULL CHARE*/
		{
			getch();
			word[k]='\0';
			error.outputError(6);
			return;						//exit , avoid following  aassert
		}
		else {								//not a null char						
			if(ch=='+' || ch=='-' || ch=='*' || ch=='/' || isdigit(ch) || ch =='_' || isalpha(ch)){		//correct constchar
				if (k < WORD_MAX) {
					word[k++] = ch;
				}
				getch();
			}
			else{						//wrong char type  
				error.outputError(4);
				getch();
			}
		}
		if(ch=='\''){
			getch();
		}
		else{
			error.outputError(46);
		}		
		word[k]='\0';
	}

	else if (ch == '\"') {
		//isConstString = !isConstString;				
		stringstream ss;
		sym = CONSTSTRING;
		getch();
		while(ch!='\"'){
			if (k < WORD_MAX) {
				word[k++] = ch;
				ss<<ch;
				getch();
			}
		}
		if(ch=='\"'){	//seems OK
			getch();
		}
		else{
			error.outputError(33);
		}
		word[k]='\0';
		constString = ss.str();
	}

	else {														//Something in CharMap
		map<char, Symbol>::const_iterator itr = CharMap.find(ch);
		if (itr == CharMap.end()) {				//What a strange char!!!!
			//error.outputError(22);
			sym=UNKNOWNSYM;
		}
		else {
			sym = itr->second;					//Maybe ()[]{}...
		}
		if (k < WORD_MAX) {
				word[k++] = ch;
			}
		getch();
		word[k]='\0';
	}
}