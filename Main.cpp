//TODO: 怪物手册 METRIC: 10
//TODO: 从NPC获得技能 METRIC: 10
//      seemed complete
//TODO: 需要在楼梯口才能发动的楼层跳跃 METRIC: 9
//TODO: 魔法警卫和巫师的电 METRIC: 8
//TODO: 宝石的回复量依楼层高度而变化 METRIC: 8
//TODO: 岩浆 METRIC: 5
//TODO: 以线程计时器重写作战延迟 METRIC: 1
#include "Main.h"
#include <vector>
#define cursorpos 23
void* Occupied[55][11][11]={0};     //存放所有对象的指针
int Map[55][11][11]={0};            //存放所有对象的实体ID
int Visited[55]={0};                //玩家是否曾到过某层
int PlayerPos[2]={10,10};           //玩家坐标
int startLev=0;                     //初始层
int Level=0;                        //当前层
int NextLevPlayerPos;               //当玩家前往其他层时
                                    //玩家应该出现在那一层的
                                    //哪一号标记
                                    //标记为楼梯旁的一个ID为楼梯IDx10的非实体
int initHp=1000, initAtk=10, initDef=10, initGold=0;
                                    //默认初始状态
int initY=0, initB=0, initR=0;      //默认初始状态（钥匙数）
int yourDaddy=0;
int debugMode=0;                    //是否使用命令行参数，0为否，1为调试模式，2为预设作弊模式，3为挖掘机作弊模式，5为预设作弊+挖掘机
string arguments="";                //命令行参数
string apppath;                     //程序路径
int doShowMsg=1;
int Stat=0;                         //在 Function.cpp 中被更改，     -1=d 0=n 1=up
                                    //然后在本文件中被访问
                                    //标记当前时刻玩家是否更改楼层
                                    //**目前该机制不适合设计楼层跳跃
                                    //**需改进  //已修改为适合楼层跳跃 但不是完美正确的结局方法
Player* Shabi;                      //玩家对象的指针
int prcIncreMode;
//int prcIncreOffset;
int prcIncreSpeed;
int prcIncreAccel;
int activate_ingame_cmd(string pre_cmd="");   //游戏内作弊控制台的函数，因只在main.cpp中使用，故在此声明
int shellroot=0;                    //标示游戏内作弊控制台是否具有提升的权限（仿linux装逼）

void showLicense();                 //显示许可协议
void showHelp();                    //显示命令行帮助
vector<string> cmd_history;         //历史输入的作弊命令
int noSound=1;                      //是否已停止音乐
char* bgm="mres.dll";               //背景音乐文件名
LPVOID lpBgm = bgm;
HANDLE hTimerMutex, hInputMutex, hMusicMutex;
HANDLE hMusicHandle;                //这些玩意我都不太会用，大多是现Google + Trial-and-error 做出来的
HANDLE hOut;
using namespace std;

// test for passing function pointers
tmr_para exittimer;                                                     ////
void* pexit;                                                            ////
void ami_vexit(void* exitcode)   //测试传递函数指针                     ////
{                                                                       ////
    cout<<"function ami_vexit() has been called"<<endl;                 ////
    exit(0);                                                            ////
}                                                                       ////
//===================================

