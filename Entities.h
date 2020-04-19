//这游戏编写的初衷就是
//“测试class的使用”
//所以请不要吐槽太多
class Entity{
protected:
    string Name1, Name2;        //实体显示名称的上下两行
    int Type;                   //实体ID
    int X,Y;                    //实体坐标
    void getName(string&n1, string&n2);
public:
    Entity(int x, int y, int type);
    int getType(){return Type;}
    void disappear();
    void show();
    void setType(int t){Type=t;return;}
    void getPos(int&rex, int&rey){rex=X, rey=Y; return;}
    void setPos(int dex, int dey){X=dex, Y=dey; return;}
    ~Entity(){disappear();}
};
//------------------------------------
class Creature:public Entity{
protected:
    int Hp, Atk, Def, Gold;         //玩家派生类中的Gold表示持有金币数，敌人类中表示打倒可获得金币数
public:
    Creature(int x, int y, int type, int H, int A, int D, int G):Entity(x,y,type)
    {
        Hp=H, Atk=A, Def=D, Gold=G;
    }
    void Move(int destX, int destY);
    //static int whoWins(Creature& attacker, Creature& defender);
    ~Creature(){disappear();}
};
//------------------------------------
class Item;
class Adder;
class Enemy;
class Blocker;
class Merchant;
class Shop;
class Player:public Creature{
private:
    int haveKeys[3];        //分别记录黄、蓝、红钥匙的数量
    int SkillSet[20];
public:
    Player(int x, int y, int H, int A, int D, int G, int Y, int B, int R):Creature(x,y,PLAYER,H,A,D,G)
    {
        haveKeys[KeyYellow]=Y;haveKeys[KeyBlue]=B;haveKeys[KeyRed]=R;
        int i=-1;
        memset(SkillSet, 0, sizeof(SkillSet));
        refreshStatus();
        return;
    }
    int attMove(Direction Dir);         //函数会在 Function.cpp 中详细说明
    int Battle(Enemy&enemy);
    void getKey(KeyColor getKeyType){haveKeys[getKeyType]++;}
    void getAdder(Adder&gotAdder);
    void addDirectly(AttribType ty, int amo);
    void setDirectly(AttribType ty, int amo);
    int purchaseFromMerchant(Merchant&merc);
    int purchaseFromShop(Shop&shop);
    int attOpen(Blocker&block);
    int isSkillOK(int skill){return SkillSet[skill];}
    void refreshStatus();
    ~Player(){disappear();}

    int floorJump(int dest);
    int showEnemyData(int floor);
};
//------------------------------------
class Enemy:public Creature{
public:
    Enemy(int x, int y, int type);
    void refreshStatus();
    
    friend int Player::Battle(Enemy&enemy);
    ~Enemy(){disappear();}
};
//------------------------------------
class Item:public Entity{
public:
    Item(int x, int y, int type):Entity(x,y,type){return;}
    ~Item(){disappear();}
};
//------------------------------------
class Key:public Item{          //这类略鸡肋……
public:
    Key(int x, int y, KeyColor type):Item(x,y,200+type){return;}
    ~Key(){disappear();}
};
//------------------------------------
class Adder:public Item{       //名字只是在动词add后面加了个er而已，取名懒。
private:
    int aHp, aA, aD;
public:
    Adder(int x, int y, AdderType type);
    ~Adder(){disappear();}
    friend void Player::getAdder(Adder&gotAdder);
};
//------------------------------------
class Blocker:public Entity{
public:
    Blocker(int x, int y, int type):Entity(x,y,type){return;}
    friend int Player::attOpen(Blocker&block);
    ~Blocker(){disappear();}
};
//------------------------------------
class Warper:public Entity{
private:
    int cX, cY; //
public:
    Warper(int x, int y, int type):Entity(x,y,type){return;}
    ~Warper(){disappear();}
};
//-------------------------------------
class Npc:public Entity{
protected:
    string text[5];
    int textLines;
    int doAutoDisappear;
public:
    int unlockFeature;
    Npc(int x, int y, int Index);
    ~Npc(){disappear();}
    int showDialog();
};
//--------------------------------------
class Merchant:public Npc{
protected:
    AttribType goodstype;
    int goodsamount;
    int price;
    int thenProvInfo;
public:
    Merchant(int x, int y, int Index);
    ~Merchant(){disappear();}
    friend int Player::purchaseFromMerchant(Merchant&merc);
    int yesNoDialog();
};
//-----------------------------------------

class Shop:public Merchant{
private:
    //thenProvInfo = 0;
    int floor;
    int speed;
    int firstprice;
    string goodsname;
public:
    Shop(int x, int y, int Index, int Floor);
    ~Shop(){disappear();}
    int nextPrice(int price);
    int nextPrice();
    friend int Player::purchaseFromShop(Shop&shop);
    string toString();
};

/* else if(Type>3899){             //shop
            switch(Type)
            {
                case 3900:
                    n1="魔王";
                    n2="买攻";
                    break;
                case 3901:
                    n1="坑爹";
                    n2="买血";
                    break;
                case 3902:
                    n1="商店";
                    n2="买防";
            }
        }   //when used, put together to get a "魔王坑爹商店\n买攻买血买防".*/
