#include "Main.h"                                                         
using namespace std;
//ifstream entityfile, npcfile, mercfile, shopfile;
int strInt(string in)   //string to int
{
    stringstream ss;
    int i;
    ss<<in;
    ss>>i;
    return i;
}
string intStr(int in)   //int to string
{
    stringstream ss;
    string i;
    ss<<in;
    i=ss.str();
    return i;
}
void mesg(string msg=" ",string msg1=" ",string msg2=" ")   //在界面的右下角黑色处显示信息，三行
{
    if(doShowMsg==0)return;
    #define InfoPos 49
    gotoxy(InfoPos,20);
    cout<<"                                "<<endl;
    gotoxy(InfoPos,21);
    cout<<"                                "<<endl;
    gotoxy(InfoPos,22);
    cout<<"                                "<<endl;
    gotoxy(InfoPos+2,20);
    cout<<msg<<endl;
    gotoxy(InfoPos,21);
    cout<<msg1<<endl;
    gotoxy(InfoPos,22);
    cout<<msg2<<endl;
    #undef InfoPos
}
const string getLevelFile(int level)    //获取关卡文件路径
{
    string tmp=apppath;
    tmp+="levels\\level";
    tmp+=intStr(level);
    tmp+=".dat";
    return tmp;
}

void Entity::getName(string&n1, string&n2)      //从数据文件中获取实体的显示名称
{
    if((Type>399&&Type<998)||(Type>2999&&Type<3300)||Type>3999)
    {
        n1="    ";
        n2="    ";
        return;
    }
    else if(Type>3299)
    {
        if(Type<3900){
            n1="隔壁";
            if(Type<3500)n2="老王";    //3300-3499
            else if(Type<3900)n2="小红"; //3500-3699 //3700-3899 -200 //npc统一命名
            //3700-3899 are merchants while others aren't.
            //only women merchants here
        }
        else if(Type>3899){             //shop
            switch(Type)
            {
                case 3900:
                    n1="魔王";
                    n2="买血";
                    break;
                case 3901:
                    n1="坑爹";
                    n2="买攻";
                    break;
                case 3902:
                    n1="商店";
                    n2="买防";
            }
        }   //when used, put together to get a "魔王坑爹商店\n买攻买血买防".
            // NOT functional yet
        return;
    }
    string tmp;
    string skip1, skip2;
    //int i;
    ifstream infile;
    locale loc("chs");
    infile.imbue(loc);
    string entpath=apppath+"entity.dat";
    infile.open(entpath.c_str());
    infile>>tmp;
    while(strInt(tmp)!=Type)
    {
        if(infile.eof())
        {
            infile.close();
            n1="没有";
            n2="数据";
            return;
        }
        infile>>skip1>>skip2>>tmp;
    }
    infile>>n1>>n2;
    infile.close();
    return;
}
void Entity::show()         //显示实体，依类型不同，取不同的前景色和背景色
{
if(Type<400)
{
    switch(Type)
    {
        case cWALL:         //including OPENABLE wall
            textbackground(YELLOW);
            textcolor(LIGHTGRAY);
            break;
        case INV_WALL:
            Occupied[Level][X][Y]=this;     //隐形墙默认不显示，仅将自己对应的全局指针指向自己后即return
            return;
        case YDOOR:
            textbackground(YELLOW);
            textcolor(YELLOW);
            break;
        case BDOOR:
            textbackground(BLUE);
            textcolor(LIGHTBLUE);
            break;
        case RDOOR:
            textbackground(RED);
            textcolor(LIGHTRED);
            break;
        case cKEY:
            textbackground(LIGHTGRAY);
            switch(Type)
            {
                case YKEY: textcolor(YELLOW);break;
                case BKEY: textcolor(BLUE);break;
                case RKEY: textcolor(LIGHTRED);break;
            }
            break;
        case cGEM:
            textcolor(WHITE);
            switch(Type)
            {
                case cRGEM: textbackground(LIGHTRED);break;
                case cBGEM: textbackground(LIGHTCYAN);break;
            }
            break;
        case cSWORD:
            textbackground(YELLOW);
            textcolor(WHITE);
            break;
        case cSHIELD:
            textbackground(CYAN);
            textcolor(WHITE);
            break;
        case cSTC:
            textbackground(BLACK);
            textcolor(WHITE);
            break;
    }
}
else if(Type>399&&Type<500)         //保留ID段
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type>499&&Type<600)         //保留ID段
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type>599&&Type<998)         //保留ID段
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type==PLAYER)
{
    textbackground(BLUE);
    textcolor(WHITE);
}
else if(Type>999&&Type<3000)        //敌人
{
    textbackground(GREEN);
    textcolor(WHITE);
}
else if(Type>2999&&Type<3300)       //为楼梯口保留，后来楼梯口因非实体的处理方式是只存储ID，不建立对象
                                    //故此段无作用
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type>=3300)                 //NPC及以后
{
    textbackground(MAGENTA);
    textcolor(WHITE);
}

    gotoxy(POS);
    cprintf("%s",Name1.c_str());
    //cout<<Name1;
    gotoxy(POS+1);
    cprintf("%s",Name2.c_str());
    gotoxy(1,1);
    Occupied[Level][X][Y]=this;
}
Entity::Entity(int x, int y, int type)      //全部实体的构造函数
{
    if(type==-998)return;
    Type=type;
    getName(Name1,Name2);
    X=x;
    Y=y;
    Map[Level][X][Y]=Type;
    show();
//    gotoxy(DEFAULTPOS);
}

