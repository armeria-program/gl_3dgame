--------------------------------------------------------------------
-- ��Ϣ�������

 
local function f_select(list,index,p)
	local self = p;
	local _stat = self._stat;
	--local index =listbox_get_index(list);
	---[[
    if (index == 0) then
		print("windows test!");
    elseif (index == 1) then
        func_gc();
    elseif (index == 2) then
        self:loadfps();
    elseif (index == 3) then
        _stat = not _stat;
--        print(_stat);

        if(_stat) then
            JEngine:getIns():setbg(0.1,0.1,0.1);
        else
            JEngine:getIns():setbg(0.4,0.4,0.4);
        end
	elseif (index == 4) then
		--����һ��mesh
		
    elseif(index == 5) then
        local cam3d = JEngine:getIns():get_cam();
        cam3d:reset();
    end
	--]]
end




InfoWin = {
	list,
	_stat,
	fps,
};
InfoWin.__index= InfoWin;
function InfoWin:new(x,y)

	local self = {};
	setmetatable(self, InfoWin);
	self:init(x,y);
	return self;
end
--�л���ʾfps
function InfoWin:loadfps()
	if(self.fps==nil) then
		self.fps = JEngine:getIns():get_plugin():load("view/FpsView");--���ز��
		self.fps:show();
	else
		JEngine:getIns():get_plugin():unload(self.fps);
		self.fps = nil;
	end
end

function InfoWin:init(x, y)
	
	
	--[[local list  = listbox_new(x or 0, y or 0);
	listbox_bind(list,f_select);
	
	
	listbox_add(list,"������Ϣ");
	listbox_add(list,"gc");
	listbox_add(list,"fps");
	listbox_add(list,"������ɫ");
	listbox_add(list,"����mesh");
	listbox_add(list,"����cam");
	listbox_set_title(list,"infowin");--]]
	
	local list = NListBox:new(x,y,128);
	list:addItem("������Ϣ");
	list:addItem("gc");
	list:addItem("fps");
	list:addItem("������ɫ");
	list:addItem("����mesh");
	list:addItem("����cam");
	list:bind(f_select,self);

	
	--listbox_del(list);
	--return list;
	
	
	--list:dispose();
	
	self.list = list;
end

function InfoWin:dispose()
	self.list:dispose();
	
	if(self.fps) then
		JEngine:getIns():get_plugin():unload(self.fps);
	end
	
	func_clearTableItem(self);
end