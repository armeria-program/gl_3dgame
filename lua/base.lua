Base = {
	
};
Base.__index = Base;
function Base:setname(name)
	self.name = name;
end

function Base:new()
	local self = {
		name = nil,--�ö��������
		type = nil,--�ö��������
		address = nil,--��ַ
	};
	setmetatable(self, Base);
	self.address=func_get_address(self);--�������ַ
	-- print("address:"..self.address);
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

--************����**************
Instance={
	ins,--��������
};
Instance.__index = Instance;

function Instance:new()
	local self = {};
	setmetatable(self, Instance);
	--print(tostring(self).."������ʼ��");
	return self;
end

--����
function Instance:getIns()
    if self.ins == nil then
        self.ins = self:new();	
    end
    return self.ins;
end