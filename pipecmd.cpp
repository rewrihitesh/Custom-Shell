#include "header.h"

bool pipecommand(vector<string> & cmd)
{
	
	f(i,0,cmd.size()-1) 
	{
		string token = cmd[i];
		
		if(token == "|")
			return true;
	}
	return false;
}

void separatePipe(vector<string> source,vector<string> &desti)
{
	string s = "";
	f(i,0,source.size()-1)
	{
		string token = source[i];
		if(token=="|" && s!="")
		{
			desti.push_back(s.substr(0,s.length()-1));			
			s = "";
		}
		else
		{
			s = s + token + " ";
		}
	}
	desti.push_back(s.substr(0,s.length()-1));
	
}
void getArgsFromString(string s, char *args[])
{
   string word = ""; 
   int i=0;
   for (auto x : s) 
   { 
       if (x == ' ') 
       {   
       		if(word!=" " && word!="")
       		{
       			args[i] = new char[256];
       			strcpy(args[i++],word.c_str());	
       			//cout<<args[i-1]<<endl;
       		}
           word = ""; 
       } 
       else
       { 
 	          word = word + x; 
       } 
   }  
   args[i] = new char[256];
   strcpy(args[i++],word.c_str());
   //cout<<args[i-1];
   args[i] = NULL;
}




void executePipeCommands(vector<string> cmd)
{
	
	vector<string> vectcmd;
	separatePipe(cmd,vectcmd);
	
	int fd[2];	
	int fdd = 0;
	f(i,0,vectcmd.size()-1)
	{
		if(pipe(fd)<0)
		{
			cout<<"error in pipe";
			return;
		}
		
		char *args[128];
		getArgsFromString(vectcmd[i],args);
	
		int pid = fork();
		if(pid==0)
		{
			if(i!=0)
				dup2(fdd,0);
			
			if(i!=(vectcmd.size()-1))
				dup2(fd[1],1);
			
			close(fd[0]);
			close(fd[1]);
			execvp(args[0],args);
		}
		else{
			wait(NULL);		
			//close(fd[0]);
			close(fd[1]);
			fdd = fd[0];
			
		}
	}

	// close(fd[0]);
	// close(fd[1]);
	// while(wait(NULL)>0);
}
