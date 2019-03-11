local g_gap = 15--(һ�еĸ߶ȼ��)
local g_width = 60 --(һ�еĿ��)

local r = 1.0;
local g = 0.0;
local b = 0.0;

local function
f_create()
local listBox=
	{
		b_drop = false,--�Ƿ��������ŵ�
		
		tflist={},--textFiled�б�
		
		bg=nil,--spirte
		
		callBack = nil,--ѡ��ص�
		
		x=0,y=0,
		
		index = 0,--ѡ�������0��ʼ,Ĭ�ϳ�ʼ��Ϊ0
		
		tf,	--�ı�
	};
	return listBox
end

--��ʾ�����ı�
local function 
f_tf_vis(list)
	local v = list.b_drop
	for key, value in pairs(list.tflist) do      
		local tf =  value
--		if(v) then
--			setv(tf,FLAGS_VISIBLE)
--		else
--			resetv(tf,FLAGS_VISIBLE);
--		end
        func_ftext_vis(tf,v == true and 1 or 0)
	end 
end
--��ȡ����(-1��ʼ)
local function
f_get_index(list)
	local x,y = func_get_sprite_mouse_xy(list.bg)
	local f = y / g_gap
	
	--print(string.format("%s\t%s\t%d",tostring(f),string.format('%#.0f',f),math.floor(f)))
	--return tonumber( string.format('%#.0f',f)) - 1
	return math.floor(f) - 1
end
--function listbox_refresh(list)
--    f_tf_vis(list);
--end
--ѡ��һ���ڵ�,������0��ʼ
function
listbox_select(list,n)
	if(n >= 0) then
		list.index = n
		--tf_setText(list.tf,listbox_get_label(list));--�����ı�
		
		if(list.callBack) then list.callBack(list)end
	end
end

function 
listbox_set_label(list,label)
--	tf_setText(list.tf,label);
    func_ftext_reset(list.tf,label);
end


local function f_select_call(list)
	list.b_drop = not list.b_drop
	local height = g_gap
	f_tf_vis(list)
	
	if(list.b_drop) then
		--չ��
		height = (func_get_table_count(list.tflist) + 1) * g_gap
	end
	
	func_set_sprite_size(list.bg,g_width,height)--�ػ汳�����
	
	if(list.b_drop == false) then listbox_select(list,f_get_index(list))end
end

--[[
--�Ƿ���ʾ��
local function 
f_is_drop(list)
	local w,h = func_get_sprite_size(list.sprite)
	
	if(h > g_gap) then
		return true
	end
	return false
end
--]]

function listbox_get_index(list)
	return list.index
end
local function f_click(name)
	--f_listBoxCallBack(b);
	local list = func_getTable(name);
	--print(list);
	f_select_call(list);
end
function 
listbox_new(_x,_y)
	_x = _x or 0
	_y = _y or 0
	
	local list = f_create()
	
	func_tableSave(list)
	list.x = _x
	list.y = _y
	
	local name = func_getTableName(list)--��ȡ������
	
	--list.bg = sprite_create(name,_x,_y,g_width,g_gap,"f_listBoxCallBack","","");
	list.bg = sprite_create(name,_x,_y,g_width,g_gap);
	
	evt_on(list.bg,EVENT_ENGINE_SPRITE_CLICK,f_click);

	func_setIcon(list.bg,"gundi.png")

	list.tf =func_ftext_create(); --tf_create(128,list.x,list.y,r,g,b);
    func_ftext_setpos(list.tf,list.x,list.y);
	return list
end

--��һ���ص�����
function 
listbox_bind(list,callBack)
	list.callBack = callBack
end

--��ȡѡ����ı��ַ�
function 
listbox_get_label(list)
	local n = listbox_get_index(list)
--	return func_get_tf_text(list.tflist[n])
    return func_ftext_str(list.tflist[n]);
end

--����һ���ڵ�
function 
listbox_add(list,str)
	local count =	func_get_table_count(list.tflist) + 1
	local tf=func_ftext_create();--tf_create(128,list.x,list.y + g_gap*(count),r,g,b);
    func_ftext_setpos(tf,list.x + g_width/2,list.y + g_gap*(count));
	list.tflist[count - 1] = tf;
	--tf_setText(tf,str);
    func_ftext_reset(tf,str);
	--resetv(tf,FLAGS_VISIBLE);
    --func_ftext_vis(tf,0);
end



--����listbox���
function 
listbox_del(list)
	evt_off(list.bg,EVENT_ENGINE_SPRITE_CLICK,f_click);

	ptr_remove(list.bg)

	for key, value in pairs(list.tflist) do
		ptr_remove(value)
	end
	--ptr_remove(list.tf)
    func_fext_dispose(list.tf);
	func_clearTableItem(list.tflist)
	func_clearTableItem(list)
end
