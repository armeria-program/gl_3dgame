local _engine;

function engine_init()
	local e = {
		
	};
	
	local cam2d,cam3d= getEngine();
	e.cam2d = cam2d;
	e.cam3d = cam3d;
	--[[e.fbo2dcam = fbo2dcam;
	e.fbo3dcam = fbo3dcam;
    e.fboTexture = fboTexture;
	e.fbo = fbo;
	--]]
	--cam_set_2dxy(e.fbo2dcam,50,100);
	
	return e;
end

--[[--����һ���µĲ㼶
function engine_newPosZ()
    return get_attr(nil,"newPosZ");
end
--]]
--������
--[[function engine_rename(o,value)
	change_attr(o,"rename",tostring(value));
end

--ˢ������Ĭ�ϵ�cam
function engine_refreshCam3d()
	change_attr(nil,"cam_refreshModel",_engine.cam3d);
end

--���һ���ڵ㵽������frame,ֻ����ӽ�ȥ�Ľڵ���ܱ���Ⱦ
function engine_addNode(n)
    change_attr(nil,"ex_add",n);
end

--]]


--[[
function ex_addnode(n)
	change_attr(nil,"ex_add",n);
end

--��ǰ��fps
function engine_get_fps()
	return get_attr(nil,"fps");
end


--ÿһ֡��Ҫ�ĺ�����
function engine_getDelayTime()
    return get_attr(nil,"delayTime");
end

--�˳�����
function engine_exit()
    change_attr(nil,"exit");
end

function engine_setBg(r,g,b)
    --���ñ���ɫ
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end
--]]
function engine_get()
    return _engine;
end

--[[
--����һ��Ĭ�ϵ�3dcam������
function engine_bind_default_3dcam(self)
	local e = engine_get();
	set_cam(self,e.c    am3d);
end

--����Ĭ�ϵ���Ӱ��
function ex_set_cam(self)
	JEngine:getIns():bind_3dcam(self);
end
--]]
_engine=engine_init();
-------------------------------------------------
JEngine ={
	atals,
	cam2d,
	cam3d,
};

JEngine.__index = JEngine;
setmetatable(JEngine, Instance);--�̳��Ե���

function JEngine:new()
	local self =  Instance:new();
	setmetatable(self, JEngine);
	local uiAtals=atals_load("//resource//texture//","1");
    self.atals = uiAtals;
	
	
	local cam2d,cam3d = getEngine();
	self.cam2d = cam2d;
	self.cam3d = cam3d;
	return self;
end
function JEngine:get_cam()
    return  self.cam2d,self.cam3d;
end
function JEngine:get_atals()
    return self.atals;
end

function JEngine:dispose()
	atals_dispose(self.atals);
	func_clearTableItem(self);
end

function JEngine:new_z()
	 return get_attr(nil,"newPosZ");
end

function JEngine:rename(o,value)
	change_attr(o,"rename",tostring(value));
end

function JEngine:refresh_cam3d()
	change_attr(nil,"cam_refreshModel",self.cam3d);
end
function JEngine:get_fps()
    return get_attr(nil,"fps");
end

function JEngine:get_delayTime()
	return get_attr(nil,"delayTime");
end

function JEngine:exit()
	change_attr(nil,"exit");
end

function JEngine:setbg(r,g,b)
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end

function JEngine:bind_3dcam(o)
	set_cam(o,self.cam3d);
end

function JEngine:add(n)
	change_attr(nil,"ex_add",n);
end
