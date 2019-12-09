//#pragma warning(disable:4996) 

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "str.h"
#include "tlgl.h"
#include "md5.h"
#include "tmat.h"
#include "tools.h"
#include "gettime.h"
#include "tl_malloc.h"
#include "base.h"
//=========================================================================


 
struct MD5_model{
	char version[G_CHAR_SIZE_];/*	�汾��Ϣ*/
	char commandline[G_CHAR_SIZE_];/*	������*/
	int numJoints;/*	��������*/
	int numMeshes;/*	mesh����*/
	struct md5_joint_t *baseSkel;/*	��������*/
	struct md5_mesh_t *meshes;/*	�����mesh����*/
	int max_verts;/*	mesh�б��е�mesh��󶥵���*/
	int max_tris;/*	�����������*/
};
/* �������� */
struct md5_anim_t{
	//int animType;//����,stand,walk
	char animName[G_BUFFER_64_SIZE];//������
	int num_frames;
	int num_joints;
	int frameRate;
	struct md5_joint_t **skelFrames;
	struct md5_bbox_t *bboxes;
};
/* Joint�ṹ�� */
struct md5_joint_t{
	char name[G_BUFFER_64_SIZE];
	int parent;
	Vec3_t sk_pos;
	Quat4_t orient;
};
/* Vertex�ṹ��*/
struct md5_vertex_t{
	Vec2_t st;//��������
	int ver_start; /* start weight */
	int verCount; /* weight count */
};
/* Triangle �ṹ��*/
struct md5_triangle_t{
	int index[3];
};
/* Weight Ȩ��*/
struct md5_weight_t{
	int joint;
	float bias;
	Vec3_t pos;
};
/* MD5 mesh */
struct md5_mesh_t{
	struct md5_vertex_t *vertices;
	struct md5_triangle_t *triangles;
	struct md5_weight_t *weights;
	int num_verts;
	int num_tris;
	int num_weights;
	/*������ͼ·��*/
	char shader[G_CHAR_SIZE_];
	int vertexCount;

	/*
	*	��Χ�ж�������
	*/
	float boxVert[BOX_SIZE];

	GLfloat* vbo;
};
/* Joint info */
struct joint_info_t{
	char name[G_BUFFER_64_SIZE];
	int parent;
	int flags;
	int startIndex;
};
/* Base frame joint */
struct baseframe_joint_t{
	Vec3_t pos;
	Quat4_t orient;
};
/* Animation info */
struct anim_info_t{
	int curr_frame;
	int next_frame;
	double last_time;
	double max_time;
};
/* Bounding box */
struct md5_bbox_t{
	Vec3_t min;
	Vec3_t max;
};
#define _MD5_VERSION_ "MD5Version"
#define _MD5_COMMAND_LINE_ "commandline"
#define _MD5_NUM_JOINTS_ "numJoints"
#define _MD5_NUM_MESHES_ "numMeshes"
#define _MD5_VERSION_VALUE_ 10
#define _SIGN_JOINT_ "joints {"
#define _SIGN_MESH_ "mesh {"

//----------------------------------------------------------------

//�ַ��ָ�ṹ��
struct StringSplit{
	char* dest;/*Դ�ַ���*/
	char* sign;/*Ŀ�������ַ���*/
	char* pOutstr;/*�ַ����������*/
	int pos;/*��ǰָ���ƶ�������*/
	int isEnd;/*isEnd == -1	��������,isEnd == 0������δ���ַ���ĩβ*/
};

static void 
f_strSplitLoop(struct StringSplit* sp){
	const char* dest = sp->dest;
	const char* sign = sp->sign;
	int *pos = &sp->pos;
	char* pOutstr = sp->pOutstr;

	int _l = (int)strlen(sign);
	int len,i,_sta = 0;
	char* __dest;
	int outStrLength = G_BUFFER_MAX_SIZE;	//����������ַ�����
	int cl = G_BUFFER_16_SIZE;				//��ʶ���ĳ���
	int cut = 0;
	//============================================
	__dest = (char*)dest + *pos; 
	len = (int)strlen(__dest);

	if((int)strlen(__dest) >= _l)
	{
		char tmp[G_BUFFER_16_SIZE];
		memset(tmp,0,G_BUFFER_16_SIZE);
		tl_strsub(__dest,tmp,len-_l,len-1);
		if(strcmp(tmp,sign) == 0)
		{
			//ȥ�����һ���ַ���
			len -= _l;
			cut = 1;
		}
	}

	if(strlen(__dest) < strlen(sign) || (int)strlen(sign) > cl)
	{
		printf("cmp is little than %d __dest:(%s) \n",cl,__dest);
		sp->isEnd = -1;
		return;
	}

	for(i = 0;i < len;i++)
	{
		char cmp[G_BUFFER_16_SIZE];
		if(len-i<_l)
		{
			break;
		}
		memset(cmp,0,cl);
		tl_strsub((const char*)__dest,cmp,i,i+_l-1);
		if(strcmp((const char*)cmp,sign) == 0)
		{
			if(i-_l>=outStrLength)
			{
				printf("error, one line is too long!\n");
				sp->isEnd = -1;
				return;
			}
			memset(pOutstr,0,outStrLength);
			tl_strsub(__dest,pOutstr,_sta,i-_sta-1);
			_sta = i+_l;
			*pos += _sta;
			sp->isEnd = 0;
			return;
		}
	}
	memset(pOutstr,0,outStrLength);
	if(cut == 0)
	{
		tl_strsub(__dest,pOutstr,_sta,_sta+len);
	}
	else
	{
		tl_strsub(__dest,pOutstr,_sta,_sta+len-_l);
	}
	sp->isEnd = -1;
}