int main(int argc, char* argv[])
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //    system("pause");
    // 测试函数指针置于结构体 tmr_para 中传递
    tmr_para exittimer;                                                 ////
    exittimer.secs=5;                                                   ////
    exittimer.pFunction=ami_vexit;                                      ////
    int ti=0, *tj=&ti;                                                  ////
    exittimer.pParameter=tj;                                            ////
    pexit=&exittimer;                                                   ////
    // ===========================================================
    //    system("pause");
    apppath=string(argv[0]);
    apppath.resize(apppath.find_last_of("\\")+1);
    if(argc>1){
        if (stricmp(argv[1],"--help")==0||
            stricmp(argv[1],"-?")==0||
            stricmp(argv[1],"/?")==0){
            cout<<endl;
            cout<<"用法: MagicTower.exe <选项1> <参数1> <选项2> <参数2> ......"<<endl<<endl;
            cout<<"    没有参数    正常启动游戏。"<<endl;
            cout<<"    /?          显示当前帮助。"<<endl<<endl;    //这里的help和游戏内作弊里的help不太一样
            cout<<"    开发模式的选项我会让你知道？少年你太Naive。"<<endl;
            return 0;
        }
        for(int i=1;i<argc;i++)             //我更改了i的增加方法，以获得更大的容错性。暂未测试。
        {
            arguments=arguments+string(argv[i])+" ";
            if(stricmp(argv[i],"--nobgm")==0)
                noSound=1;
            else if(stricmp(argv[i],"--license")==0)
                showLicense();
            else if(stricmp(argv[i],"--level")==0){
                if(debugMode>=2)continue;     //若检测到预设作弊或挖掘机模式，则忽略调试参数
                if(++i==argc)break;
                startLev=atoi(argv[i]);
                Level=startLev;
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--hp")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initHp=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--atk")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initAtk=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--def")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initDef=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--gold")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initGold=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--yellowkey")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initY=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--bluekey")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initB=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--redkey")==0){
                if(debugMode>=2)continue;
                if(++i==argc)break;
                initR=atoi(argv[i]);
                arguments=arguments+string(argv[i])+" ";
                debugMode=1;
            }
            else if(stricmp(argv[i],"--naive")==0){
                if(debugMode==2||debugMode==5)continue;       //预设作弊只能使用一个,但可与挖掘机共存
                initHp=3999;
                initAtk=99;
                initDef=99;
                initGold=999;
                initY=9;
                initB=9;
                initR=9;
                if(debugMode==3)debugMode=5;
                else debugMode=2;
            }
            else if(stricmp(argv[i],"--tooyoungtoosimple")==0){
                if(debugMode==2||debugMode==5)continue;
                initHp=65535;
                initAtk=1023;
                initDef=1023;
                initGold=32767;
                initY=255;
                initB=255;
                initR=255;
                if(debugMode==3)debugMode=5;
                else debugMode=2;
            }
            else if(stricmp(argv[i],"--overflow")==0){
                if(debugMode==2||debugMode==5)continue;
                initHp=2147483647;
                initAtk=2147483647;
                initDef=2147483647;
                initGold=2147483647;
                initY=2147483647;
                initB=2147483647;
                initR=2147483647;
                if(debugMode==3)debugMode=5;
                else debugMode=2;
            }
            else if(stricmp(argv[i],"--blueshit")==0){
                if(debugMode==2)debugMode=5;
                else debugMode=3;
            }
        }
        if(debugMode==0){    //乱打参数的孩子你好再见
            cout<<"参数错误！"<<endl;
            return ture;
        }
    }                           //命令行参数处理完成

    hTimerMutex=CreateMutex(NULL, FALSE, "screen");
    hInputMutex=CreateMutex(NULL, FALSE, "keyboard");
    hMusicMutex=CreateMutex(NULL, FALSE, "speaker");
    CreateThread(NULL, 0, timer, NULL, 0, NULL);
    int i, j;                   //全局通用临时变量，用于for
    int input;                  //全局通用临时变量，用于getVK();
    int result;                 //存放Player::attMove()的返回值
    int firstrun=1;             //玩家是否是第一次进入下面的永恒轮回
    //读取商店涨价序列
    ifstream infile;
    string priceseq=apppath+"price.dat";
    infile.open(priceseq.c_str());
    infile>>prcIncreMode;
    //input: 1--> 匀速模式，之后读取初始偏移和速度
    //       2--> 匀加速模式，之后读取初始偏移、初速度和加速度
    infile>>prcIncreSpeed;
    if(prcIncreMode==2)infile>>prcIncreAccel;
    else prcIncreAccel = 0;
    infile.close();
    //----------------
    Entity *Ent[11][11];        //当前层对象的指针
    initBoard();                //初始化游戏界面；如果有命令行参数，就覆盖上面的"↓如果这里有输入法信息，请把输入法关掉"
    cout.flush();
    if(!noSound)
        hMusicHandle = CreateThread(NULL, 0, PlayMod, lpBgm, 0, NULL);
    nextLevel:                  //我没办法只能用goto了
    if(Visited[Level]==0)loadLevel(Level);      //若第一次进入本层，则从文件读取本层
    if(firstrun==1&&startLev!=0){   //如果刚进入游戏，而初始关卡非零
                                    //说明使用了命令行参数
                                    //非零关卡是没有玩家初始点的
                                    //所以随便找一个楼梯口作为玩家出现的地方
        int entryPoint=3100;    //这是一个初始值，具体请查看实体及非实体ID表
        int flag=1;             //临时标志，存放是否在关卡中找到了对应当前entryPoint值的标志
        while(flag){
            for(int i=0;i<11;i++){     //to be changed         <-我不记得为什么tobechanged
                for(int j=0;j<11;j++){
                    if(Map[Level][i][j]==entryPoint){
                        flag=0;
                        PlayerPos[0]=i,PlayerPos[1]=j;
                        break;
                    }
                }
            if(!flag)break;
            }
            entryPoint+=10;
        }
    }
    if(firstrun==0){                //如果不是刚刚进入
        Shabi->refreshStatus();     //刷新玩家信息
        int flag=1;
        for(i=0;i<11;i++)
        {
            for(j=0;j<11;j++)
            {
                if(NextLevPlayerPos==Map[Level][i][j]){     //寻找即将进入的层中与楼梯对应的楼梯口
                    PlayerPos[0]=i,PlayerPos[1]=j;
                    Shabi->setPos(PlayerPos[0],PlayerPos[1]);
                    Shabi->show();                          //见程序结尾处 A 行。如果在这里直接使用 Creature::Move()
                                                            //会导致目标楼层上坐标与玩家在前一层所在的坐标重合的地方
                                                            //存在的任何实体或非实体被清零
                                                            //所以拆分成先在改变楼层之前消失
                                                            //然后再此找到目标楼层上的目标坐标后直接设置玩家坐标
                                                            //并显示玩家 以避免任何一层的重合处的任何东西被清除
                    flag=0;
                    if(NextLevPlayerPos==4000)Map[Level][i][j]=0;
                    break;
                }
            }
            if(flag==0)break;
        }
    }
    if(!firstrun)refresh();                         //刷新地图
    if(!Visited[Level]){
        Visited[Level]=1;                           //将本层设为已进入
        for(i=0;i<11;i++)                           //创建本层新对象
        {
            for(j=0;j<11;j++)
            {
                if((const int)Map[Level][i][j]==0){
                    (char*)Occupied[Level][i][j]=0;
                }
                else if(firstrun==1&&(const int)Map[Level][i][j]==998)PlayerPos[0]=i,PlayerPos[1]=j;
                else if((const int)Map[Level][i][j]/100==1)
                {
                    (Blocker*)Ent[i][j]=new Blocker(i,j,(const int)Map[Level][i][j]);
                }
                else if((const int)Map[Level][i][j]/10==20)(Key*)Ent[i][j]=new Key(i,j,(KeyColor)((const int)Map[Level][i][j]-200));
                else if((const int)Map[Level][i][j]/10>20&&(const int)Map[Level][i][j]/10<24)(Adder*)Ent[i][j]=new Adder(i,j,(AdderType)((const int)Map[Level][i][j]-200));
                else if((const int)Map[Level][i][j]/100==3){
                    (Warper*)Ent[i][j]=new Warper(i,j,Map[Level][i][j]);
                }
                else if((const int)Map[Level][i][j]>999&&(const int)Map[Level][i][j]<3000){(Enemy*)Ent[i][j]=new Enemy(i,j,(const int)Map[Level][i][j]);}
                //else if((const int)Map[Level][i][j]>2999&&(const int)Map[Level][i][j]<3200)
                else if((const int)Map[Level][i][j]>3299&&(const int)Map[Level][i][j]<3700){(Npc*)Ent[i][j]=new Npc(i,j,(const int)Map[Level][i][j]);}
                else if((const int)Map[Level][i][j]>3699&&(const int)Map[Level][i][j]<3900){(Merchant*)Ent[i][j]=new Merchant(i,j,(const int)Map[Level][i][j]);}
                else if((const int)Map[Level][i][j]>3899&&(const int)Map[Level][i][j]<3903){(Shop*)Ent[i][j]=new Shop(i,j,(const int)Map[Level][i][j], Level);}
            }
        }
    }
    if(firstrun==1){    //刚进入时建立玩家对象
        Shabi=new Player(PlayerPos[0],PlayerPos[1],initHp,initAtk,initDef,initGold,initY,initB,initR);
        firstrun=0;
    }
    Shabi->show();      //显示玩家对象
    Shabi->refreshStatus();
    while(1)
    {
        ReleaseMutex(hInputMutex);
        input=getVK();
        WaitForSingleObject(hInputMutex, INFINITE);
        switch(input)
        {
            case VK_ESCAPE:
                mesg("确认退出？","      [Y] 确认","      其他任意键取消");
                ReleaseMutex(hInputMutex);
                input=getVK();
                WaitForSingleObject(hInputMutex, INFINITE);
                if(input=='Y'){
                    system("color 07");
                    clrscr();
                    system("color 07");
                    return 0;
                }
                else{
                    mesg("          ","              ","                    ");
                    continue;
                }
            case VK_UP: result=Shabi->attMove(UP); break;
            case VK_DOWN: result=Shabi->attMove(DOWN); break;
            case VK_LEFT: result=Shabi->attMove(LEFT); break;
            case VK_RIGHT: result=Shabi->attMove(RIGHT); break;
            case VK_OEM_2: case VK_TAB: //command interpreter, allow in-game cheating
                WaitForSingleObject(hInputMutex, INFINITE);
                result=activate_ingame_cmd();
                ReleaseMutex(hInputMutex);
                if(result!=998)continue;
                else break;
            case 'S':
                if(noSound==0){
                    noSound=1;
                    ReleaseMutex(hMusicMutex);
                    gotoxy(1,cursorpos+1);
                    cout<<"Muted music.                                                                  ";
                    cout.flush();
                    gotoxy(1,1);
                } else {
                    noSound=0;
                    ReleaseMutex(hMusicMutex);
                    gotoxy(1,cursorpos+1);
                    cout<<"Music started.                                                                ";
                    cout.flush();
                    gotoxy(1,1);
                }
            case 'L':
                gameHelp();
                continue;
            case 'U':      //楼层跳跃，目前的机制不检查玩家是否靠近楼梯口，即玩家在任何地方都可以进行跳跃
                                //此举是为了方便某学霸满足他的夙愿
                                //夙愿，一向怀有的愿望，一个人生前或以前的愿望
                                //把任意地点跳转和楼梯口限制跳转设置不同的code吧。
                if(!Shabi->isSkillOK(FreeFly))continue;
                result=Shabi->floorJump(Level+1); break;
            case 'D':       //楼层跳跃，同上。
                if(!Shabi->isSkillOK(FreeFly))continue;
                result=Shabi->floorJump(Level-1); break;
            default:continue;
        }
        if(result==998){
            Shabi->disappear();                                 // A 先使玩家从现有坐标处消失。见上。
            if(Stat==__Up)Level++;
            else if(Stat==__Down)Level--;
            Stat=__None;
            goto nextLevel;
        } else if (result==999){
            Shabi->disappear();         //楼层跳跃，见上。
            Level=Stat;
            Stat=__None;
            goto nextLevel;
        } else if(result==-1) continue;
        Shabi->refreshStatus();
    }
}




















