Base = {
	name = nil,--�ö��������
	type = nil,--�ö��������
};
Base.__index = Base;
function Base:setname(name)
	self.name = name;
end

function Base:new()
	local self = {};
	setmetatable(self, Base);
	return self;
end

function Base:settype(t)
	--print(self,"��������"..t);
	self.type = t;	
end

function Base:gettype()
	return self.type;
end

function Base:setname(name)
	--print(string.format("��������Ϊ[%s]",name));
	self.name = name;
end

function Base:getname()
	return self.name;
end