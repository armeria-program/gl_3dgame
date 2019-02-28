#pragma comment (lib, "freetype.lib")

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include <assert.h>
#include <Windows.h>
//#include "tl_malloc.h"

#define BUILDING_DLL

#include "ftfont.h"

static unsigned char* 
red2rgba(unsigned char* rgba,unsigned char* red, int size){

	int i;
	int buffersize = size * 4 * sizeof(unsigned char);
	//unsigned char* rgba = tl_malloc(buffersize);//new BYTE[4 * size];
	
	
	
	//printf("****************************\n");
	for(i = 0 ; i < size; i++){
		rgba[i * 4 + 0] = red[i];
		rgba[i * 4 + 1] = red[i];
		rgba[i * 4 + 2] = red[i];
		rgba[i * 4 + 3] = red[i];
		/*
		printf("i = %d==>\t",i);
		{
			int k;
			for( k = 0;k < 4;k++){
				printf("%d ",rgba[i * 4 + k]);
			}
		}
		printf("\n");
		*/
	}
	return rgba;
}
/*
static const wchar_t*
getWC(const char* c){
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc,c,cSize);
	return wc;
}

*/


/*
 *char* תΪwchat*
 */
static void*
f_c(const char* pze){
	//char* pze= "sdasdsd";

	int iSize;
	wchar_t* pwsUnicode;

	iSize = MultiByteToWideChar(CP_ACP,0,pze,-1,NULL,0);
	
	pwsUnicode = (wchar_t*)malloc(iSize  * sizeof(wchar_t));
	printf("********%s,iSize = %d,%d,%d\n",pze,iSize,sizeof(wchar_t),iSize  * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP,0,pze,-1,pwsUnicode,iSize);
	return pwsUnicode;
}
//char* תΪwchar_t
static void
f_char2w_char(wchar_t* pwsUnicode,int cnt,char* pze){
	int iSize = MultiByteToWideChar(CP_ACP,0,pze,-1,NULL,0);
	if(iSize > cnt){
		printf("��������С̫С!\n");
		assert(0);
	}
	memset(pwsUnicode,0,sizeof(wchar_t) * iSize);
	MultiByteToWideChar(CP_ACP,0,pze,-1,pwsUnicode,iSize);
}

void* 
ft_load(unsigned char* outBuffer,int fontw,int fonth,int *iWidth, int *iHeight,char* str){
	const char* fileName = "C:\\Windows\\Fonts\\simsun.ttc";
	//int fontSize = 32;//�ı��ĳߴ� 12





	//wchar_t* ch = L"R";//��3720 f 1260 F1344
	//wchar_t ch[3];

	//char str[3];
	
	FT_Library library;
	FT_Error error;	
	FT_Face face;
	
	unsigned char* rgba = 0;

	int i;
	//sprintf_s(str,3,"%hs","��");
	//mbstowcs(ch,str,3);

	//wchar_t ch[64];
	//sprintf_s(ch,64,"%s","��");

	//wchar_t* ch = f_c("��");
	wchar_t ch[2];
	f_char2w_char(ch,2,str);
	
	//init=========================================================
	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("error:FT_Init\n");
		exit(0);
	}
	//load=========================================================

	//printf("�����ļ�:%s\n",fileName);
	error = FT_New_Face(library, fileName, 0, &face);	
	if (error)
	{		
		printf("δ�ҵ��ļ�(%s)\n",fileName);
		printf("error:FT_New_Face\n");//if (error == FT_Err_Unknown_File_Format)MsgBox("FT_Err_Unknown_File");	
		exit(0);
	}
	//view======================================================
	//printf("���뷽ʽ����: %d \n", face->num_charmaps);

	for(i = 0; i < face->num_charmaps; i++)
	{
		int encoding_id = face->charmaps[i]->encoding_id;
		int platform_id = face->charmaps[i]->platform_id;
		//printf("[%d]: \n encoding_id: %d \n platform_id: %d \n", i, encoding_id, platform_id);
	}
	//printf("�̶��ߴ����: %d \n", face->num_fixed_sizes);


	for(i = 0; i < face->num_fixed_sizes; i++)
	{
		int width = face->available_sizes[i].width;
		int height = face->available_sizes[i].height;
		//printf("[%d]: width: %d  height: %d \n", i, width, height);
	}
	//printf("�ַ���: %d \n", face->num_glyphs);

	//set=========================================================

	//�����С, ��Щ��fixsize��.fon����������ﱨ��
	error = FT_Set_Pixel_Sizes(face, fontw, fonth);//���õ�������Ŀ��
	if (error)
	{		
		printf("error:FT_Set_Pixel_Sizes");
		exit(0);
	}
	//get=========================================================
	{


		//ȡ�����β�
		FT_GlyphSlot slot = face->glyph;		
		//���ַ��������������
		FT_UInt glyph_index = FT_Get_Char_Index(face, *ch);
		
		//���ص�����е�����ͼ�񣨲���ǰһ����
		error = FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT);
		if (error)
		{		
			printf("error:FT_Load_Glyph");
			exit(0);
		}
		//ת��Ϊλͼ
		if(face->glyph->format != FT_GLYPH_FORMAT_BITMAP)
		{
			error = FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL);
			if (error)
			{
				printf("error:FT_Render_Glyph");
				exit(0);
			}
		}
		else
		{
			printf("FT_GLYPH_FORMAT_BITMAP\n");//fon�ļ�����bitmap,����render
		}

		//��ȡ����
		{


			unsigned char* buffer	= slot->bitmap.buffer;	//����
			int width				= slot->bitmap.width;	//�� ����
			int height				= slot->bitmap.rows;	//�� ����
			int top					= slot->bitmap_top;		//��Ժ��׼��ƫ��
			int advancex			= slot->advance.x>>6;	//�ּ�ˮƽ���
			int pitch				= slot->bitmap.pitch;	//�� byte

			//printf("[w = %d h = %d top = %d advancex = %d pitch = %d]\n",width,top,advancex,pitch);

			rgba = red2rgba(outBuffer,buffer, width * height);

			//printf("��Ҫ�Ļ�����%d�ֽ�(max:%d)\n",width *  height * 4,fontSize*fontSize*4);

			*iWidth = width;
			*iHeight= height;

			/*
			m_tex = gen_image(rgba, width, height);
			m_tex_w = width;
			m_tex_h = height;

			printf("texture: %d width = %d height = %d\n",m_tex,m_tex_w,m_tex_h);

			*/
		}


		//finish=======================================================
		//system("pause");			
		FT_Done_Face    (face);
		FT_Done_FreeType(library);
	}
	return rgba;
}