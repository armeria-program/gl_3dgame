#ifndef _WIN_H_
#define _WIN_H_
/*����windows.h��ؽӿ�*/
void win_alert(const char* title,const char* msg);/*alert������*/
void win_useFontBitmaps(unsigned int* _textFont,int size);

int wl_LoadDll(const char* dll);
#endif