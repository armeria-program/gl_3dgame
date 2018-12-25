


flags={}

flags.FLAGS_RENDER_BOUND_BOX	=	0x01
flags.FLAGS_DRAW_RAY_COLLISION	=	0x02	--�������ߺ���	(�Ƿ�������߾�̬��Χ��,���ڲ��� �鿴���߰�Χ��),��ʹû������FLAGS_RAY,��ôҲ�ǿ��Ի������ߺ��ӵ�,�������������鿴
flags.FLAGS_LINE_RENDER			=	0x04	--�Ƿ���ʾ��Ⱦ�߿�
flags.FLAGS_OUTLINE				=	0x08	--�Ƿ���ʾ��Ե����
flags.FLAGS_RAY					=	0x10	--16�Ƿ��������ʰȡ����(��ֻ����3dʰȡ,2dʰȡ����������)
flags.FLAGS_VISIBLE				=	0x20	--32��ʾ��������ģ��,(�Ƿ������Ⱦ������)
flags.FLAGS_RENDER_DRAWSKELETON	=	64		--��Ⱦ�����ڵ�(md5ģ�ͲŻ���ʾ)
flags.FLAGS_ANIM_ADAPTIVE		=	128		--�Ƿ���ݹؼ�֡��֡������ؼ�֡�Ķ���(�Ƿ�����fps 1,����  0,������)
flags.FLAGS_GLSL_OUTLINE		=	256		--��GLSLʵ�ֵ�������
flags.FLAGS_DRAW_PLOYGON_LINE	=	512		--�ڵ�һ���������������߿���Ⱦ(�̶�����ģʽ����GL_LINE)
flags.FLAGS_BUTTON_EFFECT		=	1024	--���ð�ť��Ч(���е���¼���ǰ���²Ż��и���Ч)
flags.FLAGS_DRAW_NORMAL 		= 	2048	--����
flags.FLAGS_DRAG				=	4096	--�Ƿ����ק
flags.FLAGS_DRAW_PLOYGON_POINT	= 	8192	--��ģʽ


flags.SHADER_FLAG_FLAT_COLOR	=0x01

-------------------------------------------------------

--��ֵ�洢���еĶ�������
instance={}


--[[
	������ת��ַ
	0EA02968
--]]
function tableToInt(t)
	_l = string.len("table:  ");
	s=string.sub(tostring(t),_l,-1);
	return s;
end
--#################################################################
dofile("..\\include\\lua\\core.lua")
--#################################################################
--------------------------------------------------------
function func_bindInstance(sc)
	local s = tableToInt(sc)
	
	--print("***********************"..s)
	instance[s] = sc;
	return s;
end

--��ȡ����
function func_getInstance(name)
	return instance[name]
end

--��ȡһ��������
function func_convertBgName(s)
	return s.."_bg"
end

function str_cut_decimal(f,c)
	local s =  tostring(f);
	local i = string.find(s,".")+1
	
	return string.sub(s,0,i+c)--����С�������cλ
end

--��ȡ��������
function tl_getIntegralPart(x)
	if x <= 0 then
	   return math.ceil(x);
	end

	if math.ceil(x) == x then
	   x = math.ceil(x);
	else
	   x = math.ceil(x) - 1;
	end
	return x;
end


xfun = {}
--���������ҽڵ�
function xfun.FindNode(name)
	return dofunc("FindNode",name);
end
function xfun.RotateX(obj,v)
	change_attr(obj,"rx",v)
end
function xfun.RotateZ(obj,v)
	change_attr(obj,"rz",v)
end

function xfun.Scale(obj,v)
	dofunc("SetScale",obj,v);
end

function xfun.GetName(obj)
	return dofunc("GetName",obj)
end

--������Ӱ��������
function
xfun.SetCamera(x,y,z)
		dofunc("SetCameraPosition",x,y,z);
end

