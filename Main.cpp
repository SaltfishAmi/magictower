//TODO: �����ֲ� METRIC: 10
//TODO: ��NPC��ü��� METRIC: 10
//      seemed complete
//TODO: ��Ҫ��¥�ݿڲ��ܷ�����¥����Ծ METRIC: 9
//TODO: ħ����������ʦ�ĵ� METRIC: 8
//TODO: ��ʯ�Ļظ�����¥��߶ȶ��仯 METRIC: 8
//TODO: �ҽ� METRIC: 5
//TODO: ���̼߳�ʱ����д��ս�ӳ� METRIC: 1
#include "Main.h"
#include <vector>
#define cursorpos 23
void* Occupied[55][11][11]={0};     //������ж����ָ��
int Map[55][11][11]={0};            //������ж����ʵ��ID
int Visited[55]={0};                //����Ƿ�������ĳ��
int PlayerPos[2]={10,10};           //�������
int startLev=0;                     //��ʼ��
int Level=0;                        //��ǰ��
int NextLevPlayerPos;               //�����ǰ��������ʱ
                                    //���Ӧ�ó�������һ���
                                    //��һ�ű��
                                    //���Ϊ¥���Ե�һ��IDΪ¥��IDx10�ķ�ʵ��
int initHp=1000, initAtk=10, initDef=10, initGold=0;
                                    //Ĭ�ϳ�ʼ״̬
int initY=0, initB=0, initR=0;      //Ĭ�ϳ�ʼ״̬��Կ������
int yourDaddy=0;
int debugMode=0;                    //�Ƿ�ʹ�������в�����0Ϊ��1Ϊ����ģʽ��2ΪԤ������ģʽ��3Ϊ�ھ������ģʽ��5ΪԤ������+�ھ��
string arguments="";                //�����в���
string apppath;                     //����·��
int doShowMsg=1;
int Stat=0;                         //�� Function.cpp �б����ģ�     -1=d 0=n 1=up
                                    //Ȼ���ڱ��ļ��б�����
                                    //��ǵ�ǰʱ������Ƿ����¥��
                                    //**Ŀǰ�û��Ʋ��ʺ����¥����Ծ
                                    //**��Ľ�  //���޸�Ϊ�ʺ�¥����Ծ ������������ȷ�Ľ�ַ���
Player* Shabi;                      //��Ҷ����ָ��
int prcIncreMode;
//int prcIncreOffset;
int prcIncreSpeed;
int prcIncreAccel;
int activate_ingame_cmd(string pre_cmd="");   //��Ϸ�����׿���̨�ĺ�������ֻ��main.cpp��ʹ�ã����ڴ�����
int shellroot=0;                    //��ʾ��Ϸ�����׿���̨�Ƿ����������Ȩ�ޣ���linuxװ�ƣ�

void showLicense();                 //��ʾ���Э��
void showHelp();                    //��ʾ�����а���
vector<string> cmd_history;         //��ʷ�������������
int noSound=1;                      //�Ƿ���ֹͣ����
char* bgm="mres.dll";               //���������ļ���
LPVOID lpBgm = bgm;
HANDLE hTimerMutex, hInputMutex, hMusicMutex;
HANDLE hMusicHandle;                //��Щ�����Ҷ���̫���ã��������Google + Trial-and-error ��������
HANDLE hOut;
using namespace std;

// test for passing function pointers
tmr_para exittimer;                                                     ////
void* pexit;                                                            ////
void ami_vexit(void* exitcode)   //���Դ��ݺ���ָ��                     ////
{                                                                       ////
    cout<<"function ami_vexit() has been called"<<endl;                 ////
    exit(0);                                                            ////
}                                                                       ////
//===================================

