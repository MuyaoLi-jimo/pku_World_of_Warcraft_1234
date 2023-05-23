#include<iostream>
#include<string>
#include<vector>
#include<map>
#include<memory>
using namespace std;
string show_time = "000:00";
string weapon_name[3] = {"sword", "bomb", "arrow"};
map<int, int> soldier_hp{{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}};

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
class City;
class Weapon;


class Weapon
{
public:
    int m_atk = 0;

    Weapon(int atk):m_atk(atk){}
	virtual ~Weapon(){}

    virtual int _m_id() = 0; // 因为id是const，所以基类指针不转为派生类指针，休想调用。所以用函数作为接口
    virtual string _m_name()=0;
	virtual inline int _m_arrows(){return 0;}

    virtual inline int use_weapon() = 0;                    //使用weapon，返回伤害
    virtual inline bool weapon_brocken() { return false; } // 判断是否损毁，如果损毁，返回true
};

class Sword:public Weapon
{
public:
	const int m_id = 0;
	const string m_name = "sword";
    Sword(int atk):Weapon(atk*2/10){}
    inline int _m_id(){return m_id;}
	inline string _m_name(){return m_name;}
    inline int use_weapon(){
        int atk = m_atk;
        m_atk = m_atk * 8 / 10;
        return atk;
    }
    inline bool weapon_brocken(){
        if(m_atk<=0){
            return true;
        }
        return false;
    }
};

class Bomb:public Weapon
{
public:
	const int m_id = 1;
	const string m_name = "bomb";
    Bomb(int useless):Weapon(1000000){}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
    virtual int use_weapon(){
        return m_atk;
    }
    virtual bool weapon_brocken(){return false;}//只要询问，肯定没死，所以肯定为false
};

class Arrow:public Weapon
{
public:
    int m_arrows = 3;
    const int m_id = 2;
    const string m_name = "arrow";
    static int m_ini_atk;
    Arrow(int useless):Weapon(m_ini_atk){}
    virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual int _m_arrows(){return m_arrows;}
    virtual int use_weapon(){
        m_arrows--;
        return m_atk;
    }
    virtual bool weapon_brocken(){
		if(m_arrows==0){
			return true;
		}
		return false;
	}
};
int Arrow::m_ini_atk = 0; 

Weapon* create_weapon(int num,int atk)
{
	Weapon* weapon=NULL;
    num = num % 3;
    switch (num)
    {
	case 0:weapon = new Sword(atk);break;
	case 1:weapon = new Bomb(0);break;
	case 2:weapon = new Arrow(0);break;
	default:break;
	}
	return weapon;
}

