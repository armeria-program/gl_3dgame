dofile("..\\include\\lua\\IPlugin.lua")	--����ӿڶ���

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

--���ز��
function PluginMan:load(plugin)
	local url = "../include/lua/"..plugin..".lua";
	local p = dofile(url);
	--stack_push(self.list,p);
	func_print(">>>>���ز��"..p:getName()..url);
	return p;
end

--ж�ز��
function PluginMan:unload(p)
	func_print("<<<<ж�ز��"..p:getName());
	p:unload();
	p = nil;
end