#define DEFAULTPOS 1,1
#define POS 4*X+1,2*Y+1

#define PLAYER 998
#define WALL 100
#define INV_WALL 101    //inv=invisible
#define cWALL 100: case 113
#define YDOOR 110           // Y B R = yellow blue red
#define BDOOR 111
#define RDOOR 112
#define cKEY 200: case 201: case 202
#define YKEY 200
#define BKEY 201
#define RKEY 202
#define cGEM 210: case 211: case 212: case 213
#define cRGEM 210: case 211
#define cBGEM 212: case 213
#define cSWORD 220: case 221: case 222: case 223: case 224
#define cSHIELD 230: case 231: case 232: case 233: case 234
#define STC_UP 300        //STC=staircase
#define STC_DOWN 301
#define cSTC 300: case 301: case 302: case 303: case 304: case 305: case 306: case 307: case 308: case 309 \
                    : case 310: case 311: case 312: case 313: case 314: case 315: case 316: case 317: case 318: case 319

enum Direction{ UP=0,
                DOWN,
                LEFT,
                RIGHT};
enum KeyColor{  KeyYellow=0,
                KeyBlue,
                KeyRed};
enum AdderType{ RGem=10,   //med=medicine
                RMed,
                BGem,
                BMed,
                Swd1=20,   //sword
                Swd2,
                Swd3,
                Swd4,
                Swd5,
                Shd1=30,   //shield
                Shd2,
                Shd3,
                Shd4,
                Shd5};
enum Warping{   __Down=-1,__None=0,__Up=1};
enum AttribType{__Hp=0,
                __Atk,
                __Def,
                __YKey,
                __BKey,
                __RKey,
                __Gold=9};

