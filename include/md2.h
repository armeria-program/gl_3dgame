#ifndef _MD2PARSE_H_
#define _MD2PARSE_H_



/*
*	�ؼ�֡�ṹ��
*/
struct MD2_Frame
{
	/*
	*	�ؼ�֡��
	*/
	char name[G_BUFFER_16_SIZE];

	/*
	*	�ؼ�֡������
	*/
	int index;

	/*
	*	���㶨���б�
	*/
	float* vertices;

	/*
	*	�������
	*/
	int vertCount;

	/*
	*	��������
	*/
	int* indices;
};

struct MD2_ParseObj
{
	/*
	*	����
	*/                                                                                                                                                                                                                                                                                                                                                                   
	struct BytesVO* b;
	
	//�汾��
	int _version;
	
	//�ļ���־��844121161��"IPD2"����
	int _ident;
	/**
	 * ������
	 */
	int _skinWidth;
	/**
	 * ����߶�
	 */
	int _skinHeight;
	/**
	* ÿһ֡���ֽ���
	*/
	int _frameSize;
	
	/**
	 * ������Ŀ
	 */
	int	_numSkins;
	/**
	 * ������Ŀ(ÿһ֡)
	 */    
	int numVertices;
	/**
	 * ����������Ŀ (ÿһ֡)
	 */    
	int _numST;
	/**
	 * ��������Ŀ (ÿһ֡)
	 */
	int numTris;
	/**
	 * gl������Ŀ(ÿһ֡)
	 */
	int _numGlCmds;
	/**
	 * ��֡��
	 */
	int _numFrames;
	/**
	 * �����ƫ��λ��
	 */
	int _offsetSkins;
	/**
	 * ���������ƫ��λ��
	 */
	int _offsetST;
	/**
	 * ������������ƫ��λ��
	 */
	int _offsetTris;
	/**
	 * ��һ֡��ƫ��λ��
	 */
	int _offsetFrames;
	/**
	 * OpenGL�����ƫ��λ��
	 */
	int _offsetGlCmds;
	/**
	 * �ļ���β��ƫ��λ��
	 */
	int _offsetEnd;
	/**
	*	����ͷ����Ϣ�ı�ʾ
	*/
	int _parsedHeader;
	//============================
	/*
	*	����uv�ı�ʾ
	*/
	int _parsedUV;
	/*
	*	������Ƭ��ʾ
	*/
	int _parsedFaces;
	/*
	*	�ؼ�֡��ʾ
	*/
	int _parsedFrames;

	struct MD2_Frame* pframe;

	/**
	 * ����-������������ԣ�_vertIndices��Ӧ_uvIndices��_vertIndices.length<=_indices.length;
	 */
	struct List* _vertIndices;

	/**
	*	uv����
	*/
	struct List* _uvIndices;

	/**
	* ����������������һ�飬_indices = 3 * _numTris;
	*/
	struct List* _indices;
	
	float* _uvs;

	float* _finalUV;
};

//md2����ģ��

/*
	����
*/
void md2parse_load(struct MD2_ParseObj* _md2,const char* str,int len);

/*
	����
*/
void md2parse_dispose(struct MD2_ParseObj* _md2);



#endif