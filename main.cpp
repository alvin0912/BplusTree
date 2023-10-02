#include<iostream>
#include <ctype.h>
#include"BPtree.h"
using namespace std;

void help(){
	cout << endl;
	cout << "=====================" << endl;
    cout << "Insert      : +<key>,<data>" << endl;
    cout << "Delete      : -<key>" << endl;
    cout << "Quit        : quit" << endl;
	cout << "Show List   : show" << endl;
	cout << "Display Tree: display" << endl;
	cout << "Help        : help" << endl;
	cout << "=====================" << endl;
	cout << endl;
}

bool foolproof(string command){
	if(command.find('+',0) == string::npos && command.find('-',0) == string::npos)return true;
	return false;
}

int main(){
    int order=0;
    cout << "please input order" << endl;
	cout << ">> ";
    cin >> order;
	cout << "ok,order is " << order << endl;
	BPtree bptree(order);

	help();
	string command;
	int end=0;
	int key;
	string data;
	while(1){
		cout << "please input command." << endl;
		cout << ">> ";
		cin >> command;

		if(command == "quit")break;
		else if(command == "help")help();
		else if(command.find("show",0) != string::npos){
			bptree.showList(1);
			bptree.showList(-1);
		}
		else if(command == "display"){
			cout << "===========================" << endl;
			bptree.display(bptree.getRoot());
			cout << "===========================" << endl;
		}
		else if(command[0]=='+'){
			if(foolproof(command))cout << "invalid input !" << endl;
			else {
				end=command.find(',');
				data=command.substr(1,end-1);
				key=stoi(data);
				data=command.substr(end+1);
				bptree.insert(key,data);
			}
		}
		else if(command[0]=='-'){
			if(foolproof(command))cout << "invalid input !" << endl;
            else {
                end=command.find(',');
                data=command.substr(1,end-1);
                key=stoi(data);
                bptree.drop(key);
            }
		}
		else cout << "invalid input !" << endl;

		cout << endl;
	}
}
