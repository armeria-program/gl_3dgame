#pragma comment (lib, "freetype.lib")
//#include <ft2build.h>
//#include <freetype/freetype.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>

#include "tl_malloc.h"

static unsigned char* 
red2rgba(unsigned char* red, int size){
	unsigned char* rgba = tl_malloc(size * 4 * sizeof(unsigned char));//new BYTE[4 * size];
	int i;
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

void* 
ft_load(int *iWidth, int *iHeight){
	const char* fileName = "C:\\Windows\\Fonts\\simsun.ttc";
	int fontSize = 36;//�ı��ĳߴ� 12

	

	//wchar_t* ch = L"��";
	wchar_t ch[3];

	char str[3];
	


	FT_Library library;
	FT_Error error;	
	FT_Face face;
	
	unsigned char* rgba = 0;

	int i;
	sprintf_s(str,3,"%hs","��");
	mbstowcs(ch,str,3);

	//wchar_t ch[64];
	//sprintf_s(ch,64,"%s","��");
	
	
	
	//init=========================================================
	error = FT_Init_FreeType(&library);
	if (error)
	{
		printf("error:FT_Init\n");
		exit(0);
	}
	//load=========================================================

	printf("�����ļ�:%s\n",fileName);
	error = FT_New_Face(library, fileName, 0, &face);	
	if (error)
	{		
		printf("δ�ҵ��ļ�(%s)\n",fileName);
		printf("error:FT_New_Face\n");//if (error == FT_Err_Unknown_File_Format)MsgBox("FT_Err_Unknown_File");	
		exit(0);
	}
	//view======================================================
	printf("���뷽ʽ����: %d \n", face->num_charmaps);

	for(i = 0; i < face->num_charmaps; i++)
	{
		int encoding_id = face->charmaps[i]->encoding_id;
		int platform_id = face->charmaps[i]->platform_id;
		printf("[%d]: \n encoding_id: %d \n platform_id: %d \n", i, encoding_id, platform_id);
	}
	printf("�̶��ߴ����: %d \n", face->num_fixed_sizes);


	for(i = 0; i < face->num_fixed_sizes; i++)
	{
		int width = face->available_sizes[i].width;
		int height = face->available_sizes[i].height;
		printf("[%d]: width: %d  height: %d \n", i, width, height);
	}
	printf("�ַ���: %d \n", face->num_glyphs);

	//set=========================================================

	//�����С, ��Щ��fixsize��.fon����������ﱨ��
	error = FT_Set_Pixel_Sizes(face, fontSize, fontSize);//���õ�������Ŀ��
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

			printf("[w = %d h = %d top = %d advancex = %d pitch = %d]\n",width,top,advancex,pitch);

			rgba = red2rgba(buffer, width * height);
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