void Entity::disappear()            //实体从某一坐标消失
{
    Occupied[Level][X][Y]=0;
    if(Type!=PLAYER)Map[Level][X][Y]=0;     //实际存在的玩家并不存在于实体ID表中
                                            //也即不存在于Map[Level][X][Y]中
                                            //且玩家若走到楼梯口时消失
                                            //此处若不排除玩家 则会导致楼梯口标记消失
    textbackground(LIGHTGRAY);
    gotoxy(POS); cprintf("    ");
    gotoxy(POS+1);  cprintf("    ");
//    gotoxy(DEFAULTPOS);
}

int Player::attMove(Direction Dir)          //玩家**试图**移动的函数，程序的核心
{
    int destX=X, destY=Y;                   //试图移动的目的地坐标，先初始化为玩家未移动的坐标
    switch(Dir)                             //由方向确定目的地坐标
                                            //**目前该机制不适于制作“飞到中心对称点”（50层魔塔）
                                            //**此类的道具。需改进
    {
        case UP: destY--;break;
        case DOWN: destY++;break;
        case LEFT: destX--;break;
        case RIGHT: destX++;break;}
    if(destX<0||destX>10||destY<0||destY>10)return 0;       //超出地图范围，撞墙了
    if(!Occupied[Level][destX][destY])      //若目标地点未被占据，则直接移动
    {
        Move(destX,destY);
        return 1;
    }
    int nextType=((Entity*)Occupied[Level][destX][destY])->getType();   //若已被占据，则获取占据该位置的实体ID
                                                                        //用Entity::getType()而不直接从Map[][][]取数据
                                                                        //保证了楼梯口标志不被作为障碍物
    if(nextType==100){          //直接撞墙
        mesg("撞得你头好痛。");
        return 0;
    }
    else if(nextType==101)      //隐形墙
    {
        ((Blocker*)Occupied[Level][destX][destY])->setType(100);
        Map[Level][destX][destY]=100;
        ((Blocker*)Occupied[Level][destX][destY])->show();
        mesg("想不到吧，这就是这个世界的","恶意。");
        return 0;
    }
    else if(nextType/10==11)return attOpen(*(Blocker*)Occupied[Level][destX][destY]);             //调用试图打开函数
    else if(nextType/10==20)        //占据物为钥匙
    {
        getKey((KeyColor)(nextType-200));
        ((Key*)Occupied[Level][destX][destY])->~Key();
        Map[Level][destX][destY]=0;
        Occupied[Level][destX][destY]=0;
        Move(destX,destY);
        return 1;
    }
    else if(nextType/10>20&&nextType/10<24)   //占据物为血、攻防的补给品
    {
        getAdder(*(Adder*)Occupied[Level][destX][destY]);
        ((Adder*)Occupied[Level][destX][destY])->~Adder();
        Map[Level][destX][destY]=0;
        Occupied[Level][destX][destY]=0;
        Move(destX,destY);
        return 1;
    }
    else if(nextType/100==3)   //前方为楼梯
    {
        if(nextType/10==30){        //上楼，设置标记，稍后在main()中处理
                                    //本来我希望不在main()中放入任何有关游戏流程的具体内容
                                    //而是全部写成函数 但是我终究还是没有那么无聊。而且全局变量已经太多太多了。
            Stat=__Up;
            NextLevPlayerPos=nextType*10+100;   //计算得下一楼层楼梯出口的标志ID
                                                //**这就要求本层的上楼梯ID和上层的下楼梯ID必须对应（个位相等）
                                                //**编辑器的关卡错误检查函数(isValidLevel())尚未包含这一点
                                                //**（实际上编辑器在同一时间只加载一个层，可在检查时加载上下层检查对应楼梯的位置和ID）
        }
        else if(nextType/10==31){           //下楼，同理
            Stat=__Down;
            NextLevPlayerPos=nextType*10-100;
        }
        return 998;
    }
    else if(nextType>999&&nextType<3000)    //敌人，调用战斗函数
    {
        return Battle(*(Enemy*)Occupied[Level][destX][destY]);
    }
    else if(nextType>3299&&nextType<3700)   //普通NPC（非商人）
    {
        int tmpFea = 0;
        if(((Npc*)Occupied[Level][destX][destY])->unlockFeature!=0){
            tmpFea = ((Npc*)Occupied[Level][destX][destY])->unlockFeature;
            SkillSet[((Npc*)Occupied[Level][destX][destY])->unlockFeature]=1;
        }
        if(((Npc*)Occupied[Level][destX][destY])->showDialog()==1)      //Npc::showDialog()返回的是该NPC是否只能访问一次
                                                                        //若为1则NPC消失
        {
            ((Npc*)Occupied[Level][destX][destY])->~Npc();
            Map[Level][destX][destY]=0;
            Occupied[Level][destX][destY]=0;
        }
        if(tmpFea) gameHelp(tmpFea);
        return 1;
    }
    else if(nextType>3699&&nextType<3903)   //商人
    {
        if(((Merchant*)Occupied[Level][destX][destY])->yesNoDialog()==1)    //Merchant::yesNoDialog()返回是否购买
                                                                            //若选择购买则进行下一步判定
        {
            int tmpre;
            if(nextType<3900){
                tmpre=purchaseFromMerchant((*(Merchant*)Occupied[Level][destX][destY]));
                                        //Player::purchaseFromMerchant()
                                        //返回值0为无钱 1为购买成功且无后续 -1在商店购买成功
                                        //返回其他值表示购买成功且该商人转化为另一ID为该返回值的NPC
            } else tmpre=purchaseFromShop((*(Shop*)Occupied[Level][destX][destY]));

            if(tmpre==0)
            {
                string nom[4];
                nom[0]="没钱还来买东西？滚蛋！";
                mesg("先去弄点钱吧。");
                npcStyleDialog(1,nom);  //这个函数和前面的mesg()遇到了同样的问题，就是
                                        //我不知道如何正确*按长度*拆分**含有中文**的string字符串
                                        //所以用这种办法
            }
            else if(tmpre != -1)
            {
                string bou[4];
                bou[0]="购买成功";
                mesg("购买成功");
                npcStyleDialog(1,bou);
                if (tmpre==1) return 1; //购买成功，无后续。使商人消失在Player::purchaseFromMerchant()中已完成。
                int tmpx, tmpy;         //购买成功，商人转化为另一NPC，此后其资料都以文件记载中新NPC的资料为准
                ((Entity*)Occupied[Level][destX][destY])->getPos(tmpx, tmpy);
                ((Merchant*)Occupied[Level][destX][destY])->~Merchant();
                ((Npc*)Occupied[Level][destX][destY])=new Npc(tmpx, tmpy, tmpre);
                refresh();
            }
            else
            {
                string bou[4];
                bou[0]="购买成功，欢迎下次光临";
                mesg("购买成功");
                npcStyleDialog(1,bou);
                return 1;
            }
        }
        return 1;
    }
    else return 0;
}
int Player::Battle(Enemy&enemy)     //与敌人战斗
{
    gotoxy(49,20);
    cout<<"                                "<<endl;
    gotoxy(49,21);
    cout<<"                                "<<endl;
    gotoxy(49,22);
    cout<<"                                "<<endl;
    int sleepTime=333, ffTime=33, ff=0;     //我为了手动增加打怪速度而设置的，但是很失败。
    int dmgToPlayer = (enemy.Atk-Def>0)?(enemy.Atk-Def):1;      //敌人对玩家造成的伤害，下同
    int dmgToEnemy = (Atk-enemy.Def>0)?(Atk-enemy.Def):1;
    int tmpHp = Hp;
    int tmpEhp= enemy.Hp;       //临时存储两方血量，以判定是否允许战斗
    int turn=0;
    if(enemy.Hp<=0)enemy.~Enemy();
    while(yourDaddy==0)                //预战斗，判定是否允许战斗
    {
        tmpEhp-=dmgToEnemy;
        if(tmpHp>0&&tmpEhp<=0)break;    //如果预战斗敌人输，则进行正式战斗
        tmpHp-=dmgToPlayer;
        if(tmpEhp<=0&tmpHp<=0){         //如果预战斗玩家死了，因为要计算估计伤害，
                                        //所以必须计算到敌人也死为止
            enemy.refreshStatus();
            mesg("","","不可攻击！");
            gotoxy(49,20);
            cout<<"估计伤害      "<<Hp-tmpHp<<endl;
            return 0;
        }
    }
    if(yourDaddy==1)enemy.Hp=-1;
    while(1)
    {
        ++turn;
        if(ff>=6)sleepTime=ffTime;      //我为手动控制是否加速而进行的失败尝试
        if(turn==100)sleepTime=1;
        if(turn==50)sleepTime/=2;
        if(turn==25)sleepTime/=2;
        if(turn==10)sleepTime/=2;
        if(sleepTime<=0)sleepTime=1;
        enemy.Hp-=dmgToEnemy;           //玩家砍一刀
        enemy.refreshStatus();
        if(enemy.Hp<=0)
        {
            enemy.refreshStatus();
            mesg("胜利！");
            Gold+=enemy.Gold;
            enemy.~Enemy();
            Map[Level][enemy.X][enemy.Y]=0;
            //WaitForSingleObject(hMutex, INFINITE);
            while(getVK(1));        //好像是用来清除VK缓冲的东西。不过话说回来，VK这东西有缓冲么？
                                    //同样是失败的尝试。
            //ReleaseMutex(hMutex);
            return 1;
        }
        if(!kbhit())ff=0;           //失败尝试，下同。
        else ff++;
        Sleep(sleepTime);
        //while(getVK(1));
        if(!kbhit())ff=0;
        else ff++;
        Hp-=dmgToPlayer;
        refreshStatus();
        if(!kbhit())ff=0;
        else ff++;
        Sleep(sleepTime);
        //while(getVK(1));
        if(!kbhit())ff=0;
        else ff++;
    }
}

