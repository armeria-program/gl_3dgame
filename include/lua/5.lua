-- dofile("..\\include\\lua\\core.lua")

print('#######################\t���ʲ���\t#######################')

--����glsl����
--�п��ܵ�ǰ���ʵ���ɫ��������û�йؼ���key,��ô��ʱ������ֵ��ʱ��ͻ�ʧ��
local function func_set_glsl_parms(o,key,v)
	change_attr(o,"glslParms",string.format('%s,%s',key,tostring(v)))
end

--����һ���̶�����ģ������md2,obj
local function func_fixed_load(url,scale,texpath)
	local o=load_model(func_create_name(),url or "\\resource\\obj\\torus.obj",0,0,0,scale or 1.0)
	local mat1=createMaterial("diffuse",texpath or ("\\resource\\texture\\1.tga"),"")
	setMaterial(o,mat1);
	setv(o,FLAGS_VISIBLE);
	return o;
end

--�Ƿ���õ���VBOģʽ��Ⱦ��
local function func_is_vbo(obj)
	--if(func_get_type_str(obj)=='vbo') then
    if(JEngine:getIns():get_type_str(obj)=="vbo") then
		return true
	end
	return false
end

--local _model_ptr;

-- ��ʼ����ʾ
local function f_set_obj_flags(o)
    setv(o, FLAGS_VISIBLE);
    -- setv(o,FLAGS_GLSL_OUTLINE);
    --  setv(o,FLAGS_DRAW_PLOYGON_LINE);--�߿���Ⱦ
    -- setv(o,FLAGS_DRAW_NORMAL);


    -- ����ʰȡ
    --- [[
    setv(o, FLAGS_RAY);
    setv(o, FLAGS_DRAW_RAY_COLLISION);
    setv(o, FLAGS_RENDER_BOUND_BOX);
    -- ]]

end

---- ��������б�
-- local function createObjList(cnt)
--    cnt = cnt or 1
--    local offset = 3

--    local seedNumber = - offset

--    for i = 0, cnt - 1, 1 do

--        local time = func_get_longTime()
--        seedNumber = seedNumber + offset;
--        local _m = func_loadobj("torus")
--        -- load_objVBO("torus");	--"teapot"
--        setv(_m, FLAGS_DRAW_PLOYGON_LINE);

--        func_set_x(_m, seedNumber)

--        func_print(string.format('load_objVBO ����%d ms', func_get_longTime() - time))

--    end

-- end
-- ����һ����͸��ͨ������Ҷģ��
local function
    load_alpha_model(objName)
    local time = func_get_longTime()
    objName = objName or "quad"
    local o = func_loadobj(objName)
    -- load_VBO_model(name,"\\resource\\obj\\"..objName..".obj");
    local m = createMaterial("vboDiffuse", "\\resource\\texture\\leaf.tga");
    setMaterial(o, m);
    f_set_obj_flags(o);
    func_print(string.format('load_alpha_model ���� %d ms', func_get_longTime() - time))
    return o;
end
local function f_setLabel(label, obj)

    if (label == 'diffuse') then

    elseif (label == 'ploygonLine') then

        setv(obj, FLAGS_DRAW_PLOYGON_LINE)

    elseif (label == 'outline') then

        -- setv(obj,FLAGS_DISABLE_CULL_FACE)--������˫��
        if (func_is_vbo(obj)) then
            func_error("vbo��֧��!!!")
        end
        setv(obj, FLAGS_OUTLINE)
    elseif (label == 'point') then

        setv(obj, FLAGS_DRAW_PLOYGON_POINT)

    elseif (label == 'glslOutline') then
        -- setv(obj,FLAGS_DRAW_PLOYGON_LINE)

        -- ��һЩ�����ɫ����û����Щ�����ͻ�������Ч

        func_set_glsl_parms(obj, 'lineWidth', 0.05)
        func_set_glsl_parms(obj, 'alpha', 0.5)

        setv(obj, FLAGS_GLSL_OUTLINE)
        -- print('***')
    elseif (label == 'normal') then
        if (func_is_vbo(obj)) then
            func_error("vbo��֧��!!!")
        end
        setv(obj, FLAGS_DRAW_NORMAL)
        -- ���Ʒ���

    elseif (label == 'drawCollison') then
        if (func_is_vbo(obj)) then
            func_error("vbo��֧��!!!")
        end

        setv(obj, FLAGS_DRAW_RAY_COLLISION)
    end

end

local g_model;
local function f_listCallBack(self,index,param)
	--	list
	
	--local index = listbox_get_index(list);
	 --local _l = listbox;
		
	--print(g_model);

	if (g_model) then
		func_ptr_remove(g_model);
		--return;
	end


	-- local obj =func_loadobj("torus")
	local t = func_get_longTime();
	local obj = func_fixed_load()
	JEngine:getIns():bind_3dcam(obj);



	--print("&&&",obj);

	-- func_fixed_load()--func_loadmd5('wolf',0.01)--func_loadobj("torus")
	
	if (obj) then

		g_model = obj;
		
		--ex_addnode(obj);
		
		--            _model_ptr = g_model
		--local arr = func_split(config);
		local label = self:getLabelByIndex(index);--listbox_get_select_label(list);
		
		local s = string.format('index = %d\tlabel = [%s]\t	vbo:%s',index,label,tostring(func_is_vbo(obj)))

		print(s..","..(func_get_longTime() - t));

		f_setLabel(label, g_model);
	end
end

-- ###############################################################
-- ��ʼ��listbox,�������Բ�ͬ�Ĳ���
local function f_shader_init()
	--local list = listbox_new(0,50);
		local list = NListBox:new(200,50,128);
	list:addItem("diffuse");
	list:addItem("ploygonLine");
	list:addItem("outline");
	list:addItem("point");
	list:addItem("drawCollison");
	list:addItem("glslOutline");
	list:addItem("normal");
	list:addItem("diffuse");
	list:addItem("���ʲ���");

	list:bind(f_listCallBack);

    return list;
end

-- #########################################################

-- cam:position(0,0,-2)
-- load_alpha_model()


local cam3d = JEngine:getIns():get_cam();
cam3d:set_pos(0,0,-10);
local list = f_shader_init();

-- listbox_select(list,0)--Ĭ��ѡ��һ��
