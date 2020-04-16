//#include "main.h"

//假作真时真亦假
#define flase 1
#define FLASE 1
#define ture 0
#define TURE 0

//给每个特殊功能或宝物分配编号，顺序依《加血版魔塔》中的说明顺序
//记得NPC里的unlockFeature吗？
//int SkillSet[];
//永久宝物，ture表示不能使用，flase表示可以使用；
//消耗品，ture表示不能使用，否则标记数量，不过一般都没有超过1的吧！
#define ReservedSkill 0
#define MonsterBook 1   //怪物手册
#define Notebook 2      //NPC对话记录，真不想写这玩意啊
                        //感觉真是好麻烦好麻烦，还要改NPC的代码
#define FreeFly 3       //无需楼梯口旁边的楼层跳跃
#define StairFly 4      //在楼梯口旁边才能进行的楼层跳跃
#define LavaFreezer 5   //将周围岩浆冻为平地
                        //哦操忘了写岩浆这个东西！！！！
                        //妈的反正跟墙差不多先用墙凑合着吧
#define Pickaxe 6       //镐
#define QuakeRoll 7     //地震卷轴
#define Bomb 8          //炸弹，可以炸死你周围的敌人（BOSS无效）
                        //妈的还得判断一个敌人是不是BOSS？我真是操了
#define HolyWater 9     //圣水
                        //此物于原版魔塔中原理不明，需要首席数学家某学霸先生计算指导
                        //但在某些版本中单纯为血量增加一倍
#define CentralSymmetry 10  //又学到一个新单词好高兴，中心对称
#define FlyUp 11            //上1层
#define FlyDown 12          //下1层
#define YellowKeys 13       //开所有黄门

#define HolyCross 14        //十字架，对兽人和吸血鬼攻击力加倍 攻击力加倍还是伤害加倍？
                            //虽然说吧这游戏里暂时还既没有兽人又没有吸血鬼
#define WyvernSlayer 15     //屠龙匕，对魔龙攻击力加倍 疑问同上
                            //魔龙也没有
#define LuckyCoin 16        //幸运大金币，金币收益加倍
                            //金币好像有
