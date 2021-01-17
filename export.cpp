#include "header.h"
extern map<string,string> varmap;
extern map<string,string> expmap;



bool isExportCommand(string input){
	if(input.substr(0,6)=="export")
		return true;
	return false;
}

void executeExportCommand(string input)
{
	string variable = input.substr(7);
	//cout<<variable<<"is var"<<endl;
	if(varmap.find(variable)!=varmap.end())
	{
		//expmap.insert({variable,varmap[variable]});
		expmap[variable] = varmap[variable];
		cout<<"exported to all childs "<<endl;
	}
	else
	{
		cout<<"var not found"<<endl;
	}
}





