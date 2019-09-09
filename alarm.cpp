#include "header.h"
int p[2];
extern vector<int> vectalarm;
extern vector<pair<time_t,string>> allalarm;

//When enter into shell check all alarms from alarm.txt 
//if alarm time < current time
//	alarm is missed so print appropiate message
//else
//	set alarm for this shell

void checkMissedAlarms()
{
	ifstream infile("alarm.txt");
	
	for(string line;getline( infile, line);)
	{
		time_t timestamp = stoll(line.substr(0,line.find(":")));
		string msg = line.substr(line.find(":")+1);
		if(time((time_t *)0)>timestamp)
		{
			cout<<"Missed Alarm "<<timestamp <<" : "<<msg<<endl;
			continue;
		}
		else
		{
			checkAlarm(timestamp,msg);
		}
	}
	infile.close();
}

//create a child process for each alarm
void checkAlarm(time_t alarmtime,string msg)
{
	int child=fork();
	vectalarm.push_back(child);
	if(child==0)
	{
		while(time((time_t *)0)!=alarmtime){
		}
		cout<<"Alarm : "<<msg<<"\n";
		exit(1);
	}
}
	
void alarmHandler(int x,string msg)
{
	time_t curtime = time((time_t*)0);
	time_t alarmtime	 = curtime + x;
	cout<<"Alarm set : "<<curtime<<":"<<alarmtime<<" : "<<msg<<endl<<endl;
	allalarm.push_back(make_pair(alarmtime,msg));
	checkAlarm(alarmtime,msg);
	return;
}

//when exit from shell store each alarm in text file
void alarmExit()
{
	for(int i=0;i<vectalarm.size();i++)
	{
		kill(vectalarm[i],SIGKILL);
	}
	
	int fd = open("alarm.txt",O_WRONLY | O_TRUNC | O_CREAT,0644);
	
	if(fd==-1)	
	{
		cout<<"Not able to open alarm file";
		return;
	}

	for(auto alm : allalarm)
	{
		if(alm.first > time((time_t *)0))
		{
			string alarm_record = to_string(alm.first)+":"+alm.second+"\n";
			write(fd,alarm_record.c_str(),alarm_record.size());
		}
	}
	close(fd);
}