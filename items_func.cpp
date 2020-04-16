#include "main.h"


bool checkIfOccupied(int lev, int x, int y){
    if(!Visited[lev])loadLevel(lev);
    switch(Map[lev][x][y]){
        case 998: case 0:
        case 3000:
        case 3010:
        case 3020:
        case 3030:
        case 3040:
        case 3050:
        case 3060:
        case 3070:
        case 3080:
        case 3090:
        case 3100:
        case 3110:
        case 3120:
        case 3130:
        case 3140:
        case 3150:
        case 3160:
        case 3170:
        case 3180:
        case 3190:
            return false;
        default: return true;
    }
}

int Player::floorJump(int dest) //FREE fly, only for testing. Please write a new one for normal flying
{
    if(dest<0){
        mesg("已经没有比这里更低的地方了。","还不赶紧使劲往上爬，傻逼。","我这是为你好。");
        return 0;
    } else if(dest>getMaxLevel()){
        mesg("已经没有比这里更高的地方了。","记住，爬得越高，摔得越惨。","我这是为你好。");
        return 0;
    } else if(checkIfOccupied(dest, X, Y)){
        mesg("目标楼层本位置已有东西占据。","一定要去的话，你会被憋死，","我这是为你好。");
        return 0;
    } else {
        Stat = dest;
        NextLevPlayerPos=4000;
        Map[dest][X][Y]=4000;
        return 999;
    }
}

int drawEnemyDataUI(Creature data[], int number)
{
    int page=0;
    int maxpage = (number-1)/10;
    textbackground(BLACK);
    int i,j;
    for(i=1,j=1;j<23;(i==44)?(i=1,j++):i++)
    {
        gotoxy(i,j);
        cprintf(" ");
    }
    textbackground(LIGHTBLUE);
    gotoxy(1,22);
    if(maxpage==0)
        cprintf("                               按任意键退出 ");
    else if(page==0)
        cprintf("               →下一页      其他任意键退出 ");
    else if(page==maxpage)
        cprintf(" ←上一页                    其他任意键退出 ");
    else
        cprintf(" ←上一页      →下一页      其他任意键退出 ");
    int tmpin;
    while(1){
        tmpin=getVK();
        if(maxpage==0)break;
        else if(tmpin!=VK_LEFT&&tmpin!=VK_RIGHT)break;
        else if(tmpin==VK_LEFT){
            if(page==0)break;
            //prev page
        }
        else if(tmpin==VK_RIGHT){
            if(page==maxpage)break;
            //next page
        }
    }
    refresh();
}

int Player::showEnemyData(int floor){
    //traverse map to get all monster types
    //display mode vary with monster type number
    //query and display
    //exit
    int i, j;
    int k = 0, m = 0;
    int f;
    Creature monsters[100];
    for(i=0; i<11; i++){
        for(j=0; j<11; j++){
            if(Map[Level][i][j]/1000==1||Map[Level][i][j]/1000==2){ // it is a monster
                f=0;
                while(m<k){
                    if(monsters[m].Type == Map[Level][i][j]){
                        f=1;
                        break;
                    }
                    m++;
                }
                if(f==0){
                    monsters[m].Type = Map[Level][i][j];
                    k++;
                }
            }
        }
    } //traverse complete
    return drawEnemyDataUI(monsters, k); //
}