static void 
f_strSplitInit(struct StringSplit* p,const char* dest,const char* sign,char* pOutstr){
	p->dest = (char*)dest;
	p->sign = (char*)sign;
	p->pos = 0;
	p->isEnd = 0;
	p->pOutstr = pOutstr;
}
/*
	��һ֡
	setframe:	�Զ������ùؼ�֡��ֵ,Ĭ�ϴ�1��ʼ
*/
static void anim_nextFrame(struct md5_anim_t *anim,struct anim_info_t *animInfo,double dt,int customFrame){
	int maxFrames = anim->num_frames - 1;

	animInfo->last_time += dt;

	if(customFrame == -1){
		animInfo->curr_frame++;
		animInfo->next_frame++;
	}else{
		animInfo->curr_frame = animInfo->next_frame = customFrame;
	}
	
	animInfo->last_time = 0.0;
	if (animInfo->curr_frame > maxFrames){
		animInfo->curr_frame = 0;
	}
	if (animInfo->next_frame > maxFrames){
		animInfo->next_frame = 0;
	}
}
/*
	���Ŷ���,������fps���䲥��
*/
static void anim_play (struct md5_anim_t *anim,struct anim_info_t *animInfo, double dt){
	int maxFrames = anim->num_frames - 1;
	animInfo->last_time += dt;
	if (animInfo->last_time >= animInfo->max_time) {
		animInfo->curr_frame++;
		animInfo->next_frame++;
		animInfo->last_time = 0.0;
		if (animInfo->curr_frame > maxFrames){
			animInfo->curr_frame = 0;
		}
		if (animInfo->next_frame > maxFrames){
			animInfo->next_frame = 0;
		}
	}
}
/**
* Smoothly interpolate two skeletons
*/
static void anim_interpolateSkeletons (const struct md5_joint_t *skelA,
	const struct md5_joint_t *skelB,
	int num_joints, float interp,struct md5_joint_t* out
	)
{
	int i;
	//struct md5_joint_t *out = (struct md5_joint_t *)p;
	for (i = 0; i < num_joints; ++i){
		/* Copy parent index */
		out[i].parent = skelA[i].parent;
		/* Linear interpolation for position */
		out[i].sk_pos[0] = skelA[i].sk_pos[0] + interp * (skelB[i].sk_pos[0] - skelA[i].sk_pos[0]);
		out[i].sk_pos[1] = skelA[i].sk_pos[1] + interp * (skelB[i].sk_pos[1] - skelA[i].sk_pos[1]);
		out[i].sk_pos[2] = skelA[i].sk_pos[2] + interp * (skelB[i].sk_pos[2] - skelA[i].sk_pos[2]);
		/* Spherical linear interpolation for orientation */
		Quat_slerp (skelA[i].orient, skelB[i].orient, interp, out[i].orient);
	}
}
static void setMd5Joint(struct MD5_model * _md5,const char* _chs,int* pJointIndex){
	struct md5_joint_t *j = &_md5->baseSkel[*pJointIndex];
	if(*pJointIndex >= _md5->numJoints)
		return;
	if (sscanf_s(_chs, "%s %d ( %f %f %f ) ( %f %f %f )", j->name,G_BUFFER_64_SIZE, &j->parent, &j->sk_pos[X],&j->sk_pos[Y], &j->sk_pos[Z],&j->orient[X],&j->orient[Y], &j->orient[Z]) == 8){
		Quat_computeW (j->orient);
	}
	(*pJointIndex)++;
	//printf("%d w=%f %f %f %f\n",*pJointIndex,j->orient[W] ,j->pos[X],j->pos[Y],j->pos[Z]);
}
static void setMesh(struct MD5_model * _md5,const char* _chs,int* pCurMeshIndex){
	struct md5_mesh_t *mesh = &_md5->meshes[*pCurMeshIndex];
	int vert_index = 0,len;
	int tri_index = 0;
	int weight_index = 0;
	float fdata[4];
	int idata[3];
	struct md5_vertex_t* pVertex;
	const char* sub = strstr(_chs,"shader ");
	if(sub != 0){
		/*����mesh��ͼ��*/
		tl_setString(sub,"shader",mesh->shader);
		len = (int)strlen(mesh->shader);
		tl_strsub(mesh->shader,mesh->shader,1,len);
		mesh->shader[len-2]=0;
	}else if(sscanf_s (_chs, " numverts %d", &mesh->num_verts) == 1){
		/*��ȡmesh�������*/
		if(mesh->num_verts >0){
			mesh->vertices = (struct md5_vertex_t *)tl_malloc (sizeof (struct md5_vertex_t) * mesh->num_verts);
			
		}
		/*������󶥵���*/
		if(mesh->num_verts > _md5->max_verts){
			_md5->max_verts = mesh->num_verts;
		}
	}else if (sscanf_s (_chs, " numtris %d", &mesh->num_tris) == 1){
		/*��ȡ�����εĸ���*/
		if (mesh->num_tris > 0){
			mesh->vertexCount = mesh->num_tris * 3;
			mesh->triangles = (struct md5_triangle_t *)tl_malloc (sizeof (struct md5_triangle_t) * mesh->num_tris);
			//printf("num_verts:%d\n",mesh->num_verts);
			mesh->vbo = (GLfloat*)tl_malloc(sizeof(GLfloat) * mesh->num_verts * 8);
		}
		if (mesh->num_tris > _md5->max_tris)
			_md5->max_tris = mesh->num_tris;
	}else if (sscanf_s (_chs, " numweights %d", &mesh->num_weights) == 1){
		/*Ȩ�ظ��� ����Ȩ���ڴ�*/
		if (mesh->num_weights > 0){
			mesh->weights = (struct md5_weight_t *)tl_malloc (sizeof (struct md5_weight_t) * mesh->num_weights);
		}
	}else if (sscanf_s (_chs, " vert %d ( %f %f ) %d %d", &vert_index,&fdata[0], &fdata[1], &idata[0], &idata[1]) == 5){
		/* ���ƶ������� */
		pVertex = &mesh->vertices[vert_index];
		pVertex->st[0] = fdata[0];
		pVertex->st[1] = 1.0f - fdata[1];
		pVertex->ver_start = idata[0];
		pVertex->verCount = idata[1];

//		printf("%d,",pVertex->verCount);

	}else if (sscanf_s (_chs, " tri %d %d %d %d", &tri_index,&idata[0], &idata[1], &idata[2]) == 4){
		/* ��������������*/
		mesh->triangles[tri_index ].index[0] = idata[0];
		mesh->triangles[tri_index ].index[1] = idata[1];
		mesh->triangles[tri_index ].index[2] = idata[2];
	}else if (sscanf_s(_chs, " weight %d %d %f ( %f %f %f )", &weight_index, &idata[0], &fdata[3], &fdata[0], &fdata[1], &fdata[2]) == 6){
		/* ���ƶ���Ȩ�����ݡ� */
		mesh->weights[weight_index].joint  = idata[0];//��������
		mesh->weights[weight_index].bias   = fdata[3];//Ȩ��ֵ(0~1)
		mesh->weights[weight_index].pos[0] = fdata[0];//Ȩ������x
		mesh->weights[weight_index].pos[1] = fdata[1];//Ȩ������y
		mesh->weights[weight_index].pos[2] = fdata[2];//Ȩ������z
	}
}

