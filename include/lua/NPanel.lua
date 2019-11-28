NPanel = {
	bg,--Shape
	isDrag,		--�Ƿ������ק
	isCenter,	--�Ƿ����
};

NPanel.__index = NPanel;
setmetatable(NPanel, Base);

function NPanel:new(w,h)
	w = w or 150;
	h = h or 100;
	local self = Base:new();
	setmetatable(self, NPanel);
	
	local bg = Shape:new(true,w,h);
	bg:setcolor(0.2,0.2,0.2);
	self.bg = bg;
	
	self:settype(15);
	return self;
end

--��̨�ߴ�仯�¼�
local function f_resize(evtData,self)
	local sw,sh = func_screenSize();
	local bg = self.bg;
	local sx,sy = bg:get_pos();
	local bgw,bgh = bg:get_size();
		
	--print(sx,sy,bgw,bgh);
	if(self.isDrag and sw > bgw and sh > bgh) then
		--print(-sx,-sy,sw-sx,sh-sy);
		bg:set_drag_rect(-sx,-sy,sw-sx,sh-sy);
	else
		func_error("not set ,sw="..sw..",sh="..sh);
	end
	if(self.isCenter) then
		self:center();
	end
end

function NPanel:set_pos(x,y)
	local bg = self.bg;
	bg:set_pos(x,y);
end

--��ȡShape��ǰ����
function NPanel:get_container()
	return self.bg:get_container();
end

--����
function NPanel:center()
	local bg = self.bg;

	local x,y;
	local sx,sy = func_screenSize();
	local bgw,bgh = bg:get_size();
	--print(bgw,bgh );
	x = (sx - bgw)/2;
	y = (sy - bgh)/2;
	
	self.bg:set_pos(x,y);
	--self.bg:setcolor(1);
end
--���ÿ��Ծ���
function NPanel:enable_center(v)
	local bg = self.bg;
	self.isCenter = v;
	if(v) then
		bg:on(EVENT_ENGINE_RESIZE,f_resize,self);
	else
		bg:off(EVENT_ENGINE_RESIZE,f_resize);
	end
end

function NPanel:setDrag(v)
	self.isDrag = v;
	local bg = self.bg;
	f_resize(nil,self);
	bg:mouseEnable(v);
end

function NPanel:dispose()
	local bg = self.bg;
	self:enable_center(false);
	--func_error(0);
	bg:dispose();
	func_clearTableItem(self);
end

--[[

����:

local p = NPanel:new();
p:enable_center(true);
p:setDrag(true);
p:center();

--]]
