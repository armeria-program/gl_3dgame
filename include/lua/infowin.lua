-- ��Ϣ�������
local list;
local function f_select(index)
    -- local label = listbox_get_label(_l)
    -- func_set_anim(m,label)
    -- string.format('%s vbo=%s',label,tostring(func_is_vbo(obj)) )
    -- listbox_set_label(_l,label)
    -- listbox_set_label(_l,label)

    if (index == 0) then
        func_ex_info();
    elseif (index == 1) then
        func_gc();
--    elseif (index == 2) then
--        ex:switch_spriteline()
--        print("�л�ΪSprite�߿�״̬:" .. ex:get_spriteline());
    elseif (index == 2) then
        fps();
    end
end

function infowin(x, y)
    if (list == nil) then
        list = ListBox:new(x or 0, y or 0, f_select)
        list:add("������Ϣ");
        list:add("gc");
        list:add("fps");
        -- list:add("�߿�ui");--�л��߿���Ⱦsprite
    end

    -- listbox_bind(list,f_select)
    -- listbox_select(list,0)--Ĭ��ѡ0������
    --    listbox_refresh(list);
end