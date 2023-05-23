#include<iostream>
#include<string>
#include<vector>
#include <stdlib.h>
using namespace std;
char show_time[7]={'0','0','0',':','0','0'};
string weapon_name[3]={"sword","bomb","arrow"};
void _show_time(int t)
{
	int h = t/60, min = t%60;
	show_time[0] = h/100+'0';
	show_time[1] = h/10%10+'0';
	show_time[2] = h%10+'0';
	show_time[4] = min/10+'0';
	show_time[5] = min%10+'0';
}

class Soldier;
class Headquarter;

class City  //注意，不要city和head都回收战士
{
public:
	Soldier *sred, *sblue;
	City():sred(NULL),sblue(NULL){}
};

class Weapon
{
public:
	virtual int _m_id()=0;
	virtual int _true_atk(int atk)=0;
	virtual string _m_name()=0;
	virtual int _m_arrows(){return 0;}
	virtual bool weapon_brocken(){return false; }//如果破损，返回true
	
};

class Sword:public Weapon
{
public:
	const int m_id = 0;
	const string m_name = "sword";
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual int _true_atk(int atk){ return 2*atk/10;}
};

class Bomb:public Weapon
{
public:
	const int m_id = 1;
	const string m_name = "bomb";
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual int _true_atk(int atk){ return 4*atk/10;}
	virtual bool weapon_brocken(){return true;}
};

class Arrow:public Weapon
{
public:
	int m_arrows;
	const int m_id = 2;
	const string m_name = "arrow";
	Arrow():m_arrows(2){}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual int _true_atk(int atk){ 
		m_arrows--;
		return 3*atk/10;
	}
	virtual int _m_arrows(){return m_arrows;}
	virtual bool weapon_brocken(){
		if(m_arrows==0){
			return true;
		}
		return false;
	}
};

Weapon* creat_weapon(int num)
{
	Weapon* weapon=NULL;
	switch(num)
	{
	case 0:weapon = new Sword;break;
	case 1:weapon = new Bomb;break;
	case 2:weapon = new Arrow;break;
	default:break;
	}
	return weapon;
}

