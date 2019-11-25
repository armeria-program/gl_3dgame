Shape = {
	name = nil,
	type = nil,
	w,h,
	container,--Sprite����
}

Shape.__index = Shape;

function Shape:new(issuper,w,h)
	w = w or 32;
	h = h or 32;
	local self = {};
	setmetatable(self, Shape);
	
	self.w = w;
	self.h = h;
	if(issuper) then
		self:init();
	end
	
	return self;
end

function Shape:init()
	self:settype(11);
	local sprite = sprite_create_typical(nil,0,0,self.w,self.h);
	self.container = sprite;
	engine_addNode(sprite);	
	local material = func_load("//resource//material//shape.mat");
	setMaterial(self.container,material);
	self:mouseEnable(false);--Ĭ�Ϲر�����¼�
	--self.material = material;
	
	--self:setcolor(1.0);
end

function Shape:setname(name)
	self.name = name;
end

--������ɫ
function Shape:setcolor(r,g,b)
	r = r or 0;
	g = g or 0;
	b = b or 0;
	local str = string.format("_lineColor,%f,%f,%f",r,g,b);
	local m = sprite_get_material(self.container);
	glsl_set(m,str);
end

function Shape:settype(t)
	--print(t);
	self.type = t;	
end

function Shape:gettype()
	return self.type;
end

function Shape:visible(v)
	if(v) then
		setv(self.container,FLAGS_VISIBLE);
	else
		resetv(self.container,FLAGS_VISIBLE);
	end
end

function Shape:is_visible()
	return getv(self.container,FLAGS_VISIBLE) == 1;
end

function Shape:getsize()
	return self.w,self.h;
end

--����Ϊ�߿���Ⱦ
function Shape:drawPloygonLine(v)
	if(v) then
		setv(self:get_container(),FLAGS_DRAW_PLOYGON_LINE);
	else
		resetv(self:get_container(),FLAGS_DRAW_PLOYGON_LINE);
	end
end

--��ȡShape��ǰ����
function Shape:get_container()
	return self.container;
end

function Shape:set_pos(x,y)
	func_set_local_pos(self.container,x,y);
end

function Shape:mouseEnable(v)
	local _status = v == true and 1 or 0;
	--print(status);
	sprite_enable_click(self.container,_status);
end

function Shape:on(id,func,params)
	evt_on(self.container,id,func,params);
end

function Shape:off(id,func)
	evt_off(self.container,func);
end

--����
function Shape:dispose()
	ptr_remove(self.container);
	func_clearTableItem(self);
end

--����Shape�Ŀ��
function Shape:set_width(w)
	self.w = w;
	change_attr(self.container,"sprite_set_width",w);
end

--����Shape�ĸ߶�
function Shape:set_height(h)
	self.h = h;
	change_attr(self.container,"sprite_set_height",h);	
end

--����Shape�Ŀ��
function Shape:set_size(w,h)
	self.w = w;
	self.h = h;
	func_set_sprite_size(self.container,w,h);
end

--��һ��sprite��ӵ���shape��,ʹ����Ϊ�Ӷ���
function Shape:addChild(sprite,x,y)
	func_addchild(self:get_container(),sprite,x,y);
end

--������ק�ķ���
function Shape:set_drag_direct(v)
	sprite_set_direction(self:get_container(),v);
end
--���ÿ���ק��Χ
function Shape:set_drag_rect(x,y,w,h)
	sprite_setDragScope(self:get_container(),x,y,w,h);
end