-- 信息调试面板
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
--        print("切换为Sprite线框状态:" .. ex:get_spriteline());
    elseif (index == 2) then
        fps();
    end
end

function infowin(x, y)
    if (list == nil) then
        list = ListBox:new(x or 0, y or 0, f_select)
        list:add("引擎信息");
        list:add("gc");
        list:add("fps");
        -- list:add("线框ui");--切换线框渲染sprite
    end

    -- listbox_bind(list,f_select)
    -- listbox_select(list,0)--默认选0号索引
    --    listbox_refresh(list);
end