int compare_weapon1(const void* v1,const void* v2)
{
	Weapon** w1=(Weapon**)v1;
	Weapon** w2=(Weapon**)v2;
	if((*w1)->_m_id()<(*w2)->_m_id()){
		return (*w1)->_m_id()-(*w2)->_m_id();
	}
	else if ((*w1)->_m_id()==(*w2)->_m_id()){
		if ((*w1)->_m_arrows()>(*w2)->_m_arrows()){
			return -1;
		}
		else if( (*w1)->_m_arrows()==(*w2)->_m_arrows()){
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return  (*w1)->_m_id()-(*w2)->_m_id();
	}
}
int compare_weapon2(const void* v1,const void* v2)
{
	Weapon** w1=(Weapon**)v1;
	Weapon** w2=(Weapon**)v2;
	if((*w1)->_m_id()<(*w2)->_m_id()){
		return (*w1)->_m_id()-(*w2)->_m_id();
	}
	else if ((*w1)->_m_id()==(*w2)->_m_id()){
		if ((*w1)->_m_arrows()<(*w2)->_m_arrows()){
			return -1;
		}
		else if( (*w1)->_m_arrows()==(*w2)->_m_arrows()){
			return 0;
		}
		else{
			return 1;
		}
	}
	else{
		return  (*w1)->_m_id()-(*w2)->_m_id();
	}
}

class Soldier 
{
public:
	int m_num;
	int m_hp;
	int m_atk;
	Weapon *m_weapon[11];
	int m_weapon_num;
	int m_weapon_pointer;//指向本回合使用的武器；
	int weapon_count[3];//数剑、炸弹、箭的数量

	Soldier(int num=0,int hp=0,int atk=0):
		m_num(num),m_hp(hp),m_atk(atk),m_weapon_num(0),m_weapon_pointer(0)
	{
		for(int ii=0;ii<11;++ii){
			m_weapon[ii]=NULL;
		}
	}
	virtual ~Soldier(){
		//cout<<"soldier delete"<<endl;
		for(int ii=0;ii<m_weapon_num;++ii){
			if(m_weapon[ii]!=NULL){
				delete m_weapon[ii];
				m_weapon[ii]=NULL;
			}
		}
		//cout<<"end slodier delete"<<endl;
	}
	virtual int _m_id()=0;
	virtual string _m_name()=0;
	virtual int _m_loyalty(){return 0;}		//lion的特殊接口
	virtual void matched(){}				//战士前进时发生的状况
	void loss_weapon(int n)					//战士失去序号为n的weapon，但不delete
	{
		m_weapon[n] = NULL;
		if(n>=0&&n<11)
		{
			for(int ii = n;ii<m_weapon_num-1;++ii){
				m_weapon[ii]=m_weapon[ii+1];
			}
			m_weapon[m_weapon_num-1]=NULL;
			m_weapon_num--;
		}
	}
	void get_weapon(int n,Soldier *soldier) //战士从其他战士手上得到武器
	{
		m_weapon_num++;
		m_weapon[m_weapon_num-1] = soldier->m_weapon[n];
	}
	int* count_weapon(){
		for(int ii =0;ii<3;++ii){
			weapon_count[ii]=0;
		}
		for(int ii=0;ii<m_weapon_num;++ii){
			switch(m_weapon[ii]->_m_id()){
			case 0:weapon_count[0]++;break;
			case 1:weapon_count[1]++;break;
			case 2:weapon_count[2]++;break;
			default:break;
			}
		}
		return weapon_count;
	}
	void report_soldier(int time,string name){
		_show_time(time);
		count_weapon();
		cout<<show_time	<<" "<<name<<" "<<_m_name()<<" "<<m_num;
		cout<<" has "<<weapon_count[0]<<" sword "<<weapon_count[1]<<" bomb "<<weapon_count[2]<<" arrow and ";
		cout<<m_hp<<" elements"<<endl;
	}
	virtual int attack(Soldier* soldier){
		soldier->hurt(m_weapon[m_weapon_pointer]->_true_atk(m_atk));
		if(m_weapon[m_weapon_pointer]->_m_id()==1){
			this->hurt(m_weapon[m_weapon_pointer]->_true_atk(m_atk)/2);
		}
		if(m_weapon[m_weapon_pointer]->weapon_brocken()==1){
			delete m_weapon[m_weapon_pointer];
			loss_weapon(m_weapon_pointer);
		}
		
		return 1;
	}
	void hurt(int atk){
		m_hp -= atk;
	}
	virtual void cheer(int time,string name,int ncity){}
};

class Dragon:public Soldier 
{
public:
	const int m_id = 0;
	const string m_name="dragon";
	static int m_ini_hp;
	static int m_ini_atk;

	Dragon(int n):Soldier(n,m_ini_hp,m_ini_atk){
		m_weapon[0]=creat_weapon(m_num%3);
		m_weapon_num++;
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual void cheer(int time,string name,int ncity){
		_show_time(time);
		cout<<show_time<<" "<<name<<" "<<m_name<<" "<<m_num<<" yelled in city "<<ncity<<endl;
		return;
	}
};
int Dragon::m_ini_hp = 0;
int Dragon::m_ini_atk = 0;

class Ninja: public Soldier
{
public:
	const int m_id = 1;
	const string m_name="ninja";
	static int m_ini_hp;
	static int m_ini_atk;

	Ninja(int n):Soldier(n,m_ini_hp,m_ini_atk){
		m_weapon[0]=creat_weapon(m_num%3);
		m_weapon[1]=creat_weapon((m_num+1)%3);
		m_weapon_num =2;
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int attack(Soldier* soldier){
		soldier->hurt(m_weapon[m_weapon_pointer]->_true_atk(m_atk));
		if(m_weapon[m_weapon_pointer]->weapon_brocken()==1){
			loss_weapon(m_weapon_pointer);
		}
		return 1;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
};
int Ninja::m_ini_hp = 0;
int Ninja::m_ini_atk = 0;

class Iceman: public Soldier 
{
public:
	const int m_id = 2;
	const string m_name="iceman";
	static int m_ini_hp;
	static int m_ini_atk;

	Iceman(int n):Soldier(n,m_ini_hp,m_ini_atk){
		m_weapon[0]=creat_weapon(m_num%3);
		m_weapon_num++;
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual void matched(){
		m_hp-=m_hp/10;
	}
};
int Iceman::m_ini_hp = 0;
int Iceman::m_ini_atk = 0;

class Lion: public Soldier 
{
public:
	const int m_id = 3;
	const string m_name="lion";
	static int m_ini_hp;
	static int m_ini_atk;
	static int m_k;//每前进一步减少的忠诚度
	int m_loyalty;//其值等于它降生后其司令部剩余生命元的数目。

	Lion(int n,int hp):Soldier(n,m_ini_hp,m_ini_atk){
		m_loyalty = hp;
		m_weapon[0]=creat_weapon(m_num%3);
		m_weapon_num++;
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	static void set_k(int k){
		m_k = k;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual int _m_loyalty(){return m_loyalty;}
	virtual void matched(){
		m_loyalty -= m_k;
	}
};
int Lion::m_ini_hp = 0;
int Lion::m_ini_atk = 0;
int Lion::m_k = 0;

class Wolf: public Soldier 
{
public:
	const int m_id = 4;
	const string m_name="wolf";
	static int m_ini_hp;
	static int m_ini_atk;

	Wolf(int n):Soldier(n,m_ini_hp,m_ini_atk){
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}

};
int Wolf::m_ini_hp = 0;
int Wolf::m_ini_atk = 0;

class Headquarter
{
public:
    string m_name;
	int m_hp;
	Soldier *m_soldier[101];
	int m_temp; //模仿指针,标记下一次从哪个战士开始生成
	int m_soldier_num;//生成的战士数
	int m_que[5];//战士生成顺序

	Headquarter(int *que, string name = "red",int hp=0) {
		m_name = name;
		m_hp = hp;
		m_temp= -1;
		m_soldier_num = 0;
		for(int i =0;i<5;++i){
			m_que[i] = que[i];
		}
		for(int i =0;i<101;++i){
			m_soldier[i]=NULL;
		}
	}
	~Headquarter(){
		//cout<<"head delete"<<endl;
		for(int ii =0;ii<100;++ii)
		{
			if(m_soldier[ii]!=NULL){
				delete m_soldier[ii];
				m_soldier[ii]=NULL;
			}
		}
		//cout<<"end head delete"<<endl;
	}
	bool creat_soldier(int time)
	{
		_show_time(time);

		m_temp = (m_temp + 1) % 5;
		switch (m_que[m_temp])
		{
		case 0:
			if(m_hp>=Dragon::m_ini_hp){
				m_hp-=Dragon::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1] = new Dragon(m_soldier_num);
				cout<<show_time<<" "<<m_name<<" "<<m_soldier[m_soldier_num-1]->_m_name()<<" "<<m_soldier_num<<" born"<<endl;
				//cout<<"It has a "<<m_id[m_dragon[m_dragon.size()-1].m_weapon.m_kind]<<",and it's morale is ";printf("%.2f",m_dragon[m_dragon.size()-1].m_morale);
				//cout<<endl;
				return true;
			}
			break;
		case 1:
			if(m_hp>=Ninja::m_ini_hp){
				m_hp-=Ninja::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1] = new Ninja(m_soldier_num);
				cout<<show_time<<" "<<m_name<<" "<<m_soldier[m_soldier_num-1]->_m_name()<<" "<<m_soldier_num<<" born"<<endl;
				return true;
			}
			break;
		case 2:
			if(m_hp>=Iceman::m_ini_hp){
				m_hp-=Iceman::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1] = new Iceman(m_soldier_num);
				cout<<show_time<<" "<<m_name<<" "<<m_soldier[m_soldier_num-1]->_m_name()<<" "<<m_soldier_num<<" born"<<endl;
				return true;
			}
			break;
		case 3:
			if(m_hp>=Lion::m_ini_hp){
				m_hp-=Lion::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1] = new Lion(m_soldier_num,m_hp);
				cout<<show_time<<" "<<m_name<<" "<<m_soldier[m_soldier_num-1]->_m_name()<<" "<<m_soldier_num<<" born"<<endl;
				cout<<"Its loyalty is "<<m_soldier[m_soldier_num-1]->_m_loyalty()<<endl;
				return true;
			}
			break;
		case 4:
			if(m_hp>Wolf::m_ini_hp){
				m_hp-=Wolf::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1] = new Wolf(m_soldier_num);
				cout<<show_time<<" "<<m_name<<" "<<m_soldier[m_soldier_num-1]->_m_name()<<" "<<m_soldier_num<<" born"<<endl;
				return true;
			}
			break;
		default:
			break;
		}
		
		//cout<<" "<<m_name<<" headquarter stops making warriors"<<endl;
		return false;
	}
	void soldier_dead(int n){//这里的n指编号为n的战士
		delete m_soldier[n-1];
		m_soldier[n-1]=NULL;
	}
	void soldier_run(int time,int n)//这里的n指编号为n的战士
	{
		_show_time(time);
		cout<<show_time<<" "<<m_name<<" "<<m_soldier[n-1]->_m_name()<<" "<<n<<" ran away"<<endl;
		delete m_soldier[n-1];
		m_soldier[n-1]=NULL;
	}
	void report_headquarter(int time){
		_show_time(time);
		cout<<show_time<<" "<<m_hp<<" elements in "<<m_name<<" headquarter"<<endl;
	}
};

int main()
{

	int t = 0;//数据数
	int M = 0, N = 0,K = 0,T = 0;//大本营生命值，中间城市数量，狮子每回合减少忠诚值，时间
	int dh = 0,nh = 0,ih = 0,lh = 0,wh = 0;//五种战士的初始生命力
	int da = 0,na = 0,ia = 0,la = 0,wa = 0;//五种战士的初始战斗力
	int ttime = 0, mmin = 0;//每个数据组计时
	int redsort[5]={2,3,4,1,0},bluesort[5]={3,0,1,2,4};//创造战士顺序
	cin>>t;
	for(int l =0;l<t;l++)
	{
		cout<<"Case "<<l+1<<":"<<endl;
		City city[22];
		bool flag_create_red=1, flag_create_blue=1;//判断是否还可以创造战士
		bool flag_end=0;//判断是否结束
		cin>>M>>N>>K>>T;
		cin>>dh>>nh>>ih>>lh>>wh;
		cin>>da>>na>>ia>>la>>wa;
		Headquarter red(redsort,"red",M), blue(bluesort,"blue",M);
		Dragon::set_hp(dh); Dragon::set_atk(da);
		Ninja::set_hp(nh); Ninja::set_atk(na);
		Iceman::set_hp(ih); Iceman::set_atk(ia);
		Lion::set_hp(lh); Lion::set_atk(la); Lion::set_k(K);
		Wolf::set_hp(wh); Wolf::set_atk(wa);
		for(ttime=0;ttime<=T&&flag_end==0;ttime+=5)
		{
			mmin = ttime%60;
			if(mmin == 0)
			{
				if(flag_create_red==1)
				{
					flag_create_red = red.creat_soldier(ttime);
					if(city[0].sred==NULL&&flag_create_red==1){
						city[0].sred = red.m_soldier[red.m_soldier_num-1];
					}	
				}
				if(flag_create_blue==1)
				{
					flag_create_blue = blue.creat_soldier(ttime);
					if(city[N+1].sblue==NULL&&flag_create_blue==1){
						city[N+1].sblue = blue.m_soldier[blue.m_soldier_num-1];
					}
				}
			}
			else if(mmin == 5)
			{
				for(int i=0;i<N+2;++i){
					if(city[i].sred!=NULL)
					{
						if(city[i].sred->_m_id()==3 && i!=N+1)
						{
							if(city[i].sred->_m_loyalty()<=0){
								int nnum = city[i].sred->m_num;
								city[i].sred=NULL;
								red.soldier_run(ttime,nnum);
							}
						}
					}
					if(city[i].sblue!=NULL)
					{
						if(city[i].sblue->_m_id()==3 && i!=0)
						{
							if(city[i].sblue->_m_loyalty()<=0){
								int nnum = city[i].sblue->m_num;
								city[i].sblue=NULL;
								blue.soldier_run(ttime,nnum);
							}
						}
					}
				}
			}
			else if(mmin == 10)
			{
				int red_move[25]={},blue_move[25]={};//用来记录移动的战士
				for(int i = N;i>=0;--i){
					if(city[i].sred!=NULL){
						city[i].sred->matched();
						city[i+1].sred = city[i].sred;
						red_move[i+1]=1;
						city[i].sred=NULL;
					}
				}
				for(int i = 1;i<=N+1;++i){
					if(city[i].sblue!=NULL){
						city[i].sblue->matched();
						city[i-1].sblue = city[i].sblue;
						blue_move[i-1]=1;
						city[i].sblue=NULL;
					}
				}
				if(city[0].sblue!=NULL){
					flag_end = true;
					_show_time(ttime);
					cout<<show_time<<" blue "<<city[0].sblue->_m_name()<<" "<<city[0].sblue->m_num;
					cout<<" reached red headquarter with "<<city[0].sblue->m_hp<<" elements and force "<<city[0].sblue->m_atk<<endl;
					cout<<show_time<<" red headquarter was taken"<<endl;
				}
				for(int i =1;i<=N;++i){
					if(city[i].sred!=NULL)
					{
						_show_time(ttime);
						cout<<show_time<<" red "<<city[i].sred->_m_name()<<" "<<city[i].sred->m_num;
						cout<<" marched to city "<<i<<" with "<<city[i].sred->m_hp<<" elements and force "<<city[i].sred->m_atk<<endl;
					}
					if(city[i].sblue!=NULL)
					{
						_show_time(ttime);
						cout<<show_time<<" blue "<<city[i].sblue->_m_name()<<" "<<city[i].sblue->m_num;
						cout<<" marched to city "<<i<<" with "<<city[i].sblue->m_hp<<" elements and force "<<city[i].sblue->m_atk<<endl;
					}
				}
				if(city[N+1].sred!=NULL){
					flag_end = true;
					_show_time(ttime);
					cout<<show_time<<" red "<<city[N+1].sred->_m_name()<<" "<<city[N+1].sred->m_num;
					cout<<" reached blue headquarter with "<<city[N+1].sred->m_hp<<" elements and force "<<city[N+1].sred->m_atk<<endl;
					cout<<show_time<<" blue headquarter was taken"<<endl;
				}
			}
			/*
			else if(mmin == 15){
				for(int i =0;i<N+2;i++){
					cout<<city[i].sblue<<" "<<city[i].sred<<endl;
				}
			}
			*/
			else if(mmin == 35){
				//先排序
				for(int i =1;i<N+1;++i){
					if(city[i].sred!=NULL&&city[i].sblue!=NULL){
						Soldier *sr=city[i].sred,*sb=city[i].sblue;//准备
						if(sr->_m_id()==4 && sb->_m_id()!=4&&sb->m_weapon_num!=0){
							qsort(sb->m_weapon, sb->m_weapon_num,sizeof(Weapon*),compare_weapon1);
							sb->count_weapon();
							int rob_id=0;//确定抢哪种
							int rob_num=0;
							if(sb->weapon_count[0]==0){
								if(sb->weapon_count[1]){
									rob_id=1;
								}
								else{
									if(sb->weapon_count[2]){
										rob_id=2;
									}
								}
							}
							rob_num = sb->weapon_count[rob_id];
							
							for(int j =0;j<rob_num;++j){
								if(sr->m_weapon_num<10){
									sr->get_weapon(0,sb);
								}
								else{
									delete sb->m_weapon[0];
								}
								sb->loss_weapon(0);
							}
							_show_time(ttime);
							cout<<show_time<<" red "<<sr->_m_name()<<" "<<sr->m_num<<" took "<<rob_num<<" "<<weapon_name[rob_id]<<" ";
							cout<<"from blue "<<sb->_m_name()<<" "<<sb->m_num<<" in city "<<i<<endl;
						}
						if(sb->_m_id()==4 && sr->_m_id()!=4 && sr->m_weapon_num!=0){
							qsort(sr->m_weapon, sr->m_weapon_num,sizeof(Weapon*),compare_weapon1);
							sr->count_weapon();
							int rob_id=0;//确定抢哪种
							int rob_num=0;
							if(sr->weapon_count[0]==0){
								if(sr->weapon_count[1]){
									rob_id=1;
								}
								else{
									rob_id=2;
								}
							}
							rob_num = sr->weapon_count[rob_id];
							for(int j =0;j<rob_num;++j){
								if(sb->m_weapon_num<10){
									sb->get_weapon(0,sr);
								}
								else{
									delete sr->m_weapon[0];
								}
								sr->loss_weapon(0);
							}
							_show_time(ttime);
							cout<<show_time<<" blue "<<sb->_m_name()<<" "<<sb->m_num<<" took "<<rob_num<<" "<<weapon_name[rob_id]<<" ";
							cout<<"from red "<<sr->_m_name()<<" "<<sr->m_num<<" in city "<<i<<endl;
						}
					}
				}
			}
			else if(mmin == 40){
				for(int i =1;i<N+1;++i){
					if(city[i].sred!=NULL&&city[i].sblue!=NULL){
						_show_time(ttime);
						Soldier *sr=city[i].sred;
						Soldier *sb=city[i].sblue;
						//cout<<sb->_m_id()<<" "<<sr->_m_id()<<endl;
						//战前排序；
						qsort(sr->m_weapon,sr->m_weapon_num,sizeof(Weapon*),compare_weapon2);
						qsort(sb->m_weapon,sb->m_weapon_num,sizeof(Weapon*),compare_weapon2);
						if(sr->m_weapon_num>0){
						sr->m_weapon_pointer=0;}
						if(sb->m_weapon_num>0){
						sb->m_weapon_pointer=0;}
						//开战；
						int war_flag=0;
						int red_no=0;
						int blue_no=0;
						int red_hp0=0;
						int blue_hp0=0;
						while(war_flag==0){
							//判断双方有无武器
							if(sr->m_weapon_num<=0&&sb->m_weapon_num<=0){
								war_flag=4;
								cout<<show_time<<" both red "<<sr->_m_name()<<" "<<sr->m_num<<" and blue ";
								cout<<sb->_m_name()<<" "<<sb->m_num<<" were alive in city "<<i<<endl;
								sr->cheer(ttime,"red",i);
								sb->cheer(ttime,"blue",i);
								break;
							}
							if(i%2==1){
								if(sr->m_weapon_num>0){
									blue_hp0 = sb->m_hp;
									sr->attack(sb);
									if((blue_hp0-sb->m_hp)==0){
										red_no++;
									}
									else{
										red_no=0;
									}
								}
								//判断是否有损伤
								if(sb->m_hp<=0&&sr->m_hp>0){
									cout<<show_time<<" red "<<sr->_m_name()<<" "<<sr->m_num<<" killed blue ";
									cout<<sb->_m_name()<<" "<<sb->m_num<<" in city "<<i<<" remaining "<<sr->m_hp<<" elements"<<endl;
									war_flag=1;

									//夺取武器
									qsort(sb->m_weapon,sb->m_weapon_num,sizeof(Weapon*),compare_weapon1);
									for(int j = 0;sb->m_weapon_num>0;++j){
										if(sr->m_weapon_num<10){
											sr->get_weapon(0,sb);
										}
										else{
											delete sb->m_weapon[0];
										}
										sb->loss_weapon(0);
									}
									//敌人死亡
									city[i].sblue=NULL;
									blue.soldier_dead(sb->m_num);
									sb = NULL;
									//欢呼
									sr->cheer(ttime,"red",i);
									break;
								}

								if(sb->m_weapon_num>0){
									red_hp0 = sr->m_hp;
									sb->attack(sr);
									if((red_hp0-sr->m_hp)==0){
										blue_no++;
									}
									else{
										blue_no=0;
									}
								}
						
								if(sr->m_hp<=0&&sb->m_hp>0){
									war_flag =2;
									cout<<show_time<<" blue "<<sb->_m_name()<<" "<<sb->m_num<<" killed red ";
									cout<<sr->_m_name()<<" "<<sr->m_num<<" in city "<<i<<" remaining "<<sb->m_hp<<" elements"<<endl;
									//夺取武器
									qsort(sr->m_weapon,sr->m_weapon_num,sizeof(Weapon*),compare_weapon1);
									for(int j = 0;sr->m_weapon_num>0;++j){
										if(sb->m_weapon_num<10){
											sb->get_weapon(0,sr);
										}
										else{
											delete sr->m_weapon[0];
										}
										sr->loss_weapon(0);
									}
									//敌人死亡
									city[i].sred=NULL;
									red.soldier_dead(sr->m_num);
									sr = NULL;
									//欢呼
									sb->cheer(ttime,"blue",i);
									break;
								}
							}
							else{
								if(sb->m_weapon_num>0){
									red_hp0 = sr->m_hp;
									sb->attack(sr);
									if((red_hp0-sr->m_hp)==0){
										blue_no++;
									}
									else{
										blue_no=0;
									}
								}
								if(sr->m_hp<=0&&sb->m_hp>0){
									war_flag =2;
									cout<<show_time<<" blue "<<sb->_m_name()<<" "<<sb->m_num<<" killed red ";
									cout<<sr->_m_name()<<" "<<sr->m_num<<" in city "<<i<<" remaining "<<sb->m_hp<<" elements"<<endl;
									//夺取武器
									qsort(sr->m_weapon,sr->m_weapon_num,sizeof(Weapon*),compare_weapon1);
									for(int j = 0;sr->m_weapon_num>0;++j){
										if(sb->m_weapon_num<10){
											sb->get_weapon(0,sr);
										}
										else{
											delete sr->m_weapon[0];
										}
										sr->loss_weapon(0);
									}
									//敌人死亡
									city[i].sred=NULL;
									red.soldier_dead(sr->m_num);
									sr = NULL;
									//欢呼
									sb->cheer(ttime,"red",i);
									break;
								}
								if(sr->m_weapon_num>0){
									blue_hp0 = sb->m_hp;
									sr->attack(sb);
									if((blue_hp0-sb->m_hp)==0){
										red_no++;
									}
									else{
										red_no=0;
									}
								}
								//判断是否有损伤
								if(sb->m_hp<=0&&sr->m_hp>0){
									cout<<show_time<<" red "<<sr->_m_name()<<" "<<sr->m_num<<" killed blue ";
									cout<<sb->_m_name()<<" "<<sb->m_num<<" in city "<<i<<" remaining "<<sr->m_hp<<" elements"<<endl;
									war_flag=1;

									//夺取武器
									qsort(sb->m_weapon,sb->m_weapon_num,sizeof(Weapon*),compare_weapon1);
									for(int j = 0;sb->m_weapon_num>0;++j){
										if(sr->m_weapon_num<10){
											sr->get_weapon(0,sb);
										}
										else{
											delete sb->m_weapon[0];
										}
										sb->loss_weapon(0);
									}
									//敌人死亡
									city[i].sblue=NULL;
									blue.soldier_dead(sb->m_num);
									sb = NULL;
									//欢呼
									sr->cheer(ttime,"red",i);
									break;
								}

							}
							if (sr->m_weapon_num > 0) {
								sr->m_weapon_pointer = (sr->m_weapon_pointer + 1) % sr->m_weapon_num;
							}
							if(sb->m_weapon_num > 0) {
								sb->m_weapon_pointer = (sb->m_weapon_pointer + 1) % sb->m_weapon_num;
							}
							if(sr->m_hp<=0&&sb->m_hp<=0){ //如果都死
								war_flag=3;
								cout<<show_time<<" both red "<<sr->_m_name()<<" "<<sr->m_num<<" and blue ";
								cout<<sb->_m_name()<<" "<<sb->m_num<<" died in city "<<i<<endl;
								city[i].sblue=NULL;
								city[i].sred=NULL;
								red.soldier_dead(sr->m_num);
								sr = NULL;
								blue.soldier_dead(sb->m_num);
								sb = NULL;

								break;
							}
							if((red_no>10&&blue_no>10)||(red_no>10&&sb->m_weapon_num<=0)||(blue_no>10&&sr->m_weapon_num<=0)){//都活
								war_flag=5;
								cout<<show_time<<" both red "<<sr->_m_name()<<" "<<sr->m_num<<" and blue ";
								cout<<sb->_m_name()<<" "<<sb->m_num<<" were alive in city "<<i<<endl;
								sr->cheer(ttime,"red",i);
								sb->cheer(ttime,"blue",i);
								break;
							}
						}
						//cout<<war_flag<<endl;//用来调试
					}
				}
			}
			else if(mmin == 50){
				red.report_headquarter(ttime);
				blue.report_headquarter(ttime);
			}
			else if(mmin == 55){
				for(int i=0;i<N+2;++i){
					if(city[i].sred!=NULL){
						city[i].sred->report_soldier(ttime,"red");
					}
					if(city[i].sblue!=NULL){
						city[i].sblue->report_soldier(ttime,"blue");
					}
				}
			}
		}
	}
	return 0;
}