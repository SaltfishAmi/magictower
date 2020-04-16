#ifdef DLL_FILE
extern "C" __declspec(dllexport) int __stdcall getVK(int opt);
extern "C" __declspec(dllexport) const char* intChx(int in);
#else
#pragma comment(lib, "mtlib.lib")
extern "C" __declspec(dllimport) int __stdcall getVK(int opt=0);//大概是ReadConsoleInput，如果输入是键盘就返回vk值
extern "C" __declspec(dllimport) const char* intChx(int in);//大概是整数转char*字符串，本身写这dll就是为了测试，没啥高端功能
#endif
