/*********************************************************************************************************
 * ��ȡ�ļ����������ļ���
 *********************************************************************************************************/

#include <stdio.h>
#include <io.h>
//negative positive
const char *to_search="D:\\gl_source\\sample\\*.c";    //�����ҵ��ļ���֧��ͨ���
//FILE *fp;

int main()
{
	long handle;                                                     //���ڲ��ҵľ��
	struct _finddata_t fileinfo;                                     //�ļ���Ϣ�Ľṹ��

	//fp = fopen("train_list2.txt", "w+");
	//if (fp==0) 
	/*{
		printf("can't open file\n");
		return 0;
	}*/
	handle=_findfirst(to_search,&fileinfo);                          //��һ�β���
	if(-1==handle)
		return -1;

	//fprintf(fp,"positive/%s 0\r",fileinfo.name);                 //��ӡ���ҵ����ļ����ļ���

	
	printf("%s\n",fileinfo.name);
	while(!_findnext(handle,&fileinfo))                              //ѭ�������������ϵ��ļ���ֱ���Ҳ���������Ϊֹ
	{
		//fprintf(fp,"positive/%s 0\r",fileinfo.name);
		printf("%s\n",fileinfo.name);
	}
	_findclose(handle);                                              //�رվ��
	
	printf("output done.\n");
	//fclose(fp);
	//system("pause");
	return 0;
}