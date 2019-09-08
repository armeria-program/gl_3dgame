//#define BUILDING_DLL

#include <stdio.h>     
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define BUILDING_DLL

#include "common.h"


//���·��
char* g_local_path[256];


int 
tl_is_file_exist(const char* file){
	FILE* stream;
	errno_t err=fopen_s( &stream,file, "r" );
	if(err == 0){
		fclose( stream );
		return 1;
	}
	return 0;
}

void 
tl_writeAppandFile(const char* filePath,char* buffer){
	FILE * pFile;
	errno_t err = fopen_s ( &pFile,filePath , "ab+" );
	if(err != 0){
		printf("�ļ� %s ������!\n",filePath);
		assert(0);
		return;
	}
	fwrite (buffer , strlen(buffer), 1, pFile );
	fclose (pFile);
}

void 
tl_newName(char* buffer,int bufferSize){
	static int _nameId;
	memset(buffer,0,bufferSize);
	sprintf_s(buffer,bufferSize,"%d",_nameId);
	_nameId++;
}

/*

#define FILE_BYTES_SIZE 512//Ԥ�ƻ������ߴ�
char* 
tl_loadfile(const char* xfile,int* outLengthPtr){
	unsigned int len;
	char* buffer = 0;
	FILE *stream;
	const char* file=0;

//#ifdef _MAIN_PATH_
//	char tempPath[G_BUFFER_256_SIZE];
//	//�������˸ú� ʹ�þֲ�·��
//	memset(tempPath,0,G_BUFFER_256_SIZE);
//	sprintf(tempPath,"%s%s",_MAIN_PATH_,xfile);
//	file = tempPath;
//	if((int)strlen(xfile)+(int)strlen(_MAIN_PATH_)>G_BUFFER_256_SIZE){
//		printf("·��̫��\n");
//		assert(0);
//	}
//#else
//	//δ����������ʱ�� ʹ�þ���·��
//	file = xfile;
//#endif
//# 

#ifdef _DEBUG_MODE_
	long time = get_longTime();
#endif
	char outStr[FILE_BYTES_SIZE];
	errno_t err;
	if(tl_convert_path((char*)xfile,outStr,FILE_BYTES_SIZE))
		file = outStr;
	else
		file = xfile;
	
	err = fopen_s(&stream, file, "rb+" );	//rb+ ��д��һ���������ļ��������д���ݣ��ļ�������ڡ�
	if( err == 0 ){
		fseek(stream,0,SEEK_END);
		len = ftell(stream);
		buffer = (char*)tl_malloc((unsigned int)(len+1));
		memset(buffer,0,len+1);
		fseek(stream,0,SEEK_SET);
		fread( buffer, sizeof( char ), len, stream);
		fclose( stream );
		if(outLengthPtr!=NULL){
			//pLengthָ�벻Ϊ�յ�ʱ��Ϊ�丳ֵ
			*outLengthPtr = len;
		}
	}else{
		// �Ҳ����ļ� �˴����쳣 	
		//log_code(ERROR_NOT_EXIST_FILE);
		printf("�Ҳ����ļ�%s\n",file);
		//assert(stream);
		assert(0);
	}
	//printf("%s\n",(char*)buffer);

	#ifdef _DEBUG_MODE_
	{
		log_color(0xff00ff,"tl_loadfile �����ļ� %s (%d�ֽ�) ��ʱ %ld ����\n",xfile,len,get_longTime()-time);
	}
	#endif

	return (char*)buffer;
}
*/


int tl_convert_path(char* in,char* out,int outBufferSize){
	char* p = (char*)g_local_path;
	if((int)strlen(p)>0){
		memset(out,0,outBufferSize);
		sprintf_s(out,outBufferSize,"%s%s",p,in);
		return 1;
	}
	return 0;
	//printf("%s\n%s\n",in,out);
}