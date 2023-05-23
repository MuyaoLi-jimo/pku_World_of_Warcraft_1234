#include<iostream>
#include<string>
#include<queue>
using namespace std;

class Soldier 
{
private:
protected:
	int m_num;
};

class Dragon:public Soldier 
{
private:
public:
	static int m_strength;
	static int m_id;
	int m_number;
	Dragon(int n=0){
		m_number = n;
	}
	static void set_strength(int strength){
		m_strength = strength;
	}

};
int Dragon::m_strength = 0;
int Dragon::m_id = 1;

class Ninja: public Soldier
{
private:
public:
	static int m_strength;
	static int m_id;	
	int m_number;
	Ninja(int n=0){
		m_number = n;
	}
	static void set_strength(int strength){
		m_strength = strength;
	}
};
int Ninja::m_strength = 0;
int Ninja::m_id = 2;

class Iceman: public Soldier 
{
private:
public:
	static int m_strength;
	static int m_id;
	int m_number;
	Iceman(int n=0){
		m_number = n;
	}
	static void set_strength(int strength){
		m_strength = strength;
	}
};
int Iceman::m_strength = 0;
int Iceman::m_id = 3;

class Lion: public Soldier 
{
private:
public:
	static int m_strength;
	static int m_id;
	int m_number;
	Lion(int n=0){
		m_number = n;
	}
	static void set_strength(int strength){
		m_strength = strength;
	}
};
int Lion::m_strength = 0;
int Lion::m_id = 4;

class Wolf: public Soldier 
{
private:
public:
	static int m_strength;
	static int m_id;
	int m_number;
	Wolf(int n=0){
		m_number = n;
	}
	static void set_strength(int strength){
		m_strength = strength;
	}
};
int Wolf::m_strength = 0;
int Wolf::m_id = 5;

class Headquarter
{
private:
    string m_name;
	int m_total_strength;
	queue<Dragon>m_dragon;
	queue<Ninja>m_ninja;
	queue<Iceman>m_iceman;
	queue<Lion>m_lion;
	queue<Wolf>m_wolf;
public:

	int m_temp; //模仿指针,标记下一次生成哪个战士
	int m_count;
	int _m_total_strength()
	{
		return m_total_strength;
	}
	Headquarter(string name = "red",int total_strength=0) {
		m_name = name;
		m_total_strength = total_strength;
		m_temp=-1;
		m_count = 0;
	}
	~Headquarter()
	{
		if(m_dragon.empty()==1){
			queue<Dragon>empty;
			swap(empty, m_dragon);
		}
		if(m_ninja.empty()==1){
			queue<Ninja>empty;
			swap(empty, m_ninja);
		}
		if(m_iceman.empty()==1){
			queue<Iceman>empty;
			swap(empty, m_iceman);
		}
		if(m_lion.empty()==1){
			queue<Lion>empty;
			swap(empty, m_lion);
		}
		if(m_wolf.empty()==1){
			queue<Wolf>empty;
			swap(empty, m_wolf);
		}
	}


	bool creat_soldier(int time,int sort[],int len=5)
	{
		char char_time[4]={'0','0','0'};
		if(time/10==0){
			char_time[2]+= time;
		}
		else{
			if(time/100==0){
				char_time[1] += time/10;
				char_time[2] += time%10;
			}
			else{
				char_time[0] += time/100;
				char_time[1] += time/10%10;
				char_time[2] += time%10;
			}
		}
		for(int i =0; i < 5; i++)
		{
			m_temp = (m_temp + 1) % 5;
			switch (sort[m_temp])
			{
			case 0:
				if(m_total_strength>=Dragon::m_strength){
					m_total_strength-=Dragon::m_strength;
					m_count++;
					Dragon dragon(m_count);
					m_dragon.push(dragon);
					cout<<char_time<<" "<<m_name<<" dragon "<<m_count<<" born with strength "<<Dragon::m_strength<<",";
					cout<<m_dragon.size()<<" "<<"dragon"<<" in "<<m_name<<" headquarter"<<endl;
					return true;
				}
				break;
			case 1:
				if(m_total_strength>=Ninja::m_strength){
					m_total_strength-=Ninja::m_strength;
					m_count++;
					Ninja ninja(m_count);
					m_ninja.push(ninja);
					cout<<char_time<<" "<<m_name<<" ninja "<<m_count<<" born with strength "<<Ninja::m_strength<<",";
					cout<<m_ninja.size()<<" "<<"ninja"<<" in "<<m_name<<" headquarter"<<endl;
					return true;
				}
				break;
			case 2:
				if(m_total_strength>=Iceman::m_strength){
					m_total_strength-=Iceman::m_strength;
					m_count++;
					Iceman iceman(m_count);
					m_iceman.push(iceman);
					cout<<char_time<<" "<<m_name<<" iceman "<<m_count<<" born with strength "<<Iceman::m_strength<<",";
					cout<<m_iceman.size()<<" "<<"iceman"<<" in "<<m_name<<" headquarter"<<endl;
					return true;
				}
				break;
			case 3:
				if(m_total_strength>=Lion::m_strength){
					m_total_strength-=Lion::m_strength;
					m_count++;
					Lion lion(m_count);
					m_lion.push(lion);
					cout<<char_time<<" "<<m_name<<" lion "<<m_count<<" born with strength "<<Lion::m_strength<<",";
					cout<<m_lion.size()<<" "<<"lion"<<" in "<<m_name<<" headquarter"<<endl;
					return true;
				}
				break;
			case 4:
				if(m_total_strength>Wolf::m_strength){
					m_total_strength-=Wolf::m_strength;
					m_count++;
					Wolf wolf(m_count);
					m_wolf.push(wolf);
					cout<<char_time<<" "<<m_name<<" wolf "<<m_count<<" born with strength "<<Wolf::m_strength<<",";
					cout<<m_wolf.size()<<" "<<"wolf"<<" in "<<m_name<<" headquarter"<<endl;
					return true;
				}
				break;
			default:
				break;
			}
		}
		cout<<char_time<<" "<<m_name<<" headquarter stops making warriors"<<endl;
		return false;
	}
};

int main()
{
	int n = 0;
	cin>>n;
	for(int l =0;l<n;l++)
	{
		cout<<"Case:"<<l+1<<endl;
		int m = 0;
		int ds=0,ns=0,is=0,ls=0,ws=0;
		int time = 0;
		int redsort[5]={2,3,4,1,0},bluesort[5]={3,0,1,2,4};
		cin>>m>>ds>>ns>>is>>ls>>ws;
		Headquarter red("red",m), blue("blue",m);
		Dragon::set_strength(ds);
		Ninja::set_strength(ns);
		Iceman::set_strength(is);
		Lion::set_strength(ls);
		Wolf::set_strength(ws);
		bool flag1 =1,flag2 =1;
		for(time=0;;time++)
		{
			if(flag1==1){
				flag1 = red.creat_soldier(time,redsort);
			}
			if(flag2==1){
				flag2 = blue.creat_soldier(time,bluesort);

			}
			if(flag1==0&&flag2==0)
			{
				break;
			}
		}
	}
	return 0;
}