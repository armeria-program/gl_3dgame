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
ex = EX:new();