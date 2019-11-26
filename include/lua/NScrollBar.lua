local DIRECTION_HORIZONTAL = 0	--ˮƽ,�� 
local DIRECTION_VERTICAL   = 1 	--��ֱ,��

NScrollBar = {
	name = nil,
	type = 12,
	
	bg,--�ɹ����ı���(Image)
	btn,--��������С��ť
	
	--�ص����ص�����
	callBack,
	callBackParam,
};

NScrollBar.__index = NScrollBar;

local function f_scrollBarBG_Click2(name,p)
	print(name,p);
end
--�������α��ƶ���ʱ�򴥷�
local function f_f_LuaDrag_move(progress,self)    
	--print(data,p);
	--print(data);;
	if(self.callBack) then
		self.callBack(progress,self.callBackParam);
	end
end

function NScrollBar:bindCallback(callBack,callBackParam)
	self.callBack = callBack;
	self.callBackParam = callBackParam;
end

function NScrollBar:new(cw,ch)
	local self = {};
	setmetatable(self, NScrollBar);
	cw = cw or 100;
	ch = ch or 15;
	
	local _dragDirection;	--�����ķ���
	local barSize;			--������ĳߴ�
	if(cw > ch) then
		_dragDirection=DIRECTION_HORIZONTAL;
		barSize = ch;
	else
		_dragDirection=DIRECTION_VERTICAL;
		barSize = cw;
	end
	
	--�����ɻ����ı���
	local bg = Image:new(cw,ch);
	bg:mouseEnable(true);
	bg:seticon("checkbox.png");
	self.bg = bg;	
	--����С��ť
	local btn = Shape:new(true,barSize,barSize);
	--local btn = Image:new(barSize,barSize); 
	--btn:seticon("gundi.png");
	
	btn:mouseEnable(true);
	btn:setcolor(0,1,0);
	btn:set_drag_direct(_dragDirection);
	--���ÿ���ק��Χ
	--btn:set_drag_rect(0,0,cw,ch);
	
	self.btn = btn;
	
	bg:addChild(btn:get_container());
	
	--bg:drawPloygonLine(true);
	
	--bg:on(EVENT_ENGINE_SPRITE_CLICK,f_scrollBarBG_Click2,self);
	btn:on(EVENT_ENGINE_SPRITE_CLICK_MOVE,f_f_LuaDrag_move,self);
	
	self:set_pos(0,0);
	return self;
end

--��������
function NScrollBar:set_pos(x,y)
	local bg = self.bg;
	local btn = self.btn;
	bg:set_pos(x,y);
	local cw,ch = bg:get_size();
	btn:set_drag_rect(0,0,cw,ch);
end
function NScrollBar:get_container()
	local bg = self.bg;
	return bg:get_container();
end

function NScrollBar:dispose()
	local bg = self.bg;
	local btn = self.btn;
	bg:off(EVENT_ENGINE_SPRITE_CLICK,f_scrollBarBG_Click2);
	bg:dispose();
	
	btn:dispose();
	btn:off(EVENT_ENGINE_SPRITE_CLICK_MOVE,f_f_LuaDrag_move);
	
	func_clearTableItem(self);
end
