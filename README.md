# MagicTower
A "Magic Tower" game on the windows command line interface.

# Build
Please use Borland(R) C++Builder 6.0 running on a Windows system whose 
non-unicode program character coding is set to Simplified Chinese 
(CP936) to build this.

### "MagicTowerDll.h"
Only god knows why I put those in a dll, and even god doesn't know where I put them. Anyway, there seem to be only two simple functions inside:

```C++
int getVK(int opt=0);
// Pauses and waits until a keyboard input, then returns its VK code.
const char* intChx(int in);
// Converts an integer to a char* string.
```

Implement them yourself, please?

# Features
You have:

* Magic Tower like gameplay and graphics on command line interface.
* Exploring the tower, fighting monsters, gathering keys, potions, rubies and sapphires.
* Talking to NPCs, buying items from shops and getting special abilities from big sages.
* An in-game command line interface for you to type in cheat commands (???)
* Background music
* A stand-alone level editor.

You don't have:
* Fancy graphics
* Actual maps (only the test maps)
* Any language other than Simplified Chinese

# TO-DO list (Priority)
* Monster Index (10)
* Floor jump that only usable near a staircase (9)
* The special attack of Magical Guards (which you lose half of your HP after walking between two of these guys) and Wizards (which you lose a static amount of HP after walking near one of these guys) (8)
* Power-up items' effects should vary by floor (8)
* Lava tiles (5)
* Reimplement the delay of turn-based fights with the multi-thread timer (1)
* 
# License

WTFPLv2

"bass" is from Un4seen Developments. I never used it for commercial purposes, and thank you Ian.

SaltfishAmi 2017/10/02
