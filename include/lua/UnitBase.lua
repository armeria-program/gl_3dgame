UnitBaseEvent = 10000;
UnitBaseEndMsg = "UnitBaseEndMsg";--�ƶ�����


--����������
local Animator = {
	p,--	TYPE_OBJ_VBO_FILE����
};
Animator.__index = Animator;
function Animator:new(p)
	local s = {};
    setmetatable(s,Animator);
	s.p = p;
    return s;
end

function Animator:dispose()
	func_clearTableItem(self);
end

--��ȡ��������
function Animator:total()
	local p = self.p;
	--if(self:get_type() == TYPE_OBJ_VBO_FILE) then
	local total = change_attr(p,"animtor_total");
	return total;
	--else
	--	func_error("����δʵ��get_anim_total");
	--end
end

--�Ƿ��ڲ���
function Animator:isPlaying()
	if(change_attr(self.p,"animtor_isPlaying") == 1) then
		return true;
	end
end

--[[
	��ʼ������(����ɶ�̬���������ļ�,������һ���༭���༭��Щ���ź�ƫ���������md2�ļ�)
	����ָ���Ķ���
	"stand",0,39
	"run",40,45
	"jump",66,71
--]]
function Animator:play(anim)
	local o = self.p;
	--self:pause();
	if(anim) then
		change_attr(o,"animtor_setcur",anim);--ָ����ǰ�Ķ���
	end
	change_attr(o,"animtor_play");
end

--�ָ��
function Animator:push(animname,s,e)
	change_attr(self.p,"animtor_push",animname,string.format('%s,%s',s,e));
end

--�������䲥�Ŷ���
function Animator:play_to(s,e)
	change_attr(self.p,"animtor_play_start_end",string.format('%d,%d',s,e));
	self:play();
end


--��ͣ
function Animator:pause()
	change_attr(self.p,"animtor_pause");
end

--*************************************************************
--ֻʵ�� p = TYPE_OBJ_VBO_FILE ���͵Ķ���
UnitBase = {

    p,--��ɫ���,�������е�md2,obj,md5���õ�ֵ
    speed,--�ƶ��ٶ�	
	anim,--�������������
};
UnitBase.__index = UnitBase;
function UnitBase:new()
    local s = {};
    setmetatable(s,UnitBase);
   --s.speed = 1000;
    return s;
end

function UnitBase:dispose()
	self.anim:dispose();
	ptr_remove(self.p);
	func_clearTableItem(self);
end

--���ö����cam�ľ���ռ�
local function f_set_cam(self)
	JEngine:getIns():bind_3dcam(self.p);
end
--��ȡ���
function UnitBase:get_p()
    return self.p;
end

--�������������
function UnitBase:get_anim()
	return self.anim;
end

--��ȡģ�͵�����
function UnitBase:get_type()
	return JEngine:getIns():get_type(self.p);
end

--���õ�ǰ�Ķ����cam
function UnitBase:set_cam(cam)
	if(cam) then
		set_cam(self.p,cam);--ʹ��ָ����cam
	else
		JEngine:getIns():bind_3dcam(self.p);--ʹ��Ĭ�ϵ�cam
	end
end

--����VBOģ��
function UnitBase:loadvbo(modelURL,maturl,cam)
            
    --print("**������ɫ",modelURL,name);

    local md2=JEngine:getIns():load(modelURL);
	
	--load_VBO_model(name,modelURL);
    local material = func_load(maturl);
    setMaterial(md2,material);
	setv(md2,FLAGS_VISIBLE);--��ʾģ�Ͷ���
    
	--f_split_init(md2);
	
	
    --setv(md2,FLAGS_DISABLE_CULL_FACE)--����˫�涼����Ⱦ
    --local ss =new_sharp;
   
    self.p = md2;
	
	JEngine:getIns():add(self.p);
	self:set_cam(cam);
	--f_set_cam(self);
	
	self.anim = Animator:new(self.p);
