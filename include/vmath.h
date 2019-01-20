#ifndef _VMATH_H_
#define _VMATH_H_
//��Ԫ����ѧ��
/* Quaternion (x, y, z, w) */
typedef float Quat4_t[4];
//#ifdef _cplusplus
//extern "C" {
//#endif
/*
0(0,0)	1(0,1)		2(0,2)		3(0,3)
4(1,0)	5(1,1)		6(1,2)		7(1,3)
8(2,0)	9(2,1)		10(2,2)		11(2,3)
12(3,0)	13(3,1)		14(3,2)		15(3,3)
*/
typedef float Matrix44f[16];

struct Vec2{
	float x,y;
};
/*
*	3D����
*/
struct Vec3{
	float x,y,z;
};

struct Vec4
{
	float x,y,z,w;
};


/* Vectors */
typedef float Vec2_t[2];
typedef float Vec3_t[3];
typedef float Vec8_t[8];

enum {X, Y, Z, W};

/*
��ӡ����
*/
void mat4x4_printf(const char* name,Matrix44f M);
void mat4x4_double_printf(const char* name,double m[16]);
/*
�����һ��
*/
void mat4x4_identity(Matrix44f M);

/*
*��ת����(�������)
*/
void mat4x4_invert(Matrix44f out,Matrix44f a);

void mat4x4_transformMat4(float out[4],float a[4],Matrix44f m);
/*
����λ��
*/
void mat4x4_translate(Matrix44f M, float x, float y, float z);
//sx,sy,sz	����
void mat4x4_scale(Matrix44f M, float x, float y, float z);
////��ʼ����һ����
//void mat4x4_identity(Matrix44f M);
/*
��ά�������ֵ
*/
void mat4x4_2t1(Matrix44f M,int m,int n,float value);
/*
��a�����ֵcopy��out��
*/
void mat4x4_copy(Matrix44f M,Matrix44f out);
/*
XYZ��ת
*/
void mat4x4_rotateX(Matrix44f M, float radian);

void mat4x4_rotateY(Matrix44f M, float radian);

void mat4x4_rotateZ(Matrix44f M, float radian);

void mat4x4_rotateXYZ(Matrix44f M, float radian, float x, float y, float z); 

void perspectiveFrustum(Matrix44f M, float left, float right, float top, float bottom, float near, float far); //͸��ͶӰ
void orthoPerspect(Matrix44f M, float left, float right, float top, float bottom, float near, float far);  //����ͶӰ

//void matrix_m(float **a_matrix, const float **b_matrix, const float **c_matrix,
//			  int krow, int kline, int kmiddle, int ktrl);
/*
1ά����������,4x4 ע�����˷����Ⱥ�˳��
*/
void multi2(Matrix44f out,Matrix44f a, Matrix44f b);
/*
�����ÿ��ѡ���0
*/
void mat4x4_zero(Matrix44f M);

/**
* ��������ת��
* Transpose the values of a mat4
*
* @param {mat4} out the receiving matrix
* @param {mat4} a the source matrix
* @returns {mat4} out
*/
void mat4x4_transpose(Matrix44f a);
/*
*�þ�����ĳ��Ŀ��
*/
void mat4x4_lookAt(Matrix44f out,struct Vec3* eye,struct Vec3* center,struct Vec3* up);

//��������
void mat4x4_mult(int c,Matrix44f o,...);

/*
*	����Ԫ��ת��Ϊ����
*/
void 
Quat_to_matrrix(const Quat4_t qa,Matrix44f out);

void Quat_computeW (Quat4_t q);
void Quat_normalize (Quat4_t q);
void Quat_multQuat (const Quat4_t qa, const Quat4_t qb, Quat4_t out);
void Quat_multVec (const Quat4_t q, const Vec3_t v, Quat4_t out);
void Quat_rotatePoint (const Quat4_t q, const Vec3_t in, Vec3_t out);
/*���*/
float Quat_dotProduct (const Quat4_t qa, const Quat4_t qb);

void  Quat_slerp (const Quat4_t qa, const Quat4_t qb, float t, Quat4_t out);



/*vec����*/
void tl_set_vec(Vec3_t vec,float x,float y,float z);
/*
	c = a + b
*/
void tl_add_vec(Vec3_t a,Vec3_t b,Vec3_t c);

/*
	��ƫ��
*/
void tl_offset_vec(Vec3_t pos,float x,float y,float z);

/*vec����*/
void tl_scale_vec(Vec3_t vec,Vec3_t res,float scale);


/*
*	���һ������
*/
void vec3Set(struct Vec3* v,float x,float y,float z);
/*
*	������� out = a + b
*/
void vec3Add(struct Vec3* a,struct Vec3* b,struct Vec3* out);
/*
*	������� out = a - b
*/
void vec3Sub(struct Vec3* a,struct Vec3* b,struct Vec3* out);

/*
	����a,b,c��������ķ�������(�淨������),�����out
*/
void vec3CalculateNormal(struct Vec3* a,struct Vec3* b,struct Vec3* c,struct Vec3* out);
/*
*	�������(���)
*/
void vec3Cross(struct Vec3* a,struct Vec3* b,struct Vec3* out);
/*
*	�������ĳ���
*/
float vec3Length(struct Vec3* v);
/*
 *	�Ƿ���0����
 */
int vec3IsZero(struct Vec3* v);

/*
*	��������һ������
*/
void vec3Mult(struct Vec3* a,float value);

/*
*�������
*/
float vecDot(struct Vec3* a,struct Vec3* b);

/*
*	���������ľ���
*/
float vec3Distance(struct Vec3* a,struct Vec3* b);
/*
	��񻯣���λ����Normalize��
	��Ҫ˵ʸ���ĳ��ȣ�
	ʸ��Vector��x��y��z��
	ʸ������Length��Vector��= |Vector|=sqr��x*x+y*y+z*z��
	Normalize��
	��x/Length��Vector����y/Length��Vector����z/Length��Vector����
	���򲻱䣬����Ϊ1����λ
*/
void vec3Normalize(struct Vec3* v);

/*
	��������һ��float
*/
void vec3Div(struct Vec3* o,float v);

/*
	���������ļн�0~180֮��
*/
float vecToAngle(struct Vec3* a,struct Vec3* b);

/*
 *  ����ʰȡ
 *  
 *  ��ʰȡ����������ʱ��÷���ֻȡ�뽻������Ľ���
 *	tri			:��������������
 *	dataCount	:tri����ĳ��� sizeof(float) * dataCount
 *	nearPoint	:�����
 *	farPoint	:��Զ��
 *	fpos		:��������
 */
void tl_pickTriangle(float* tri,int dataCount,struct Vec3* nearPoint,struct Vec3* farPoint,struct Vec3* fpos,struct HitResultObject* pResult);

//#ifdef _cplusplus
//}
//#endif


#endif