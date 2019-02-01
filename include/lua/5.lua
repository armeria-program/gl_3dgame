dofile("..\\include\\lua\\core.lua")

func_print('#######################\t���ʲ���\t#######################',0xff0000)
local _model_ptr;

--��ʼ����ʾ
local function 
f_set_obj_flags(o)
	setv(o,FLAGS_VISIBLE);
	--	setv(o,FLAGS_GLSL_OUTLINE);
	--  setv(o,FLAGS_DRAW_PLOYGON_LINE);--�߿���Ⱦ
	--	setv(o,FLAGS_DRAW_NORMAL);
	
	
	--����ʰȡ
	---[[
	setv(o,FLAGS_RAY);
	setv(o,FLAGS_DRAW_RAY_COLLISION);
	setv(o,FLAGS_RENDER_BOUND_BOX);
	--]]
	
end

--��������б�
local function 
createObjList(cnt)
	cnt = cnt or 1
	local offset = 3
	
	local seedNumber = -offset
	
	for i=0,cnt-1,1 do
		
		local time = func_get_longTime()
		seedNumber = seedNumber + offset;
		local _m=	func_loadobj("torus")			--load_objVBO("torus");	--"teapot"
		setv(_m,FLAGS_DRAW_PLOYGON_LINE);
	
		func_set_x(_m,seedNumber)
		
		func_print(string.format('load_objVBO ����%d ms',func_get_longTime() - time))
		
	end
	
end
--����һ����͸��ͨ������Ҷģ��
local function 
load_alpha_model(objName)
	local time = func_get_longTime()
	objName = objName or "quad"
	local o=func_loadobj(objName)--load_VBO_model(name,"\\resource\\obj\\"..objName..".obj");
	local m=createMaterial("vboDiffuse","\\resource\\texture\\leaf.tga");
	setMaterial(o,m);
	f_set_obj_flags(o);
	func_print(string.format('load_alpha_model ���� %d ms',func_get_longTime() - time))
	return o;
end

local function
f_setLabel(_l,obj)
	local label = listbox_get_label(_l)
				
	listbox_set_label(_l,string.format('%s vbo=%s',label,tostring(func_is_vbo(obj)) ))
	
	if(label == 'diffuse') then 
		
	elseif (label == 'ploygonLine') then
		
		setv(obj,FLAGS_DRAW_PLOYGON_LINE)

	elseif (label == 'outline') then
		
		--setv(obj,FLAGS_DISABLE_CULL_FACE)--������˫��
		if(func_is_vbo(obj)) then
			func_error("vbo��֧��!!!")
		end
		setv(obj,FLAGS_OUTLINE)
	elseif (label == 'point') then
	
		setv(obj,FLAGS_DRAW_PLOYGON_POINT)

	elseif(label == 'glslOutline') then
		--setv(obj,FLAGS_DRAW_PLOYGON_LINE)
		
		--��һЩ�����ɫ����û����Щ�����ͻ�������Ч
		
		func_set_glsl_parms(obj,'lineWidth',0.05)
		func_set_glsl_parms(obj,'alpha',0.5)
		
		setv(obj,FLAGS_GLSL_OUTLINE)
		--print('***')
	elseif(label == 'normal')then	
		if(func_is_vbo(obj)) then
			func_error("vbo��֧��!!!")
		end
		setv(obj,FLAGS_DRAW_NORMAL)--���Ʒ���

	elseif (label == 'drawCollison') then
		if(func_is_vbo(obj)) then
			func_error("vbo��֧��!!!")
		end
		
		setv(obj,FLAGS_DRAW_RAY_COLLISION)
	end

end
--###############################################################
--��Ⱦ�ص�
local _ticket = 0;
local tt = 0;

function func_drawCall(v)
	--print(func_fps())
	--drawCall�ص�
	if(func_get_longTime() - tt<=20) then
		--��غ���һ��
		return
	end
	
	tt = func_get_longTime();
	
	if(_model_ptr) then
		--_temp += PI--func_fps()*PI
		local f = func_get_longTime()/ func_fps()--*PI 
		
		_ticket=_ticket+(1000/func_fps()) * 0.1
		--print(string.format("f = %.3f fps = %d ticket = %.3f",f, func_fps(),_ticket))
		
		func_setRotateX(_model_ptr,_ticket)
	end
	
end
--###############################################################
--��ʼ��listbox,�������Բ�ͬ�Ĳ���
local function
f_shader_init()
	local list =  listbox_new()
	
	listbox_add(list,"diffuse")	--������
	listbox_add(list,"ploygonLine")
	listbox_add(list,"outline")
	listbox_add(list,"point")
	listbox_add(list,"drawCollison")
	listbox_add(list,"glslOutline")
	listbox_add(list,"normal")

	--######################################################
	local g_model
	listbox_bind(list,
		function(_l)
			
		
			if(g_model) then
				func_ptr_remove(g_model)
			end
			
			
			--local obj =func_loadobj("torus")
			
			local obj = func_fixed_load()
			
			--func_fixed_load()--func_loadmd5('wolf',0.01)--func_loadobj("torus")
			
			if(obj) then
			
				g_model = obj
				
				_model_ptr = g_model
				
				local s = string.format('index = %d\tlabel = [%s]\t	vbo:%s',
					listbox_get_index(_l),
					listbox_get_label(_l),
					tostring(func_is_vbo(obj)) 
					)
				
				func_print(s,0xffff00)
				
				f_setLabel(_l,obj)
			
			end
			
		end
	)
	
	return list
end

--#########################################################
--func_set_camera_pos(0,0,-10)
--createObjList(3)

--func_set_camera_pos(0,0,-2)
--load_alpha_model()

func_set_camera_pos(0,0,-10)
local list = f_shader_init()
--listbox_select(list,0)--Ĭ��ѡ��һ��
