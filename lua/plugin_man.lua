require("IPlugin")	--����ӿڶ���

--print("�����������ʼ��");
PluginMan = {
	--list,--����б�
};
PluginMan.__index = PluginMan;
function PluginMan:new()
	local self = {};
	setmetatable(self, PluginMan);
	--self.list = stack_new();
	return self;
end

function PluginMan:getInfo(p)
	return p:getName().." "..tostring(p);
end

--���ز��

function PluginMan:load(plugin)
	-- local url = "../include/lua/"..plugin.."";
	local p = require(plugin):new();
	func_print(">>>>�����ʼ�����"..self:getInfo(p).." url="..plugin);
	return p;
end

--ж�ز��
--�ڴ�й©,����֮
--[[
function PluginMan:unload(p)
	p:unload();
	func_print("<<<<ж�ز��"..f_getInfo(p));	
	
	--setmetatable(getmetatable(p),nil);
	--setmetatable(p, nil);
	
	--print(getmetatable(p));
end
--]]

