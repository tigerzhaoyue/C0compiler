#ifndef __TABLE_H__
#define	__TABLE_H__

#include <vector>
#include <string>

using namespace std;

enum Object {
	CONST, VAR,ARRAY, VOIDFUNC, FUNC, PARA, TEMP
};

enum Type {
	UNKNOWN,CHAR,INT,VOID,ARRAYS
};

class Item{
public:
	string name;
	Object object;
	int numvalue;
	char charvalue;
	Type type;
	int addr;		//offset
	int size;		//if array how long?
	int ref;			//for function
	int paranum;

};

class TableItem{
public:
	int howlong;
	vector<Item> thistable;
};

class Table{
public:
	Table();
	vector<TableItem> table;
	vector<string> stab;

	void enter(int current, string name, Object object, Type type, int addr,int size=-1,int numvalue=0,char charvalue=' ',int ref=0,int paranum=0);
	int enterString(string s);
	int newTable();
	int index(int current);
	bool AssertEnter(int current, string name);

	Item find(int tabx, int index);
	Item get(int current, string ident, int &tabx,int& index);
	string findString(int index);
	int getStabSize();
	void Filladdr();
	void outputTable();
	int howlong(int current);
	int getparanum(Item item);
};
#endif