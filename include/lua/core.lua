CONST_DIRECTION_HORIZONTAL = 0	--ˮƽ,��
CONST_DIRECTION_VERTICAL   = 1 	--��ֱ,��

CONST_PI	=	3.1415929
PI	= 3.1415929

FLAGS_RENDER_BOUND_BOX	=	0x01
FLAGS_DRAW_RAY_COLLISION	=	0x02	--�������ߺ���	(�Ƿ�������߾�̬��Χ��,���ڲ��� �鿴���߰�Χ��),��ʹû������FLAGS_RAY,��ôҲ�ǿ��Ի������ߺ��ӵ�,�������������鿴
FLAGS_LINE_RENDER			=	0x04	--�Ƿ���ʾ��Ⱦ�߿�
FLAGS_OUTLINE				=	0x08	--�Ƿ���ʾ��Ե����
FLAGS_RAY					=	0x10	--16�Ƿ��������ʰȡ����(��ֻ����3dʰȡ,2dʰȡ����������)
FLAGS_VISIBLE				=	0x20	--32��ʾ��������ģ��,(�Ƿ������Ⱦ������)
FLAGS_RENDER_DRAWSKELETON	=	64		--��Ⱦ�����ڵ�(md5ģ�ͲŻ���ʾ)
FLAGS_ANIM_ADAPTIVE		=	128		--�Ƿ���ݹؼ�֡��֡������ؼ�֡�Ķ���(�Ƿ�����fps 1,����  0,������)
FLAGS_GLSL_OUTLINE		=	256		--��GLSLʵ�ֵ�������
FLAGS_DRAW_PLOYGON_LINE	=	512		--�ڵ�һ���������������߿���Ⱦ(�̶�����ģʽ����GL_LINE)
FLAGS_BUTTON_EFFECT		=	1024	--���ð�ť��Ч(���е���¼���ǰ���²Ż��и���Ч)
FLAGS_DRAW_NORMAL 		= 	2048	--����
FLAGS_DRAG				=	4096	--�Ƿ����ק
FLAGS_DRAW_PLOYGON_POINT= 	8192	--��ģʽ
FLAGS_DISABLE_CULL_FACE	=	16384	--������ʾ˫��
FLAGS_REVERSE_FACE		=	32768	--��ת��Ⱦ��


--//�ļ����Ͷ���
TYPE_OBJ_FILE = 0--	//obj����
TYPE_MD5_FILE =1 --//md5����
TYPE_MD2_FILE =2 --//md2����
TYPE_SPRITE_FLIE =3--//UI���������е�sprite
TYPE_TEXT_FILE	=4	--//�ı�����
TYPE_OBJ_VBO_FILE=	5--//VBO�ļ�����

SUFFIX_OBJ ="obj"
SUFFIX_MD5MESH ="md5mesh"
SUFFIX_MD2 ="md2"

------------------------------------------------------------
EVENT_RAY_PICK = 1						--ʰȡ����ص�
EVENT_ENGINE_RENDER_3D =100				--������Ⱦ�ص�
EVENT_ENGINE_KEYBOARD  =101				--ȫ�ּ����¼�
EVENT_ENGINE_RESIZE	   =102				--resize�¼�

EVENT_ENGINE_BASE_UPDATE	 =  102		--base�����¼�
EVENT_ENGINE_BASE_END		 =  103		--base�����¼�

dofile("..\\include\\lua\\xml.lua")	--xml���
dofile("..\\include\\lua\\vec3.lua")	--�Զ�����ѧ��
dofile("..\\include\\lua\\evt.lua")	--�¼�������

local defalutTex = '1.tga'
local defalutObjModel = 'quad'

local tableInstance={}
--ת����һ����ַ
--"=table: 0082E758"  ===>8578904
local function 
getDddress(value)
	local len = string.len("table:  ")
	local a = string.len(tostring(value))
	local v = string.sub(tostring(value),len,a)
	local s = tonumber('0x'..v)
	return s
end

--��ȡһ��Ĭ�ϵ���ͼ
function func_get_default_tex()
	return defalutTex
end

