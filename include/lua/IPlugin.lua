--����ӿڶ���
IPlugin = {
	
};

IPlugin.__index = IPlugin;

--�������
function IPlugin:load()
	func_error("IPlugin:load()�ӿ�δʵ��");
end

--���ж��,�˽ӿڻ�ж�ز����ٸò�����е���Դ
function IPlugin:unload()
	func_error("IPlugin:unload()�ӿ�δʵ��");
end

function IPlugin:getName()
	func_error("IPlugin:getName()�ӿ�δʵ��");	
end