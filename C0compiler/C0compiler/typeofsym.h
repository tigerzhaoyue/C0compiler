#include <string>

using namespace std;




string typeofsym[41];

void inittypeofsym(){
	typeofsym[0]="IDENT";
    typeofsym[1]="NUMBER"; 
	typeofsym[2]="PLUS"; 
	typeofsym[3]="MINUS"; 
	typeofsym[4]="MULTI"; 
	typeofsym[5]="DIVID"; 
	typeofsym[6]="EQL"; 
	typeofsym[7]="NEQ"; 
	typeofsym[8]="LSS"; 
	typeofsym[9]="LEQ"; 
	typeofsym[10]="GTR"; 
	typeofsym[11]="GEQ"; 
	typeofsym[12]="LPARENT"; 
	typeofsym[13]="RPARENT";
	typeofsym[14]="LBRACKET";
	typeofsym[15]="RBRACKET";
	typeofsym[16]="LBRACE";
	typeofsym[17]="RBRACE";
	typeofsym[18]="SINGQUO";
	typeofsym[19]="DOUBQUO";
	typeofsym[20]="COMMA";
	typeofsym[21]="SEMICOLON";
	typeofsym[22]="BECOMES";
	typeofsym[23]="IFSYM";
	typeofsym[24]="ELSESYM";
	typeofsym[25]="FORSYM";
	typeofsym[26]="WHILESYM";
	typeofsym[27]="SCANFSYM";
	typeofsym[28]="PRINTFSYM";
	typeofsym[29]="SWITCHSYM";
	typeofsym[30]="CASESYM";
	typeofsym[31]="DEFAULTSYM";
	typeofsym[32]="VOIDSYM";
	typeofsym[33]="RETURNSYM";
	typeofsym[34]="MAINSYM";
	typeofsym[35]="CONSTSYM";
	typeofsym[36]="INTSYM";
	typeofsym[37]="CHARSYM";
	typeofsym[38]="CONSTCHAR";
	typeofsym[39]="CONSTSTRING";
	typeofsym[40]="COLON";
	typeofsym[41]="POSNUMBER";
	typeofsym[42]="ZERONUBMER";
	typeofsym[43]="UNKNOWNSYM";
}