//==============================================================================
//============================================以下内容我没打多少注释
//====该部分允许玩家在游戏中输入命令，以增加自己的属性值、开启蓝翔模式或做其他蛋
//疼的事情。并模仿linux终端的说话风格，以达到装逼的效果。
//================================目前可用的命令请在最后的 showHelp() 函数中查找


#define cursorxpos 21
inline void flush_cmd()
{
    gotoxy(1,cursorpos);
    cout<<"                                                     \
                                                               "<<endl;
    gotoxy(1,1);
}
inline void flush_input_only(int pos=cursorxpos)
{
    gotoxy(pos,cursorpos);
    cout<<"                                                                               "<<endl;
    gotoxy(1,1);
}
#undef cursorxpos

//==============================================================================
int input_cmd_text(string& ingame_cmd,  bool pw=0, int cursorxpos=21)    //以读取VK为基础的增强型字符串输入函数，编写原因是cin>>太弱。此函数不支持中文。
{
    unsigned int cmd_pos=0;
    int input=0;
    int shifted=0;
    unsigned int history_pos=cmd_history.size()+1;
    gotoxy(cursorxpos, cursorpos);
    while(input!=VK_RETURN&&input!=VK_ESCAPE)
    {
        input=getVK();
        shifted=0x8000 & GetKeyState(VK_SHIFT);
        if(input>=0x60&&input<=0x69)input-=0x30;
        if(input==VK_SPACE)
        {
            ingame_cmd.insert(cmd_pos,1,' ');
            cmd_pos++;
            gotoxy(cursorxpos,cursorpos);
            if(pw==0)cout<<ingame_cmd;
            //else for(int i=0;i<ingame_cmd.length();i++)cout<<'*';
            cout.flush();
            gotoxy(cursorxpos+cmd_pos,cursorpos);
            history_pos=cmd_history.size()+1;
        }
        else if(input>='0'&&input<='9')
        {
            if(shifted){
                char ins=' ';
                switch(input)
                {
                    case '1': ins='!'; break;
                    case '2': ins='@'; break;
                    case '3': ins='#'; break;
                    case '4': ins='$'; break;
                    case '5': ins='%'; break;
                    case '6': ins='^'; break;
                    case '7': ins='&'; break;
                    case '8': ins='*'; break;
                    case '9': ins='('; break;
                    case '0': ins=')'; break;
                }
                ingame_cmd.insert(cmd_pos,1,ins);
            }
            else ingame_cmd.insert(cmd_pos,1,input);
            cmd_pos++;
            gotoxy(cursorxpos,cursorpos);
            if(pw==0)cout<<ingame_cmd;
            //else for(int i=0;i<ingame_cmd.length();i++)cout<<'*';
            cout.flush();
            gotoxy(cursorxpos+cmd_pos,cursorpos);
            history_pos=cmd_history.size()+1;
        }
        else if (input>='A'&&input<='Z')
        {

            if ((GetKeyState(VK_CAPITAL) & 0x0001)!=0||shifted) //caps ON
                ingame_cmd.insert(cmd_pos,1,input);
            else ingame_cmd.insert(cmd_pos,1,input+32);
            cmd_pos++;
            gotoxy(cursorxpos,cursorpos);
            if(pw==0)cout<<ingame_cmd;
            //else for(int i=0;i<ingame_cmd.length();i++)cout<<'*';
            cout.flush();
            gotoxy(cursorxpos+cmd_pos,cursorpos);
            history_pos=cmd_history.size()+1;
        }
        else if (input==VK_UP)
        {
            if(pw==1)continue;
            if(cmd_history.size()==0)continue;
            if(history_pos-1>0)history_pos--;
            flush_input_only();
            gotoxy(cursorxpos,cursorpos);
            cout<<cmd_history[history_pos-1];
            cout.flush();
            ingame_cmd=cmd_history[history_pos-1];
            cmd_pos=ingame_cmd.length();
        }
        else if (input==VK_DOWN)
        {
            if(pw==1)continue;
            if(history_pos>=cmd_history.size())continue;
            history_pos++;
            if(history_pos>cmd_history.size())history_pos--;
            flush_input_only();
            gotoxy(cursorxpos,cursorpos);
            cout<<cmd_history[history_pos-1];
            cout.flush();
            ingame_cmd=cmd_history[history_pos-1];
            cmd_pos=ingame_cmd.length();
        }
        else if(input==VK_LEFT)
        {
            if(pw==1)continue;
            if(cmd_pos==0)continue;
            cmd_pos--;
            gotoxy(cursorxpos+cmd_pos,cursorpos);
        }
        else if(input==VK_RIGHT)
        {
            if(pw==1)continue;
            if(cmd_pos==ingame_cmd.length())continue;
            cmd_pos++;
            gotoxy(cursorxpos+cmd_pos,cursorpos);
        }
        else if(input==VK_DELETE)
        {
            if(pw==1)continue;
            if(cmd_pos==ingame_cmd.length())continue;
            ingame_cmd.erase(cmd_pos, 1);
            flush_input_only();
            gotoxy(cursorxpos,cursorpos);
            if(pw==0)cout<<ingame_cmd;
            //else for(int i=0;i<ingame_cmd.length();i++)cout<<'*';
            cout.flush();
            gotoxy(cursorxpos+cmd_pos,cursorpos);
        }
        else if(input==VK_BACK)
        {
            if(cmd_pos==0)continue;
            ingame_cmd.erase(cmd_pos-1, 1);
            cmd_pos--;
            flush_input_only();
            gotoxy(cursorxpos,cursorpos);
            if(pw==0)cout<<ingame_cmd;
            //else for(int i=0;i<ingame_cmd.length();i++)cout<<'*';
            cout.flush();
            gotoxy(cursorxpos+cmd_pos,cursorpos);
        }
    }
    if(input==VK_ESCAPE||(input==VK_RETURN&&ingame_cmd==""))
    {
        flush_cmd();
        return 0;
    }
    else return 1;
}

