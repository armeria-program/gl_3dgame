local EVENT = require("event");

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
	nskin = nil,
	centerFlag,--�Ƿ������ʾ
};

IPluginView.__index = IPluginView;
setmetatable(IPluginView, IPlugin);

--��ʾ/���ز��
-- function IPluginView:visible(v)
--     self.nskin:visible(v);
-- end

--v = true����
function IPluginView:show()
	if(self.centerFlag) then
		self.nskin:center();
	end
	-- self:onDisplay();
	self.nskin:visible(true);
	evt_dispatch(self,EVENT.DISPLAY,self);
end

function IPluginView:hide()
	-- self:unDisplay();
	self.nskin:visible(false);
	evt_dispatch(self,EVENT.UNDISPLAY,self);
end

--����Ƿ���ʾ��
function IPluginView:is_visible()
 	return self.nskin:is_visible();
end

--����
function IPluginView:center()
	self.nskin:center();
end


function IPluginView:setCenter(v)
	self.centerFlag = v;
end
--��������
function IPluginView:setData(data)
end