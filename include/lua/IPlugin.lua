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