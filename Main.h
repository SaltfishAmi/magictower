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
extern int doShowMsg;     // 是否显示信息（右下角三行字）
extern void* Occupied[55][11][11];   //指向对象的指针（仅包括实体）
extern int Map[55][11][11];          //存储地图每一格中实体或标记的ID，若为空地则为0。
                                    //以上两个玩意均不会存储实际存在的玩家的信息（Map[0][][]中有一个“玩家出生点”，但也不是实际的玩家）
                                    //也即 玩家只在空地间移动时这两个玩意不会变化
extern string arguments;  //命令行参数？我不太确定为什么它也是extern
extern int PlayerPos[2];  //玩家坐标
extern int Level;         //当前关卡号
extern int debugMode;     //main.cpp里解释过
extern int yourDaddy;     //请看最新作弊命令列表最后一条
extern int Visited[55];
extern int Stat;      //function.cpp里解释过（大概？
extern int NextLevPlayerPos;   //要进入的一层里玩家进入的位置，不是坐标，而是地图上的一个编号特定的标志
extern string apppath;    //如其名
void initBoard();         //初始化界面
void loadLevel(int level);//从文件读取关卡
const string getLevelFile(int level);//获取关卡文件路径
void mesg(string msg,string msg1,string msg2);///右下角那三行
void refresh();           //不重新初始化界面的情况下刷新地图内容
void npcStyleDialog(int lines, string cont[]); //显示对话框
int npcStyleYesNoDialog(int lines, string cont[]);  //显示带有选择功能的对话框
int getMaxLevel();
void gameHelp(int index=-1);
extern long globalTime;  //全局时间
extern HANDLE hTimerMutex, hInputMutex, hMusicMutex;
extern int noSound;     //main里解释过
DWORD WINAPI timer(LPVOID Param);  //大概是计时器函数？
struct parameters_for_timer{
public:
    int secs;
    void (*pFunction) (void*);
    void* pParameter;
};
typedef parameters_for_timer tmr_para;
DWORD WINAPI doAfter(LPVOID param);
DWORD WINAPI PlayMod(void* argv);    //放bgm函数，从库的例程里抄的。一开始放的是module音乐文件所以起的这个名