class Soldier 
{
public:
	int m_num; //武士编号
	int m_hp;
	int m_atk;
	string m_color;
	shared_ptr<Weapon> m_weapons[3];
    Soldier(int num,int hp,int atk,string Color):
		m_num(num),m_hp(hp),m_atk(atk),m_color(Color)
	{
		for(int ii=0;ii<3;++ii){
			m_weapons[ii]=nullptr;
		}
	}
	virtual ~Soldier(){
		for(int ii=0;ii<3;++ii){
            m_weapons[ii].reset();
        }
    }
	virtual int _m_id()=0;                  //id接口
	virtual string _m_name()=0;             //种类接口
	virtual inline int& _m_loyalty(){
		int *a = new int(0);
		return *a;
	} // lion的特殊接口
	virtual double& _m_morale() {
		double *a = new double(0);
		return *a;
	} // dragon的特殊接口
	virtual inline void march(){}				
	virtual inline void morale_change(){}
    virtual inline void cheer(int ttime,int city){}              //TODO:战士欢呼
    void lose_weapon(){  //检查是否有武器损毁，若有，则战士失去某武器
        for (int ii = 0; ii < 3;++ii){
            if(m_weapons[ii]!=nullptr){
                if(m_weapons[ii]->weapon_brocken()){
                    m_weapons[ii].reset();
                }
            }

        }
    }
	void get_weapon(Soldier *soldier){  //战士从其他战士手上得到武器，只有wolf做得到
        for (int ii = 0; ii < 3;++ii){
            if(m_weapons[ii]==nullptr){
                if(soldier->m_weapons[ii]!=nullptr){
                    m_weapons[ii] = soldier->m_weapons[ii];
                    soldier->m_weapons[ii].reset();
				}
			}
        }
    }
	void hurt(int atk){                 //受伤
		m_hp -= atk;
	}
    virtual void attack(int ttime, Soldier* soldier,int weapon_number,int city = 0){  //包括了用剑、用炸弹、用箭三种形式
		if (weapon_number == 2)
		{
			if(m_weapons[weapon_number]!=nullptr){
				soldier->hurt(m_weapons[weapon_number]->use_weapon());
			}
		}
		else{
			if(m_weapons[weapon_number]!=nullptr){
				soldier->hurt(m_weapons[weapon_number]->use_weapon() + m_atk);
			}
			else{
				soldier->hurt(m_atk);
			}
			if(weapon_number==0){
				cout << show_time << " " << m_color << " " << _m_name() << " " << m_num << " attacked ";
				cout << soldier->m_color << " " << soldier->_m_name() << " " << soldier->m_num << " in city " << city;
				cout << " with " << m_hp << " elements and force " << m_atk << endl;
			}
		}
        if(weapon_number==1){
            m_hp = 0;
			cout << show_time << " " << m_color << " " << _m_name() << " " << m_num << " used a bomb and killed ";
			cout << soldier->m_color << " " << soldier->_m_name() << " " << soldier->m_num << endl;
		}
        lose_weapon();
    }
    virtual void counterattack(int ttime, int city_num,Soldier* soldier){   //只能用剑
		_show_time(ttime);
		if(m_weapons[0]!=nullptr){
			soldier->hurt(m_weapons[0]->use_weapon() + m_atk/2);
		}
		else{
			soldier->hurt(m_atk/2);
		}
		cout << show_time << " " << m_color << " " << _m_name() << " " << m_num << " fought back against ";
		cout << soldier->m_color << " " << soldier->_m_name() << " " << soldier->m_num << " in city " << city_num << endl;
		lose_weapon();
	}
    virtual bool whether_dead(){
		if (m_hp <= 0){
			return true;
		}
		return false;
    }
	virtual bool whether_runaway() { return false; }
	virtual bool predict_dead(Soldier*soldier,string ccolor){	//判断是否会被杀
		int atk = 0;
		if (ccolor == m_color)
		{
			if(m_weapons[0]!=nullptr){	//如果我能直接杀死它
				if(m_weapons[0]->m_atk+m_atk>=soldier->m_hp){
					return false;
				}
			}
			else{
				if(m_atk>=soldier->m_hp){
					return false;
				}
			}
			if(soldier->_m_id()!=1){
				if(soldier->m_weapons[0]!=nullptr){
					atk = soldier->m_weapons[0]->m_atk + soldier->m_atk/2;
				}
				else{
					atk = soldier->m_atk/2;
				}
			}

		}
		else{
			if(soldier->m_weapons[0]!=nullptr){
				atk = soldier->m_weapons[0]->m_atk + soldier->m_atk;
			}
			else{
				atk = soldier->m_atk;
			}
		}
		if (atk >= m_hp)
		{
			return true;
		}
		return false;
	}
	virtual void report(int ttime){
		int weapon_num = 0;
		_show_time(ttime);
		cout << show_time << " " << m_color << " " << _m_name() << " " << m_num << " has ";
		for (int i = 2; i >= 0; --i)
		{
			switch (i)
			{
			case 0:
				if (m_weapons[0] != nullptr)
				{
					if (weapon_num > 0)
					{
						cout << ",";
					}
					cout << weapon_name[0] << "(" << m_weapons[0]->m_atk << ")";
					weapon_num++;
				}
				break;
			case 1:
				if(m_weapons[1]!=nullptr){
					if(weapon_num>0){
						cout << ",";
					}
					cout << weapon_name[1];
					weapon_num++;
				}
				break;
			case 2:
				if(m_weapons[2]!=nullptr){
					if(weapon_num>0){
						cout << ",";
					}
					cout << weapon_name[2] << "(" << m_weapons[2]->_m_arrows() << ")";
					weapon_num++;
				}
				break;
			default:
				break;
			}
		}
		if(weapon_num==0){
			cout << "no weapon";
		}
		cout << endl;
	}
};

class Dragon:public Soldier 
{
public:
    double m_morale;
    const int m_id = 0;
    const string m_name="dragon";
	static int m_ini_hp;
	static int m_ini_atk;

