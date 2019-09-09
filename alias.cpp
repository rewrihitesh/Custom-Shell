#include "header.h"
extern unordered_map<string,string> aliasmap;

bool isAliasCommand(string s)
{
	if(s.substr(0,5)=="alias")
		return true;
	else
		return false;
}

void executeAliasCommand(string input)
{
	if(input=="alias -p")
	{
		printAlias();
		return;	
	}

	int i=5;
	while(i<input.length() && input[i]==' ')
		i++;

	if(i==input.length() || input.find("\"")==-1 && input.find("'")==-1)
	{
		cout<<"invalid aliasing, User below syntax"<<endl;
		cout<<"alias name='value'";
		return;
	}
	
	input=input.substr(i);

	int equ_index = input.find('=');
	
	if(equ_index==-1)
	{
		cout<<"invalid aliasing"<<endl;
		return;
	}

	string lhs = input.substr(0,equ_index);
	string rhs="";
	f(i,equ_index+2,input.length()-2)
		rhs = rhs + input[i];
	
	aliasmap[lhs] = rhs;
	cout<<"alias "<<lhs<<" : "<<rhs<<endl;
}

void printAlias()
{
	for(auto x : aliasmap)
	{
		cout<<"alias "<<x.first<<" : "<<x.second<<endl;
	}
	return;
}



string  aliasFilter(string s)
{
	
    string word = ""; 
    string ans = "";
   for (auto x : s) 
   { 
       if (x == ' ') 
       { 
       
           if(aliasmap.find(word)!=aliasmap.end())           
           		ans = ans + aliasmap[word]+ " ";           
           else           
           		ans = ans + word + " ";
           
           word = ""; 
       } 
       else
       { 
           word = word + x; 
       } 
   }  
	 
	if(aliasmap.find(word)!=aliasmap.end())           
			ans = ans + aliasmap[word];           
	else           
			ans = ans + word;
     
    return ans;

}