//==============================================================================
#define cursorxpos 21
int activate_ingame_cmd(string pre_cmd)
{
    if(shellroot==-1) return 0;
    unsigned int sppos;
    int flush=0;
    string ingame_cmd, tmp_cmd;
    gotoxy(1,cursorpos);
    cout<<"                                                     \
                                                                                          "<<endl;
    gotoxy(1,1);
    gotoxy(1,cursorpos);
    if(shellroot==0)cout<<"[shabi @ mtower ~] $ ";
    else cout<<"[root @ mtower ~] # ";
    cout.flush();
    int input_res=0;
    if(pre_cmd=="")
    {
        input_res=input_cmd_text(ingame_cmd, 0, cursorxpos+1-shellroot);
        if(input_res==0)return 0;
    }
    else ingame_cmd=pre_cmd;
    cmd_history.push_back(ingame_cmd);
    gotoxy(1,1);
    sppos=ingame_cmd.find(' ', 0);
    tmp_cmd=ingame_cmd.substr(0,sppos);
    if(stricmp(tmp_cmd.c_str(),"help")==0){
        gotoxy(1,cursorpos+1);
        showHelp();
        flush=1;
    }
    else if(stricmp(tmp_cmd.c_str(),"pause")==0)
    {
        gotoxy(1,cursorpos+1);
        if(pre_cmd==""){
            cout<<"Only for scripts.";
            cout.flush();
        }
        else {
            string pau[1]={"已暂停。"};
            npcStyleDialog(1, pau);
        }
    }
    else if(stricmp(tmp_cmd.c_str(),"cls")==0)
    {
        mesg(" "," "," ");
        flush=1;
    }

/*------------------------------------------------------------------------------
    //妈的！傻逼！别玩太大了！
    else if(stricmp(tmp_cmd.c_str(),"ls")==0)
    {

    }
    else if(stricmp(tmp_cmd.c_str(),"cd")==0)
    {

    }
*///------------------------------------------------------------------------------
    else if(stricmp(tmp_cmd.c_str(),"license")==0)
    {
        showLicense();
        flush=1;
    }
    // test for timer
    else if(stricmp(tmp_cmd.c_str(),"time")==0)                         ////
    {                                                                   ////
        WaitForSingleObject(hTimerMutex, INFINITE);                     ////
        gotoxy(1,cursorpos+1);                                          ////
        cout<<globalTime<<" seconds since program began to run.";       ////
        cout.flush();                                                   ////
        ReleaseMutex(hTimerMutex);                                      ////
    }                                                                   ////
    // ==============

    // test for multithreading, timers and function pointers
    else if(stricmp(tmp_cmd.c_str(),"exittime")==0)                     ////
    {                                                                   ////
        int tmppos = sppos;                                             ////
        sppos=ingame_cmd.find(' ', sppos+1);                            ////
        tmp_cmd=ingame_cmd.substr(tmppos+1,sppos);                      ////
        int countdown = atoi(tmp_cmd.c_str());                          ////
        if(countdown >= 5) ((tmr_para*)pexit)->secs = countdown;        ////
        CreateThread(NULL, 0, doAfter, pexit, 0, NULL);                 ////
        gotoxy(1,cursorpos+1);                                          ////
        cout<<"Set exit timer for "<<(((tmr_para*)pexit)->secs)<<" seconds.";
        cout.flush();
    }                                                                   ////
    // ======================================================

    else if(stricmp(tmp_cmd.c_str(),"mute")==0)
    {
        WaitForSingleObject(hMusicMutex, INFINITE);
        if(noSound){
            ReleaseMutex(hMusicMutex);
            gotoxy(1,cursorpos+1);
            cout<<"Already muted.";
            cout.flush();
        } else {
            noSound=1;
            ReleaseMutex(hMusicMutex);
            gotoxy(1,cursorpos+1);
            cout<<"Muted.";
        }
    }

    else if(stricmp(tmp_cmd.c_str(),"music")==0)
    {
        //system("pause");
        WaitForSingleObject(hMusicMutex, INFINITE);
        //system("pause");
        if(!noSound){
            ReleaseMutex(hMusicMutex);
            gotoxy(1,cursorpos+1);
            cout<<"Already playing.";
            cout.flush();
        } else {
            //system("pause");
            noSound=0;
            ReleaseMutex(hMusicMutex);
            gotoxy(1,cursorpos+1);
            cout<<"Music started.";
        }
    }

    else if(stricmp(tmp_cmd.c_str(),"msg")==0)
    {
        mesg(" "," "," ");
        int tmppos = sppos;
        sppos=ingame_cmd.find(' ', sppos+1);
        tmp_cmd=ingame_cmd.substr(tmppos+1,sppos);
        if(stricmp(tmp_cmd.c_str(), "on")==0)
        {
            doShowMsg=1;
            gotoxy(1,cursorpos+1);
            cout<<"Message set to ON.";
            cout.flush();
        }
        else if(stricmp(tmp_cmd.c_str(), "off")==0)
        {
            doShowMsg=0;
            gotoxy(1,cursorpos+1);
            cout<<"Message set to OFF.";
            cout.flush();
        }
        else flush=1;
    }
    else if(stricmp(tmp_cmd.c_str(),"rootsh")==0)
    {
        if(shellroot==0)
        {
            gotoxy(1,cursorpos);
            cout<<"password:                                                             ";
            gotoxy(1,1);
            cout.flush();
            string pw;
            input_cmd_text(pw, 1, 11);
            gotoxy(1,1);
            if(pw=="admin")
            {
                shellroot=1;
                return activate_ingame_cmd();
            }
            else if(pw=="")shellroot=0;
            else shellroot=-1;
            flush=1;
        }
        flush=1;
    }
    else if(stricmp(tmp_cmd.c_str(),"exit")==0){
        exit(1);
    }
    else if(stricmp(tmp_cmd.c_str(),"refresh")==0){
        refresh();
    }
    else if(stricmp(tmp_cmd.c_str(),"enemydata")==0){
        Shabi->showEnemyData(Level);
    }
    //if ((GetKeyState(VK_CAPITAL) & 0x0001)!=0) Caps Lock ON!
    else if(stricmp(tmp_cmd.c_str(),"blueshit")==0){
        if(shellroot==0)
        {
            gotoxy(1,cursorpos+1);
            cout<<"请使用root权限执行本操作。";
            cout.flush();
        }
        else if(shellroot==1)
        {
            string rep[4]={"    将开启蓝翔模式，只对本层及未访问","过的楼层生效，且本层将回复为进入前的","样子，不保留玩家所作的任何改变。","是否确认？"};
            int bsres=0;
            bsres=npcStyleYesNoDialog(4, rep);
            if(bsres==1)
            {
                if(debugMode==2)debugMode=5;
                else debugMode=3;
                Visited[Level]=0;
                if(Level==0)NextLevPlayerPos=998;
                gotoxy(1,1);
                return 998;
            }
            flush=1;
        }
    }
    else if(stricmp(tmp_cmd.c_str(),"watermeterchecker")==0){
        if(shellroot==0)
        {
            gotoxy(1,cursorpos+1);
            cout<<"请使用root权限执行本操作。";
            cout.flush();
        }
        else if(shellroot==1)
        {
            string rep[3]={"    开门，查水表！","（挖掘机一组，准备！）","是否确认？"};
            int bsres=0;
            bsres=npcStyleYesNoDialog(3, rep);
            if(bsres==1)
            {
                if(debugMode==2)debugMode=5;
                else debugMode=3;
                Visited[Level]=0;
                if(Level==0)NextLevPlayerPos=998;
                Shabi->setDirectly(__YKey, 998);
                Shabi->setDirectly(__BKey, 998);
                Shabi->setDirectly(__RKey, 998);
                gotoxy(1,1);
                return 998;
            }
            flush=1;
        }
    }
    else if(stricmp(tmp_cmd.c_str(),"whosyourdaddy")==0){
        if(shellroot==0)
        {
            gotoxy(1,cursorpos+1);
            cout<<"请使用root权限执行本操作。";
            cout.flush();
        }
        else if(shellroot==1 && yourDaddy == 0)
        {
            yourDaddy=1;
            gotoxy(1, cursorpos+1);
            cout<<"I am.";
            cout.flush();
        }
        else if(shellroot==1 && yourDaddy == 1)
        {
            yourDaddy=0;
            gotoxy(1, cursorpos+1);
            cout<<"Cheat code cancelled.";
            cout.flush();
        }
    }
    else if(stricmp(tmp_cmd.c_str(),"give")==0)
    {
        int tmppos = sppos;
        sppos=ingame_cmd.find(' ', sppos+1);
        tmp_cmd=ingame_cmd.substr(tmppos+1,sppos-tmppos-1);
        int give_amo;
        tmppos = sppos;
        sppos=ingame_cmd.find(' ', sppos+1);
        give_amo=atoi (ingame_cmd.substr (tmppos+1,sppos-tmppos-1).c_str());
        if(give_amo>32767)give_amo=32767;
        if(tmp_cmd.length()>=5 && give_amo>99)
            give_amo=99;
        gotoxy(1,cursorpos+1);
        if(stricmp(tmp_cmd.c_str(), "hp")==0)
        {
            Shabi->addDirectly(__Hp, give_amo);
            cout<<"Given "<<give_amo<<" HP to player.";
        }
        else if(stricmp(tmp_cmd.c_str(), "atk")==0)
        {
            Shabi->addDirectly(__Atk, give_amo);
            cout<<"Given "<<give_amo<<" ATK to player.";
        }
        else if(stricmp(tmp_cmd.c_str(), "def")==0)
        {
            Shabi->addDirectly(__Def, give_amo);
            cout<<"Given "<<give_amo<<" DEF to player.";
        }
        else if(stricmp(tmp_cmd.c_str(), "gold")==0)
        {
            Shabi->addDirectly(__Gold, give_amo);
            cout<<"Given "<<give_amo<<" GOLD to player.";
        }
        else if(stricmp(tmp_cmd.c_str(), "yellowkey")==0)
        {
            Shabi->addDirectly(__YKey, give_amo);
            cout<<"Given "<<give_amo<<" yellow "<<(give_amo==1?"key":"keys")<<" to player.";
        }
        else if(stricmp(tmp_cmd.c_str(), "bluekey")==0)
        {
            Shabi->addDirectly(__BKey, give_amo);
            cout<<"Given "<<give_amo<<" blue "<<(give_amo==1?"key":"keys")<<" to player.";
        }
        else if(stricmp(tmp_cmd.c_str(), "redkey")==0)
        {
            Shabi->addDirectly(__RKey, give_amo);
            cout<<"Given "<<give_amo<<" red "<<(give_amo==1?"key":"keys")<<" to player.";
        }
        else cout<<"Specified attribute does not exist.";
        cout.flush();
        Shabi->refreshStatus();
    }
    else {
        gotoxy(1,cursorpos+1);
        cout<<"mtower: "<<tmp_cmd<<": command not found";
        cout.flush();
    }
    if(flush==1)flush_cmd();
    gotoxy(1,1);
    Shabi->refreshStatus();
    return 1;
}
#undef cursorpos
#undef cursorxpos

