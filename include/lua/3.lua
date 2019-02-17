--ģ�Ϳ�����

dofile("..\\include\\lua\\core.lua")

local modelList={}

local function f_selectPick(p)
	for key, value in pairs(modelList) do      
		if( tonumber(value) == p) then
			func_set_box_color(value,1,1,0)			
		else
			func_set_box_color(value,1,0,0)
		end
	end 
end

function f_pick(p)
	moduleUI_bind(p)
	f_selectPick(p)
end

--��ӵ��б�,����ѡ����ģ����ײ��
local function f_add_model(model)
	local cnt = func_get_table_count(modelList)--��ȡtable�е�����
	modelList[cnt] =  model
	--f_selectPick(model)
end

local function f_init(model)
	func_changeFlags(model,FLAGS_ANIM_ADAPTIVE)			--���ù̶���fpsģʽ,��һ������Ӧ����BUG
	func_changeFlags(model,FLAGS_DRAW_PLOYGON_LINE) 	--�����߿���Ⱦ
	func_changeFlags(model,FLAGS_RENDER_DRAWSKELETON)	--������Ⱦ
	func_changeFlags(model,FLAGS_DISABLE_CULL_FACE)		--��ʾ˫����ʾ
	func_changeFlags(model,FLAGS_RAY)					--����Ϊ��ʰȡ״̬
	func_changeFlags(model,FLAGS_DRAW_RAY_COLLISION)	--�������ߺ���
	
	func_set_pick(model,"f_pick")
	
	f_add_model(model)
end


local function loadMD5(name)
	local model
	--x = x or 0
	--scale = scale or 0.015
	
	--[[
	if(name == 'wolf') then
		model = func_loadmd5('wolf',scale,"\\resource\\texture\\wolf.tga")		
		func_set_rayRadius(model,30)--���þ�̬��ײ����
	else
		model  = func_loadmd5('cube',1.0,"\\resource\\texture\\wolf.tga","pCube.md5mesh","pCube_a7.md5anim")
	end
	--]]
	model=load_model(name,"\\resource\\md5\\wolf\\body.md5mesh");	
	setMaterial(model,func_load("//resource//material//wolf.mat"));
	md5_loadAnim(model, "\\resource\\md5\\wolf\\walk.md5anim","walk");

	setv(model,FLAGS_VISIBLE);
	--func_set_x(model,x)
	
	f_init(model)
	
	return model
end
--local model = func_loadobj()--����һ��objģ��
--local model = func_loadmd2('triangle') ;func_setCameraPos(0,0,-5)--md2ģ��
local function loadObj(model)
	
	local name = func_create_name();
	model = model or 'tri'--'box'--'torus'
	--local obj = func_loadobj(model)--box	'torus'
	local obj = load_model(name,string.format("\\resource\\obj\\%s.obj",model));	
	setMaterial(obj,func_load("//resource//material//wolf.mat"));
	setv(obj,FLAGS_VISIBLE);
	---[[
	func_changeFlags(obj,FLAGS_RAY)					--����Ϊ��ʰȡ״̬
	func_changeFlags(obj,FLAGS_DRAW_RAY_COLLISION)	--�������ߺ���
	func_changeFlags(obj,FLAGS_DISABLE_CULL_FACE)		--��ʾ˫����ʾ
	--func_changeFlags(obj,FLAGS_GLSL_OUTLINE)
	func_changeFlags(obj,FLAGS_DRAW_PLOYGON_LINE)

	func_bind_collide(obj)
	--moduleUI_bind(obj)
	f_add_model(obj)

	func_set_pick(obj,"f_pick")
	--]]
	
	
	return obj
end

local function loadMd2(x)

	--local obj=func_loadmd2()--'horse'
	
	local obj=load_VBO_model(func_create_name(),"\\resource\\md2\\horse.md2");
	setMaterial(obj,func_load("//resource//material//horse.mat"));
	setv(obj,FLAGS_VISIBLE);
	--func_set_x(obj,x or 0)
	func_changeFlags(obj,FLAGS_RAY)					--����Ϊ��ʰȡ״̬
	func_changeFlags(obj,FLAGS_DRAW_RAY_COLLISION)	--�������ߺ���
	func_changeFlags(obj,FLAGS_DISABLE_CULL_FACE)		--��ʾ˫����ʾ
	--func_changeFlags(obj,FLAGS_GLSL_OUTLINE)
	
	func_bind_collide(obj)
	f_add_model(obj)
	func_set_pick(obj,"f_pick")
	return obj
end 

---[[
dofile("..\\include\\lua\\modelUI.lua")

func_set_camera_pos(0,0,-5)-- -10

--��ʼ�����������
modelUI_init()

local _md5 = loadMD5('wolf')
func_set_scale(_md5,0.02)
func_set_rayRadius(_md5,30)--���þ�̬��ײ����
--loadMD5('wolf1',1)

local obj1 = loadObj()
func_set_x(obj1,1)

local _md2 = loadMd2()
func_set_scale(_md2,0.02)
func_set_rayRadius(_md2,30)
func_set_x(_md2,2)



