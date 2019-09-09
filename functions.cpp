#include "header.h"
extern unordered_map<string,string> appmap;
extern map<string,string> varmap;
extern map<string,string> expmap;
extern int last_cmd_status;
extern unordered_map<string,string> aliasmap;
void printWecomeMessage()
{
	cout<<"\n******************************************"; 
    cout<<"\n\t****Dh@ma SHELL****"; 

    cout<<"\n*******************"
        "***********************\n\n";    
}

void executeHistoryCommand()
{
	char * line = NULL;
    size_t len = 0;
    ssize_t read;

	FILE * fp = fopen("history","r");
	 while ((read = getline(&line, &len, fp)) != -1) {
        	cout<<line;
    }

	fclose(fp);
}

void getArguments(string str,vector<string> &args) 
{ 
   string word = ""; 
   for (auto x : str) 
   { 
       if (x == ' ') 
       {   
       		if(word!=" ")
           		args.push_back(word);
           word = ""; 
       } 
       else
       { 
 	          word = word + x; 
       } 
   }  
   args.push_back(word);
} 

int getArgumentsArray(vector<string> args_vector,char  *args[])
{

	int vect_size = args_vector.size();
	
	f(i,0,vect_size-1)
	{
		args[i] = new char[256];

		string s = args_vector[i];
		if(s[0]=='$')
		{
			string str = checkForVar(s);
			strcpy(args[i],str.c_str());			
		}
		else if(s == "~")
		{
	
			strcpy(args[i],getenv("HOME"));
		}

		else
		{
			strcpy(args[i],args_vector[i].c_str());			
		}
	}
	args[vect_size] = NULL;
	return 1;
}


bool isAssignmentCommand(string s)
{
	int fg=0;
	for(auto x : s)
	{
		if(x==' ')
			return false;
		if(x=='=')
			fg=1;
	}
	return fg==1;
}

string checkForVar(string s)
{	
	
	if(s=="$")
		return s;
	
	if(s=="~")
	{
		return string(getenv("HOME"));
	}

	if(s=="$$")
	{
		int id = getpid();
		return to_string(id);
	}

	if(s=="$?")
	{	
		return to_string(last_cmd_status);
	}

	if(s[0]!='$')
		return s;

	string s1 = s.substr(1);
	char *envar = getenv(s1.c_str());
	if(envar != NULL)
		return string(envar);
	if(varmap.find(s1)!=varmap.end())
		return varmap[s1];
	return "";
}

bool executeAssignmentCommand(string s)
{
	 for(auto x : s)
	{
		if(x==' ')
		{
			cout<<"Invalid = command"<<endl;
			return false;
		}
	}
	int index = s.find("=");
	string name = s.substr(0,index);
	string value = s.substr(index+1);
	cout<<"variable : "<<name<<"="<<checkForVar(value)<<endl;
	//enviornment var exist
	if(getenv(name.c_str()) != NULL)
		setenv(name.c_str(),checkForVar(value).c_str(),1);
	else
	{
		varmap[name] = checkForVar(value);
		if(expmap.find(name)!=expmap.end())
			expmap[name]=checkForVar(value);
	}
	return true;
}


void executeOpenCommand(vector<string> args_vector)
{

	char *args[3];
	args[0] = new char[256];
	args[1] = new char[256];
	args[2] = new char[256];
	string file = args_vector[1];
	string ext = file.substr(file.find("."));
	string path = appmap[ext];
	cout<<"Ext "<<ext<<endl;
	cout<<"Path "<<path<<endl;
	strcpy(args[0],path.c_str());
	strcpy(args[1],file.c_str());
	args[2]=NULL;
	int child = fork();
	if(child==0)
	{
		execvp(args[0],args);
	}
}


void startRecording()
{
	
	
	int fdrecord = open("record.txt",O_WRONLY | O_CREAT | O_TRUNC ,0644);
	char buffer[50];

	while(1)
	{
		int fdbuf = open("Buffer.txt", O_RDWR | O_CREAT | O_TRUNC,0644);
		string input;
		cout<<varmap["PS1"];
		getline(cin,input);
		write(fdrecord, (varmap["PS1"]).c_str(),varmap["PS1"].length());
		write(fdrecord, (input+"\n").c_str(),input.length()+1);

		if(input=="record stop")
		{	
			close(fdrecord);
			close(fdbuf);
			cout<<"Recorded Successfully in record.txt file"<<endl;
			return;
		}

		vector<string> args_vector;
		getArguments(input,args_vector);
		//cout<<"input "<<input<<endl;
		int vect_size = args_vector.size();
	    char  *args[vect_size+1];
	    getArgumentsArray(args_vector,args);
	    
	    
		if(fork()==0)
		{
			dup2(fdbuf,1);
		    if(execvp(args[0], args)==-1)
			{
				exit(1);
			}
		}
		else
		{
			wait(NULL);
			char wbuffer[50];
			lseek(fdbuf,0,SEEK_SET);
			close(fdbuf);

			fdbuf = open("Buffer.txt", O_RDONLY,0644);
			if(fdbuf==-1)
			{
				cout<<"error buffer fle";
				return;
			}
			int n=read(fdbuf,wbuffer,50);

			while (n>0)
			{
				
				if(n>0)
				{
					write(fdrecord,wbuffer,n);
					write(1,wbuffer,n);
				    n=read(fdbuf,wbuffer,50);
				}
			}
			close(fdbuf);
		}		
		
	}
	
}