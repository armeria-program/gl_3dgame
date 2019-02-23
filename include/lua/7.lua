dofile("..\\include\\lua\\core.lua");



--[[
function f_load_md2()
	md2=load_VBO_model(func_create_name(),"\\resource\\md2\\horse.md2");
	setMaterial(md2,func_load("//resource//material//horse.mat"));
	setv(md2,FLAGS_VISIBLE);
end
local btn = btn_create(0,40)
btn_label(btn,"load md2")
btn_bindClick(btn,f_load_md2)--�л���ʾ�Ƿ�Ҫ�߿���Ⱦ
--]]




--����һ��ָ���뾶�ĵذ�
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
	--setv(_floor,FLAGS_DRAW_PLOYGON_LINE)
	setv(_floor,FLAGS_DISABLE_CULL_FACE)
	func_set_glsl_parms(_floor,'uvScale',scale)--����diffuse.vs (uniform float _uvScale)uv�ظ�ֵ
	return _floor
end


fps();

--����һ����ɫ
local unit = unit_create();
func_set_y(unit.p,0.5);

--����һ���ذ�
f_create_floor(1);