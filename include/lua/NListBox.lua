--�����б����
NListBox = {
	bg,--Shape
	tf,--NLabel
	h,--����ʱ��ĸ߶�
	
	data,--tabel�б��е�����

	index,--��ǰ������

	labelstr,--��һ�ε�labelֵ
	
	callBack,
	param,
};

NListBox.__index = NListBox;
setmetatable(NListBox, Base);

--��ȡ�б�ĳ���
function NListBox:get_cnt()
	local list = self;
	local cnt = 0;
	if(list.data) then
		cnt = #list.data;
	end
	return cnt;
end
--��ȡѡ����ı��ַ�
local function f_get_label(list,index)
	local label;
	if(list.data) then
		label = list.data[index + 1];
	end
	return label;
end

local function f_refresh(self,index,isSpread)
	local list = self;
	--local i = index;
	
	
	
	local c = f_get_label(self,index) or "";
	--print(index,isSpread,c);
	
	if(c~="") then
		self.labelstr = c;
	else
		c = self.labelstr;
	end
	
	
	if(list.data) then
		local n;
		local str = c.."\n";
		local _len = #list.data;
		if(isSpread) then
			for n = 1,_len do
				str = str..list.data[n].."\n";
			end
		end
		
		if(#str > 0 ) then
			local s = string.sub(str,0,#str - 1);--ȥ������һ�����з�
			local label = list.tf;
			
			label:set_text(s);
			--print(s);
		end
	end
	if(self.callBack and index ~= -1)then
		self.callBack(self,index,self.param);
	end
end

function NListBox:get_container()
	return self.bg:get_container();
end
function NListBox:selectIndex(v)
	f_refresh(self,v);
end

local function f_click(name,self)
--	f_select_call(list);
	local bg = self.bg;
	local w,h = bg:get_size();
	local height = self.h;
	--print(height,h);
		
	local isSpread;
	
	if(h <= self.h) then
		--չ��
		height = (self:get_cnt() + 1) * self.h;
		--print(0);
		isSpread = true;
	else
		--print(1);
	end
	bg:set_height(height);
	bg:hit_rect(0,0,w,height);
	
	local mx,my = bg:local_mouse_xy();
	local index = math.floor(my / self.h) - 1;
	
	--print(index,name,self,h,height,self.h );
	--print(height,h,self.h);
	if(index>=0) then
		self.index = index;
	end
	f_refresh(self,index,isSpread);
end

function NListBox:new(w)
	
	local self = Base:new();
	setmetatable(self, NListBox);
	self:settype(13);
	
	local h = 12;
	w = w or 60;
	--local label = NLabel:new();
	--label:set_text("R");
	--local w,h = label:get_size();
	
	--print(w,h);
	
	local bg = Shape:new(true,w,h);
	self.bg = bg;
	bg:mouseEnable(true);
	bg:on(EVENT_ENGINE_SPRITE_CLICK,f_click,self);
	self.h = h;
	self.index = -1;
	local label = NLabel:new();
	self.tf = label;
	bg:addChild(label:get_container());
	return self;
end

function NListBox:addItem(str)
	local list = self;
	if(list.data==nil)then
		list.data = {};
	end
	list.data[#list.data+1]=str;
end

function NListBox:dispose()
	local bg = self.bg;
	bg:off(EVENT_ENGINE_SPRITE_CLICK,f_click);
	local tf = self.tf;
	tf:dispose();
	bg:dispose();
	func_clearTableItem(self);
end

function NListBox:bind(func,p)
	self.callBack = func;
	self.param = p;
end