Camera = {
	p,--camera���
};

Camera.__index = Camera;

function Camera:new(p)
	local self = {};
	setmetatable(self,Camera);
	self.p = p;
	return self;
end

--����camera������
function Camera:set_pos(x,y,z)
	-- print(x,y,z);
	cam(self.p,"set_xyz",x or 0,y or 0,z or 0);
	self:refresh();
end

--����camera
function Camera:reset()
	cam(self.p,"reset");
end

--��ȡcamera��C��ľ��
function Camera:get_p()
	return self.p;
end

--ˢ��model����
function Camera:refresh()
	cam(self.p,"refresh");
end

--����camera��rx��ֵ
function Camera:rx(v)
	cam(self.p,"rx",v);
	self:refresh();
end

--����cam
function Camera:dispose()
	func_clearTableItem(self);
end

--��һ������cam
function Camera:bind(o)
	set_cam(o,self.p);
end