	Dragon(int n,int head_hp,string ccolor):Soldier(n,m_ini_hp,m_ini_atk,ccolor){
        Weapon *temp = create_weapon(m_num, m_atk);
        m_weapons[temp->_m_id()].reset(temp);
        m_morale = double(head_hp)/m_ini_hp;
		lose_weapon();
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
        soldier_hp[0] = m_ini_hp;
    }
    static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual double& _m_morale() { return m_morale; }
	virtual void cheer(int ttime,int city){
		_show_time(ttime);
		cout << show_time << " " << m_color << " " << m_name << " " << m_num << " yelled in city " << city << endl;
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

	Ninja(int n,string ccolor):Soldier(n,m_ini_hp,m_ini_atk,ccolor){
        Weapon *temp1 = create_weapon(m_num, m_atk);
        Weapon *temp2 = create_weapon(m_num+1, m_atk);
        m_weapons[temp1->_m_id()].reset(temp1);
        m_weapons[temp2->_m_id()].reset(temp2);
		lose_weapon();
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
        soldier_hp[1] = m_ini_hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
    virtual void counterattack(Soldier* soldier){   //忍者选择忍着
    }
	virtual inline int _m_id(){return m_id;}
	virtual inline string _m_name(){return m_name;}
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
    int m_steps = 0;

    Iceman(int n,string ccolor):Soldier(n,m_ini_hp,m_ini_atk,ccolor){
        Weapon *temp = create_weapon(m_num, m_atk);
        m_weapons[temp->_m_id()].reset(temp);
		lose_weapon();
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
        soldier_hp[2] = m_ini_hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual void march(){
        m_steps++;
        if(m_steps%2==0){
            m_hp -= 9;
            m_atk += 20;
            if(m_hp<=0){
                m_hp = 1;
            }
        }
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
	int m_loyalty;//其值等于它降生后其司令部剩余生命元的数目。
	static int m_k;	//每次失败减少的K数
	Lion(int n, int hp,string ccolor) : Soldier(n, m_ini_hp, m_ini_atk,ccolor)
	{
		m_loyalty = hp;
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
        soldier_hp[3] = m_ini_hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	static void set_k(int k){
		m_k = k;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}
	virtual int& _m_loyalty(){return m_loyalty;}
	virtual bool whether_runaway(){
		if(m_loyalty<=0){
			cout << show_time << " " << m_color << " " << m_name << " " << m_num << " ran away" << endl;
			return true;
		}
		return false;
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

	Wolf(int n,string ccolor):Soldier(n,m_ini_hp,m_ini_atk,ccolor){
	}
	static void set_hp(int hp){
		m_ini_hp = hp;
        soldier_hp[4] = m_ini_hp;
	}
	static void set_atk(int atk){
		m_ini_atk = atk;
	}
	virtual int _m_id(){return m_id;}
	virtual string _m_name(){return m_name;}

};
int Wolf::m_ini_hp = 0;
int Wolf::m_ini_atk = 0;

class City
{
public:
	int m_num = 0;
	int m_hp = 0;
	bool m_whether_fighting = false;	//开打前判断是否在战斗
	bool m_whether_red_killed_by_arrow = false;
	bool m_whether_blue_killed_by_arrow = false;
	shared_ptr<Soldier> m_red_soldier = nullptr, m_blue_soldier = nullptr;
	string m_flag_color="\0";
	string m_tem_winner = "\0";
	int m_win_times = 0;
	City(int num = 0) : m_num(num) {}
	virtual inline void increase_hp() final{
        m_hp += 10;
    }
	virtual int give_hp(int ttime,Soldier*soldier){
		int temt = m_hp;
		cout << show_time << " " << soldier->m_color << " " << soldier->_m_name() << " " << soldier->m_num << " earned " << temt << " elements for his headquarter" << endl;
		m_hp = 0;
		return temt;
	}
	virtual void change_flag(int ttime,string flag_color) final{
		_show_time(ttime);
		m_flag_color = flag_color;
		cout << show_time << " " << m_flag_color << " flag raised in city " << m_num << endl;
	}
	virtual string who_attack(){
		if(m_flag_color!="\0"){
			return m_flag_color;
		}
		if(m_num%2==1){
			return "red";
		}
		else{
			return "blue";
		}
	}
	virtual int win(int ttime,Soldier*soldier) final{		//获胜后从获得奖励开始干一系列事情
		_show_time(ttime);
		if (m_tem_winner == "\0" || m_tem_winner == soldier->m_color){
			m_win_times++;
		}
		else{
			m_win_times = 1;
		}
		m_tem_winner = soldier->m_color;//无论连胜还是另一方赢，都改变颜色
		int hp = give_hp(ttime,soldier);
		if (m_win_times == 2&&m_flag_color!=m_tem_winner)
		{
			change_flag(ttime, m_tem_winner);
		}
		return hp;
	}
	virtual void draw() final{ //平局
		m_tem_winner = "\0";
		m_win_times = 0;
	}
};

class Headquarter:public City
{
public:
    string m_name;
	int m_hp;
	shared_ptr<Soldier> m_soldier[101]; //鬼知道它最大多少
	int m_temp=-1; //模仿指针,标记下一次从哪个战士开始生成
	int m_soldier_num=0;//生成的战士数
    bool m_stop_create = false;//查看是否还能生成
    int m_que[5]; // 战士生成顺序
	int m_enemy = 0;//大本营里有多少敌人
	Headquarter(int *que, string name = "red", int hp = 0) : m_name(name), m_hp(hp)
	{
		for(int i =0;i<5;++i){
			m_que[i] = que[i];
		}
		for(int i =0;i<101;++i){
			m_soldier[i]=nullptr;
		}
	}
	~Headquarter(){
		for(int ii =0;ii<100;++ii)
		{
			if(m_soldier[ii]!=nullptr){
                m_soldier[ii].reset();
            }
		}
	}
	bool creat_soldier(int time)    //生产soldier
	{
        if(m_stop_create==true){
            if(m_hp>=soldier_hp[m_temp]){  //暂停和开始
            }
            else{
                return false;
            }
        }
        else{
            m_temp = (m_temp + 1) % 5;
        }
		_show_time(time);
		
        //记得生产不了就改m_stop_create
		switch (m_que[m_temp])
		{
		case 0:
			if(m_hp>=Dragon::m_ini_hp){
				m_hp-=Dragon::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1].reset(new Dragon(m_soldier_num,m_hp,m_name));
                m_stop_create = false;
            }
            else{
                m_stop_create = true;
                return false;
            }
            break;
		case 1:
			if(m_hp>=Ninja::m_ini_hp){
				m_hp-=Ninja::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1].reset(new Ninja(m_soldier_num,m_name));
                m_stop_create = false;
			}
            else{
                m_stop_create = true;
                return false;
            }
			break;
		case 2:
			if(m_hp>=Iceman::m_ini_hp){
				m_hp-=Iceman::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1].reset(new Iceman(m_soldier_num,m_name));
                m_stop_create = false;
			}
            else{
                m_stop_create = true;
                return false;
            }
			break;
		case 3:
			if(m_hp>=Lion::m_ini_hp){
				m_hp-=Lion::m_ini_hp;
				m_soldier_num++;
                m_soldier[m_soldier_num-1].reset(new Lion(m_soldier_num,m_hp,m_name));
                m_stop_create = false;
			}
            else{
                m_stop_create = true;
                return false;
            }
			break;
		case 4:
			if(m_hp>=Wolf::m_ini_hp){
				m_hp-=Wolf::m_ini_hp;
				m_soldier_num++;
				m_soldier[m_soldier_num-1].reset(new Wolf(m_soldier_num,m_name));
                m_stop_create = false;
			}
            else{
                m_stop_create = true;
                return false;
            }
			break;
		default:
			break;
        }
        cout << show_time << " " << m_name << " " << m_soldier[m_soldier_num - 1]->_m_name() << " " << m_soldier_num << " born" << endl;
        switch (m_que[m_temp])
		{
		case 0:
			cout << "Its morale is ";
			printf("%.2f", m_soldier[m_soldier_num - 1]->_m_morale());
			cout << endl;
			break;
		case 3:
			cout << "Its loyalty is " << m_soldier[m_soldier_num - 1]->_m_loyalty() << endl;
			break;
		default:
			break;
        }
		if(m_name=="red"){
			m_red_soldier = m_soldier[m_soldier_num - 1];
		}
		else if(m_name=="blue"){
			m_blue_soldier = m_soldier[m_soldier_num - 1];
		}
		return true;
	}
	void soldier_dead(int n){//这里的n指编号为n的战士
		m_soldier[n - 1].reset();
	}
	void soldier_run(int time,int n)//这里的n指编号为n的战士
	{
		_show_time(time);
		cout<<show_time<<" "<<m_name<<" "<<m_soldier[n-1]->_m_name()<<" "<<n<<" ran away"<<endl;
		m_soldier[n - 1].reset();
	}
	void report_headquarter(int time){
		_show_time(time);
		cout<<show_time<<" "<<m_hp<<" elements in "<<m_name<<" headquarter"<<endl;
	}
	int change_hp(int hp){	//改变hp大小，用于获得城市生命值，发放奖励；
		m_hp += hp;
		return -hp;
	}
};


int main()
{
	//freopen("some.txt", "w", stdout);
	int t = 0;							   // 数据数
	int M = 0, N = 0, R = 0, K = 0, T = 0; // 大本营生命值，中间城市数量，弓箭的伤害，狮子每回合减少忠诚值，时间
	int dh = 0, nh = 0, ih = 0, lh = 0, wh = 0; // 五种战士的初始生命力
	int da = 0, na = 0, ia = 0, la = 0, wa = 0; // 五种战士的初始战斗力
	int ttime = 0, mmin = 0;//每个数据组计时
	int redsort[5] = {2, 3, 4, 1, 0}, bluesort[5] = {3, 0, 1, 2, 4}; // 创造战士顺序
	cin>>t;
	for(int l =0;l<t;l++)
	{
		cout<<"Case "<<l+1<<":"<<endl;
		City* city[22];
		bool flag_end = false; // 判断是否结束
		bool boom_happened = false;//判断是否爆炸，38时使用
		cin>>M>>N>>R>>K>>T;
		cin>>dh>>nh>>ih>>lh>>wh;
		cin>>da>>na>>ia>>la>>wa;
		Headquarter red(redsort,"red",M), blue(bluesort,"blue",M);
		for (int i = 1; i <= N; i++)
		{
			city[i]=new City(i);
		}
		city[0] = &red;
		city[N + 1] = &blue;
		Arrow::m_ini_atk = R;
		Dragon::set_hp(dh); Dragon::set_atk(da);
		Ninja::set_hp(nh); Ninja::set_atk(na);
		Iceman::set_hp(ih); Iceman::set_atk(ia);
		Lion::set_hp(lh); Lion::set_atk(la); Lion::set_k(K);
		Wolf::set_hp(wh); Wolf::set_atk(wa);
		for (ttime = 0; ttime <= T && flag_end == false; ++ttime) // 开始
		{
			mmin = ttime%60;
			if(mmin == 0)
			{
				red.creat_soldier(ttime);
				blue.creat_soldier(ttime);
			}
			if(mmin==5){
				_show_time(ttime);
				if (city[0]->m_red_soldier != nullptr)
				{
					if(city[0]->m_red_soldier->whether_runaway()){
						red.m_soldier[city[0]->m_red_soldier->m_num-1].reset();
						city[0]->m_red_soldier.reset();
					}
				}
				for (int i = 1; i <= N + 1;++i){
					if(city[i]->m_red_soldier!=nullptr){
						if(city[i]->m_red_soldier->whether_runaway()){
							red.m_soldier[city[i]->m_red_soldier->m_num-1].reset();
							city[i]->m_red_soldier.reset();
						}
					}
					if(city[i]->m_blue_soldier!=nullptr){
						if(city[i]->m_blue_soldier->whether_runaway()){
							blue.m_soldier[city[i]->m_blue_soldier->m_num-1].reset();
							city[i]->m_blue_soldier.reset();
						}
					}
				}
				
				if(city[N+1]->m_blue_soldier!=nullptr){
					if(city[N+1]->m_blue_soldier->whether_runaway()){
						blue.m_soldier[city[N+1]->m_blue_soldier->m_num-1].reset();
						city[N+1]->m_blue_soldier.reset();
					}
				}
			}
			if(mmin == 10){
				_show_time(ttime);
				int red_move[25] = {}, blue_move[25] = {}; // 用来记录移动的战士
				if(city[N]->m_red_soldier!=nullptr&&city[N+1]->m_red_soldier!=nullptr){
					city[N+1]->m_red_soldier.reset();
				}
				if(city[1]->m_blue_soldier!=nullptr&&city[0]->m_blue_soldier!=nullptr){
					city[0]->m_blue_soldier.reset();
				}
				for(int i = N;i>=0;--i){		//先移动
					if (city[i]->m_red_soldier != nullptr)
					{
						if(i==N){
							blue.m_enemy++;
						}
						city[i]->m_red_soldier->march();
						city[i+1]->m_red_soldier = city[i]->m_red_soldier;
						red_move[i+1]=1;
						city[i]->m_red_soldier.reset();
					}
				}
				for(int i = 1;i<=N+1;++i){
					if (city[i]->m_blue_soldier != nullptr)
					{
						if(i==1){
							red.m_enemy++;
						}
						city[i]->m_blue_soldier->march();
						city[i-1]->m_blue_soldier = city[i]->m_blue_soldier;
						blue_move[i-1]=1;
						city[i]->m_blue_soldier.reset();
					}
				}
				_show_time(ttime);
				if (blue_move[0]>0)
				{
					cout << show_time << " blue " << city[0]->m_blue_soldier->_m_name() << " " << city[0]->m_blue_soldier->m_num;
					cout << " reached red headquarter with " << city[0]->m_blue_soldier->m_hp << " elements and force " << city[0]->m_blue_soldier->m_atk << endl;
				}
				if(blue_move[0]>0){
					if(red.m_enemy==2){
						cout << show_time << " red headquarter was taken" << endl;
						flag_end = true;
					}
				}
				for(int i =1;i<=N;++i){
					if(red_move[i]>0)
					{
						cout<<show_time<<" red "<<city[i]->m_red_soldier->_m_name()<<" "<<city[i]->m_red_soldier->m_num;
						cout<<" marched to city "<<i<<" with "<<city[i]->m_red_soldier->m_hp<<" elements and force "<<city[i]->m_red_soldier->m_atk<<endl;
					}
					if(blue_move[i]>0)
					{
						cout<<show_time<<" blue "<<city[i]->m_blue_soldier->_m_name()<<" "<<city[i]->m_blue_soldier->m_num;
						cout<<" marched to city "<<i<<" with "<<city[i]->m_blue_soldier->m_hp<<" elements and force "<<city[i]->m_blue_soldier->m_atk<<endl;
					}
				}
				if(red_move[N+1]>0){
					cout<<show_time<<" red "<<city[N+1]->m_red_soldier->_m_name()<<" "<<city[N+1]->m_red_soldier->m_num;
					cout<<" reached blue headquarter with "<<city[N+1]->m_red_soldier->m_hp<<" elements and force "<<city[N+1]->m_red_soldier->m_atk<<endl;
					
				}
				if(red_move[N+1]>0){
					if(blue.m_enemy==2){
						cout << show_time << " blue headquarter was taken" << endl;
						flag_end = true;
					}
				}
			}
			if(mmin==20){
				for (int i = 1; i <= N;++i){
					city[i]->increase_hp();
				}
			}
			if(mmin==30){
				_show_time(ttime);
				for (int i = 1; i <= N; ++i)
				{
					if(city[i]->m_blue_soldier==nullptr&&city[i]->m_red_soldier!=nullptr){
						red.change_hp(city[i]->give_hp(ttime,city[i]->m_red_soldier.get()));
					}
					if(city[i]->m_blue_soldier!=nullptr&&city[i]->m_red_soldier==nullptr){
						blue.change_hp(city[i]->give_hp(ttime,city[i]->m_blue_soldier.get()));
					}
				}
			}
			if(mmin == 35){
				_show_time(ttime);
				// 启动战争状态
				for (int i = 1; i <= N; ++i)
				{
					if(city[i]->m_red_soldier!=nullptr&&city[i]->m_blue_soldier!=nullptr){
						city[i]->m_whether_fighting = true;
					}
				}
				int red_use_arrow[23] = {};
				int blue_use_arrow[23] = {};
				for (int i = 1; i < N; ++i)
				{
					if (city[i]->m_red_soldier != nullptr&&city[i]->m_red_soldier->m_weapons[2]!=nullptr)
					{
						if (city[i + 1]->m_blue_soldier != nullptr)
						{
							city[i]->m_red_soldier->attack(ttime,city[i + 1]->m_blue_soldier.get(), 2);
							red_use_arrow[i] = 1;
						}
					}
				}
				for (int i = N; i > 1; --i){
					if(city[i]->m_blue_soldier!=nullptr&&city[i]->m_blue_soldier->m_weapons[2]!=nullptr){
						if(city[i-1]->m_red_soldier!=nullptr){
							city[i]->m_blue_soldier->attack(ttime,city[i - 1]->m_red_soldier.get(), 2);
							blue_use_arrow[i] = 1;
						}
					}
				}
				for (int i = 1; i <= N; ++i)
				{

					if (red_use_arrow[i] == 1){
						cout << show_time << " red " << city[i]->m_red_soldier->_m_name() << " " << city[i]->m_red_soldier->m_num << " shot";
						if (city[i + 1]->m_blue_soldier->whether_dead()){
							cout << " and killed blue " << city[i+1]->m_blue_soldier->_m_name() << " " << city[i+1]->m_blue_soldier->m_num << endl;
							city[i+1]->m_whether_blue_killed_by_arrow = true;
						}
						else{
							cout << endl;
						}
					}
					if (blue_use_arrow[i] == 1)
					{
						cout << show_time << " blue ";
						cout << city[i]->m_blue_soldier->_m_name() << " " << city[i]->m_blue_soldier->m_num << " shot";
						if(city[i-1]->m_red_soldier->whether_dead()){
							cout << " and killed red " << city[i-1]->m_red_soldier->_m_name() << " " << city[i-1]->m_red_soldier->m_num << endl;
							city[i-1]->m_whether_red_killed_by_arrow = true;
						}
						else{
							cout << endl;
						}
					}
				}
			}
			if(mmin==38){
				_show_time(ttime);
				for (int i = 1; i <= N; ++i)
				{
					boom_happened = false;
					if (city[i]->m_whether_fighting&&city[i]->m_whether_red_killed_by_arrow==false&&city[i]->m_whether_blue_killed_by_arrow==false){
						string ccolor = city[i]->who_attack();
						if (city[i]->m_red_soldier->m_weapons[1] != nullptr && city[i]->m_red_soldier->predict_dead(city[i]->m_blue_soldier.get(), ccolor))
						{
							city[i]->m_red_soldier->attack(ttime, city[i]->m_blue_soldier.get(), 1);
							boom_happened = true;
						}
						if (city[i]->m_blue_soldier->m_weapons[1] != nullptr&& city[i]->m_blue_soldier->predict_dead(city[i]->m_red_soldier.get(), ccolor) )
						{
							
							city[i]->m_blue_soldier->attack(ttime, city[i]->m_red_soldier.get(), 1);
							boom_happened = true;
						}
					}
					if (boom_happened)
					{
						red.soldier_dead(city[i]->m_red_soldier->m_num);
						blue.soldier_dead(city[i]->m_blue_soldier->m_num);
						city[i]->m_red_soldier.reset();
						city[i]->m_blue_soldier.reset();
						city[i]->m_whether_fighting = false;
					}
				}
			}
			if(mmin==40){
				int red_dead = 0;
				int blue_dead = 0;
				int red_before_hp = 0;
				int blue_before_hp = 0;
				for (int i = 1; i <= N; ++i)
				{
					_show_time(ttime);
					string ccolor = "\0"; // 记录谁先手
					if (city[i]->m_whether_fighting)
					{
						ccolor = city[i]->who_attack();
						if (city[i]->m_whether_red_killed_by_arrow == false && city[i]->m_whether_blue_killed_by_arrow == false)
						{
							red_before_hp = city[i]->m_red_soldier->m_hp;
							blue_before_hp = city[i]->m_blue_soldier->m_hp;
							if(ccolor=="red"){
								city[i]->m_red_soldier->attack(ttime, city[i]->m_blue_soldier.get(),0,i);
								if(city[i]->m_blue_soldier->_m_id()!=1&&!city[i]->m_blue_soldier->whether_dead()){
									city[i]->m_blue_soldier->counterattack(ttime, i, city[i]->m_red_soldier.get());
								}
							}
							else{
								city[i]->m_blue_soldier->attack(ttime, city[i]->m_red_soldier.get(),0,i);
								if(city[i]->m_red_soldier->_m_id()!=1&&!city[i]->m_red_soldier->whether_dead()){
									city[i]->m_red_soldier->counterattack(ttime, i, city[i]->m_blue_soldier.get());
								}
							}
						}

						//判断输赢
						if(city[i]->m_blue_soldier->whether_dead()){
							blue_dead = 1;
							if(city[i]->m_whether_red_killed_by_arrow==false&&city[i]->m_whether_blue_killed_by_arrow==false){
								cout << show_time << " blue " << city[i]->m_blue_soldier->_m_name() << " " << city[i]->m_blue_soldier->m_num << " was killed in city " << city[i]->m_num << endl;
							}
						}
						if(city[i]->m_red_soldier->whether_dead()){
							red_dead = 1;
							if(city[i]->m_whether_red_killed_by_arrow==false&&city[i]->m_whether_blue_killed_by_arrow==false){
								cout << show_time << " red " << city[i]->m_red_soldier->_m_name() << " " << city[i]->m_red_soldier->m_num << " was killed in city " << city[i]->m_num << endl;
							}
						}
						if(red_dead+blue_dead==1){
							if(red_dead==1){
								if(city[i]->m_blue_soldier->_m_id()==0){//如果是龙
									city[i]->m_blue_soldier->_m_morale() += 0.2;
									if(ccolor=="blue"&&city[i]->m_blue_soldier->_m_morale()>=0.8){
										city[i]->m_blue_soldier->cheer(ttime, i);
									}
								}
								if(city[i]->m_red_soldier->_m_id()==3){	//如果是虎
									city[i]->m_blue_soldier->m_hp += red_before_hp;
								}
								if(city[i]->m_blue_soldier->_m_id()==4){//如何是狼
									city[i]->m_blue_soldier->get_weapon(city[i]->m_red_soldier.get());
								}
								if(blue.m_hp>=8){	//奖励生命元
									city[i]->m_blue_soldier->m_hp += blue.change_hp(-8);
								}
								blue.change_hp(city[i]->win(ttime, city[i]->m_blue_soldier.get()));//回收生命元
							}
							if(blue_dead==1){
								if(city[i]->m_red_soldier->_m_id()==0){//如果是龙
									city[i]->m_red_soldier->_m_morale() += 0.2;
									if(ccolor=="red"&&city[i]->m_red_soldier->_m_morale()>=0.8){
										city[i]->m_red_soldier->cheer(ttime, i);
									}
								}
								if(city[i]->m_blue_soldier->_m_id()==3){	//如果是虎
									city[i]->m_red_soldier->m_hp += blue_before_hp;
								}
								if(city[i]->m_red_soldier->_m_id()==4){//如果是狼
									city[i]->m_red_soldier->get_weapon(city[i]->m_blue_soldier.get());
								}
								if(red.m_hp>=8){
									city[i]->m_red_soldier->m_hp += red.change_hp(-8);
								}
								red.change_hp(city[i]->win(ttime, city[i]->m_red_soldier.get()));
							}
						}
						else if(red_dead+blue_dead==0){					
							if(city[i]->m_red_soldier->_m_id()==0){//如果是龙
								city[i]->m_red_soldier->_m_morale() -= 0.2;
								if(ccolor=="red"&&city[i]->m_red_soldier->_m_morale()>0.8){
									city[i]->m_red_soldier->cheer(ttime, i);
								}
							}
							if(city[i]->m_blue_soldier->_m_id()==0){//如果是龙
								city[i]->m_blue_soldier->_m_morale() -= 0.2;
								if(ccolor=="blue"&&city[i]->m_blue_soldier->_m_morale()>0.8){
									city[i]->m_blue_soldier->cheer(ttime, i);
								}
							}
							if(city[i]->m_red_soldier->_m_id()==3){//如果是老虎,这地方按理说可以有面向对象的解决办法，但懒了
								city[i]->m_red_soldier->_m_loyalty() -= K;
							}
							if(city[i]->m_blue_soldier->_m_id()==3){//如果是老虎
								city[i]->m_blue_soldier->_m_loyalty() -= K;
							}
							city[i]->draw();
						}
					}
					else{
						if(city[i]->m_blue_soldier!=nullptr){
							if(city[i]->m_blue_soldier->whether_dead()){
								blue_dead = 1;
							}
						}
						if(city[i]->m_red_soldier!=nullptr){
							if(city[i]->m_red_soldier->whether_dead()){
								red_dead = 1;
							}
						}
					}
					if(red_dead){
						red.soldier_dead(city[i]->m_red_soldier->m_num);
						city[i]->m_red_soldier.reset();
					}
					if(blue_dead){
						blue.soldier_dead(city[i]->m_blue_soldier->m_num);
						city[i]->m_blue_soldier.reset();
					}
					city[i]->m_whether_fighting = false;
					city[i]->m_whether_red_killed_by_arrow = false;
					city[i]->m_whether_blue_killed_by_arrow = false;
					red_dead = 0;
					blue_dead = 0;
					red_before_hp = 0;
					blue_before_hp = 0;
				}
			}
			if(mmin==50){
				red.report_headquarter(ttime);
				blue.report_headquarter(ttime);
			}
			if(mmin==55){
				for (int i = 0; i <= N+1;++i){
					if (city[i]->m_red_soldier != nullptr)
					{
						city[i]->m_red_soldier->report(ttime);
					}
				}
				for(int i = 0; i <= N+1;++i){
					if(city[i]->m_blue_soldier!=nullptr){
						city[i]->m_blue_soldier->report(ttime);
					}
				}
			}
		}
	}
	return 0;
}
