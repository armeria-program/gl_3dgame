dofile("..\\include\\lua\\core.lua");
function f_onkey(data)
	local key = tonumber(data);
	if(key == KEY_ESC) then
        ex:exit();	
	end
end
evt_on(cam,EVENT_ENGINE_KEYBOARD,f_onkey);

--dofile("..\\include\\lua\\7.lua");


--[[

A = {x = 0,y = 1};
A.__index = A

function A:new(x)
    local s = {};
    setmetatable(s,A);
    s.x = x or 0;
    return s;
end

function A:test()
    print(self.x);
end


local obj = A:new();
obj:test();

local obj1 = A:new(2);
obj1:test();


obj:test();
--]]



--��λ������
UnitBase  = {
    p,--��ɫ���
    speed = 1000,--��ɫ�ƶ����ٶ�
    name,--��ɫ������
};

function UnitBase:new()
    local s = {};
    setmetatable(s,UnitBase);
    --s.x = x or 0;
    return s;
end


