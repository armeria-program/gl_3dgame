--[[
--�ο�����


local s= LineNode:new(3);
s:push(-0.5,0.4,0.5);
s:push( 0.5, 0.0, 0.5);
s:push( -0.5, -0.49, -0.5);
s:setcolor(1,0,0);
s:pushend();
--s:dispose();--����

]]

--�����Զ�����߶�
LineNode = {
	p,--C���linenodeָ��
}
LineNode.__index = LineNode;

function LineNode:new(cnt,name)
	local self = {};
	setmetatable(self, LineNode);
	self.p=linenode("create",name or func_create_name(),cnt);
	self:setcolor(1,0,0);--Ĭ������line����ɫΪ��ɫ
	return self;
end

function LineNode:push(x,y,z)
	linenode("push",self.p,x,y,z);
end

function LineNode:pushend()
	linenode("end",self.p);
end

function LineNode:setcolor(r,g,b)
	linenode("setcolor",self.p,r,g,b);
end

function LineNode:dispose()
	linenode("dispose",self.p);
end

