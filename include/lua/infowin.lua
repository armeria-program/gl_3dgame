--��Ϣ�������

local function f_select(_l)
	local label = listbox_get_label(_l)
	--func_set_anim(m,label)
	--string.format('%s vbo=%s',label,tostring(func_is_vbo(obj)) )
	--listbox_set_label(_l,label)
	listbox_set_label(_l,label)
	
	if(label == "������Ϣ") then
		func_ex_info();
	elseif(label == "gc") then
		func_gc();
    elseif(label == "line") then
		ex:switch_spriteline()
        print("�л�ΪSprite�߿�״̬:"..ex:get_spriteline());
	end
end

function infowin(x,y)
	local list =  listbox_new(x or 0,y or 0)
	listbox_add(list,"������Ϣ")
--	listbox_add(list,"gc")
--    listbox_add(list,"line")--�л��߿���Ⱦsprite

	
	listbox_bind(list,f_select)
	--listbox_select(list,0)--Ĭ��ѡ0������
--    listbox_refresh(list);
end