--[[
	����һ�����ü�������һ�����ݶ���

--]]
function func_load(url)
	
	local suffix = get_suffix(url);
	--print(s);
	local result;
	local xml = xml_load(url);
	
	if(suffix == "mat") then
		local node = xml_get_node_by_index(xml,0);
		local shader = xml_get_str(node,"shader");
		
		local shaderParm = xml_get_str(node,"shaderParm");
		result = createMaterial(shader,
			xml_get_str(node,"tex0"),
			xml_get_str(node,"tex1"),
			xml_get_str(node,"tex2"),
			xml_get_str(node,"tex3"),
			xml_get_str(node,"tex4"),
			xml_get_str(node,"tex5"),
			xml_get_str(node,"tex6"),
			xml_get_str(node,"tex7")
		);
		
	elseif(suffix == "obj") then
	
	end
	
	xml_del(xml);
	
	return result;
end



--[[
	point:tabel���ñ�����һ��ȫ��tabel��
--]]
function func_tableSave(point)
	--point['index'] = getIndex()
	
	--local s = tableToInt(point)
	--instance[s] =  point
	
	--print(tostring(point)..','..getDddress(point))
	--getDddress
	tableInstance[tostring(point)]=point
end
--[[
	point:ɾ��һ��������
--]]
function func_tableDel(point)
	--�����table�е�Ԫ��
	func_clearTableItem(point)
	
	--����table������
	tableInstance[tostring(point)] = nil
end
--ɾ��table�����Ԫ��(�������еı�Ԫ������)
function func_clearTableItem(point)
	for k, v in pairs(point) do
		point[k] = nil
	end
end
--������ӡ��
function func_printTable(t)
	func_print("start print table: "..tostring(t),0xffff00)
	--print("start print table: "..tostring(t))
	for key, value in pairs(t) do      
		--print('key=['..key..']'..'value['..value..']')
		local s = tostring(value)
		if(tonumber(value)) then
			
			--ת��Ϊ16��������
			s =  string.format("%#x",tonumber(value)).."\t("..value..")"
		end
		
		print(tostring(key).."\t\t\t="..s)
		--print(value)
		--print("-------------")
		
		--print(string.format("%#x",tonumber(value)))
		
		--print(tonumber(value))
	end 
	func_print("end print table:   ["..tostring(t).."]",0xffff00)
	
end

--��ȡtable�е��ֶ���
function func_get_table_count(t)
	local n = 0
	for key, value in pairs(t) do      
		n = n + 1
	end 
	return n
end

--[[
	��ӡtabel�е�������
--]]
function func_print_tableInstance()
	print("####func_print_tableInstance Start...")
--	local size
	
	local i = 0
	for key, value in pairs(tableInstance) do		
		
	--	if(size) then
	--		print("table size = "..(getDddress(value) - size).." bytes\n")
	--	end
	
		
		print("\n***** index = "..i..",key="..tostring(key)..",value="..tostring(value)..'-->'..getDddress(value).." *****")
		func_printTable(value)
		i = i + 1
		
	--	size = getDddress(value)
		
	end
	
	
	
	print("####func_print_tableInstance End...")
end

--����һ��Sprite
function func_sprite_create(name,x,y,w,h)
	local sprite = sprite_create(name,x,y,w,h);
	return sprite
end

--[[
	��ȡ��table����
--]]
function func_getTable(name)
	--return instance[name]
	
	--print("func_getTable-->"..name)
	return tableInstance[name]
end

--[[
	��ȡ��table��(��ʵ��table��lua�ռ��ַ)
	0EA02968
--]]
function func_getTableName(point)
--	return tableToInt(point)
	return tostring(point)
end
--[[
	��ȡ���������
#define TYPE_OBJ_FILE 0	//obj����
#define TYPE_MD5_FILE 1 //md5����
#define TYPE_MD2_FILE 2 //md2����
#define TYPE_SPRITE_FLIE 3//UI���������е�sprite
#define TYPE_TEXT_FILE	4	//�ı�����
#define TYPE_OBJ_VBO_FILE	5//VBO�ļ�����

--]]
function func_get_type(p)--func_getGameObjectType
	return	get_attr(p,"type")
end


function func_get_type_str(p)
	local t = func_get_type(p)
	if(t == 0) then
		return 'obj'
	elseif(t == 1)then
		return 'md5'
	elseif(t == 2)then
		return 'md2'
	elseif(t == 3)then
		return 'sprite'
	elseif(t == 4)then
		return 'text'
	elseif(t == 5)then
		return 'vbo'
	end
end

--[[
	��ȡ��ǰʱ�� ��ȷ������
--]]
function func_getTime()
	return  dofunc("GetLongTime")
end