#define G_SIGN_RIGHT_BRACKETS "}"

static void parseLine(char *_oneline,struct MD5_model* _md5,int* jointIndex,
	int * pCurLine,int* pCurMeshIndex){
	//printf("index:%d\n",*index);
	char _chs[G_BUFFER_MAX_SIZE];
	memset(_chs,0,G_BUFFER_MAX_SIZE);
	memcpy(_chs,_oneline,strlen(_oneline));//ɾ�������һ���ַ�'\n'
	if(strncmp(_chs,_MD5_VERSION_,strlen(_MD5_VERSION_)) == 0){
		//�汾��
		tl_setString(_chs,_MD5_VERSION_,_md5->version);
	}else if(strncmp(_chs,_MD5_COMMAND_LINE_,strlen(_MD5_COMMAND_LINE_)) == 0){
		//������
		tl_setString(_chs,_MD5_COMMAND_LINE_,_md5->commandline);
	}else if(strncmp(_chs,_MD5_NUM_JOINTS_,strlen(_MD5_NUM_JOINTS_)) == 0){
		//printf("(%s)\n",_oneline);
		//������
		tl_setInt(_oneline,_MD5_NUM_JOINTS_,&_md5->numJoints);
	}else if(strncmp(_chs,_MD5_NUM_MESHES_,strlen(_MD5_NUM_MESHES_)) == 0){
		//����θ���
		tl_setInt(_oneline,_MD5_NUM_MESHES_,&_md5->numMeshes);
		//����mesh������
		_md5->meshes = (struct md5_mesh_t*)tl_malloc(sizeof(struct md5_mesh_t) * _md5->numMeshes);
	}
	//��������
	if(_md5->baseSkel!=0 && *jointIndex<= _md5->numJoints){
		setMd5Joint(_md5,_chs,jointIndex);
	}
	//����mesh
	if(_md5->meshes!=0 && *pCurMeshIndex <= _md5->numMeshes){
		setMesh(_md5,_chs,pCurMeshIndex);
	}
	if(strncmp(_chs,_SIGN_JOINT_,8)== 0){
		//�����ڵ� ��������������
		_md5->baseSkel = (struct md5_joint_t * )tl_malloc(sizeof(struct md5_joint_t) * _md5->numJoints);
	}else if(strncmp(_chs,_SIGN_MESH_,6)== 0){
		//mesh�ڵ�
		(*pCurMeshIndex)++;
	}else if(strncmp(_chs,G_SIGN_RIGHT_BRACKETS,1)== 0){
		//	printf("end\n");
	}
}

