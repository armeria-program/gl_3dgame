local _engine;

function engine_init()
	local e = {
		
	};
	
	local cam2d,cam3d= getEngine();
	e.cam2d = cam2d;
	e.cam3d = cam3d;
	--[[e.fbo2dcam = fbo2dcam;
	e.fbo3dcam = fbo3dcam;
    e.fboTexture = fboTexture;
	e.fbo = fbo;
	--]]
	--cam_set_2dxy(e.fbo2dcam,50,100);
	
	return e;
end


--[[
--��ǰ�����е�FBO��������
function engine_get_fbo()
	local e = engine_get();
	return e.fbo;
end
function engine_get_3dcam()
	return e.cam3d;
end
--]]


--[[
--����һ��װ��fbotextue��sprite
local function f_createFboSprite()
	local size = 256;
    local spr = sprite_createEmptyTex(size,size);
    local tex = engine_get().fboTexture;
    if(tex)then
        local mat = sprite_get_material(spr);
        tmat_pushTex(mat,tex);
    else
        print("fboTexture is nil");
    end
    return spr;
end
--]]
--����һ���µĲ㼶
function engine_newPosZ()
    return get_attr(nil,"newPosZ");
end

--������
function engine_rename(o,value)
	change_attr(o,"rename",tostring(value));
end

--ˢ������Ĭ�ϵ�cam
function engine_refreshCam3d()
	change_attr(nil,"cam_refreshModel",_engine.cam3d);
end
--���һ���ڵ㵽������frame,ֻ����ӽ�ȥ�Ľڵ���ܱ���Ⱦ
function engine_addNode(n)
    change_attr(nil,"ex_add",n);
end
--[[
--��ȡװ��fbo��Sprite
function engine_get_fbo_sprite()
    local e = engine_get();
    if(e.fboSprite==nil) then 
        e.fboTexture = f_createFboSprite();
    end
    return _engine.fboTexture;
end
--]]
--��ǰ��fps
function engine_get_fps()
	return get_attr(nil,"fps");
end

--ÿһ֡��Ҫ�ĺ�����
function engine_getDelayTime()
    return get_attr(nil,"delayTime");
end

--�˳�����
function engine_exit()
    change_attr(nil,"exit");
end

function engine_setBg(r,g,b)
    --���ñ���ɫ
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end
function engine_get()
    return _engine;
end
--����һ��Ĭ�ϵ�3dcam������
function engine_bind_default_3dcam(self)
	local e = engine_get();
	set_cam(self,e.cam3d);
end

_engine=engine_init();
