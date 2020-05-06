--[[
--�ο�����


local s= LineNode:new(3);
s:push(-0.5,0.4,0.5);
s:push( 0.5, 0.0, 0.5);
s:push( -0.5, -0.49, -0.5);
s:setcolor(1,0,0);
s:graphics_end();
--s:dispose();--����

]]

--�����Զ�����߶�
LineNode = {
	
};
LineNode.__index = LineNode;

--isPoint = true ʹ�õ�ģʽ��Ⱦ
function LineNode:new(cnt,isPoint,name)
	local self = {
		p,--C���linenodeָ��
	};
	setmetatable(self, LineNode);
	local _type = 0;
	if(isPoint) then
		_type = 1;
	end
	
	self.p=linenode("create",name or func_create_name(),cnt,_type);
	
	if(isPoint) then
		self:setcolor(1,1,0);
	else
		self:setcolor(1,0,0);
	end
	return self;
end

--[[
	�޸Ķ�Ӧ����������
	index ��0��ʼ
--]]
function LineNode:mod(index,x,y,z)
	linenode("mod",self.p,index,x,y,z);
end

function LineNode:push(x,y,z)
	linenode("push",self.p,x,y,z);
end

--���ƽ���
function LineNode:graphics_end()
	linenode("end",self.p);
end

function LineNode:setcolor(r,g,b)
	linenode("setcolor",self.p,r,g,b);
end

function LineNode:dispose()
	linenode("dispose",self.p);
end

