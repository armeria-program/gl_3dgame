--����ӿڶ���
IPlugin = {
	
};

IPlugin.__index = IPlugin;

--�������
function IPlugin:new()
	func_error("IPlugin:new()�ӿ�δʵ��");
end

--���ж��,�˽ӿڻ�ж�ز����ٸò�����е���Դ
function IPlugin:dispose()
	func_error("IPlugin:dispose()�ӿ�δʵ��");
end

function IPlugin:getName()
	func_error("IPlugin:getName()�ӿ�δʵ��");	
end
--[[
	��ͼ���͵Ĳ���̳д���
]]
IPluginView = {
	
};

IPluginView.__index = IPluginView;
setmetatable(IPluginView, IPlugin);

--��ʾ/���ز��
-- function IPluginView:visible(v)
--     self.nskin:visible(v);
-- end

--v = true����
function IPluginView:show(v)
	if(v) then
		self.nskin:center();
	end
	self.nskin:visible(true);
end

function IPluginView:hide()
    self.nskin:visible(false);
end

--����Ƿ���ʾ��
function IPluginView:is_visible()
 	return self.nskin:is_visible();
end

--����
function IPluginView:center()
	self.nskin:center();
end

