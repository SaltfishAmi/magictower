#ifdef DLL_FILE
extern "C" __declspec(dllexport) int __stdcall getVK(int opt);
extern "C" __declspec(dllexport) const char* intChx(int in);
#else
#pragma comment(lib, "mtlib.lib")
extern "C" __declspec(dllimport) int __stdcall getVK(int opt=0);//�����ReadConsoleInput����������Ǽ��̾ͷ���vkֵ
extern "C" __declspec(dllimport) const char* intChx(int in);//���������תchar*�ַ���������д��dll����Ϊ�˲��ԣ�ûɶ�߶˹���
#endif
