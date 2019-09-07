local _engine;

function engine_init()
	local e = {
		_3dcam,
		fbocam,
		fboTexSprite,
	};
	
	local _3dcam,fbocam,fboTexSprite = getEngine();
	--print(_3dcam..","..fbocam);
	e._3dcam = _3dcam;
	e.fbocam = fbocam;
    e.fboTexSprite = fboTexSprite;
	return e;
end
--����һ���µĲ㼶
function engine_newPosZ()
    return get_attr(nil,"newPosZ");
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
function engine_getFBO_sprite()
    return _engine.fboTexSprite;
end

function engine_setBg(r,g,b)
    --���ñ���ɫ
    change_attr(nil,"setBgColor",string.format("%s,%s,%s",r or 0,g or 0,b or 0));
end

_engine=engine_init();