void Creature::Move(int destX, int destY)   //与**试图**移动不同，该函数进行实际的移动，无论目标地点是否被占据。
                                            //**不会检查移动的初始点和目标地点是否存在玩家以外的任何东西。
{
    disappear();
    X=destX, Y=destY;
    show();
}
void Player::getAdder(Adder&gotAdder)       //获得血、攻防的补给
{
    Hp+=gotAdder.aHp;
    Atk+=gotAdder.aA;
    Def+=gotAdder.aD;
    if(gotAdder.Type==210||gotAdder.Type==212)
        mesg("吃了好吃。");
    else if(gotAdder.Type==211||gotAdder.Type==213)
        mesg("喝了好喝。");
    return;
}
void Player::addDirectly(AttribType ty, int amo)
{
    switch(ty)
    {
        case __Hp: Hp+=amo; break;
        case __Atk: Atk+=amo;break;
        case __Def: Def+=amo; break;
        case __YKey: haveKeys[KeyYellow]+=amo; break;
        case __BKey: haveKeys[KeyBlue]+=amo; break;
        case __RKey: haveKeys[KeyRed]+=amo; break;
        case __Gold: Gold+=amo; break;
    }
}
void Player::setDirectly(AttribType ty, int amo)
{
    switch(ty)
    {
        case __Hp: Hp=amo; break;
        case __Atk: Atk=amo;break;
        case __Def: Def=amo; break;
        case __YKey: haveKeys[KeyYellow]=amo; break;
        case __BKey: haveKeys[KeyBlue]=amo; break;
        case __RKey: haveKeys[KeyRed]=amo; break;
        case __Gold: Gold=amo; break;
    }
}
Enemy::Enemy(int x, int y, const int type):Creature(x,y,type,0,0,0,0)   //构造敌人对象时
                                                                        //从文件读取其战力
{
    int temp;
    int tdata[4];
    ifstream infile;
    string enepath=apppath+"enemy.dat";
    infile.open(enepath.c_str());
    infile>>temp;
    while(temp!=type)infile>>tdata[0]>>tdata[1]>>tdata[2]>>tdata[3]>>temp;
    infile>>Hp>>Atk>>Def>>Gold;
    infile.close();
    return;
}

