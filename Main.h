#include <conio.h>
#include <cstdlib>
#include <iostream>
#include <Windows.h>
#include <sstream.h>
#include <fstream>
#include <iomanip>

#include "MagicTowerDll.h"
#include "Definitions.h"
#include "Entities.h"
#include "Variable.h"
extern int prcIncreMode;
//int prcIncreOffset;
extern int prcIncreSpeed;
extern int prcIncreAccel;
string intStr(int in);    // int to string
extern int doShowMsg;     // �Ƿ���ʾ��Ϣ�����½������֣�
extern void* Occupied[55][11][11];   //ָ������ָ�루������ʵ�壩
extern int Map[55][11][11];          //�洢��ͼÿһ����ʵ����ǵ�ID����Ϊ�յ���Ϊ0��
                                    //�����������������洢ʵ�ʴ��ڵ���ҵ���Ϣ��Map[0][][]����һ������ҳ����㡱����Ҳ����ʵ�ʵ���ң�
                                    //Ҳ�� ���ֻ�ڿյؼ��ƶ�ʱ���������ⲻ��仯
extern string arguments;  //�����в������Ҳ�̫ȷ��Ϊʲô��Ҳ��extern
extern int PlayerPos[2];  //�������
extern int Level;         //��ǰ�ؿ���
extern int debugMode;     //main.cpp����͹�
extern int yourDaddy;     //�뿴�������������б����һ��
extern int Visited[55];
extern int Stat;      //function.cpp����͹�����ţ�
extern int NextLevPlayerPos;   //Ҫ�����һ������ҽ����λ�ã��������꣬���ǵ�ͼ�ϵ�һ������ض��ı�־
extern string apppath;    //������
void initBoard();         //��ʼ������
void loadLevel(int level);//���ļ���ȡ�ؿ�
const string getLevelFile(int level);//��ȡ�ؿ��ļ�·��
void mesg(string msg,string msg1,string msg2);///���½�������
void refresh();           //�����³�ʼ������������ˢ�µ�ͼ����
void npcStyleDialog(int lines, string cont[]); //��ʾ�Ի���
int npcStyleYesNoDialog(int lines, string cont[]);  //��ʾ����ѡ���ܵĶԻ���
int getMaxLevel();
void gameHelp(int index=-1);
extern long globalTime;  //ȫ��ʱ��
extern HANDLE hTimerMutex, hInputMutex, hMusicMutex;
extern int noSound;     //main����͹�
DWORD WINAPI timer(LPVOID Param);  //����Ǽ�ʱ��������
struct parameters_for_timer{
public:
    int secs;
    void (*pFunction) (void*);
    void* pParameter;
};
typedef parameters_for_timer tmr_para;
DWORD WINAPI doAfter(LPVOID param);
DWORD WINAPI PlayMod(void* argv);    //��bgm�������ӿ�������ﳭ�ġ�һ��ʼ�ŵ���module�����ļ�������������
