#include "tools.h"
#include "gettime.h"

#include "ex.h"
#include "tmat.h"

#include "md5.h"
#include "jgl.h"
#include "atlas.h"

#include "sprite.h"

#include "obj_vbo.h"

#include "node.h"
#include "animtor.h"
#include "tl_malloc.h"

static struct Vec3* pos1;
static float n1Len;

static struct Vec3 axis;

static struct Vec3 outDirection;

//static struct Vec3 start;
//axis.x = 0;
//axis.y = 0;
//axis.z = 1;
//pos1.x = -0.45;
//pos1.y = -0.75;
//pos1.z = 0;

///************************************************************************/
///*  m:��ת��    
//	n:Ŀ����
//	o:��ֱ��
///************************************************************************/
//static void
//f_get_direction(struct Vec3* n,struct Vec3* m,struct Vec3* o){
//	
//}

//��ӡ����
static void
f_print_vec(char* key,struct Vec3* p){
	printf("%s p = %.3f\t%.3f\t%.3f\n",key,p->x,p->y,p->z);
}

//��Ԫ�����Է���
int
REG_test_unit_01(lua_State *L){
	const char* name=lua_tostring(L,1);
	float value = lua_tonumber(L,2);
	const char* name1=lua_tostring(L,3);

	int n = (int)ex_find_node(name);

	int n1 = (int)ex_find_node(name1);
	struct HeadInfo* b =  base_get2(n1);
	Matrix44f m;//Ŀ�����

	//�뾶��
	float len = 1.0;
	
	Quat4_t s;
	Quat4_t e;
	Quat4_t o;
	struct Vec3 normalpos1;//Ŀ�������ĵ�λ����
	struct Vec3 subpos1;
	struct Vec3 down;
	//down.x = 0;
	//down.y = -1;
	//down.z = 0;
	


	if(!pos1){
		
		//��ʼ��pos1����
		pos1 = malloc(sizeof(struct Vec3));
		pos1->x = -0.25;
		pos1->y = -0.35;
		pos1->z = 0;

		n1Len = vec3Length(pos1);
	}
	vec3Set(&normalpos1,pos1-> x,pos1->y,pos1->z);
	vec3Normalize(&normalpos1);
	//��pos1�ı���

	s[W] = 0;
	s[X] = 0;
	s[Y] = -1;
	s[Z] = 0;
	/*
	//��ת90��
	e[W] = 1;
	e[X] = 0;
	e[Y] = 1;
	e[Z] = 0;
	*/
	/*
	//��ת45��
	e[W] = 1;
	e[X] = 0.707;
	e[Y] = 0.707;
	e[Z] = 0;
	*/
	/*
	e[W] = 1;
	e[X] =  0.707;
	e[Y] = -0.707;
	e[Z] = 0;
	*/
	e[W] = 1;
	e[X] =  -1;
	e[Y] = 0;
	e[Z] = 0;
	Quat_slerp(s,e,value,o);//��Ԫ������ƽ����ֵ
	
	
	//printf("t = %.2f\tw = %.2f\tx = %.2f\ty = %.2f\tz = %.2f\n",value,o[W],o[X],o[Y],o[Z]);
	
	//ת��Ϊ����
	
	Quat_to_matrrix(o,m);

	//��ӡ����
	//mat4x4_printf("��Ԫ������",m);
	
	base_setPos(base_get2(n),o[X] * len,o[Y] * len,o[Z] * len);//����������
	
	//vec3Sub()
	subpos1.x = o[X] - normalpos1.x;
	subpos1.y = o[Y] - normalpos1.y;
	subpos1.z = o[Z] - normalpos1.z;


	//=====================================================
	//������
	axis.x = axis.y = 0;axis.z = 1.0;

	vec3Set(&down,o[X],o[Y],o[Z]);      

	//��ȡ��ֱ��
	vec3Cross(&down,&axis,&outDirection);//��˻�ȡ��ֱ����
	//f_print_vec(&normalpos1);
	f_print_vec("outDirection",&outDirection);
	//printf("\n");


	//=====================================================
	{
		                                                                                                                                                                                                     
		////Ŀ������
		//struct Vec3 target;
		//struct Vec3 c;
		//c.x = 0;c.y = 0;c.z = 1;
		//vec3Cross(&normalpos1,&c,&target);//��˻�ȡ��ֱ����
		//
		
		//base_setPos(base_get2(n1),
		//	(subpos1.x)*n1Len,
		//	(subpos1.y)*n1Len,
		//	(subpos1.z)*n1Len);
		

		//subpos1.x = subpos1.x + e[X] ;
		//subpos1.y = subpos1.y + e[Y] ;
		//subpos1.z = subpos1.z + e[Z] ;
		
		
		/*base_setPos(base_get2(n1),
			(pos1->x),
			(pos1->y),
			(pos1->z));*/
		//base_set_position(b,&normalpos1);
		//vec3Mult(&subpos1,-1);

		//base_set_position(b,&subpos1);//	subpos1	,	normalpos1
		 {
			 //float f = vecDot(&normalpos1,&down);
			 //double d = f;
			 //printf("%f\n",f/*(1-f) * 180*/);//�����ֵ

			 struct Vec3 oa;
			 struct Vec3 oc;
			 struct Vec3 od;
			 struct Vec3 oe;
			 vec3Set(&oa,s[X],s[Y],s[Z]);
			 //vec3Set(&oc,e[X],e[Y],e[Z]);
			 //vec3Sub(&oc,&oa,&od);
			 //
			 ////vec3Add(&od,&normalpos1,&oe);
			 //vec3Normalize(&od);
			 //base_set_position(b,&od);
			
			 base_set_position(base_get2(n1),&oa);
			 f_print_vec("oa",&oa);
			 //vec3Cross(&oa,&c,&target);

		 }
		
	}
	
	return 0;
}