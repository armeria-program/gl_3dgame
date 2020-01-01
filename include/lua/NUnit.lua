NUnit = {
	p,
};
NUnit.__index = NUnit;

function NUnit:new()
	local self = {};
	setmetatable(self, NUnit);
	return self;
end

--��ȡģ�͵�����
function NUnit:get_type()
	return JEngine:getIns():get_type(self.p);
end

--���õ�ǰ�Ķ����cam
function NUnit:set_cam(cam)
	if(cam) then
		set_cam(self.p,cam);--ʹ��ָ����cam
	else
		JEngine:getIns():bind_3dcam(self.p);--ʹ��Ĭ�ϵ�cam
	end
end

--��ȡ���
function NUnit:get_p()
    return self.p;
end

function NUnit:visible(v)
	if(v) then
		setv(self.p,FLAGS_VISIBLE);--��ʾ
	else	
		resetv(self.p,FLAGS_VISIBLE);--����
	end
end

--����Ϊ�߿���Ⱦ
function NUnit:drawPloygonLine(v)
	local flag = FLAGS_DRAW_PLOYGON_LINE;
	if(v) then
		self:setv(flag);
	else
		self:resetv(flag);
	end
end
function NUnit:is_visible()
	return getv(self.p,FLAGS_VISIBLE) == 1;
end
function NUnit:scale(value)
	--print(value);
    func_set_scale(self.p,value);
end

function NUnit:setv(v)
    setv(self.p,v)
end
function NUnit:resetv(v)
    resetv(self.p,v)
end
function NUnit:getv(v)
    return getv(self.p,v)
end

function NUnit:get_scale()
    return func_get_scale(self.p);
end

function NUnit:rx(v)
    func_setRotateX(self.p,v)
end
function NUnit:ry(v)
    func_setRotateY(self.p,v)
end
function NUnit:rz(v)
    func_setRotateZ(self.p,v)
end
function NUnit:x(v)
    func_set_x(self.p,v);
end

function NUnit:y(v)
    func_set_y(self.p,v);
end
function NUnit:z(v)
    func_set_z(self.p,v);
end
function NUnit:set_position(x,y,z)
    func_set_position(self.p,x,y,z);
end
function NUnit:get_name()
    return func_get_name(self.p);
end

----���ö���Ĺؼ�֡֡��
function NUnit:set_fps(v)
	change_attr(self.p,"fps",tostring(v))
	
end