/*
*	���ƹ����ڵ�
*/
void md5_drawSkeleton (void* ptr)
{	
	struct MD5* _md5= (struct MD5*)ptr;
	struct HeadInfo* base=(struct HeadInfo*)_md5->base;
	struct md5_joint_t *skeleton = (struct md5_joint_t*)_md5->skeleton;
	struct MD5_model* model = (struct MD5_model*)_md5->model;
	int num_joints = model->numJoints;
	
	//Vec3_t offset_pos;
	//����
	float scale = base->scale;//base->scale*base->scale;
	int i;
	
	if(!getv(&base->flags,FLAGS_RENDER_DRAWSKELETON))
	{
		return;
	}
	//printf("%f,%f,%f\n",base->x,base->y,base->z);
	//tl_set_vec(offset_pos,base->x,base->y,base->z);

	/* ����ÿ������ */
	glBindTexture(GL_TEXTURE_2D,(GLuint)0);
	glPointSize (5.0f);
	glColor3f (1.0f, 0.0f, 0.0f);
	glBegin (GL_POINTS);
	//glTranslatef(base->x,base->y,base->z);
	for (i = 0; i < num_joints; ++i){
		
		Vec3_t pos;
		tl_scale_vec(skeleton[i].sk_pos,pos,scale);
		
		tl_offset_vec(pos,base->x,base->y,base->z);

		glVertex3fv (pos);
	
		
	}
	glEnd ();
	glPointSize (1.0f);

	/* ����ÿ������ */
	glColor3f (1.0f, 1.0f, 0.0f);
	glBegin (GL_LINES);
	for (i = 0; i < num_joints; ++i){
		int index = skeleton[i].parent;
		if (index != -1 && index < num_joints && index > 0){
			Vec3_t start,end;
			tl_scale_vec(skeleton[index].sk_pos,start,scale);
			tl_scale_vec(skeleton[i].sk_pos,end,scale);
			tl_offset_vec(start,base->x,base->y,base->z);
			tl_offset_vec(end,base->x,base->y,base->z);

			glVertex3fv (start);
			glVertex3fv (end);
		}
	}
	glEnd();
}
//static int curMax = 0;
static void 
buildArrays (struct MD5* _md5,struct md5_mesh_t *mesh,
	const struct md5_joint_t *skeleton){
	//MD5_Data* _md5 = this;
	int i,j,k,n;

	// ���ö������� 
	for (k = 0, i = 0; i < mesh->num_tris; ++i){
		for (j = 0; j < 3; ++j, ++k){
			_md5->vertexIndices[k] = mesh->triangles[i].index[j];
		}
	}

	// ���ö������� 
	for (i = 0; i < mesh->num_verts; ++i){
		Vec8_t _fVert ;
		struct md5_vertex_t* vert =  &(mesh->vertices[i]);
		memset(_fVert,0,sizeof(Vec8_t));
		//���uv	_fVert[0],_fVert[1]
		memcpy(_fVert,vert->st,sizeof(Vec2_t));

		//normal����
		_fVert[2] = 0.0f;
		_fVert[3] = 0.0f;
		_fVert[4] = 0.0f;		

		// ����Ȩ��ֵ���������ƵĶ���
		
		for (j = 0; j < mesh->vertices[i].verCount; ++j){
			int index = mesh->vertices[i].ver_start;
			
			const struct md5_weight_t *weight= &mesh->weights[index + j];
			const struct md5_joint_t *joint= &skeleton[weight->joint];
			//����Ȩ�ؼ���������λ��
			Vec3_t wv;			

			//����
			float scale = ((struct HeadInfo *)_md5->base)->scale;

			Quat_rotatePoint (joint->orient, weight->pos, wv);
			//���е�Ȩ��ֵ�ܺ�Ϊ1.0
			_fVert[5] += (joint->sk_pos[0] + wv[0]) * weight->bias * scale;
			_fVert[6] += (joint->sk_pos[1] + wv[1]) * weight->bias * scale;
			_fVert[7] += (joint->sk_pos[2] + wv[2]) * weight->bias * scale;
		}

		//����GL_T2F_C3F_V3F�������ڹ�����Ⱦ,_md5->vertexOut[i] = finalVertex
		memcpy(&(_md5->vertexOut[i]),_fVert,sizeof(Vec8_t));
	}
	//��_md5->trianglesArray���и�ֵ
	for(n = 0;n < mesh->vertexCount;n++){
		memcpy(_md5->trianglesArray[n],_md5->vertexOut[_md5->vertexIndices[n]],sizeof(Vec8_t));
	}
}
/*
	���䲥�Ŷ���
*/
static void 
anim_adaptivePlay(struct MD5* p,int fps,double curent_time){
	struct md5_anim_t* md5anim = (struct md5_anim_t*)p->curAnim;
	struct anim_info_t* animInfo = (struct anim_info_t*)p->animInfo;
	struct MD5_model* _model = (struct MD5_model*)(p->model);
	struct md5_joint_t* skeleton = (struct md5_joint_t*)p->skeleton;
	int *frameTicket = &p->frameTicket;
	
	if(!md5anim){
		return;
	}

	(*frameTicket)++;
	//if((*frameTicket) <= fps / md5anim->frameRate){
	if((*frameTicket) <= fps ){
		if (animInfo->curr_frame > md5anim->num_frames-1){
			animInfo->curr_frame = 0;
		}
		if (animInfo->next_frame > md5anim->num_frames-1){
			animInfo->next_frame = 0;
		}
	}else{
		(*frameTicket) = 0;
		anim_nextFrame( md5anim,animInfo,curent_time,p->custframe);
	}


}
static void 
anim_VertexArray(struct MD5* p,int fps,double curent_time)
{
	struct HeadInfo* base = base_get((void*)p);
	struct md5_anim_t* md5anim = (struct md5_anim_t*)p->curAnim;
	struct anim_info_t* animInfo = (struct anim_info_t*)p->animInfo;
	struct MD5_model* _model = (struct MD5_model*)(p->model);
	struct md5_joint_t* skeleton = (struct md5_joint_t*)p->skeleton;
	//==========================================================
	if(!md5anim)
		return;
	
	if(getv(&base->flags,FLAGS_ANIM_ADAPTIVE)){
		if(fps > 0){
			anim_adaptivePlay(p,fps,curent_time);		//����
		}else{
			// fps = 0�����йؼ�֡�����л�
		}
	}
	else
	{
		//==========    ������BUG    ========================
		anim_play( md5anim,animInfo,curent_time);	//������ؼ�֡�Ķ���
	}
	
	anim_interpolateSkeletons(
		md5anim->skelFrames[animInfo->curr_frame],
		md5anim->skelFrames[animInfo->next_frame],
		md5anim->num_joints,
		animInfo->last_time * md5anim->frameRate,
		skeleton);
}

