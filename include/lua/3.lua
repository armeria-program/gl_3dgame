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


local function loadMD5(name,x,scale)
	local model
	x = x or 0
	scale = scale or 0.015
	if(name == 'wolf') then
		model = func_loadmd5('wolf',scale,"\\resource\\texture\\wolf.tga")		
		func_set_rayRadius(model,30)--���þ�̬��ײ����
	else
		model  = func_loadmd5('cube',1.0,"\\resource\\texture\\wolf.tga","pCube.md5mesh","pCube_a7.md5anim")
	end
	
	func_set_x(model,x)
	
	f_init(model)
	
	return model
end
--local model = func_loadobj()--����һ��objģ��
--local model = func_loadmd2('triangle') ;func_setCameraPos(0,0,-5)--md2ģ��
local function loadObj(x,model)
	x = x or 0
	model = model or 'tri'--'box'--'torus'
	local obj = func_loadobj(model)--box	'torus'
	
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
	func_set_x(obj,x)
end

local function loadMd2(x)

	local obj=func_loadmd2()--'horse'
	func_set_x(obj,x or 0)
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

func_set_camera_pos(0,0,-10)-- -10
modelUI_init()
loadMD5('wolf',0,0.04)
--loadMD5('wolf1',1)

--loadObj(0)
--loadMd2(3)
--]]