Adder::Adder(int x, int y, AdderType type):Item(x,y,200+type)   //构造血攻防补给对象时
                                                                //设置其补给的内容及数量
{                                                               //!!我忘了这玩意是可以依楼层变化的了！
    aHp=0, aA=0, aD=0;
    if(type<20)
    {
        switch(type)
        {
            case RGem: aA=2; break;
            case BGem: aD=2; break;
            case RMed: aHp=150; break;
            case BMed: aHp=400; break;
        }
    }
    else if(type<30)aA=(type-19)*10;
    else aD=(type-29)*10;
    return;
}
string Shop::toString()
{
    //示例：“20金币可购买300生命”
    return intStr(price) + "金币 可购买 " + intStr(goodsamount) + goodsname;
}
int Player::attOpen(Blocker&block)      //试图打开障壁的函数。障壁包括门。
{
    int tmpType=block.getType();
    switch(tmpType)
    {
        case 100: return 0;         //实墙。
        case 113: block.~Blocker(); //假墙。
                  Map[Level][block.X][block.Y]=0;
                  if(debugMode>=3)mesg("学挖掘机技术哪家强？","中国山东找蓝翔！");
                  else mesg("在这深深的善意背后，隐藏着","更深的、人所未知的恶意。");
                  return 1;
        case 110: case 111: case 112:       //门。
            if(haveKeys[tmpType-110]==0){
                mesg("没有钥匙！");
                return 0;
            }
            else
            {
                haveKeys[tmpType-110]--;
                block.~Blocker();
                Map[Level][block.X][block.Y]=0;
                mesg("门开了！");
                return 1;
            }
    }
    return 0;
}
void Player::refreshStatus()        //刷新玩家状态
{
    #define InfoPos 49
    textbackground(BLACK);
    gotoxy(InfoPos+12,2);
    cout<<(Hp>0?Hp:0)<<"      ";
    gotoxy(InfoPos+12,4);
    cout<<Atk<<"      ";
    gotoxy(InfoPos+12,6);
    cout<<Def<<"      ";
    gotoxy(InfoPos+12,8);
    cout<<Gold<<"      ";
    //gotoxy(InfoPos+12,10);
    //cout<<"                ";
    gotoxy(InfoPos+9,10);
    cout<<setw(2)<<(haveKeys[0]>=100?-1:(haveKeys[0]));
    gotoxy(InfoPos+15,10);
    cout<<setw(2)<<(haveKeys[1]>=100?-1:haveKeys[1]);
    gotoxy(InfoPos+21,10);
    cout<<setw(2)<<(haveKeys[2]>=100?-1:haveKeys[2]);
    gotoxy(1,1);
    gotoxy(InfoPos+24,1);
    cout<<"楼层 "<<setw(2)<<Level;
}
void Enemy::refreshStatus()         //刷新敌人状态
{
    gotoxy(InfoPos+14,12);
    cout<<(Hp>0?Hp:0)<<"      ";
    gotoxy(InfoPos+14,14);
    cout<<Atk<<"      ";
    gotoxy(InfoPos+14,16);
    cout<<Def<<"      ";
    gotoxy(InfoPos+14,18);
    cout<<Gold<<"      ";
    gotoxy(InfoPos+2,20);
    gotoxy(1,1);

}
Npc::Npc(int x, int y, int Index):Entity(x,y,Index)     //从文件读取NPC信息
{
    if(Index>3899)return;
    ifstream infile;
    locale loc("chs");
    infile.imbue(loc);
    string npcpath=apppath+"npctext.dat";
    infile.open(npcpath.c_str());   //存储于npctext.dat中的NPC信息的格式为
                                    //3300 1 0 2 一二三四五六七八九十一二三四五六七八 欢迎来到魔塔！
                                    //3300为NPCID，整型；
                                    //1标记该NPC只能访问一次，后自动消失。0反之。
                                    //0标记该NPC是否为你解锁新功能（道具等），暂未开发。
                                    //2标记信息的行数。
                                    //**fstream将空格作为分隔符，导致信息内不能含有空格
                                    //**但这个问题在Avis程序中已有解决方案
                                    //****但是我丢了Avis的源码
                                    //一二三四五六七八九十一二三四五六七八 为第一行信息
                                    //欢迎来到魔塔！ 为第二行信息
                                    //此后用任意分隔符分隔后可以记录下一条。
                                    //建议使用换行作为这里的分隔
    int tmpindex;
    int tmplines;
    int autodiss;
    int unlock;
    string tmptext;
    infile>>tmpindex;
    while(tmpindex!=Index)
    {
        if(infile.eof()){
            textLines=1;
            text[0]="error";
            infile.close();
            return;
        }
        infile>>autodiss>>unlock>>tmplines;
        while(tmplines--)infile>>tmptext;
        infile>>tmpindex;
    }
    infile>>doAutoDisappear;
    infile>>unlockFeature;
    infile>>tmplines;
    textLines=tmplines;
    while(tmplines--)
    {
        infile>>text[textLines-tmplines-1];
    }
    infile.close();
}
Shop::Shop(int x, int y, int Index, int Floor):Merchant(x, y, Index)
{
    floor = Floor;
    thenProvInfo = 0;
    doAutoDisappear = 0;
    unlockFeature = 0;
    textLines = 1;
    ifstream infile;
    locale loc("chs");
    infile.imbue(loc);
    string shopinfo=apppath+"shopinfo.dat";
    infile.open(shopinfo.c_str());
    int fl, init_pr, hp_am, atk_am, def_am;
    infile>>fl;
    while(fl!=Floor&&!infile.eof())
    {
        infile>>init_pr>>hp_am>>atk_am>>def_am;
        infile>>fl;
    }
    infile>>init_pr>>hp_am>>atk_am>>def_am;
    price = init_pr;
    firstprice = price;
    switch(Index){
        case 3900:
            goodstype = __Hp;
            goodsamount = hp_am;
            goodsname = "生命";break;
        case 3901:
            goodstype = __Atk;
            goodsamount = atk_am;
            goodsname = "攻击";break;
        case 3902:
            goodstype = __Def;
            goodsamount = def_am;
            goodsname = "防御";break;
    }
    //示例：“20金币可购买300生命”
    infile.close();
    speed = prcIncreSpeed;
}