void showLicense()
{
    int response = MessageBox(
        NULL,
        (LPCTSTR)"\
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE\n\
                       Version 2, December 2004\n\
\n\
 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>\n\
\n\
 Everyone is permitted to copy and distribute verbatim or modified\n\
 copies of this license document, and changing it is allowed as long\n\
 as the name is changed.\n\
\n\
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE\n\
TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION \n\
\n\
    0. You just DO WHAT THE FUCK YOU WANT TO.\n\n\n\
WARNING: Independent from the program itself, the background music is property of its owner.\n\
",
        (LPCTSTR)"License",
        MB_APPLMODAL |MB_TOPMOST| MB_YESNO | MB_DEFBUTTON2
    );
    if( response != IDYES ) exit(0);
}

void showHelp()
{
    MessageBox(
        NULL,
        (LPCTSTR)"\
Available commands are listed below:\n\n\
blueshit  :  Enable Excavator Mode. \n\
cls  :  Clear the game message text field\n\
        (NOT this console output field) \n\
exit  :  Quit the program. \n\
exittime <time>  :  Set an exit timer in seconds. \n\
give <type> <amount>  :  Give a specific item to player. \n\
help  :  Show this help dialog. \n\
license  :  Show License Agreement dialog.\n\
        You can then choose to decline the license and exit. \n\
msg <on|off>  :  Toggle the game message text output. \n\
music  :  Start to play background music. \n\
mute  :  Stop playing background music. \n\
pause  :  Not availble for now. \n\
rootsh  :  Lift your priviliges. \n\
time  :  Show how many seconds has passed\n\
        since the program launched. \n\
watermeterchecker  :  Enable Excavator Mode\n\
        and give you infinite Keys. \n\
whosyourdaddy  :  Make you capable for\n\
        killing any enemy instantly.\
",
        (LPCTSTR)"Help",
        MB_APPLMODAL |MB_TOPMOST| MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION
    );
}

void gameHelp(int index)
{
    string helpText="";
    //get help text from player info
    ifstream fin;
    fin.open("helptext.dat");
    string tmp;
    int ind;
    int j=0;
    int f=0;
    if(index==-1){
        for(j=0; j<20; j++) {
            if(Shabi->isSkillOK(j))
            {
                fin>>ind;
                while(ind!=j)fin>>tmp>>ind;
                fin>>tmp;
                helpText = helpText + tmp;
                f=1;
            }
        }
        if(f==0)helpText = "There's nothing to tell you!";
    } else {
        fin>>ind;
        while(ind!=index)fin>>tmp>>ind;
        fin>>tmp;
        helpText=tmp;
    }
    MessageBox(
        NULL,
        (LPCTSTR)helpText.c_str(),
        (LPCTSTR)"Help",
        MB_APPLMODAL |MB_TOPMOST| MB_OK | MB_DEFBUTTON1 | MB_ICONINFORMATION
    );
}
