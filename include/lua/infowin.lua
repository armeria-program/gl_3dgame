function f_pick(p)
	--moduleUI_bind(p)
	--f_selectPick(p)
    --print("f_pick:"..p);
    --crl:bind(p);
	print("pick now!");--ʰȡ�Ķ���
end
--����һ��objģ��
local function f_loadObj(model)
	
	local name = func_create_name();
	model = model or 'tri'--'box'--'torus'
	--local obj = func_loadobj(model)--box	'torus'
	local obj = load_model(name,string.format("\\resource\\obj\\%s.obj",model));	

	engine_bind_default_3dcam(obj);
	
	setMaterial(obj,func_load("//resource//material//wolf.mat"));
	setv(obj,FLAGS_VISIBLE);
	---[[
	func_changeFlags(obj,FLAGS_RAY);				--����Ϊ��ʰȡ״̬
	func_changeFlags(obj,FLAGS_DRAW_RAY_COLLISION);	--�������ߺ���
	func_changeFlags(obj,FLAGS_DISABLE_CULL_FACE);	--��ʾ˫����ʾ
	--func_changeFlags(obj,FLAGS_GLSL_OUTLINE);
	func_changeFlags(obj,FLAGS_DRAW_PLOYGON_LINE);

	func_bind_collide(obj)
	--moduleUI_bind(obj)
	--f_add_model(obj)

	func_set_pick(obj,"f_pick");
	func_set_x(obj,1);
	--]]
	return obj
end

--------------------------------------------------------------------
-- ��Ϣ�������

--[[local mc = FpsView:new();
mc:show();--]]
local _stat;
local function f_select(self,index,param)
	
	
	
	--local index =listbox_get_index(list);
	---[[
    if (index == 0) then
        --func_ex_info();


--	    alert("���ڲ���:������Ϣ");
		print("windows test!");
		
		FpsView:getIns():show();
		
    elseif (index == 1) then
        func_gc();
--    elseif (index == 2) then
--        ex:switch_spriteline()
--        print("�л�ΪSprite�߿�״̬:" .. ex:get_spriteline());
    elseif (index == 2) then
        --fps();
    elseif (index == 3) then
        _stat = not _stat;
--        print(_stat);

        if(_stat) then
            engine_setBg(0.1,0.1,0.1);
        else
            engine_setBg(0.4,0.4,0.4);
        end
	elseif (index == 4) then
		--����һ��mesh
		f_loadObj();
    elseif(index == 5) then
        cam_reset();
    end
	--]]
end



function infowin(x, y)
	
	
	--[[local list  = listbox_new(x or 0, y or 0);
	listbox_bind(list,f_select);
	
	
	listbox_add(list,"������Ϣ");
	listbox_add(list,"gc");
	listbox_add(list,"fps");
	listbox_add(list,"������ɫ");
	listbox_add(list,"����mesh");
	listbox_add(list,"����cam");
	listbox_set_title(list,"infowin");--]]
	
	local list = NListBox:new(x,y,128);
	list:addItem("������Ϣ");
	list:addItem("gc");
	list:addItem("fps");
	list:addItem("������ɫ");
	list:addItem("����mesh");
	list:addItem("����cam");
	list:bind(f_select);

	
	--listbox_del(list);
	--return list;
end