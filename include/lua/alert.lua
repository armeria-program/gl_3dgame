function alert_init()
    local new_sharp = {
		name=nil;
		type = 4,
	    bg;			--sprite
	    closeBtn;	--btn
	    label;
	    isDrag;		--�Ƿ������ק
        closeSize = 30;--�رհ�ť�Ŀ��
		w = nil,
		h = nil,
		close_url = nil,
		bg_url = nil,
    };
    return new_sharp;
end

local function f_drag(evtData,self)
	--��ק�ƶ��¼�
end

--����
local function f_center(self)
	local x,y;
	local sx,sy = func_screenSize();
	local bgw,bgh = func_get_sprite_size(self.bg);
	x = (sx - bgw)/2;
	y = (sy - bgh)/2;
	func_setPos(self.bg,x,y);
end

function alert_set_pos(self,x,y)
	func_setPos(self.bg,x,y);
end

local function f_resize(evtData,self)
	local sw,sh = func_screenSize();
	
	local sx,sy = func_get_sprite_xy(self.bg);
	local bgw,bgh = func_get_sprite_size(self.bg)

	if(self.isDrag and sw > bgw and sh > bgh) then
		--print(-sx,-sy,sw-sx,sh-sy);
		sprite_setDragScope(self.bg,-sx,-sy,sw-sx,sh-sy);
	end
	f_center(self);
end;

--���ؽ���
local function f_hide(self)
	resetv(self.bg,FLAGS_VISIBLE);
	if(self.label) then
		ftext_vis(self.label,0);
	end
	if(self.closeBtn) then
		btn_visible(self.closeBtn,false);
	end
end
--���ÿ���ק
function alert_set_drag(self,value)
	self.isDrag = value;
	--if(value) then
	--	evt_on(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag,self);--������ק�¼�
	--else
	--	evt_off(self.bg,EVENT_ENGINE_SPRITE_CLICK_MOVE,f_drag);		
	--end
end

local function f_closeCallBack(data,param)
	f_hide(param);
end

--����label
local function f_create_label(self)
	local w = self.w;
	local closeSize = self.closeSize
	self.label=ftext_create(w-closeSize,w-closeSize);
    ftext_set_buffer(self.label,256);
	func_addchild(self.bg,ftext_get_container(self.label));
end

--����closeBtn 
local function f_create_close_btn(self)
	local x = 0;
	local y = 0;
	local closeSize = self.closeSize;
	--print(self.close_url);
	if(self.close_url == nil) then 
		func_error("self.close_url = nil");
	end
	self.closeBtn=btn_create(x,y,closeSize,closeSize,self.close_url);
	
	btn_bindClick(self.closeBtn,f_closeCallBack,self);
	func_addchild(self.bg,btn_get_container(self.closeBtn),self.w-self.closeSize,0);
end

--����
function alert_get_container(self)
	return self.bg;
end

--������ʾ(�رհ�ť)
function alert_add_closebtn(self)
	if(self.closeBtn == nil) then
		f_create_close_btn(self);
	end
end

local function f_alert_create(self,w,h)
	
	local x = 0;
	local y = 0;
	
	local sprite=func_create_grid_sprite(x,y,w,h,self.bg_url,name);
	self.bg = sprite;
	
	self.w = w;
	self.h = h;

	alert_set_drag(self,false);

    
	return self;
end

function alert_enable_resize(self,v)
	if(v) then
		evt_on(self,EVENT_ENGINE_RESIZE,f_resize,self);
		f_resize(nil,self);
	else
		evt_off(self,EVENT_ENGINE_RESIZE,f_resize);
	end
end

--������ͼ
function alert_dispose(self)
	ptr_remove(self.bg);
	if(self.label)then
		fext_dispose(self.label);
	end
	if(self.closeBtn) then
		btn_dispose(self.closeBtn);
	end
end

function alert_show(self)
	if(self.closeBtn) then
		btn_visible(self.closeBtn,true);
	end
	
	setv(self.bg,FLAGS_VISIBLE);
end

local function f_tex_complete(n)
	local self = n.self;
	f_alert_create(self,n.width,n.height);
	alert_show(self);
	
	local obj = func_get_address(self);
	--evt_dispatch(obj,EVENT_ENGINE_COMPLETE,obj);
end
--�����ı�
function alert_set_label(self,str)
	if(str) then
		if(self.label == nil) then
			f_create_label(self)
		end
		
		ftext_vis(self.label,1);	
		ftext_reset(self.label,str);
	end
end

--str���ݷ�nilֵ��ʱ��ᴴ��һ��label����
--url�ָ������1��λ�Ǳ���,2��λ�ǹرհ�ť
function alert_start(self,url,width,height)
	
	width = width or 300;
	height = height or 150;
	url = url or "gundi.png;checkbox.png";--gundi
	local arr=func_split(url,";");
	--print(arr[1],arr[2]);
	self.bg_url = arr[1];
	self.close_url = arr[2];
	--loadtexs(url,f_tex_complete, {self=self});
	
	f_tex_complete({self=self;width=width;height=height});
end

--******************************************************
Alert = {
	str,--�ı�
	nskin,
};
Alert.__index = Alert;
setmetatable(Alert, Instance);--�̳��Ե���

function Alert:new()
	local self = Instance:new();
	setmetatable(self, Alert);
	return self;
end

local function f_refresh(self)
	local nskin = self.nskin;
	
	local label = nskin:find("label");
	label:set_text(self.str);
	
	nskin:visible(true);
end

--�رհ�ť�ص�
local function btnClick(self)
	--print(btnName,self);--p = abc
	self:hide();
end
local function f_rx_handle(progress,p)

end

local function f_cpmlete(skin,self)
	--print(skin,self,self.str);
	local closebtn = skin:find("close");
	closebtn:bind_click(btnClick,self);
	f_refresh(self);
	
	--[[
	local nl = skin_find(skin,"nb1");--NListBox:new();
	nl:addItem("a1");
	nl:addItem("b2");
	
	
	--btn_bindClick(closebtn,btnClick,self);
	
	
	local pb1 = skin_find(skin,"pb1");--NListBox:new();	
	pb1:progress(0.7);
	--pb1:dispose();
	local sc = skin_find(skin,"sc");
	sc:bindCallback(f_rx_handle,self);
--]]
	
	--self:dispose();
end

function Alert:hide()
	self.nskin:visible(false);
end

function Alert:show(str)
	self.str = str;
	if(self.nskin==nil) then
		local nskin = NSkin:new();
		self.nskin = nskin;
		evt_once(nskin,ENGINE_EVENT_COMPLETE,f_cpmlete,self);
		nskin:load("\\resource\\alert.xml","smallbtn.png;checkbox.png");
	else
		--f_cpmlete(self.nskin,self);
		f_refresh(self);
	end
end
function Alert:dispose()
	self.nskin:dispose();
	func_clearTableItem(self);
end