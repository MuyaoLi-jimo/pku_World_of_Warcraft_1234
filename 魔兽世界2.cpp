#include<iostream>
#include<string>
#include<vector>
using namespace std;
string m_id[3]={"sword","bomb","arrow"};
class Headquarter;
class Weapon
{
	public:

	int m_kind;
};

class Soldier 
{
protected:
	int m_num;
	int m_strength;
	int m_atk;
	Headquarter *m_head;
public:
	Soldier(int num=0,int strength=0,Headquarter *head=NULL,int atk=0):
	m_num(num),m_strength(strength),m_atk(atk),m_head(head){
	}
};

class Dragon:public Soldier 
{
friend Headquarter;
private:
	static int m_ini_strength;
	Weapon m_weapon;
	double m_morale;//其值为它降生后其司令部剩余生命元的数量除以造dragon所需的生命元数量
public:
	Dragon(int n,Headquarter* head,double hp):Soldier(n,m_ini_strength,head){
		m_morale = hp / m_ini_strength;
		m_weapon.m_kind = n%3;
	}
	static void set_strength(int strength){
		m_ini_strength = strength;
	}

};
int Dragon::m_ini_strength = 0;

class Ninja: public Soldier
{
friend Headquarter;
private:
	static int m_ini_strength;
	Weapon m_weapon[2];
public:
	Ninja(int n,Headquarter* head):Soldier(n,m_ini_strength,head){
		m_weapon[0].m_kind=n%3;
		m_weapon[1].m_kind=(n+1)%3;
	}
	static void set_strength(int strength){
		m_ini_strength = strength;
	}
};
int Ninja::m_ini_strength = 0;

class Iceman: public Soldier 
{
friend Headquarter;
private:
	static int m_ini_strength;
	Weapon m_weapon;
public:
	Iceman(int n,Headquarter* head):Soldier(n,m_ini_strength,head){
		m_weapon.m_kind = n%3;
	}
	static void set_strength(int strength){
		m_ini_strength = strength;
	}
};
int Iceman::m_ini_strength = 0;

class Lion: public Soldier 
{
	friend Headquarter;
private:
	int m_loyalty; //其值等于它降生后其司令部剩余生命元的数目。
	static int m_ini_strength;
public:
	Lion(int n,Headquarter* head,double hp):Soldier(n,m_ini_strength,head){
		m_loyalty = hp;
	}
	static void set_strength(int strength){
		m_ini_strength = strength;
	}
};
int Lion::m_ini_strength=0;

class Wolf: public Soldier 
{
	friend Headquarter;
private:
	static int m_ini_strength;
public:
	Wolf(int n,Headquarter* head):Soldier(n,m_ini_strength,head){
	}
	static void set_strength(int strength){
		m_ini_strength = strength;
	}
};
int Wolf::m_ini_strength=0;

class Headquarter
{
friend Dragon;
friend Ninja;
friend Iceman;
friend Lion;
friend Wolf;
private:
    string m_name;
	int m_hp;
	int m_temp; //模仿指针,标记下一次从哪个战士开始生成
	int m_count;//生成的战士数
	vector<Dragon>m_dragon;
	vector<Ninja>m_ninja;
	vector<Iceman>m_iceman;
	vector<Lion>m_lion;
	vector<Wolf>m_wolf;
public:
	int m_que[5];
	Headquarter(int *que, string name = "red",int hp=0) {
		m_name = name;
		m_hp = hp;
		m_temp= -1;
		m_count = 0;
		for(int i =0;i<5;++i){
			m_que[i] = que[i];
		}
	}
	int _m_hp(){ return m_hp; }

	bool creat_soldier(int time)
	{
		char char_time[4]={'0','0','0',};
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
			switch (m_que[m_temp])
			{
			case 0:
				if(m_hp>=Dragon::m_ini_strength){
					m_hp-=Dragon::m_ini_strength;
					m_count++;
					Dragon dragon(m_count,this,m_hp);
					m_dragon.push_back(dragon);
					cout<<char_time<<" "<<m_name<<" dragon "<<m_count<<" born with strength "<<Dragon::m_ini_strength<<",";
					cout<<m_dragon.size()<<" "<<"dragon"<<" in "<<m_name<<" headquarter"<<endl;
					cout<<"It has a "<<m_id[m_dragon[m_dragon.size()-1].m_weapon.m_kind]<<",and it's morale is ";printf("%.2f",m_dragon[m_dragon.size()-1].m_morale);
					cout<<endl;
					return true;
				}
				break;
			case 1:
				if(m_hp>=Ninja::m_ini_strength){
					m_hp-=Ninja::m_ini_strength;
					m_count++;
					Ninja ninja(m_count,this);
					m_ninja.push_back(ninja);
					cout<<char_time<<" "<<m_name<<" ninja "<<m_count<<" born with strength "<<Ninja::m_ini_strength<<",";
					cout<<m_ninja.size()<<" "<<"ninja"<<" in "<<m_name<<" headquarter"<<endl;
					cout<<"It has a "<<m_id[m_ninja[m_ninja.size()-1].m_weapon[0].m_kind]<<" and a "<<m_id[m_ninja[m_ninja.size()-1].m_weapon[1].m_kind]<<endl;
					return true;
				}
				break;
			case 2:
				if(m_hp>=Iceman::m_ini_strength){
					m_hp-=Iceman::m_ini_strength;
					m_count++;
					Iceman iceman(m_count,this);
					m_iceman.push_back(iceman);
					cout<<char_time<<" "<<m_name<<" iceman "<<m_count<<" born with strength "<<Iceman::m_ini_strength<<",";
					cout<<m_iceman.size()<<" "<<"iceman"<<" in "<<m_name<<" headquarter"<<endl;
					cout<<"It has a "<<m_id[m_iceman[m_iceman.size()-1].m_weapon.m_kind]<<endl;
					return true;
				}
				break;
			case 3:
				if(m_hp>=Lion::m_ini_strength){
					m_hp-=Lion::m_ini_strength;
					m_count++;
					Lion lion(m_count,this,m_hp);
					m_lion.push_back(lion);
					cout<<char_time<<" "<<m_name<<" lion "<<m_count<<" born with strength "<<Lion::m_ini_strength<<",";
					cout<<m_lion.size()<<" "<<"lion"<<" in "<<m_name<<" headquarter"<<endl;
					cout<<"It's loyalty is "<<m_lion[m_lion.size()-1].m_loyalty<<endl;
					return true;
				}
				break;
			case 4:
				if(m_hp>Wolf::m_ini_strength){
					m_hp-=Wolf::m_ini_strength;
					m_count++;
					Wolf wolf(m_count,this);
					m_wolf.push_back(wolf);
					cout<<char_time<<" "<<m_name<<" wolf "<<m_count<<" born with strength "<<Wolf::m_ini_strength<<",";
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
		Headquarter red(redsort,"red",m), blue(bluesort,"blue",m);
		Dragon::set_strength(ds);
		Ninja::set_strength(ns);
		Iceman::set_strength(is);
		Lion::set_strength(ls);
		Wolf::set_strength(ws);
		bool flag1 =1,flag2 =1;
		for(time=0;;time++)
		{
			if(flag1==1){
				flag1 = red.creat_soldier(time);
			}
			if(flag2==1){
				flag2 = blue.creat_soldier(time);
			}
			if(flag1==0&&flag2==0)
			{
				break;
			}
		}
	}
	return 0;
}