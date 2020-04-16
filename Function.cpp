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
void mesg(string msg=" ",string msg1=" ",string msg2=" ")   //�ڽ�������½Ǻ�ɫ����ʾ��Ϣ������
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
const string getLevelFile(int level)    //��ȡ�ؿ��ļ�·��
{
    string tmp=apppath;
    tmp+="levels\\level";
    tmp+=intStr(level);
    tmp+=".dat";
    return tmp;
}

void Entity::getName(string&n1, string&n2)      //�������ļ��л�ȡʵ�����ʾ����
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
            n1="����";
            if(Type<3500)n2="����";    //3300-3499
            else if(Type<3900)n2="С��"; //3500-3699 //3700-3899 -200 //npcͳһ����
            //3700-3899 are merchants while others aren't.
            //only women merchants here
        }
        else if(Type>3899){             //shop
            switch(Type)
            {
                case 3900:
                    n1="ħ��";
                    n2="��Ѫ";
                    break;
                case 3901:
                    n1="�ӵ�";
                    n2="��";
                    break;
                case 3902:
                    n1="�̵�";
                    n2="���";
            }
        }   //when used, put together to get a "ħ���ӵ��̵�\n����Ѫ���".
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
            n1="û��";
            n2="����";
            return;
        }
        infile>>skip1>>skip2>>tmp;
    }
    infile>>n1>>n2;
    infile.close();
    return;
}
void Entity::show()         //��ʾʵ�壬�����Ͳ�ͬ��ȡ��ͬ��ǰ��ɫ�ͱ���ɫ
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
            Occupied[Level][X][Y]=this;     //����ǽĬ�ϲ���ʾ�������Լ���Ӧ��ȫ��ָ��ָ���Լ���return
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
else if(Type>399&&Type<500)         //����ID��
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type>499&&Type<600)         //����ID��
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type>599&&Type<998)         //����ID��
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
else if(Type>999&&Type<3000)        //����
{
    textbackground(GREEN);
    textcolor(WHITE);
}
else if(Type>2999&&Type<3300)       //Ϊ¥�ݿڱ���������¥�ݿ����ʵ��Ĵ���ʽ��ֻ�洢ID������������
                                    //�ʴ˶�������
{
    Occupied[Level][X][Y]=this;
    gotoxy(1,1);
    return;
}
else if(Type>=3300)                 //NPC���Ժ�
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
Entity::Entity(int x, int y, int type)      //ȫ��ʵ��Ĺ��캯��
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

void Entity::disappear()            //ʵ���ĳһ������ʧ
{
    Occupied[Level][X][Y]=0;
    if(Type!=PLAYER)Map[Level][X][Y]=0;     //ʵ�ʴ��ڵ���Ҳ���������ʵ��ID����
                                            //Ҳ����������Map[Level][X][Y]��
                                            //��������ߵ�¥�ݿ�ʱ��ʧ
                                            //�˴������ų���� ��ᵼ��¥�ݿڱ����ʧ
    textbackground(LIGHTGRAY);
    gotoxy(POS); cprintf("    ");
    gotoxy(POS+1);  cprintf("    ");
//    gotoxy(DEFAULTPOS);
}

