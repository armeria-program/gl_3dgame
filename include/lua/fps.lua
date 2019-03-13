local tf;
local DELAT_TIME = 1000;--ˢ���ӳ�
local ticket = 0;

local _default = false;--�Ƿ�ʹ�þɵ�������Ⱦ

local function f_render()
	--if(tf)then		tf_setText(tf,string.format(%.3f),func_fps());end
	local _time = func_get_longTime()
	if(_time - ticket < DELAT_TIME) then
		return
	end
	
	ticket = _time
	if(tf) then
		local s = string.format("fps-%d-%d",ex:fps(),ex:delayTime())
        if(_default) then
		    tf_setText(tf,s);
        else
            func_ftext_reset(tf,s);
        end
	end
end

--��ʾfps
function fps(x,y)
	x = x or 0
	y = y or 0
    if(_default) then
	    if(tf == nil) then
		    tf = tf_create(128,x,y,0,0,0);
		    evt_on(tf,EVENT_ENGINE_RENDER_3D,f_render);
	    end
	    tf_setPos(tf,x,y)
    else
        if(tf == nil) then
		    tf = func_ftext_create(128,128);
            func_ftext_set_buffer(tf,128);
		    evt_on(tf,EVENT_ENGINE_RENDER_3D,f_render);
	    end
	    func_ftext_setpos(tf,x,y)
    end
end