--��ӡһ������ɫ����־������̨
function func_print(s,c)
	c = c or 0xffff00
	dofunc("LogPut",string.format("%s\n",s),c)
end
function func_error(msg,func)
	local s = ''
	
	s = 'lua:'..msg..''
	--func_print(s,0xff0000)
	--func_print('lua error:'..s,0xff0000)
	
	func_print(s,0xff0000)
	
	s = ''
	if(func) then
		--print ('line num:'..debug.getinfo(1).currentline..',file name:'..debug.getinfo(1).name )
		local info = debug.getinfo(func)
		for k,v in pairs(info) do
			
			
				--ȫ����ӡ
				--print(k, ':', info[k])
				
				
				
				--linedefined				
				--short_src
				--s = s..tostring(k)..':\t\t'..tostring(info[k])..'\n'
				
				if(tostring(k) == 'linedefined') then
					s = s..'lineNum:'..tostring(info[k])..'\t'
				end
				
				if (tostring(k) == 'short_src') then
					s = s..'file:'..tostring(info[k])..'\t'
				end
		end
	end
	func_print(s,0xffff00)
	--func_printTable(debug.getinfo(1))
	print(debug.traceback())
	
	--assert(nil,s)
	--error(msg)
    --print ( debug.getinfo(1).name )
end
--[[
	��������(����ģʽ)
--]]
function func_setPos(p,x,y)
	
	local t = func_get_type(p)
	
	local pos = x..","..y
	
	if(t == TYPE_SPRITE_FLIE) then
		
		change_attr(p,"sprite_position",pos)
	
	elseif(t == TYPE_TEXT_FILE)	then
	
		change_attr(p,"txt_position",pos)
	
	end
	
end




--�ڴ����
function func_gc()
	change_attr(nil,"gc")
	--print('gc')
end

--��ʰȡ�ص�
function func_set_pick(o,func)
	change_attr(o,"set_pickBind",func);
end
--������ײ������ɫ
function func_set_box_color(o,r,g,b)
	change_attr(o,"set_box_color",string.format("%f,%f,%f",r,g,b))
end

--�ı�����Rotate x��ֵ
function func_setRotateX(o,value)
	change_attr(o,"rx",tostring(value));
end




--�ı�����Rotate y��ֵ
function func_setRotateY(o,value)
	change_attr(o,"ry",tostring(value));
end


--�ı�����Rotate z��ֵ
function func_setRotateZ(o,value)
	change_attr(o,"rz",tostring(value));
end

--������
function func_rename(o,value)
	change_attr(o,"rename",tostring(value));
end

--����rayʰȡ�뾶
function func_set_rayRadius(o,value)
	change_attr(o,"set_rayRadius",tostring(value));
end
--��������
function func_set_x(o,x)
	change_attr(o,"x",x)
end
function func_set_y(o,y)
	change_attr(o,"y",y)
end
function func_set_z(o,z)
	change_attr(o,"z",z)
end

--�������ű���
function func_set_scale(o,v)
	change_attr(o,"scale",v)
end
--[[
function func_set_camera_pos(x,y,z)
		dofunc("SetCameraPosition",x,y,z);
end
--]]
--��ȡ������
function func_get_name(o)
	return get_attr(o,"get_name")
end

--��ȡ����ĺ�׺�����ַ���
function func_get_suffix(o)
	return get_attr(o,"suffix")
end

--����һ���̶�����ģ������md2,obj
function func_fixed_load(url,scale,texpath)
	local o=load_model(func_create_name(),url or "\\resource\\obj\\torus.obj",0,0,0,scale or 1.0)
	local mat1=createMaterial("diffuse",texpath or ("\\resource\\texture\\"..defalutTex),"")
	setMaterial(o,mat1)
	setv(o,FLAGS_VISIBLE);
	return o
end

--����һ��md5ģ��
--"\\resource\\texture\\wolf.bmp"
function func_loadmd5(name,scale,texpath,meshName,animName)
	
	--if (name == nil or name == '') then name = "wolf" end
	name = name or 'wolf'
		
	--if(scale == nil) then scale = 1.0 end
	scale = scale or 1.0

	local path = "\\resource\\md5\\"..name
	
	if(texpath == nil) then texpath =  "\\resource\\texture\\"..defalutTex end
	
	--if(meshName == nil) then meshName = "body.md5mesh" end
	meshName = meshName or 'body.md5mesh'
	--if(animName == nil) then animName = "walk.md5anim" end
	animName = animName or 'walk.md5anim'

	local md5=load_model(name,path.."\\"..meshName,0,0,0,scale);	
	local mat1=createMaterial("diffuse",texpath,"");
	setMaterial(md5,mat1);
	
	md5_loadAnim(md5, path.."\\"..animName,"walk");
	setanim(md5,"walk");
	
