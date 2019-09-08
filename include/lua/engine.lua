local _engine;

function engine_init()
	local e = {
		_3dcam,
		fbocam,
		fboTexture,
        fboSprite,
	};
	
	local _3dcam,fbocam,fboTexture = getEngine();
	--print(_3dcam..","..fbocam);
	e._3dcam = _3dcam;
	e.fbocam = fbocam;
    e.fboTexture = fboTexture;
	return e;
end
--����һ��װ��fbotextue��sprite
local function f_createFboSprite()
    local spr = sprite_createEmptyTex(256,256);
    local tex = engine_get().fboTexture;
    if(tex)then
        local mat = sprite_get_material(spr);
        tmat_pushTex(mat,tex);
    else
        print("fboTexture is nil");
    end
    return spr;
end

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
	change_attr(nil,"cam_refreshModel",_engine._3dcam);
end
--���һ���ڵ㵽������frame,ֻ����ӽ�ȥ�Ľڵ���ܱ���Ⱦ
function engine_addNode(n)
    change_attr(nil,"ex_add",n);
end

--��ȡװ��fbo��Sprite
function engine_get_fbo_sprite()
    local e = engine_get();
    if(e.fboSprite==nil) then 
        e.fboTexture = f_createFboSprite();
    end
    return _engine.fboTexture;
end


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
_engine=engine_init();
