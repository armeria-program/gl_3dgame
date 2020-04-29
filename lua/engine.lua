
--				���������
--������ظ������ò��,����fpsView���ֹ���ʽС����
--����һ�ּ�����ɢ��ϵ�ģʽ,������ʹ�ÿ�ܲ��㹻С��,
--��չ���ܶ����������ģʽ,�Ƚ��ʺ����󾭳������䶯�����,
--���ǿ��Խ����ֶ��Ĳ��ַ�װ��һ��������й���ʽ��Ӧ��
require("plugin_man");
-------------------------------------------------
JEngine ={
	atals,
	cam2d,
	m_cam3d,
	
	plugin,--���������
};

JEngine.__index = JEngine;
setmetatable(JEngine, Instance);--�̳��Ե���

function JEngine:new()
	local self =  Instance:new();
	setmetatable(self, JEngine);
	
	--����һ��ͼ��
	local uiAtals=atals_load("//resource//texture//","1");
    self.atals = uiAtals;
	
	
	local cam2d,cam3d = getEngine();
	self.cam2d = cam2d;
	
	self.m_cam3d = Camera:new(cam3d);

	self.plugin = PluginMan:new();
	-- func_error(1111);
	return self;
end
--��ȡ����������ľ��
function JEngine:get_plugin()
	return self.plugin;
end

function JEngine:get_cam()
    return self.m_cam3d;
end
--��ȡ��Ļ�ĳߴ�
function JEngine:screen_size()
	return get_attr(nil,"screenSize");
end

--��ȡ���������
function JEngine:get_hit()
	return get_attr(nil,"hit");
end

function JEngine:get_atals()
    return self.atals;
end

function JEngine:dispose()
	atals_dispose(self.atals);
	func_clearTableItem(self);
end

--��ӡ��Ϣ
function JEngine:print_info()
	get_attr(nil,"ex_info");
end

function JEngine:rename(o,value)
	change_attr(o,"rename",tostring(value));
end

function JEngine:get_fps()
	--return get_attr(nil,"fps");

	local f = 1000/self:get_delayTime();
	return math.floor(f*10)/10;
	-- return self:get_delayTime();
end

function JEngine:get_delayTime()
	return get_attr(nil,"delayTime");
end

function JEngine:exit()
	change_attr(nil,"exit");
end

--��������ȡ�Զ���
function JEngine:find(name)
	return dofunc("find_node",name);
end

function JEngine:setbg(r,g,b)
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end
--����ÿһ֡��Ҫ�ļ��ʱ��
function JEngine:setDelayMs(ms)
    change_attr(nil,"custDelayMs",ms);
end
function JEngine:setfps(v)
	local a = math.ceil(1000/v);
	--print("a = "..a);
	self:setDelayMs(a);
end
--������o�󶨵�����Ĭ�ϵ�3dcam�ռ�
function JEngine:bind_3dcam(o)
	self:get_cam():bind(o);
end
--���һ����������
function JEngine:add(n)
	change_attr(nil,"ex_add",n);
end

--����һ����Դ,ֻ�Ǽ���,��������ص���Ⱦ�б�
--Ĭ�϶��Ǽ���vbo���͵�����
function JEngine:load(url,name)
	name = name or func_create_name();
	return change_attr(nil,"ex_loadVBO",name,url);
end

function JEngine:get_type(p)
	return	get_attr(p,"type");
end

function JEngine:get_type_str(p)
	local t = self:get_type(p);
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

-- return  JEngine:getIns();