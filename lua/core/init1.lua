print(string.format("version = [%s]",_VERSION));

package.path=";c:\\gl_3dgame\\lua\\?.lua";--�������

--����ҵ��ģ��
package.path=package.path..";c:\\gl_3dgame\\lua\\src\\?.lua";

print('package.path = '..package.path);



-- require("a");
-- require("a");


-- local a = {};
-- print(#a);
-- print(tonumber(a));



local core = require("core");

JEngine:getIns():setbg(0.4,0.4,0.4);


require("editor");
--require("editor.lua");
--print(core.now());





local function f_fps_timer(data,param)


end



--func_enable_debug(false);--�رյ�����־
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        JEngine:getIns():exit();
	elseif(key == 13) then
		--�س�
	elseif(key == 49) then
		--1
		JEngine:getIns():print_info();	
	elseif(key == 50) then
		--2
		func_lua_gc("2");
		
	elseif(key == 51) then
		--3

		func_lua_gc("3_1");

----[[
		local f = JEngine:getIns():get_plugin():load("view/FpsView");--���ز��
		f:show();
		f:dispose();
		--f = nil;
		
--[[
		evt_on(2,EVENT_TIMER,f_fps_timer,"a");
		evt_off(2,EVENT_TIMER,f_fps_timer);
--]]
	
		
	


		func_lua_gc("3_2");

		
		
		
		
		
	elseif(key == 52) then
		--4

		
		func_lua_gc("4_1");

		
----[[
		local tf = JEngine:getIns():get_plugin():load("view/tf");
		tf:init();
		tf:show("message");
        tf:unload();
--]]

		
		
		func_lua_gc("4_2");

	end
end

local function init()
	--print(core.now());
	--print(cam);	
	JEngine:getIns():setbg(0.3,0.3,0.3);
	JEngine:getIns():get_cam():set_pos(0,-4,-15);

	local e1 = Editor:new();
	JEngine:getIns():get_cam():refresh();

	-- evt_on(nil,EVENT_ENGINE_KEYBOARD,f_onkey);

end

-- init();
local btn = Button:new();
btn:set_pos(100,100);
btn:bind_click(function()
	init();
	-- core.gc();
	end
);

-- local win = JEngine:getIns():get_plugin():load("view/InfoView");
--win:set_pos(150,0);


--core.setTimeout(5000,init);



--*******************************************

--func_lua_gc();

--###############################################################


----[[
func_lua_gc();
local win = JEngine:getIns():get_plugin():load("view/InfoView");
win:set_pos(150,0);
-- win:dispose();
-- win = nil;
--func_lua_gc();
--]]





--]]
--###############################################################




--require("5.lua");

--require("quaternion");

--]]




--[[
local s= LineNode:new(3);
s:push(-0.5,0.4,0.5);
s:push( 0.5, 0.0, 0.5);
s:push( -0.5, -0.49, -0.5);
s:setcolor(1,0,0);
s:pushend();

s:mod(0,1,0,0);
s:mod(2,0,0,0);
--s:dispose();--����
--]]