int main(int argc, char* argv[])
{
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    //    system("pause");
    // ���Ժ���ָ�����ڽṹ�� tmr_para �д���
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
            cout<<"�÷�: MagicTower.exe <ѡ��1> <����1> <ѡ��2> <����2> ......"<<endl<<endl;
            cout<<"    û�в���    ����������Ϸ��"<<endl;
            cout<<"    /?          ��ʾ��ǰ������"<<endl<<endl;    //�����help����Ϸ���������help��̫һ��
            cout<<"    ����ģʽ��ѡ���һ�����֪����������̫Naive��"<<endl;
            return 0;
        }
        for(int i=1;i<argc;i++)             //�Ҹ�����i�����ӷ������Ի�ø�����ݴ��ԡ���δ���ԡ�
        {
            arguments=arguments+string(argv[i])+" ";
            if(stricmp(argv[i],"--nobgm")==0)
                noSound=1;
            else if(stricmp(argv[i],"--license")==0)
                showLicense();
            else if(stricmp(argv[i],"--level")==0){
                if(debugMode>=2)continue;     //����⵽Ԥ�����׻��ھ��ģʽ������Ե��Բ���
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
                if(debugMode==2||debugMode==5)continue;       //Ԥ������ֻ��ʹ��һ��,�������ھ������
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
        if(debugMode==0){    //�Ҵ�����ĺ�������ټ�
            cout<<"��������"<<endl;
            return ture;
        }
    }                           //�����в����������

    hTimerMutex=CreateMutex(NULL, FALSE, "screen");
    hInputMutex=CreateMutex(NULL, FALSE, "keyboard");
    hMusicMutex=CreateMutex(NULL, FALSE, "speaker");
    CreateThread(NULL, 0, timer, NULL, 0, NULL);
    int i, j;                   //ȫ��ͨ����ʱ����������for
    int input;                  //ȫ��ͨ����ʱ����������getVK();
    int result;                 //���Player::attMove()�ķ���ֵ
    int firstrun=1;             //����Ƿ��ǵ�һ�ν�������������ֻ�
    //��ȡ�̵��Ǽ�����
    ifstream infile;
    string priceseq=apppath+"price.dat";
    infile.open(priceseq.c_str());
    infile>>prcIncreMode;
    //input: 1--> ����ģʽ��֮���ȡ��ʼƫ�ƺ��ٶ�
    //       2--> �ȼ���ģʽ��֮���ȡ��ʼƫ�ơ����ٶȺͼ��ٶ�
    infile>>prcIncreSpeed;
    if(prcIncreMode==2)infile>>prcIncreAccel;
    else prcIncreAccel = 0;
    infile.close();
    //----------------
    Entity *Ent[11][11];        //��ǰ������ָ��
    initBoard();                //��ʼ����Ϸ���棻����������в������͸��������"��������������뷨��Ϣ��������뷨�ص�"
    cout.flush();
    if(!noSound)
        hMusicHandle = CreateThread(NULL, 0, PlayMod, lpBgm, 0, NULL);
    nextLevel:                  //��û�취ֻ����goto��
    if(Visited[Level]==0)loadLevel(Level);      //����һ�ν��뱾�㣬����ļ���ȡ����
    if(firstrun==1&&startLev!=0){   //����ս�����Ϸ������ʼ�ؿ�����
                                    //˵��ʹ���������в���
                                    //����ؿ���û����ҳ�ʼ���
                                    //���������һ��¥�ݿ���Ϊ��ҳ��ֵĵط�
        int entryPoint=3100;    //����һ����ʼֵ��������鿴ʵ�弰��ʵ��ID��
        int flag=1;             //��ʱ��־������Ƿ��ڹؿ����ҵ��˶�Ӧ��ǰentryPointֵ�ı�־
        while(flag){
            for(int i=0;i<11;i++){     //to be changed         <-�Ҳ��ǵ�Ϊʲôtobechanged
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
    if(firstrun==0){                //������Ǹոս���
        Shabi->refreshStatus();     //ˢ�������Ϣ
        int flag=1;
        for(i=0;i<11;i++)
        {
            for(j=0;j<11;j++)
            {
                if(NextLevPlayerPos==Map[Level][i][j]){     //Ѱ�Ҽ�������Ĳ�����¥�ݶ�Ӧ��¥�ݿ�
                    PlayerPos[0]=i,PlayerPos[1]=j;
                    Shabi->setPos(PlayerPos[0],PlayerPos[1]);
                    Shabi->show();                          //�������β�� A �С����������ֱ��ʹ�� Creature::Move()
                                                            //�ᵼ��Ŀ��¥���������������ǰһ�����ڵ������غϵĵط�
                                                            //���ڵ��κ�ʵ����ʵ�屻����
                                                            //���Բ�ֳ����ڸı�¥��֮ǰ��ʧ
                                                            //Ȼ���ٴ��ҵ�Ŀ��¥���ϵ�Ŀ�������ֱ�������������
                                                            //����ʾ��� �Ա����κ�һ����غϴ����κζ��������
                    flag=0;
                    if(NextLevPlayerPos==4000)Map[Level][i][j]=0;
                    break;
                }
            }
            if(flag==0)break;
        }
    }
    if(!firstrun)refresh();                         //ˢ�µ�ͼ
    if(!Visited[Level]){
        Visited[Level]=1;                           //��������Ϊ�ѽ���
        for(i=0;i<11;i++)                           //���������¶���
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
    if(firstrun==1){    //�ս���ʱ������Ҷ���
        Shabi=new Player(PlayerPos[0],PlayerPos[1],initHp,initAtk,initDef,initGold,initY,initB,initR);
        firstrun=0;
    }
    Shabi->show();      //��ʾ��Ҷ���
    Shabi->refreshStatus();
    while(1)
    {
        ReleaseMutex(hInputMutex);
        input=getVK();
        WaitForSingleObject(hInputMutex, INFINITE);
        switch(input)
        {
            case VK_ESCAPE:
                mesg("ȷ���˳���","      [Y] ȷ��","      ���������ȡ��");
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
            case 'U':      //¥����Ծ��Ŀǰ�Ļ��Ʋ��������Ƿ񿿽�¥�ݿڣ���������κεط������Խ�����Ծ
                                //�˾���Ϊ�˷���ĳѧ������������Ը
                                //��Ը��һ���е�Ը����һ������ǰ����ǰ��Ը��
                                //������ص���ת��¥�ݿ�������ת���ò�ͬ��code�ɡ�
                if(!Shabi->isSkillOK(FreeFly))continue;
                result=Shabi->floorJump(Level+1); break;
            case 'D':       //¥����Ծ��ͬ�ϡ�
                if(!Shabi->isSkillOK(FreeFly))continue;
                result=Shabi->floorJump(Level-1); break;
            default:continue;
        }
        if(result==998){
            Shabi->disappear();                                 // A ��ʹ��Ҵ��������괦��ʧ�����ϡ�
            if(Stat==__Up)Level++;
            else if(Stat==__Down)Level--;
            Stat=__None;
            goto nextLevel;
        } else if (result==999){
            Shabi->disappear();         //¥����Ծ�����ϡ�
            Level=Stat;
            Stat=__None;
            goto nextLevel;
        } else if(result==-1) continue;
        Shabi->refreshStatus();
    }
}




















//==============================================================================
//============================================����������û�����ע��
//====�ò��������������Ϸ����������������Լ�������ֵ����������ģʽ����������
//�۵����顣��ģ��linux�ն˵�˵������Դﵽװ�Ƶ�Ч����
//================================Ŀǰ���õ������������� showHelp() �����в���


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
int input_cmd_text(string& ingame_cmd,  bool pw=0, int cursorxpos=21)    //�Զ�ȡVKΪ��������ǿ���ַ������뺯������дԭ����cin>>̫�����˺�����֧�����ġ�
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
            string pau[1]={"����ͣ��"};
            npcStyleDialog(1, pau);
        }
    }
    else if(stricmp(tmp_cmd.c_str(),"cls")==0)
    {
        mesg(" "," "," ");
        flush=1;
    }

/*------------------------------------------------------------------------------
    //��ģ�ɵ�ƣ�����̫���ˣ�
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
            cout<<"��ʹ��rootȨ��ִ�б�������";
            cout.flush();
        }
        else if(shellroot==1)
        {
            string rep[4]={"    ����������ģʽ��ֻ�Ա��㼰δ����","����¥����Ч���ұ��㽫�ظ�Ϊ����ǰ��","���ӣ�����������������κθı䡣","�Ƿ�ȷ�ϣ�"};
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
            cout<<"��ʹ��rootȨ��ִ�б�������";
            cout.flush();
        }
        else if(shellroot==1)
        {
            string rep[3]={"    ���ţ���ˮ��","���ھ��һ�飬׼������","�Ƿ�ȷ�ϣ�"};
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
            cout<<"��ʹ��rootȨ��ִ�б�������";
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