static int CallBack_delAnimNode(char* parms){
	int* point = (int*)parms;
	struct md5_anim_t* anim = (struct md5_anim_t*)point[0];
	int i;
	tl_free(anim->bboxes);
	for(i = 0;i < anim->num_frames;i++){
		tl_free(anim->skelFrames[i]);
	}
	tl_free(anim->skelFrames);
	tl_free(anim);
	return 0;
}
static void f_parseInit(struct MD5_model* _md5){
	//memset(_md5->md5MeshName,0,G_BUFFER_128_SIZE);
	memset(_md5->version,0,G_CHAR_SIZE_);
	memset(_md5->commandline,0,G_CHAR_SIZE_);
	_md5->numJoints = 0;
	_md5->numMeshes = 0;
	_md5->max_verts = 0;
	_md5->max_tris = 0;
	_md5->baseSkel = 0;
	_md5->meshes = 0;
}

int callBack_Transform(char* _p)
{
	struct MD5* p = (struct MD5*)_p;
	//f_rotateMd5((struct MD5*)_p);
	struct HeadInfo* _base = (struct HeadInfo*)p->base;
	
	glLoadIdentity ();//���õ�ǰָ���ľ���Ϊ��λ����.
	glTranslatef (_base->x,_base->y,_base->z);
	glScalef(_base->scale,_base->scale,_base->scale);
	//glRotatef (-90.0f, 1.0, 0.0, 0.0);
	//glRotatef (p->_curtime / 100, 0.0, 0.0, 1.0);
	
	return 0;
}

