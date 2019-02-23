dofile("..\\include\\lua\\core.lua");
func_print('��Ԫ����ת����',0xff0000)

local _floorObj--�ذ����
local _target;--ʰȡ����
local animsc,animscTf,uvScaleTf;
--[[
= scrollBar_new(0,20)
scrollBar_setRange(animsc,0,1)
local tf = scrollBar_add_text(animsc,'')
--]]
--------------------------------------------------------------------------
--����С����
local function f_create_cube()
	local _scale = 1
	--С����
	local obj1 = load_model("myObj1","\\resource\\obj\\quad.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
	setMaterial(obj1,func_load("//resource//material//wolf.mat"));
	setv(obj1,FLAGS_VISIBLE);
	setv(obj1,FLAGS_RAY)		
	setv(obj1,FLAGS_DRAW_RAY_COLLISION)
	setv(obj1,FLAGS_DRAW_PLOYGON_LINE)
	func_set_scale(obj1,_scale)
	return obj1
end
local function f_animscHandle(sc)
	test_unit_01(sc.value)
	--print(sc.value)
	tf_setText(animscTf,sc.value)
end

--�л�״̬
local function f_onFloorHandle(b)
	--btn_label(b,"ployline:"..tostring(func_changeFlags( f_getModel(),FLAGS_DRAW_PLOYGON_LINE)))
	local _stat = func_changeFlags(_floorObj,FLAGS_DRAW_PLOYGON_LINE);
	btn_label(b,string.format("floor %d",(_stat and 1 or 0)));
end

local function f_uvScHandle(sc)
	func_set_glsl_parms(_floorObj,'uvScale',sc.value)
	tf_setText(uvScaleTf,string.format("uv %.1f",sc.value))
end
local function f_switchCamBtnHandle(b)
	print("swotch cam")
end

local function f_gcList(_l)
	local s = string.format('index = %d\tlabel = [%s]',
				listbox_get_index(_l),
				listbox_get_label(_l))
			
	print(s)
	
	local label = listbox_get_label(_l)
		
	if(label == 'delete') then 
		listbox_del(_l) --ɾ�����
	end
	
	if(label== 'gc') then func_gc() end--gc
end

--��ʼ������
local function f_init_ui()
	--layout ui
	
	animsc = scrollBar_new(0,20)
	scrollBar_setRange(animsc,0,1)
	animscTf = scrollBar_add_text(animsc,'')
	scrollBar_bind(animsc,f_animscHandle)
	------------------------------
	
	local btn = btn_create(0,40)
	btn_label(btn,"floor")
	btn_bindClick(btn,f_onFloorHandle)--�л���ʾ�Ƿ�Ҫ�߿���Ⱦ
	
	------------------------------	
	local uvScaleSc = scrollBar_new(0,60)
	scrollBar_setRange(uvScaleSc,1,20)
	scrollBar_bind(uvScaleSc,f_uvScHandle)
	uvScaleTf = scrollBar_add_text(uvScaleSc,'')
	
	local switchCamBtn=btn_create(0,80)
	btn_label(switchCamBtn,"switchCam")
	btn_bindClick(switchCamBtn,f_switchCamBtnHandle)
	
		
		
	local list =  listbox_new(0,100)
	listbox_set_label(list,"gclist");
	listbox_add(list,"gc")
	listbox_add(list,"delete")
	listbox_bind(list,f_gcList)
	
end

--local md5file = func_loadmd5('wolf',0.02,"\\resource\\texture\\wolf.tga")

local function f_create_box()
	--����һ��objģ��
	--VBOû����Ⱦ������
	local box = load_model("myBox","\\resource\\obj\\arrow.obj")--func_loadobj('arrow',nil,'myBox',false)--'box' 'torus' 'teapot' 'arrow'
	--setv(box,FLAGS_RAY)					--����Ϊ��ʰȡ״̬
	setMaterial(box,func_load("//resource//material//wolf.mat"));
	setv(box,FLAGS_VISIBLE);
	setv(box,FLAGS_DRAW_RAY_COLLISION)
	setv(box,FLAGS_DRAW_PLOYGON_LINE)
end
------------------------------------------------------------------
local function f_create_floor(scale)
	--����һ���ɵ���ĵذ�
	--local _floor = func_loadobj('plane','box.tga','_floor',false);
	local _floor = load_model("_floor","\\resource\\obj\\plane.obj")		-- func_loadobj('quad',nil,'myObj1',false)--quad
	setMaterial(_floor,func_load("//resource//material//floor.mat"));	
	setv(_floor,FLAGS_VISIBLE);
	--local _floorRadius = 30--�ذ�뾶
	--func_set_scale(_floor,_floorRadius*2);
	--func_set_y(_floor,-_floorRadius);
	
	func_set_scale(_floor,scale);
	setv(_floor,FLAGS_RAY)
	setv(_floor,FLAGS_DRAW_RAY_COLLISION)
	setv(_floor,FLAGS_DRAW_PLOYGON_LINE)
	setv(_floor,FLAGS_DISABLE_CULL_FACE)
	func_set_glsl_parms(_floor,'uvScale',scale)--����diffuse.vs (uniform float _uvScale)uv�ظ�ֵ
	return _floor
end
------------------------------------------------------------------

--����һ����ɫģ��
local function f_init_character()
	local url = 'triangle';--bauul
	url = 'bauul'--��ɫģ�ͻ���صıȽ���,������һ�����������
	--url = 'horse'
	
	local horse=func_loadmd2(url,'bauul.tga','vbo')--'horse'
	func_rename(horse,'_horse');
	
	func_setRotateX(horse,PI/2)--��תһ������
		
	func_set_scale(horse,url == 'bauul' and 1/50 or 1)
	
	--func_set_x(horse,-5)
	--func_set_z(horse,-5)
	--setv(horse,FLAGS_DRAW_RAY_COLLISION)

	func_anim_push(horse,"stand",0,39);
	func_anim_push(horse,"run",40,45);
	func_anim_push(horse,"jump",66,71);
	
	func_set_anim(horse,"stand")
	
	func_set_ptr_fps(horse,7)
	
	change_attr(horse,"animtor_play");--����
	
	return horse;
end


---[
f_init_ui();

infowin_show(100,0);

f_create_box();
f_create_cube();

_floorObj = f_create_floor(30);
f_init_character()
_target = func_loadobj('box',nil,'_target',false);
setv(_target,FLAGS_DRAW_PLOYGON_LINE)

--func_set_camera_pos(0,-5,-20)
local _f = 0;
--drawCall�ص�


local function f_drawCall(v)
	---[[
	_f = _f + func_fps()*0.00001;
	func_setRotateZ(_target,_f);
	--]]
end

evt_on(_target,EVENT_ENGINE_RENDER_3D,f_drawCall);


test_unit_01_init();

--xml �ӿڲ���

--[[
local xml = core_xml("load","//resource//monster.xml");
local node = core_xml("getNodeByIndex",xml,0);
node = core_xml("getrow",xml,"id","arrow");
local s = core_xml("getstr",node,"mesh",128);
local n = core_xml("getfloat",node,"ray") + 1
print("["..s.."]n = "..n);

core_xml("del",xml);

--]]


--[[
local xml = xml_load("//resource//monster.xml");
local node = xml_get_node(xml,"id","arrow");
local str = xml_get_str(node,"mesh");

local f = xml_get_float(node,"ray")

print(string.format("%s,%.3f",str,f));
xml_del(xml);

func_load("//resource//monster.xml");

--]]