end
--����һ������������
function UnitBase:loadbox()
    local url = "\\resource\\obj\\box.obj";--tri
    local maturl = maturl or "//resource//material//triangle.mat"
    local name = func_create_name();--f_createName(self);
    local obj=load_VBO_model(name,url);--box	arrow
   
    local mat = func_load(maturl);
    --self.material = mat;
	setMaterial(obj,mat);
    setv(obj,FLAGS_DRAW_PLOYGON_LINE)--�߿�
    --setv(obj,FLAGS_DISABLE_CULL_FACE);--����˫�涼����Ⱦ
    setv(obj,FLAGS_REVERSE_FACE);
    setv(obj,FLAGS_VISIBLE);
    self.p = obj;
	f_set_cam(self);
end

function UnitBase:load_model(url,maturl)
    url =  url or "\\resource\\obj\\plane.obj";
    maturl = maturl or "//resource//material//triangle.mat"
    local _floor = load_model(func_create_name(),url)		-- func_loadobj('quad',nil,'myObj1',false)--quad
	local mat = func_load(maturl);
    --self.material = mat;
    setMaterial(_floor,mat);
    --glsl_set(mat,string.format("_lineColor,%s","0.5,0.5,0.5"));
	
	setv(_floor,FLAGS_VISIBLE);
	

--	setv(_floor,FLAGS_RAY)
--	--setv(_floor,FLAGS_DRAW_RAY_COLLISION)
--	setv(_floor,FLAGS_DRAW_PLOYGON_LINE)--�߿�
--	setv(_floor,FLAGS_DISABLE_CULL_FACE)
    self.p = _floor;
	f_set_cam(self);
end

function UnitBase:setv(v)
    setv(self.p,v)
end
function UnitBase:getv(v)
    return getv(self.p,v)
end
function UnitBase:scale(value)
    func_set_scale(self.p,value);
end
function UnitBase:get_scale()
    return func_get_scale(self.p);
end

function UnitBase:rx(v)
    func_setRotateX(self.p,v)
end
function UnitBase:ry(v)
    func_setRotateY(self.p,v)
end
function UnitBase:rz(v)
    func_setRotateZ(self.p,v)
end
function UnitBase:x(v)
    func_set_x(self.p,v);
end

function UnitBase:y(v)
    func_set_y(self.p,v);
end
function UnitBase:z(v)
    func_set_z(self.p,v);
end
function UnitBase:set_position(x,y,z)
    func_set_position(self.p,x,y,z);
end
function UnitBase:get_name()
    return func_get_name(self.p);
end

function UnitBase:set_speed(v)
    self.speed = v;
end
--[[
function UnitBase:refresh()
	func_update_mat4x4(self.p);
end
--]]
--��һ������¼�
function UnitBase:bindRayPick(func)
    --print(self:getv(FLAGS_RAY));
    self:setv(FLAGS_RAY);
    evt_on(self.p,EVENT_RAY_PICK,func);
end
local function f_endCall(data)
    local p = func_find_obj(data);
--    local u = allUnits[data];
    func_set_anim(p,"stand");
    evt_off(p,EVENT_ENGINE_BASE_END,f_endCall);

    evt_dispatch(p,UnitBaseEvent,UnitBaseEndMsg);
    --print(data);
end
--������ײ��
function UnitBase:load_collide(model)
    func_bind_collide(self.p,model or "\\resource\\obj\\box.obj",frame);
end

function UnitBase:move(x,y,z)
    --print(x,y,z);
    x = tonumber(x);
    y = tonumber(y);
    z = tonumber(z);
    local o = self.p;
    
    --print("==============>name=",func_get_name(o));

	if(o==nil) then
        func_error("unit.p = nil");
        return;
    end
--    if(self.speed==nil) then
--        func_error("unit.speed = nil");%
--        return;
--    end
    self.speed = self.speed or 1000;--Ĭ��ʹ��1000�����ƶ�һ��������λ������

	local px,py,pz = func_get_xyz(o);
	
	y = py;
	--print(self.offset_y);
	local distance = vec_distance(px,py,pz,x,y,z);--����ƽ�����
	
	func_look_at(o,x,y,z);--ת��Ŀ������
	
	func_set_anim(self.p,"run");
	
    evt_off(o,EVENT_ENGINE_BASE_END,f_endCall);
	evt_on(o,EVENT_ENGINE_BASE_END,f_endCall);

	func_move(o,distance * self.speed,x,y,z);
end