--	md5_loadAnim(md5, path.."\\stand.md5anim","stand");
--	setanim(md5,"stand");
	
	setv(md5,FLAGS_VISIBLE);
	return md5;
	
	
	--print('error������md5�ļ�:['..name..']')
	--assert(nil,"my Error!")
end

local nameKey = -1;
--����һ������
local function getName()
	nameKey=nameKey+1;
	return "instance_"..tostring(nameKey);
end

--��������
function 
func_create_name()
	return getName()
end


--����md2ģ��
function func_loadmd2(objName,tex,type,modelName)
	local time = func_getTime()
	--if(type == nil) then type = "vbo" end
	type = type or 'vbo'
	
	
	local name = modelName or getName();
	
	--if( objName == nil or objName == "") then objName = "bauul" end
	objName = objName or "triangle"
	
	--if( tex == nil) then tex = defalutTex end
	tex = tex or defalutTex
	
	local o
	if(type == "vbo") then
		o=load_VBO_model(name,"\\resource\\md2\\"..objName..".md2");
		setv(o,FLAGS_VISIBLE);
		
		local m=createMaterial("vboDiffuse","\\resource\\texture\\"..tex);
		setMaterial(o,m);
	end
	
	local s = string.format("[���ؽ���%s��ʱ %d ����]",objName,(func_getTime() - time))
	--if(DEBUG) then func_print(s) end
	return o;
end
--����һ��obj ʹ��vboģʽ
function func_loadobj(objName,tex,nName,vbo)
	local name
	if(nName==nil) then
		name = getName()
	else
		name = nName
	end
--	if( objName == nil or objName == "") then objName = defalutObjModel end
	objName = objName or defalutObjModel
	
	--func_print(	string.format("[%s]",tostring(objName)),0xff0000)
	
	--if( tex == nil) then tex = defalutTex end
	
	tex = tex or defalutTex
	if(vbo == nil) then
		vbo = true
	end
	
	
	--vbo = vbo or true
	
	---[[

	local o;
	local _path = "\\resource\\obj\\"..objName..".obj";
	local _shader;
	local _texturePath = "\\resource\\texture\\"..tex;

	if(vbo == true) then
		_shader = "vboDiffuse";
		o=load_VBO_model(name,_path);
	else
		_shader = "diffuse";
		o=load_model(name,_path,0,0,0,1.0);	
	end
	setv(o,FLAGS_VISIBLE);
	
	--print("func_loadobj===============>",_texturePath,string.format("���ص�ģ��(%s),ģ����(%s)�Ƿ���VBOģʽ:%s",_path,nName,tostring(vbo)));

	local m=createMaterial(_shader,_texturePath);
	--local m = core_load("//resource//mat1.mat");
	--local m = func_load("//resource//mat1.mat");
	
	setMaterial(o,m);
	return o;
	--]]
end
--��һ����ײ��
function func_bind_collide(o,model,frame)
	change_attr(o,"collide_load",model or "\\resource\\obj\\box.obj",frame or 0);
end
------------------------------------------------------
--�л�״̬
--����true or false
------------------------------------------------------
function func_changeFlags(_o,_flag)
	if(_o==nil) then
		func_error("switchFlags Ŀ����� = nil")
		return;
	end;
	
	if(_flag == nil)then
		func_error("flag = nil ")
	else
		
		if	getv(_o,_flag) == 1	then
			resetv(_o,_flag);
		else
			setv(_o,_flag);
		end
		
		--print('����'..string.format("%#x",_o).."���ñ�ʾ".._flag.."״̬Ϊ:"..getv(_o,_flag) );
	end
--[[	
	local _stat = getv(_o,_flag)
	if (_stat == 1) then 
		return true
	end
	return false
--]]
	return getv(_o,_flag) == 1
end

local uiAtals

--��ȡһ������ͼ��(û�оʹ���,�ڵ��õ�ʱ�򴴽�)
function func_getUI_Atals()
	--print(uiAtals)
	
	if(uiAtals == nil) then		
		uiAtals=atals_load("//resource//texture//","1")--�������ͼ��	
	end
	return uiAtals