int Player::attMove(Direction Dir)          //���**��ͼ**�ƶ��ĺ���������ĺ���
{
    int destX=X, destY=Y;                   //��ͼ�ƶ���Ŀ�ĵ����꣬�ȳ�ʼ��Ϊ���δ�ƶ�������
    switch(Dir)                             //�ɷ���ȷ��Ŀ�ĵ�����
                                            //**Ŀǰ�û��Ʋ������������ɵ����ĶԳƵ㡱��50��ħ����
                                            //**����ĵ��ߡ���Ľ�
    {
        case UP: destY--;break;
        case DOWN: destY++;break;
        case LEFT: destX--;break;
        case RIGHT: destX++;break;}
    if(destX<0||destX>10||destY<0||destY>10)return 0;       //������ͼ��Χ��ײǽ��
    if(!Occupied[Level][destX][destY])      //��Ŀ��ص�δ��ռ�ݣ���ֱ���ƶ�
    {
        Move(destX,destY);
        return 1;
    }
    int nextType=((Entity*)Occupied[Level][destX][destY])->getType();   //���ѱ�ռ�ݣ����ȡռ�ݸ�λ�õ�ʵ��ID
                                                                        //��Entity::getType()����ֱ�Ӵ�Map[][][]ȡ����
                                                                        //��֤��¥�ݿڱ�־������Ϊ�ϰ���
    if(nextType==100){          //ֱ��ײǽ
        mesg("ײ����ͷ��ʹ��");
        return 0;
    }
    else if(nextType==101)      //����ǽ
    {
        ((Blocker*)Occupied[Level][destX][destY])->setType(100);
        Map[Level][destX][destY]=100;
        ((Blocker*)Occupied[Level][destX][destY])->show();
        mesg("�벻���ɣ��������������","���⡣");
        return 0;
    }
    else if(nextType/10==11)return attOpen(*(Blocker*)Occupied[Level][destX][destY]);             //������ͼ�򿪺���
    else if(nextType/10==20)        //ռ����ΪԿ��
    {
        getKey((KeyColor)(nextType-200));
        ((Key*)Occupied[Level][destX][destY])->~Key();
        Map[Level][destX][destY]=0;
        Occupied[Level][destX][destY]=0;
        Move(destX,destY);
        return 1;
    }
    else if(nextType/10>20&&nextType/10<24)   //ռ����ΪѪ�������Ĳ���Ʒ
    {
        getAdder(*(Adder*)Occupied[Level][destX][destY]);
        ((Adder*)Occupied[Level][destX][destY])->~Adder();
        Map[Level][destX][destY]=0;
        Occupied[Level][destX][destY]=0;
        Move(destX,destY);
        return 1;
    }
    else if(nextType/100==3)   //ǰ��Ϊ¥��
    {
        if(nextType/10==30){        //��¥�����ñ�ǣ��Ժ���main()�д���
                                    //������ϣ������main()�з����κ��й���Ϸ���̵ľ�������
                                    //����ȫ��д�ɺ��� �������վ�����û����ô���ġ�����ȫ�ֱ����Ѿ�̫��̫���ˡ�
            Stat=__Up;
            NextLevPlayerPos=nextType*10+100;   //�������һ¥��¥�ݳ��ڵı�־ID
                                                //**���Ҫ�󱾲����¥��ID���ϲ����¥��ID�����Ӧ����λ��ȣ�
                                                //**�༭���Ĺؿ������麯��(isValidLevel())��δ������һ��
                                                //**��ʵ���ϱ༭����ͬһʱ��ֻ����һ���㣬���ڼ��ʱ�������²����Ӧ¥�ݵ�λ�ú�ID��
        }
        else if(nextType/10==31){           //��¥��ͬ��
            Stat=__Down;
            NextLevPlayerPos=nextType*10-100;
        }
        return 998;
    }
    else if(nextType>999&&nextType<3000)    //���ˣ�����ս������
    {
        return Battle(*(Enemy*)Occupied[Level][destX][destY]);
    }
    else if(nextType>3299&&nextType<3700)   //��ͨNPC�������ˣ�
    {
        int tmpFea = 0;
        if(((Npc*)Occupied[Level][destX][destY])->unlockFeature!=0){
            tmpFea = ((Npc*)Occupied[Level][destX][destY])->unlockFeature;
            SkillSet[((Npc*)Occupied[Level][destX][destY])->unlockFeature]=1;
        }
        if(((Npc*)Occupied[Level][destX][destY])->showDialog()==1)      //Npc::showDialog()���ص��Ǹ�NPC�Ƿ�ֻ�ܷ���һ��
                                                                        //��Ϊ1��NPC��ʧ
        {
            ((Npc*)Occupied[Level][destX][destY])->~Npc();
            Map[Level][destX][destY]=0;
            Occupied[Level][destX][destY]=0;
        }
        if(tmpFea) gameHelp(tmpFea);
        return 1;
    }
    else if(nextType>3699&&nextType<3903)   //����
    {
        if(((Merchant*)Occupied[Level][destX][destY])->yesNoDialog()==1)    //Merchant::yesNoDialog()�����Ƿ���
                                                                            //��ѡ�����������һ���ж�
        {
            int tmpre;
            if(nextType<3900){
                tmpre=purchaseFromMerchant((*(Merchant*)Occupied[Level][destX][destY]));
                                        //Player::purchaseFromMerchant()
                                        //����ֵ0Ϊ��Ǯ 1Ϊ����ɹ����޺��� -1���̵깺��ɹ�
                                        //��������ֵ��ʾ����ɹ��Ҹ�����ת��Ϊ��һIDΪ�÷���ֵ��NPC
            } else tmpre=purchaseFromShop((*(Shop*)Occupied[Level][destX][destY]));

            if(tmpre==0)
            {
                string nom[4];
                nom[0]="ûǮ����������������";
                mesg("��ȥŪ��Ǯ�ɡ�");
                npcStyleDialog(1,nom);  //���������ǰ���mesg()������ͬ�������⣬����
                                        //�Ҳ�֪�������ȷ*������*���**��������**��string�ַ���
                                        //���������ְ취
            }
            else if(tmpre != -1)
            {
                string bou[4];
                bou[0]="����ɹ�";
                mesg("����ɹ�");
                npcStyleDialog(1,bou);
                if (tmpre==1) return 1; //����ɹ����޺�����ʹ������ʧ��Player::purchaseFromMerchant()������ɡ�
                int tmpx, tmpy;         //����ɹ�������ת��Ϊ��һNPC���˺������϶����ļ���������NPC������Ϊ׼
                ((Entity*)Occupied[Level][destX][destY])->getPos(tmpx, tmpy);
                ((Merchant*)Occupied[Level][destX][destY])->~Merchant();
                ((Npc*)Occupied[Level][destX][destY])=new Npc(tmpx, tmpy, tmpre);
                refresh();
            }
            else
            {
                string bou[4];
                bou[0]="����ɹ�����ӭ�´ι���";
                mesg("����ɹ�");
                npcStyleDialog(1,bou);
                return 1;
            }
        }
        return 1;
    }
    else return 0;
}
int Player::Battle(Enemy&enemy)     //�����ս��
{
    gotoxy(49,20);
    cout<<"                                "<<endl;
    gotoxy(49,21);
    cout<<"                                "<<endl;
    gotoxy(49,22);
    cout<<"                                "<<endl;
    int sleepTime=333, ffTime=33, ff=0;     //��Ϊ���ֶ����Ӵ���ٶȶ����õģ����Ǻ�ʧ�ܡ�
    int dmgToPlayer = (enemy.Atk-Def>0)?(enemy.Atk-Def):1;      //���˶������ɵ��˺�����ͬ
    int dmgToEnemy = (Atk-enemy.Def>0)?(Atk-enemy.Def):1;
    int tmpHp = Hp;
    int tmpEhp= enemy.Hp;       //��ʱ�洢����Ѫ�������ж��Ƿ�����ս��
    int turn=0;
    if(enemy.Hp<=0)enemy.~Enemy();
    while(yourDaddy==0)                //Ԥս�����ж��Ƿ�����ս��
    {
        tmpEhp-=dmgToEnemy;
        if(tmpHp>0&&tmpEhp<=0)break;    //���Ԥս�������䣬�������ʽս��
        tmpHp-=dmgToPlayer;
        if(tmpEhp<=0&tmpHp<=0){         //���Ԥս��������ˣ���ΪҪ��������˺���
                                        //���Ա�����㵽����Ҳ��Ϊֹ
            enemy.refreshStatus();
            mesg("","","���ɹ�����");
            gotoxy(49,20);
            cout<<"�����˺�      "<<Hp-tmpHp<<endl;
            return 0;
        }
    }
    if(yourDaddy==1)enemy.Hp=-1;
    while(1)
    {
        ++turn;
        if(ff>=6)sleepTime=ffTime;      //��Ϊ�ֶ������Ƿ���ٶ����е�ʧ�ܳ���
        if(turn==100)sleepTime=1;
        if(turn==50)sleepTime/=2;
        if(turn==25)sleepTime/=2;
        if(turn==10)sleepTime/=2;
        if(sleepTime<=0)sleepTime=1;
        enemy.Hp-=dmgToEnemy;           //��ҿ�һ��
        enemy.refreshStatus();
        if(enemy.Hp<=0)
        {
            enemy.refreshStatus();
            mesg("ʤ����");
            Gold+=enemy.Gold;
            enemy.~Enemy();
            Map[Level][enemy.X][enemy.Y]=0;
            //WaitForSingleObject(hMutex, INFINITE);
            while(getVK(1));        //�������������VK����Ķ�����������˵������VK�ⶫ���л���ô��
                                    //ͬ����ʧ�ܵĳ��ԡ�
            //ReleaseMutex(hMutex);
            return 1;
        }
        if(!kbhit())ff=0;           //ʧ�ܳ��ԣ���ͬ��
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

void Creature::Move(int destX, int destY)   //��**��ͼ**�ƶ���ͬ���ú�������ʵ�ʵ��ƶ�������Ŀ��ص��Ƿ�ռ�ݡ�
                                            //**�������ƶ��ĳ�ʼ���Ŀ��ص��Ƿ�������������κζ�����
{
    disappear();
    X=destX, Y=destY;
    show();
}
void Player::getAdder(Adder&gotAdder)       //���Ѫ�������Ĳ���
{
    Hp+=gotAdder.aHp;
    Atk+=gotAdder.aA;
    Def+=gotAdder.aD;
    if(gotAdder.Type==210||gotAdder.Type==212)
        mesg("���˺óԡ�");
    else if(gotAdder.Type==211||gotAdder.Type==213)
        mesg("���˺úȡ�");
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
Enemy::Enemy(int x, int y, const int type):Creature(x,y,type,0,0,0,0)   //������˶���ʱ
                                                                        //���ļ���ȡ��ս��
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

Adder::Adder(int x, int y, AdderType type):Item(x,y,200+type)   //����Ѫ������������ʱ
                                                                //�����䲹�������ݼ�����
{                                                               //!!�������������ǿ�����¥��仯���ˣ�
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
    //ʾ������20��ҿɹ���300������
    return intStr(price) + "��� �ɹ��� " + intStr(goodsamount) + goodsname;
}
int Player::attOpen(Blocker&block)      //��ͼ���ϱڵĺ������ϱڰ����š�
{
    int tmpType=block.getType();
    switch(tmpType)
    {
        case 100: return 0;         //ʵǽ��
        case 113: block.~Blocker(); //��ǽ��
                  Map[Level][block.X][block.Y]=0;
                  if(debugMode>=3)mesg("ѧ�ھ�������ļ�ǿ��","�й�ɽ�������裡");
                  else mesg("������������ⱳ��������","����ġ�����δ֪�Ķ��⡣");
                  return 1;
        case 110: case 111: case 112:       //�š�
            if(haveKeys[tmpType-110]==0){
                mesg("û��Կ�ף�");
                return 0;
            }
            else
            {
                haveKeys[tmpType-110]--;
                block.~Blocker();
                Map[Level][block.X][block.Y]=0;
                mesg("�ſ��ˣ�");
                return 1;
            }
    }
    return 0;
}
void Player::refreshStatus()        //ˢ�����״̬
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
    cout<<"¥�� "<<setw(2)<<Level;
}
void Enemy::refreshStatus()         //ˢ�µ���״̬
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
Npc::Npc(int x, int y, int Index):Entity(x,y,Index)     //���ļ���ȡNPC��Ϣ
{
    if(Index>3899)return;
    ifstream infile;
    locale loc("chs");
    infile.imbue(loc);
    string npcpath=apppath+"npctext.dat";
    infile.open(npcpath.c_str());   //�洢��npctext.dat�е�NPC��Ϣ�ĸ�ʽΪ
                                    //3300 1 0 2 һ�����������߰˾�ʮһ�����������߰� ��ӭ����ħ����
                                    //3300ΪNPCID�����ͣ�
                                    //1��Ǹ�NPCֻ�ܷ���һ�Σ����Զ���ʧ��0��֮��
                                    //0��Ǹ�NPC�Ƿ�Ϊ������¹��ܣ����ߵȣ�����δ������
                                    //2�����Ϣ��������
                                    //**fstream���ո���Ϊ�ָ�����������Ϣ�ڲ��ܺ��пո�
                                    //**�����������Avis���������н������
                                    //****�����Ҷ���Avis��Դ��
                                    //һ�����������߰˾�ʮһ�����������߰� Ϊ��һ����Ϣ
                                    //��ӭ����ħ���� Ϊ�ڶ�����Ϣ
                                    //�˺�������ָ����ָ�����Լ�¼��һ����
                                    //����ʹ�û�����Ϊ����ķָ�
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
            goodsname = "����";break;
        case 3901:
            goodstype = __Atk;
            goodsamount = atk_am;
            goodsname = "����";break;
        case 3902:
            goodstype = __Def;
            goodsamount = def_am;
            goodsname = "����";break;
    }
    //ʾ������20��ҿɹ���300������
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
    infile.open(merpath.c_str());       //�洢��merchant.dat�е�������Ϣ��ʽ��
                                        //3701 1 200 5000 0
                                        //��ʾ��
                                        //NPCIDΪ3701��������5000��Ϊ�۸����200�㹥����(1)
                                        //Ȼ��ֱ����ʧ(0)�����ṩ�κ���Ϣ��
                                        //3701ΪNPCID������
                                        //1Ϊö�����ͣ���Ʒ���� enum AttribType{
                                        //  __Hp=0,
                                        //  __Atk,
                                        //  __Def,
                                        //  __YKey,
                                        //  __BKey,
                                        //  __RKey};
                                        //200Ϊ������Ʒ��������
                                        //5000Ϊ������Ʒ���ܼ�
                                        //0Ϊ��־���˽��׺��Ƿ�ת��Ϊ����NPC��ֵ����Player::attMove()������˵��
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
int Npc::showDialog()       //NPC�Ի���
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
    gotoxy(29,12);cout<<"�����������";
    //ReleaseMutex(hMutex);
    getVK();
    //WaitForSingleObject(hMutex, INFINITE);
    refresh();
    if(doAutoDisappear) return 1;       //������һ��������NPC�Զ���ʧ
    else return 0;
}
int Merchant::yesNoDialog()     //���˵ġ���Y���򡱶Ի���
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
    gotoxy(16,12);cout<<"�� Y �����������������";
    int tmpin;
    ReleaseMutex(hInputMutex);
    tmpin=getVK();
    WaitForSingleObject(hInputMutex, INFINITE);
    if(tmpin!='Y'){refresh();return 0;}     //�������Y��ȡ��
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

    // 0 = ��Ǯ
    // -1 = ����ɹ�

}
int Player::purchaseFromMerchant(Merchant&merc)
{
    gotoxy(1,1);
    if(Gold<merc.price)return 0;        //�����Ǯ��ȡ��
    Gold=Gold-merc.price;       //��Ǯ
    if(merc.goodstype==0)Hp+=merc.goodsamount;
    else if(merc.goodstype==1)Atk+=merc.goodsamount;
    else if(merc.goodstype==2)Def+=merc.goodsamount;
    else if(merc.goodstype==3)haveKeys[KeyYellow]+=merc.goodsamount;
    else if(merc.goodstype==4)haveKeys[KeyBlue]+=merc.goodsamount;
    else if(merc.goodstype==5)haveKeys[KeyRed]+=merc.goodsamount;
    refreshStatus();
    refresh();
    if(merc.Type>3899)return -1;
    else if(merc.thenProvInfo==0)    //thenProvInfo��ǰ�Ľ��ܶ��
        merc.~Merchant();
    else return merc.thenProvInfo;
    return 1;
}
void initBoard()            //��ʼ�����棬�������⡢��ɫ�ذ塢�Ҳ���Ϣ�������ı�ǩ
{
    int i,j;
    clrscr();
    gotoxy(1,24);
    cout<<"��������������뷨��Ϣ��������뷨�ص�";
    if(debugMode==1){
        system("title ħ�� v0.1 Ԥ���� ����ģʽ������");
        gotoxy(1,24);
        cout<<"                                      ";
        gotoxy(1,24);
        cout<<"������: "<<arguments<<endl;
        gotoxy(1,1);
    }
    else if (debugMode==2)system("title ħ�� v0.1 Ԥ���� ��������������");
    else if (debugMode>=3)system("title ħ�� v0.1 Ԥ���� ����ģʽ������");
    else system("title ħ�� v0.1 Ԥ���� �� L ����ʾ����");
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
    //cout<<"�� L �鿴����   ¥�� "<<setw(2)<<Level;
    gotoxy(InfoPos+24,1);
    cout<<"¥�� "<<setw(2)<<Level;
    gotoxy(InfoPos,2);
    cprintf("������    ");
    textcolor(LIGHTRED);
    gotoxy(InfoPos,4);
    cprintf("������    ");
    textcolor(LIGHTCYAN);
    gotoxy(InfoPos,6);
    cprintf("������    ");
    textcolor(YELLOW);
    gotoxy(InfoPos,8);
    cprintf("�����    ");
    //textcolor(LIGHTGREEN);
    //gotoxy(InfoPos,10);
    //cprintf("����ֵ    ");                //����ֵ�ѱ��Ƴ�
    textcolor(WHITE);
    gotoxy(InfoPos,10);
    cprintf("Կ��  ");
    textcolor(YELLOW);
    cprintf("��    ");
    textcolor(LIGHTCYAN);
    cprintf("��    ");
    textcolor(LIGHTRED);
    cprintf("��");
    gotoxy(InfoPos,11);
    cout<<"------------------------------"<<endl;
    textcolor(WHITE);
    gotoxy(InfoPos,12);
    cprintf("�з�������");
    textcolor(LIGHTRED);
    gotoxy(InfoPos,14);
    cprintf("�з�������");
    textcolor(LIGHTCYAN);
    gotoxy(InfoPos,16);
    cprintf("�з�������");
    textcolor(YELLOW);
    gotoxy(InfoPos,18);
    cprintf("�з������");
    //textcolor(LIGHTGREEN);
    //gotoxy(InfoPos,22);
    //cprintf("�з�����ֵ");
    gotoxy(1,23);
    //for(i=1;i<InfoPos+10;i++)cout<<'-';
    //gotoxy(InfoPos+11,23);
    //cout<<" �� L �鿴����  ---"<<endl;      //���ڻ�û��
    //cout<<getLevelFile(Level).c_str();
    gotoxy(1,1);
    #undef InfoPos
}
void loadLevel(int level)               //�Թؿ��Ŵ��ļ���ȡ�ؿ�
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
void loadLevel(string level)            //���ļ������ļ���ȡ�ؿ�
                                        //��Ϊdebugģʽ�Ĺ��� �������в���ָ���ؿ��ļ�
                                        //*������������δʵ��
                                        //**ע������ level Ϊstring���͵Ĺؿ��ļ���
                                        //**�� Level Ϊ���͵Ĺؿ��ţ������ǵ�������������Ҫ��main()�������Լ�ָ��
                                        //***ͬ��û��ʵ�֡�
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
void refresh()                      //�ڲ������ػ�ذ��������ػ��ͼ�ϵĶ���
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
void npcStyleDialog(int lines, string cont[])   //NPC���Ի���
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
    gotoxy(29,12);cout<<"�����������";
    getVK();
    refresh();
}
int npcStyleYesNoDialog(int lines, string cont[])     //���˵ġ���Y���򡱶Ի���
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
    gotoxy(16,12);cout<<"�� Y ȷ�����������������";
    int tmpin;
    tmpin=getVK();
    if(tmpin!='Y'){refresh();return 0;}     //�������Y��ȡ��
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