/**
* Check if an animation can be used for a given model.  Model's
* skeleton and animation's skeleton must match.
*/
static int anim_CheckAnimValidity(struct MD5_model *mdl,struct md5_anim_t *anim){
	int i;
	/* md5mesh and md5anim must have the same number of joints */
	if (mdl->numJoints != anim->num_joints)
		return 0;

	/* We just check with frame[0] */
	for (i = 0; i < mdl->numJoints; ++i){
		/* Joints must have the same parent index */
		if (mdl->baseSkel[i].parent != anim->skelFrames[0][i].parent){
			return 0;
		}
		/* Joints must have the same name */
		if (strcmp (mdl->baseSkel[i].name, anim->skelFrames[0][i].name) != 0){
			return 0;
		}
	}
	return 1;
}
/**
* Build skeleton for a given frame data.
*/
static void BuildFrameSkeleton (const struct joint_info_t *jointInfos,
	const struct baseframe_joint_t *baseFrame,
	const float *animFrameData,
struct md5_joint_t *skelFrame,
	int num_joints)
{
	int i;
	int parent;
	struct md5_joint_t *parentJoint;
	Vec3_t animatedPos;
	Quat4_t animatedOrient;
	int j = 0;
	const struct baseframe_joint_t *baseJoint;
	struct md5_joint_t *thisJoint;
	Vec3_t rpos; /* Rotated position */
	for (i = 0; i < num_joints; ++i){
		baseJoint = &baseFrame[i];
		j = 0;

		memcpy (animatedPos, baseJoint->pos, sizeof (Vec3_t));
		memcpy (animatedOrient, baseJoint->orient, sizeof (Quat4_t));

		if(jointInfos[i].flags & 1) /* Tx */{
			animatedPos[0] = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if(jointInfos[i].flags & 2) /* Ty */{
			animatedPos[1] = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if(jointInfos[i].flags & 4) /* Tz */{
			animatedPos[2] = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if(jointInfos[i].flags & 8) /* Qx */{
			animatedOrient[0] = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if(jointInfos[i].flags & 16) /* Qy */{
			animatedOrient[1] = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		if(jointInfos[i].flags & 32) /* Qz */{
			animatedOrient[2] = animFrameData[jointInfos[i].startIndex + j];
			++j;
		}

		/* Compute orient quaternion's w value */
		Quat_computeW (animatedOrient);

		/* NOTE: we assume that this joint's parent has
		already been calculated, i.e. joint's ID should
		never be smaller than its parent ID. */
		thisJoint = &skelFrame[i];

		parent = jointInfos[i].parent;
		thisJoint->parent = parent;
		//strcpy (thisJoint->name, jointInfos[i].name);
		strcpy_s(thisJoint->name,strlen(jointInfos[i].name) + 1,jointInfos[i].name);

		/* Has parent? */
		if (thisJoint->parent < 0){
			memcpy (thisJoint->sk_pos, animatedPos, sizeof (Vec3_t));
			memcpy (thisJoint->orient, animatedOrient, sizeof (Quat4_t));
		}else{
			parentJoint = &skelFrame[parent];


			/* Add positions */
			Quat_rotatePoint (parentJoint->orient, animatedPos, rpos);
			thisJoint->sk_pos[0] = rpos[0] + parentJoint->sk_pos[0];
			thisJoint->sk_pos[1] = rpos[1] + parentJoint->sk_pos[1];
			thisJoint->sk_pos[2] = rpos[2] + parentJoint->sk_pos[2];

			/* Concatenate rotations */
			Quat_multQuat (parentJoint->orient, animatedOrient, thisJoint->orient);
			Quat_normalize (thisJoint->orient);
		}
	}
}

/*
	����joint����
*/
static void parseJoints(char* str,struct joint_info_t* pJoint){
	int pos,len;
	char _temp[G_BUFFER_256_SIZE];
	memset(_temp,0,G_BUFFER_256_SIZE);
	memset(pJoint->name,0,G_BUFFER_64_SIZE);
	pos = str_pos(str,"\"\t",1);
	if(pos == -1){
		printf("pos = %d(%s)\n",pos,str);
		assert(0);
	}
	len = (int)strlen("\"\t");
	memcpy(pJoint->name,str+len,pos-len);
	len = (int)strlen("\t");
	memcpy(_temp,str+pos+len,strlen(str)-pos-len);	
	sscanf_s(_temp, "%d %d %d", &(pJoint->parent), &(pJoint->flags),&(pJoint->startIndex));
	//printf("[%s] %d %d %d \n",pJoint->name,pJoint->parent,pJoint->flags,pJoint->startIndex);
}
/*
	���frame�ؼ�֡����
*/
static void parseFrame(const char* data,int* curPos,int offset,float *animFrameData){
	struct StringSplit sp;
	char b[G_BUFFER_MAX_SIZE];
	int	index=0;
	char _out[G_BUFFER_8192_SIZE];
	if(offset > G_BUFFER_8192_SIZE)
	{
		//���_out�Ŀռ��Ƿ���
		printf("memory is bigger,check out need length (%d)\n",offset);
		return;
	}
	memset(_out,0,offset);
	memcpy(_out,data+*curPos,offset-1);
	
	f_strSplitInit(&sp,_out," ",b);

	while(sp.isEnd!=-1)
	{
		f_strSplitLoop(&sp);
		if(((int)strlen(b) == 1) && (b[0] == 13 || (strcmp(b,"\n") == 0))){
		}else{
			sscanf_s(b,"%f",&animFrameData[index]);
			index++;
		}
	}
}
/*
	���ö�����
*/
static void
setAnimName(struct md5_anim_t *anim,char* animName)
{
	if(strlen(animName)>G_BUFFER_64_SIZE){
		printf("������[%s]̫��\n",animName);
		assert(0);
	}
	memset(anim->animName,0,G_BUFFER_64_SIZE);
	memcpy(anim->animName,animName,strlen(animName));
}

/**
* (anim)Load an MD5 animation from file.
*/
static void 
animRead(const char *filename,char* animName, struct md5_anim_t *anim)
{
	//FILE* fp = NULL;
	char* _animData = 0;
	char buff[G_BUFFER_MAX_SIZE];
	struct joint_info_t *jointInfos = NULL;
	struct baseframe_joint_t *baseFrame = NULL;
	float *animFrameData = NULL;
	int version;
	int numAnimatedComponents;
	int frame_index;
	int i;
	struct StringSplit _sp;
	
	setAnimName(anim,animName);

	_animData=tl_loadfile(filename,NULL);
	
	printf("md5Anim:%s,len:%d kb\n",anim->animName,(int)strlen(_animData)/1024);

	f_strSplitInit(&_sp,_animData,"\n",buff);

	while(_sp.isEnd!=-1)
	{
		f_strSplitLoop(&_sp);

		if (sscanf_s (buff, " MD5Version %d", &version) == 1)
		{
			if (version != 10)
			{
				/* Bad version */
				fprintf (stderr, "Error: bad animation version\n");
				//fclose (fp);
				return;
			}
		}
		else if (sscanf_s (buff, " numFrames %d", &anim->num_frames) == 1)
		{
			/* Allocate memory for skeleton frames and bounding boxes */
			if (anim->num_frames > 0)
			{
				anim->skelFrames = (struct md5_joint_t **)tl_malloc (sizeof (struct md5_joint_t*) * anim->num_frames);
				anim->bboxes = (struct md5_bbox_t *)tl_malloc (sizeof(struct md5_bbox_t)*anim->num_frames);
			}
		}
		else if (sscanf_s (buff, " numJoints %d", &anim->num_joints) == 1)
		{
			if (anim->num_joints > 0)
			{
				for (i = 0; i < anim->num_frames; ++i)
				{
					/* Allocate memory for joints of each frame */
					anim->skelFrames[i] = (struct md5_joint_t *)
						tl_malloc (sizeof (struct md5_joint_t) * anim->num_joints);
				}

				/* Allocate temporary memory for building skeleton frames */
				jointInfos = (struct joint_info_t *)
					tl_malloc (sizeof (struct joint_info_t) * anim->num_joints);

				baseFrame = (struct baseframe_joint_t *)
					tl_malloc (sizeof (struct baseframe_joint_t) * anim->num_joints);
			}
		}
		else if (sscanf_s (buff, " frameRate %d", &anim->frameRate) == 1)
		{

			/*
			printf ("md5anim: animation's frame rate is %d\n", anim->frameRate);
			*/
		}
		else if (sscanf_s (buff, " numAnimatedComponents %d", &numAnimatedComponents) == 1)
		{
			if (numAnimatedComponents > 0)
			{
				/* Allocate memory for animation frame data */
				animFrameData = (float *)tl_malloc (sizeof (float) * numAnimatedComponents);
			}
		}
		else if (strncmp (buff, "hierarchy {", 11) == 0)
		{
			for (i = 0; i < anim->num_joints; ++i)
			{
				f_strSplitLoop(&_sp);
				
				/* Read joint info */
				parseJoints(buff,&jointInfos[i]);
			}

		}
		else if (strncmp (buff, "bounds {", 8) == 0)
		{
			for (i = 0; i < anim->num_frames; ++i)
			{
				f_strSplitLoop(&_sp);

				/* Read bounding box */
				sscanf_s (buff, " ( %f %f %f ) ( %f %f %f )",
					&anim->bboxes[i].min[0], &anim->bboxes[i].min[1],
					&anim->bboxes[i].min[2], &anim->bboxes[i].max[0],
					&anim->bboxes[i].max[1], &anim->bboxes[i].max[2]);
			}
		}
		else if (strncmp (buff, "baseframe {", 10) == 0)
		{
			for (i = 0; i < anim->num_joints; ++i)
			{
				f_strSplitLoop(&_sp);

				/* Read base frame joint */
				if (sscanf_s (buff, " ( %f %f %f ) ( %f %f %f )",
					&baseFrame[i].pos[0], &baseFrame[i].pos[1],
					&baseFrame[i].pos[2], &baseFrame[i].orient[0],
					&baseFrame[i].orient[1], &baseFrame[i].orient[2]) == 6)
				{
					/* Compute the w component */
					Quat_computeW (baseFrame[i].orient);
				}
			}
		}
		else if (sscanf_s (buff, " frame %d", &frame_index) == 1)
		{
			
			int _len = str_pos(_animData+_sp.pos,"}",1);//��ѯ������"}"��λ��

			parseFrame(_animData,&_sp.pos,_len,animFrameData);

			_sp.pos+=_len;

			/* Build frame skeleton from the collected data */
			BuildFrameSkeleton (jointInfos, baseFrame, animFrameData,anim->skelFrames[frame_index], anim->num_joints);
		}
	}

	//fclose (fp);

	/* Free temporary data allocated */
	if (animFrameData){
		tl_free (animFrameData);
	}
	if (baseFrame){
		tl_free (baseFrame);
	}
	if (jointInfos){
		tl_free (jointInfos);
	}
	if(_animData!=NULL){
		tl_free(_animData);
		_animData = NULL;
	}
}

void md5_loadAnim(struct MD5* p, const char* animFile,const char* animName){
	//MD5_Data* p = this;
	struct md5_anim_t* pAnim = NULL;
	struct anim_info_t* animInfo = NULL;
	struct md5_joint_t * skeleton = NULL;
	if(p->pAnimList == NULL){
		pAnim = (struct md5_anim_t*)tl_malloc(sizeof(struct md5_anim_t));
		memset(pAnim,0,sizeof(struct md5_anim_t));

		animInfo = (struct anim_info_t*)tl_malloc(sizeof(struct anim_info_t));
		p->pAnimList = (struct LStackNode*)LStack_create();
		animRead(animFile,(char*)animName,pAnim);
		animInfo->curr_frame = 0;
		animInfo->next_frame = 1;

		animInfo->last_time = 0;
		animInfo->max_time = 1000.0f / pAnim->frameRate;

		skeleton = (struct md5_joint_t *)tl_malloc (sizeof (struct md5_joint_t) * pAnim->num_joints);//������
		p->curAnim = pAnim;
		p->animInfo = animInfo;
		p->skeleton = skeleton;
		LStack_push(p->pAnimList,pAnim);
	}else{
		pAnim=(struct md5_anim_t*)tl_malloc(sizeof(struct md5_anim_t));
		animRead(animFile,(char*)animName,pAnim);
		LStack_push(p->pAnimList,pAnim);
	}
}
static void f_md5_parse_mesh(struct MD5* out,const char* meshData){
	//����ģ������
	int pos=0,_startTime,curLineNum=0,i=0;
	int jointIndex= 0;		/*������ǰ������*/
	int curMeshIndex = -1;	/*��ǰ�Ĺ�����*/
	struct MD5_model* _md5;
	char _oneline[G_BUFFER_MAX_SIZE];
	int meshLength = (int)strlen(meshData);//mesh�����ݳ���

	out->pAnimList = NULL;
	_startTime = get_longTime();
	_md5 = (struct MD5_model*)tl_malloc(sizeof(struct MD5_model));
	f_parseInit(_md5);
	memset(_oneline,0,G_BUFFER_MAX_SIZE);
	for(i = 0;i < meshLength;i++){
		if(meshData[i]== '\n'){
			curLineNum++;
			memcpy(_oneline,meshData+i-pos,pos);
			if(strlen(_oneline)>0){
				parseLine(_oneline,_md5,&jointIndex,&curLineNum,&curMeshIndex);
			}
			memset(_oneline,0,G_BUFFER_MAX_SIZE);
			pos = 0;
		}else{
			pos++;
		}
	}
	//����md5�������
	//vec3 * mesh�б������Ķ������
	out->vertexArray = (Vec3_t *)tl_malloc (sizeof (Vec3_t) * _md5->max_verts);
	//����t2f_c3f_v3f�ṹ������
	out->vertexOut = (Vec8_t *)tl_malloc(sizeof(Vec8_t) * _md5->max_tris * 3);
	//���������黺����
	out->trianglesArray = (Vec8_t * )tl_malloc(sizeof(Vec8_t) * _md5->max_tris * 3);
	//mesh�б������������θ��� * 3 * GLuint���ֽ���
	out->vertexIndices = (GLuint *)tl_malloc (sizeof (GLuint) * _md5->max_tris * 3);
	out->model = (void*)_md5;
	//f_Dispose(out);

	/*
	printf(	"parse file id = %d (%d bytes) usd (%d) ms \n"
		"version:		%s \n"
		"numMeshes:		%d\n"
		"numJoints:		%d\n"
		//"num_frames:		%d\n"
		"max_verts:		%d\n"
		"max_tris:		%d\n"
		,
		out->id,
		meshLength,get_longTime()-_startTime,//�����ļ���ʱ������
		_md5->version,_md5->numMeshes,_md5->numJoints,_md5->max_verts,
		_md5->max_tris
		);
	printf("load and parse md5 file:(%d) used (%d)ms \n\n",out->id,get_longTime()-_startTime);
	*/
}

//��ӡmd5ģ����Ϣ
static void showMd5Info(char* _md5MeshData,const char* path)
{
	char meshName[G_BUFFER_32_SIZE];
	tl_getPathName(path,meshName);//	Լ��md5��Ψһ��ʾ��*.md5mesh����������
	printf("(%s md5Mesh:%s,%d bytes)\n",path,meshName,(int)strlen(_md5MeshData));
}

void md5_loadMesh(struct MD5* p,const char* path){
	char* _md5MeshData = NULL;
	int length;
	
	//char suffix[G_BUFFER_16_SIZE];
	struct md5_anim_t* pAnim=NULL;
	struct anim_info_t* animInfo=NULL;
	struct md5_joint_t *skeleton = NULL;
	struct HeadInfo* base = (struct HeadInfo*)p->base;
	p->frameTicket = 0;

	//���õ�ǰ����
	//memset(suffix,0,G_BUFFER_16_SIZE);
	//tl_getSuffixByPath((char*)path,suffix,G_BUFFER_16_SIZE);

	//base->curType = TYPE_MD5_FILE;
		
		//tl_typeStr(FILE_FORMAT_STR,suffix);

	//base_set_suffix((struct HeadInfo*)p->base,path);

	//=========����mesh
	_md5MeshData=tl_loadfile(path,&length);
	
	f_md5_parse_mesh(p,_md5MeshData);
	
	//showMd5Info(_md5MeshData,path);

	tl_free(_md5MeshData);
	_md5MeshData = NULL;
}
/*
	���ݶ�������ȡ�����ṹ
*/
static struct md5_anim_t* 
getAnimByName(void* _md5,const char* animName)
{
	struct MD5* md5 = (struct MD5*)_md5;
	void *top;
	void *p;
	int data;
	int isChange = 0;
	struct md5_anim_t* cur;
	struct md5_anim_t* res = NULL;
	void* s = (void*)md5->pAnimList;
	if(s != NULL)
	{
		top = s;
		p=top;
		while((int)LStack_next(p)){
			p=(void*)LStack_next(p);
			data = LStack_data(p);
			cur = (struct md5_anim_t*)data;
			//if(animType == cur->animType){
			if(!strcmp(animName,cur->animName)){
				isChange = 1;
				res = cur;
				break;
			}
		}
		if(!isChange){
			//δ�ҵ��˶���
			printf("�޷��ҵ�����:%s\n",animName);
			assert(0);
		}
	}
	return res;
}


void md5_setAnim(void* _md5,const char* animName){
	struct MD5* md5 = (struct MD5*)_md5;
	struct md5_anim_t* res = getAnimByName(_md5,animName);//getAnimType(_md5,animType);
	if(res!=NULL){
		md5->curAnim = res;
	}
}

int md5_getFrameIndex(void* _md5){
	struct MD5* md5 = (struct MD5*)_md5;
	return ((struct anim_info_t*)(md5->animInfo))->curr_frame;
}

int md5_getTotalFrame(void* p){
	struct md5_anim_t* md5anim = (struct md5_anim_t*)((struct MD5*)p)->curAnim;
	return md5anim->num_frames;
}

int md5_getFrameNum(void* _md5,char* animName){
	int frame = 0;
	struct MD5* md5 = (struct MD5*)_md5;
	struct md5_anim_t* res = getAnimByName(_md5,animName);//getAnimType(_md5,animType);
	if(res!=NULL){
		frame = res->num_frames;
	}
	return frame;
}

void md5_render(struct MD5* md5,int fps){
	int i;
	struct MD5_model* _model = (struct MD5_model*)(md5->model);

	struct HeadInfo* base = base_get((void*)md5);

	double curtime = (double)get_longTime()*0.001;//tlgl_getCurTime();
	
	//printf("c=%.3f,",curtime);
	
	//����fps��ȡ��ǰ�Ĺؼ�֡��������
	anim_VertexArray(md5,fps,curtime);

	//����ģ�Ͷ���
	for (i = 0; i < _model->numMeshes; ++i){

		struct md5_joint_t* _md5_joint_t = (struct md5_joint_t*)md5->skeleton;
		struct md5_mesh_t* _mesh = &(_model->meshes[i]);

		struct VertexData* p=(base->rData);

		if(_md5_joint_t)
		{
			p->vertex = (GLfloat*)md5->trianglesArray;
			p->vertLen= _mesh->vertexCount;

			//��ȡ�ڵ�mesh ��������,�����ж��mesh
			buildArrays(md5,_mesh,_md5_joint_t);

			//ȡ��һ֡Ϊ����ʰȡ����
			//if(!base->rayVertexData.vertex)
			//{
				//base_createRayVertex(&base->rayVertexData,p->vertex,p->vertLen,base->scale);
			//}

			//==================================
			base_renderFill(base);
			//==================================
		}
	}
}
void md5_dispose(struct MD5* out){
	struct MD5_model* mdl;
	int i;
	int param[1];
	param[0] = 0;
	mdl = (struct MD5_model*)out->model;
	if (mdl->baseSkel){
		tl_free (mdl->baseSkel);
	}
	LStack_IErgodic_S(out->pAnimList,CallBack_delAnimNode,param);
	LStack_delete((struct LStackNode*)out->pAnimList);

	for (i = 0; i < mdl->numMeshes; ++i){
		struct md5_mesh_t* _mesh = &mdl->meshes[i];
		tl_free(_mesh->vbo);
		tl_free (_mesh->vertices);
		tl_free (_mesh->triangles);
		tl_free (_mesh->weights);
	}
	
	tl_free (mdl->meshes);
	tl_free(out->skeleton);
	tl_free(out->animInfo);
	tl_free(out->vertexArray );
	tl_free(out->vertexOut);
	tl_free(out->trianglesArray);
	tl_free(out->vertexIndices);
	tl_free(out->base);
	tl_free(mdl);
	tl_free(out);
}