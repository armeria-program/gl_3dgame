dofile("..\\include\\lua\\core.lua");
--[[
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
         engine_exit();
	end
end
--]]
--evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);


engine_setBg(0.3,0.3,0.3);

--fps(0,15);

infowin(0,0);


--作为输入框的背景
local sprite =sprite_create("bg11",100,50,128,14);
func_setIcon(sprite,"gundi.png");
--setv(sprite,FLAGS_DRAW_PLOYGON_LINE);


--local sprite1 =func_sprite_createNoHit(100,100,256,256);
--func_setIcon(sprite1,"gundi.png");
--setv(sprite1,FLAGS_DRAW_PLOYGON_LINE);

local _in = input_create();
input_set_pos(_in,100,50);
local function f_onkey(data)
	local key = tonumber(data);
	--print('my key = '..key);
	if(key == 13) then
		--print(input_isFocusIn(_in));
		if(input_isFocusIn(_in)) then
			print("inout str = ["..input_get_str(_in).."]");
			input_clear(_in);
		end
	elseif(key == KEY_B)then
       
    elseif(key == KEY_ESC) then
        
	end
end
evt_on(_in,EVENT_ENGINE_KEYBOARD,f_onkey);

--input_dispose(_in);

--func_fixed_load("\\resource\\obj\\torus.obj");

--dofile("..\\include\\lua\\quaternion.lua");--


local function loadFloor()
	--cam:position(1.5,-5,-6.5);
	--cam:rx(PI * 1.8);
	--地板
	local plane = UnitBase:new();
	--plane:load_model(nil,"//resource//material//floor.mat");
	plane:load_model();
	plane:setv(FLAGS_REVERSE_FACE);
	plane:setv(FLAGS_DRAW_PLOYGON_LINE);
plane:scale(10);
end


--btn_create(0,0,128,128);

--loadFloor();
dofile("..\\include\\lua\\7.lua");

--//汉字第一个字节的范围
--static int
--f_is_first_code(unsigned char c){
--	return (0x80 <= c) && (0xF7 >= c);
--}
--//汉字第二个字节的范围
--static int
--f_is_second_code(unsigned char c){
--	return (0xA1 <= c) && (0xFE > c);
--}


--print(0x80,0xF7,0xA1,0xFE);


--local t = func_get_longTime();
--for i=1,1000000,1 do

--    math.random();
--end
--print(func_get_longTime()-t);

			

engine_refreshCam3d();