Merchant::Merchant(int x, int y, int Index):Npc(x,y,Index)
{
    if(Index > 3899)return;
    ifstream infile;
    locale loc("chs");
    infile.imbue(loc);
    string merpath=apppath+"merchant.dat";
    infile.open(merpath.c_str());       //存储于merchant.dat中的商人信息格式：
                                        //3701 1 200 5000 0
                                        //表示：
                                        //NPCID为3701的商人以5000金为价格出售200点攻击力(1)
                                        //然后直接消失(0)，不提供任何信息。
                                        //3701为NPCID，整型
                                        //1为枚举类型，商品种类 enum AttribType{
                                        //  __Hp=0,
                                        //  __Atk,
                                        //  __Def,
                                        //  __YKey,
                                        //  __BKey,
                                        //  __RKey};
                                        //200为所售商品的数量。
                                        //5000为所售商品的总价
                                        //0为标志商人交易后是否转变为其他NPC的值，在Player::attMove()中已有说明
                                        //  likewise, 3702 2 8 40 3602
                                        //  meaning an npcid 3702 selling 8 Def at a price of 40,
                                        //  and turn into npcid 3602(will say what 3602 says and behave as 3602 written in npctext.dat.
                                        //  a merchant can turn into another non-merchant or merchant npc
                                        //  as this is not strictly discipilined in the programming process,
                                        //  however it's not recommended to turn a merchant into another merchant.)
                                        //end, use any delimiter to seperate from next entry.
                                        //CrLf is recommended.
    int tmpindex,tmptype,tmpamount,tmpprice,tmpnextindex;
    infile>>tmpindex;
    //cout<<tmpindex;
    while(tmpindex!=Index)
    {
        if(infile.eof())
        {
            infile.close();
            return;
        }
        infile>>tmptype>>tmpamount>>tmpprice>>tmpnextindex>>tmpindex;
    }
    infile>>tmptype>>goodsamount>>price>>thenProvInfo;
    goodstype=tmptype;
}
int Npc::showDialog()       //NPC对话框
{
    gotoxy(3,6);
    textbackground(LIGHTBLUE);
    for(int i=3, j=6;j<14;(i==42)?(i=3,j++):i++)
    {
        gotoxy(i,j);
        cprintf(" ");
    }
    int tmplines=textLines;
    while(tmplines--){
        gotoxy(5,textLines-tmplines+6);
        //cout<<text[textLines-tmplines-1]<<endl;
        cprintf(text[textLines-tmplines-1].c_str());
        cout<<endl;
    }
    gotoxy(29,12);cout<<"按任意键继续";
    //ReleaseMutex(hMutex);
    getVK();
    //WaitForSingleObject(hMutex, INFINITE);
    refresh();
    if(doAutoDisappear) return 1;       //告诉上一级函数让NPC自动消失
    else return 0;
}
int Merchant::yesNoDialog()     //商人的“按Y购买”对话框
{
    gotoxy(3,6);
    textbackground(LIGHTBLUE);
    for(int i=3, j=6;j<14;(i==42)?(i=3,j++):i++)
    {
        gotoxy(i,j);
        cprintf(" ");
    }
    if(this->Type<3900){
        int tmplines=textLines;
        while(tmplines--){
            gotoxy(5,textLines-tmplines+6);
            //cout<<text[textLines-tmplines-1]<<endl;
            cprintf(text[textLines-tmplines-1].c_str());
            cout<<endl;
        }
    } else {
        gotoxy(5, 7);
        cout<<(((Shop*)this)->toString())<<endl;
    }
    gotoxy(16,12);cout<<"按 Y 购买，其他任意键返回";
    int tmpin;
    ReleaseMutex(hInputMutex);
    tmpin=getVK();
    WaitForSingleObject(hInputMutex, INFINITE);
    if(tmpin!='Y'){refresh();return 0;}     //若输入非Y，取消
    else return 1;
}
int Shop::nextPrice()
{
    price += speed;
    speed+=prcIncreAccel;
    return price;
}
int Shop::nextPrice(int price)
{
    int i=firstprice;
    int v=prcIncreSpeed;
    while(i!=price)
    {
        i += v;
        v += prcIncreAccel;
    }
    return i + v;
}
int Player::purchaseFromShop(Shop&shop)
{
    if(purchaseFromMerchant(shop) == -1){
        shop.nextPrice();
        if(shop.Type==3900)
        {
            ((Shop*)(Occupied[shop.floor][shop.X+1][shop.Y]))->nextPrice();
            ((Shop*)(Occupied[shop.floor][shop.X+2][shop.Y]))->nextPrice();
        }
        else if(shop.Type==3901)
        {
            ((Shop*)(Occupied[shop.floor][shop.X+1][shop.Y]))->nextPrice();
            ((Shop*)(Occupied[shop.floor][shop.X-1][shop.Y]))->nextPrice();
        }
        else if(shop.Type==3902)
        {
            ((Shop*)(Occupied[shop.floor][shop.X-1][shop.Y]))->nextPrice();
            ((Shop*)(Occupied[shop.floor][shop.X-2][shop.Y]))->nextPrice();
        }
        return -1;
    } else return 0;

    // 0 = 无钱
    // -1 = 购买成功

}
int Player::purchaseFromMerchant(Merchant&merc)
{
    gotoxy(1,1);
    if(Gold<merc.price)return 0;        //玩家无钱，取消
    Gold=Gold-merc.price;       //扣钱
    if(merc.goodstype==0)Hp+=merc.goodsamount;
    else if(merc.goodstype==1)Atk+=merc.goodsamount;
    else if(merc.goodstype==2)Def+=merc.goodsamount;
    else if(merc.goodstype==3)haveKeys[KeyYellow]+=merc.goodsamount;
    else if(merc.goodstype==4)haveKeys[KeyBlue]+=merc.goodsamount;
    else if(merc.goodstype==5)haveKeys[KeyRed]+=merc.goodsamount;
    refreshStatus();
    refresh();
    if(merc.Type>3899)return -1;
    else if(merc.thenProvInfo==0)    //thenProvInfo于前文介绍多次
        merc.~Merchant();
    else return merc.thenProvInfo;
    return 1;
}
void initBoard()            //初始化界面，包括标题、灰色地板、右侧信息区的中文标签
{
    int i,j;
    clrscr();
    gotoxy(1,24);
    cout<<"↓如果这里有输入法信息，请把输入法关掉";
    if(debugMode==1){
        system("title 魔塔 v0.1 预览版 开发模式运行中");
        gotoxy(1,24);
        cout<<"                                      ";
        gotoxy(1,24);
        cout<<"命令行: "<<arguments<<endl;
        gotoxy(1,1);
    }
    else if (debugMode==2)system("title 魔塔 v0.1 预览版 作弊密码已输入");
    else if (debugMode>=3)system("title 魔塔 v0.1 预览版 蓝翔模式运行中");
    else system("title 魔塔 v0.1 预览版 按 L 键显示帮助");
    system("color 0f");
    textbackground(LIGHTGRAY);
    for(i=1,j=1;j<23;(i==44)?(i=1,j++):i++)
    {
        gotoxy(i,j);
        cprintf(" ");
    }
    textbackground(BLACK);
    textcolor(WHITE);
    //gotoxy(InfoPos+8,1);
    //cout<<"按 L 查看帮助   楼层 "<<setw(2)<<Level;
    gotoxy(InfoPos+24,1);
    cout<<"楼层 "<<setw(2)<<Level;
    gotoxy(InfoPos,2);
    cprintf("生命力    ");
    textcolor(LIGHTRED);
    gotoxy(InfoPos,4);
    cprintf("攻击力    ");
    textcolor(LIGHTCYAN);
    gotoxy(InfoPos,6);
    cprintf("防御力    ");
    textcolor(YELLOW);
    gotoxy(InfoPos,8);
    cprintf("金币数    ");
    //textcolor(LIGHTGREEN);
    //gotoxy(InfoPos,10);
    //cprintf("经验值    ");                //经验值已被移除
    textcolor(WHITE);
    gotoxy(InfoPos,10);
    cprintf("钥匙  ");
    textcolor(YELLOW);
    cprintf("黄    ");
    textcolor(LIGHTCYAN);
    cprintf("蓝    ");
    textcolor(LIGHTRED);
    cprintf("红");
    gotoxy(InfoPos,11);
    cout<<"------------------------------"<<endl;
    textcolor(WHITE);
    gotoxy(InfoPos,12);
    cprintf("敌方生命力");
    textcolor(LIGHTRED);
    gotoxy(InfoPos,14);
    cprintf("敌方攻击力");
    textcolor(LIGHTCYAN);
    gotoxy(InfoPos,16);
    cprintf("敌方防御力");
    textcolor(YELLOW);
    gotoxy(InfoPos,18);
    cprintf("敌方金币数");
    //textcolor(LIGHTGREEN);
    //gotoxy(InfoPos,22);
    //cprintf("敌方经验值");
    gotoxy(1,23);
    //for(i=1;i<InfoPos+10;i++)cout<<'-';
    //gotoxy(InfoPos+11,23);
    //cout<<" 按 L 查看帮助  ---"<<endl;      //现在还没有
    //cout<<getLevelFile(Level).c_str();
    gotoxy(1,1);
    #undef InfoPos
}
void loadLevel(int level)               //以关卡号从文件读取关卡
{
    int i,j;
    ifstream infile;
    infile.open(getLevelFile(level).c_str());
    int tmp;
    //initBoard();
    for(i=0;i<11;i++)
        for(j=0;j<11;j++)
        {
            infile>>Map[level][j][i];
            if(debugMode>=3)
            {
                if(Map[level][j][i]==100)Map[level][j][i]=113;
                else if(Map[level][j][i]==101)Map[level][j][i]=0;
            }
        }
    infile.close();
}
void loadLevel(string level)            //以文件名从文件读取关卡
                                        //作为debug模式的功能 以命令行参数指定关卡文件
                                        //*在主函数中尚未实现
                                        //**注意这里 level 为string类型的关卡文件名
                                        //**而 Level 为整型的关卡号，由于是调试载入所以需要在main()函数中自己指定
                                        //***同样没有实现。
{
    int i,j;
    ifstream infile;
    infile.open(level.c_str());
    int tmp;
    initBoard();
    for(i=0;i<11;i++)
        for(j=0;j<11;j++)
        {
            infile>>Map[Level][j][i];
            if(debugMode>=3)
            {
                if(Map[Level][j][i]==100)Map[Level][j][i]=113;
                else if(Map[Level][j][i]==101)Map[Level][j][i]=0;
            }
        }
    infile.close();
}
void refresh()                      //在不完整重绘地板的情况下重绘地图上的对象
{
    int i,j;
    for(i=0;i<11;i++)
    {
        for(j=0;j<11;j++)
        {
            if(Occupied[Level][i][j]&&((Entity*)(Occupied[Level][i][j]))->getType()!=INV_WALL)
            {
                ((Entity*)(Occupied[Level][i][j]))->show();
            }
            else {
                //system("pause");
                textbackground(LIGHTGRAY);
                for(int p=4*i+1,q=2*j+1;p<4*i+5,q<2*j+3;(p==4*i+4)?(p=4*i+1,q++):p++)
                {
                    gotoxy(p,q);
                    cprintf(" ");
                }
            }
        }
    }
}
void npcStyleDialog(int lines, string cont[])   //NPC风格对话框
{
    gotoxy(3,6);
    textbackground(LIGHTBLUE);
    for(int i=3, j=6;j<14;(i==42)?(i=3,j++):i++)
    {
        gotoxy(i,j);
        cprintf(" ");
    }
    int tmplines=lines;
    textcolor(WHITE);
    while(tmplines--){
        gotoxy(5,lines-tmplines+6);
        //cout<<cont[lines-tmplines-1]<<endl;
        cprintf(cont[lines-tmplines-1].c_str());
        cout<<endl;
    }
    gotoxy(29,12);cout<<"按任意键继续";
    getVK();
    refresh();
}
int npcStyleYesNoDialog(int lines, string cont[])     //商人的“按Y购买”对话框
{
    gotoxy(3,6);
    textbackground(LIGHTBLUE);
    for(int i=3, j=6;j<14;(i==42)?(i=3,j++):i++)
    {
        gotoxy(i,j);
        cprintf(" ");
    }
    int tmplines=lines;
    textcolor(WHITE);
    while(tmplines--){
        gotoxy(5,lines-tmplines+6);
        //cout<<cont[lines-tmplines-1]<<endl;
        cprintf(cont[lines-tmplines-1].c_str());
        cout<<endl;
    }
    gotoxy(16,12);cout<<"按 Y 确定，其他任意键返回";
    int tmpin;
    tmpin=getVK();
    if(tmpin!='Y'){refresh();return 0;}     //若输入非Y，取消
    else return 1;
}

int getMaxLevel()
{
    ifstream infile;
    infile.open(getLevelFile(0).c_str());
    if(!infile){
        infile.close();
        ofstream outfile;
        outfile.open(getLevelFile(0).c_str(),ios::out|ios::trunc);
        for(int j=0;j<121;j++)
            outfile<<0<<(j%11==10?'\n':'\t');
        outfile.close();
        return 0;
    }
    infile.close();
    int i=0;
    while(++i)
    {
        infile.open(getLevelFile(i).c_str());
        if(!infile)return i-1;
        infile.close();
    }
    return 0;
}
