--����sprite�߿�
function func_set_sprite_line(v)
	change_attr(nil,"g_sprite_line",tostring(v));
end


local EX = {}
function EX:new()
	local e = {
        spriteline;
	}
	
	self.__index = self;
    setmetatable(e, self);
    e.spriteline = 0;
    return e;
end

--����sprite�߿���Ⱦ
function EX:set_spriteline(v)
   
    self.spriteline = change_attr(nil,"g_sprite_line",tostring(v));
     print(self.spriteline);
end

function  EX:get_spriteline()
    return self.spriteline;
end
--�˳�����
function EX:exit()
    change_attr(nil,"exit");
end

--[[
--��ȡfps
function func_fps()
    func_error("fps",func_fps);
    return 1;
end

--��ȡdelayTime
function func_delayTime()
	return get_attr(nil,"delayTime");
end
function func_delayTime()
	return get_attr(nil,"delayTime");
end
--]]




--��ǰ��fps
function EX:fps()
	return get_attr(nil,"fps");
end
--ÿһ֡��Ҫ�ĺ�����
function EX:delayTime()
    return get_attr(nil,"delayTime");
end

ex = EX:new();