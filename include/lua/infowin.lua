

function f_pick(p)
	--moduleUI_bind(p)
	--f_selectPick(p)
    --print("f_pick:"..p);
    --crl:bind(p);
	print("pick now!");--ʰȡ�Ķ���
end

--------------------------------------------------------------------
-- ��Ϣ�������

--[[local mc = FpsView:new();
mc:show();--]]
local _stat;
local function f_select(self,index,param)
	
	
	
	--local index =listbox_get_index(list);
	---[[
    if (index == 0) then
        --func_ex_info();


		print("windows test!");
		
		FpsView:getIns():show();--��ʾfps
		
		Alert:getIns():show(tostring(math.random()));
		
    elseif (index == 1) then
        func_gc();
    elseif (index == 2) then
        --fps();
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



function infowin(x, y)
	
	
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
	list:bind(f_select);

	
	--listbox_del(list);
	--return list;
end