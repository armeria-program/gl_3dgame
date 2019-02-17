#ifndef _XML_H_
#define _XML_H_

/*
*	Type:XMLSList
*	-------------
*	
*	XML �ڵ��б�
	
	outlen		--����ĳ���
	out			--��ʱ����ȥ ���ڴ�������

	pNodeKey	--�����ĸ��ڵ�
	pKey		--���������Ĺؼ���
	pRes		--����������������ֵ
	pNode		--��������XmlNodeָ��

*/
typedef struct XMLSList{
	struct LStackNode* list;
	char* data;
	char* buffer;
};
/*
����һ�����ڵ�ṹ��

xml			--xmlָ��
parent		--���ڵ�����
start		--��ʼ����
end			--��������
row			--����������
*/
typedef struct XmlNode{
	//���������
	struct XMLSList* parentXml;
	int parent;
	int start;
	int end;
};

/*
Type:XmlNode
-----------------

/*
	����:����xml�ַ���
	-------------
	str				--	xml�ı�
	outMaxLen		--	parms����ĳ���,�˳���Ϊ��󳤶�

	--------------------------------------
	example:

	struct XMLSList* xml = NULL;
	char* xmlstr =NULL;
	char out[G_BUFFER_MAX_SIZE];
	struct XmlNode* node=NULL;
	tl_memInit();


	xmlstr=tl_loadfile("d:\\1.xml",NULL);

	xml = xml_parse(xmlstr,G_BUFFER_MAX_SIZE);

	node =	xml_getrow(xml,"id","10");
	if(node!=NULL)
	{
	int num;
	xml_getstr(node,"id",out);
	sscanf(out,"%d",&num);
	printf("(%s,%d)\n",out,num);
	xml_getstr(node,"path",out);
	printf("(%s)\n",out);
	xml_getstr(node,"language",out);
	printf("(%s)\n",out);
	node =	xml_getrow(xml,"id","2");
	p->nameMaxSize = xml_getint(node,"value");

	node =	xml_getrow(xml,"id","3");
	rate = xml_getfloat(node,"value");
	}
	tl_free(xmlstr);
	getchar();
	--------------------------------------
*/
struct XMLSList* xml_parse(const char* str);

/*
Function:	xml_getRow
---------------
��ȡ���ڵ�һ��

*/
struct XmlNode* 
xml_getrow(struct XMLSList* xml,const char* parms,const char* parmsValue);
/*
 *����������ȡnode����
 **/
void* 
xml_getNodeByIndex(void* xml,int index);
/*
����xml���
*/
void xml_del(struct XMLSList* xml);

/*
��ȡ����ֵ

buffer		--������
bufferSize	--��������С

return	--1����ȡ�ɹ�, 2:��ȡʧ��

*/
int xml_getstr(const struct XmlNode* node,const char* key,
			   char* buffer,int bufferSize);

/*
	��ȡint
*/
int xml_getint(const struct XmlNode* node,const char* key);

/*
	��ȡfloat
*/
float xml_getfloat(const struct XmlNode* node,const char* key);

/*
	����һ��tl_malloc���ַ�������,�ڶ���,����Ҫ�ͷ�
*/
char* xml_getCopystr(const struct XmlNode* node,const char* key);
#endif