end
--Ϊsprite������ͼ
--sprite_texName(listBox.sprite,"gundi.png");����
function func_setIcon(sprite,url)
	--��ȡһ��atalsͼ��,û��ͼ���Ľ����Ǻ�ɫ��
	local atals = func_getUI_Atals()
	if(atals) then
		sprite_bindAtals(sprite,atals)

		sprite_texName(sprite,url);
	end
end
--��������ȡ�Զ���
function func_find_obj(name)--func_getObj_ByName_EG
	return dofunc("FindNode",name);
end
--�������䲥�Ŷ���
function func_play_anim(o,s,e)
	change_attr(o,"animtor_play_start_end",string.format('%d,%d',s,e))
end

--���Ŷ���
function func_play(o)
	change_attr(o,"animtor_play");--����
end
--��ȡ�����ؼ�֡��(md5)
function func_get_frame_count(o)
	return get_attr(o,"frameCount")
end

--��ȡsprite�Ŀ��
function func_get_sprite_size(o)
	return get_attr(o,"spriteSize")	
end

--����sprite�Ŀ��
function func_set_sprite_size(o,w,h)
	change_attr(o,"sprite_resize",string.format('%d,%d',w,h));
end

--��ȡ���ʰȡ��sprite�������
function func_get_sprite_mouse_xy(o)
	local x , y=get_attr(o,"spriteLocalXY");
	return x,y
end
--��ȡ��������ʱ��
function func_get_longTime()
	return get_attr(nil,"get_longTime");
end

--��ȡ��������ʱ��
function func_screenSize()
	local w,h=get_attr(nil,"screenSize");
	return w,h
end

--��ȡfps
function func_fps()
	return get_attr(nil,"fps");
end
--��ȡdelayTime
function func_delayTime()
	return get_attr(nil,"delayTime");
end
--���ö���Ĺؼ�֡֡��
function func_set_ptr_fps(o,v)
	change_attr(o,"fps",tostring(v))
end
--���ö���
function func_set_anim(o,anim)
	change_attr(o,"animtor_setcur",anim);
end

--��ȡ�ı�
function func_get_tf_text(tf)
	return dofunc("tf_get_str",tf);
end

--�Ƴ��������(md5,md2,textfiled,sprite,obj)
function func_ptr_remove(o)
	return ptr_remove(o)
end

--����glsl����
--�п��ܵ�ǰ���ʵ���ɫ��������û�йؼ���key,��ô��ʱ������ֵ��ʱ��ͻ�ʧ��
function func_set_glsl_parms(o,key,v)
	change_attr(o,"glslParms",string.format('%s,%s',key,tostring(v)))
end

--�Ƿ���õ���VBOģʽ��Ⱦ��
function func_is_vbo(obj,tips)
	if(func_get_type_str(obj)=='vbo') then
		return true
	end
	return false
end

--�ָ����ö���
function func_anim_push(obj,animname,s,e)
	change_attr(obj,"animtor_push",animname,string.format('%s,%s',s,e));
end

--��ȡ������Ϣ
function func_ex_info()
	get_attr(nil,"ex_info");
end

function func_get_xyz(o)
	return get_attr(o,"xyz");
end
--�ý�ɫ����ĳ������
function func_look_at(o,x,y,z)
	change_attr(o,"lookat",string.format("%f,%f,%f",x,y,z));
end

function func_set_position(o,x,y,z)
	change_attr(o,"set_position",string.format("%f,%f,%f",x,y,z));
end

--�ý�ɫ����ĳ������
function func_move(o,ms,x,y,z)
	change_attr(o,"move",string.format("%d,%f,%f,%f",ms,x,y,z));
end

--���¶������
function func_update_mat4x4(o)
	change_attr(o,"base_updateMat4x4");
end

--function func

dofile("..\\include\\lua\\cam.lua")	--cam
dofile("..\\include\\lua\\fps.lua")	--fps���

dofile("..\\include\\lua\\UListBox.lua")	--ѡ���б����
dofile("..\\include\\lua\\UScrollBar.lua")	--��un�������
dofile("..\\include\\lua\\UIlib.lua")		--��ť���


dofile("..\\include\\lua\\infowin.lua")	--fps���

dofile("..\\include\\lua\\alert.lua")		--���������
dofile("..\\include\\lua\\arrow.lua")		--��ͷ���
dofile("..\\include\\lua\\unit.lua")		--��ɫ��λ