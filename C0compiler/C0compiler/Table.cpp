#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "Symbol.h"
#include "Table.h"
extern fstream fileTab;
using namespace std;


Table::Table() {}

int Table::newTable() {				//return the index of new table
	TableItem newtab;
	table.push_back(newtab);			//add new table in the tail
	/*stringstream ss1;
	stringstream ss2;
	ss1 << "\n\n Table" << setw(4) << table.size() - 1 <<"\n" ;
	ss2<<std::right<<setw(10)<< "name"<<std::right <<setw(10)<< "object"<<std::right<<setw(10)<<"type"<<std::right<<setw(10)<<"addr"<<std::right<<setw(10)<<"size"<<std::right<<setw(10)<<"intvalue"<<std::right<<setw(10)<<"chvalue"<<setw(10)<<"ref"<<"\n";
	cout << "Table" << setw(4) << table.size() - 1 <<endl ;
	cout<<std::right<<setw(10)<< "name"<<std::right <<setw(10)<< "object"<<std::right<<setw(10)<<"type"<<std::right<<setw(10)<<"addr"<<std::right<<setw(10)<<"size"<<std::right<<setw(10)<<"intvalue"<<std::right<<setw(10)<<"chvalue"<<setw(10)<<"ref"<<endl;
	fileTab << ss1.str()<<ss2.str();*/
	return table.size() - 1;
}

void Table::enter(int current, string name, Object object, Type type, int addr,int size,int numvalue, char charvalue,int ref,int paranum){
	Item item;
	item.name=name;
	item.object=object;
	item.type=type;
	item.addr=addr;
	item.size=size;
	item.numvalue=numvalue;
	item.charvalue=charvalue;
	item.ref=ref;
	item.paranum = paranum;
	table[current].thistable.push_back(item);
/*	stringstream ss2;
	ss2<<setw(10)<< name ;
	ss2<<setw(10)<< object;
	ss2<<setw(10)<<type;
	ss2<<setw(10)<<addr;
	ss2<<setw(10)<<size;
	ss2<<setw(10)<<numvalue;
	ss2<<setw(10)<<charvalue;
	ss2<<setw(10)<<ref<<"\n";
	//cout<<std::right<<setw(10)<< name <<std::right<<setw(10)<< object<<std::right<<setw(10)<<type<<std::right<<setw(10)<<addr<<std::right<<setw(10)<<size<<std::right<<setw(10)<<numvalue<<std::right<<setw(10)<<charvalue<<setw(10)<<ref<<"\t";
	fileTab <<ss2.str();*/
	//cout<<std::right<<setw(8)<<name<<"¡ª¡ª> table"<<current<<endl;
}

int Table::index(int current){					//return the No.para table's size-1
	return table[current].thistable.size()-1;
}

int Table::enterString(string s) {		   //return the tail of stab
	stab.push_back(s);
	return stab.size() - 1;
}

bool Table::AssertEnter(int current,string name){
	for(int i=this->index(current); i>=0 ; i--){				//is current exit such a ident? if so return false
		if(table[current].thistable[i].name == name)
			return false;
	}
	return true;
}

Item Table::find(int tabx, int index) {
	return table[tabx].thistable[index];
}

Item Table::get(int current, string ident, int& tabx, int& index){
	int i = current;
	int j = table[i].thistable.size() - 1;
	while (j >= 0) {
			if (table[i].thistable[j].name == ident) {
				tabx = i;
				index = j;
				return table[i].thistable[j];
			}
			j--;
	}
	i = 0;
	j = table[i].thistable.size() - 1;
	while (j >= 0) {
			if (table[i].thistable[j].name == ident) {
				tabx = i;
				index = j;
				return table[i].thistable[j];
			}
			j--;
	}
	return Item();
}

string Table::findString(int index) {
	return stab[index];
}

int Table::getStabSize() {
	return stab.size();
}

void Table::Filladdr(){
	for(int i=0;i<(int)table.size();i++){
		table[i].howlong=0;
		for(int j = 0;j<(int)table[i].thistable.size();j++){
			table[i].thistable[j].addr = table[i].howlong;
			if(table[i].thistable[j].size == -1){
				table[i].howlong=table[i].howlong+4;
			}
			else{
				table[i].howlong = table[i].howlong+4*table[i].thistable[j].size;
			}
		}
	}
}

void Table::outputTable(){
	stringstream ss;
	for(int i=0;i<(int)table.size();i++){
			ss.str("");
			ss<<"\n\n Table"<<i<<"\n";
			ss<<setw(10)<<"name" ;
			ss<<setw(10)<<"object";
			ss<<setw(10)<<"type";
			ss<<setw(10)<<"addr";
			ss<<setw(10)<<"size";
			ss<<setw(10)<<"cnum";
			ss<<setw(10)<<"cchar";
			ss<<setw(10)<<"ref";
			ss<<setw(10)<<"paranum"<<"\n";
			fileTab <<ss.str();
		for(int j = 0;j<(int)table[i].thistable.size();j++){
			ss.str("");
			ss<<setw(10)<<table[i].thistable[j].name ;
			ss<<setw(10)<<table[i].thistable[j].object;
			ss<<setw(10)<<table[i].thistable[j].type;
			ss<<setw(10)<<table[i].thistable[j].addr;
			ss<<setw(10)<<table[i].thistable[j].size;
			ss<<setw(10)<<table[i].thistable[j].numvalue;
			ss<<setw(10)<<table[i].thistable[j].charvalue;
			ss<<setw(10)<<table[i].thistable[j].ref;
			ss<<setw(10)<<table[i].thistable[j].paranum<<"\n";
			fileTab <<ss.str();
		}
	}
	//cout<<std::right<<setw(10)<< name <<std::right<<setw(10)<< object<<std::right<<setw(10)<<type<<std::right<<setw(10)<<addr<<std::right<<setw(10)<<size<<std::right<<setw(10)<<numvalue<<std::right<<setw(10)<<charvalue<<setw(10)<<ref<<"\t";
	
}

int Table::howlong(int current){
	return table[current].howlong;
}

int Table::getparanum(Item item){
	int num = 0;
	while(table[item.ref].thistable[num].object == PARA){
		num++;
	}
	return num;
}