--����һ��װ��fbotextue��sprite
local function f_createSprite(size,tex)
	local size = size;
    local spr = sprite_createEmptyTex(size,size);
    if(tex)then
        local mat = sprite_get_material(spr);
        tmat_pushTex(mat,tex);
    else
        print("fboTexture is nil");
    end
    return spr;
end

function fboobj_init(tw,th)
    local fbo = {};
    local ptr,cam3d,cam2d,tex = fbo_init(tw,th);--����fbo����
	
	fbo.ptr = ptr;

	fbo.cam3d = cam3d;
	
	fbo.cam2d = cam2d;

	fbo.tex = tex;
	
    fbo.size = tw;
	
	fbo.spr = f_createSprite(tw,tex);
	
	engine_addNode(fbo.spr);--��spr���ӵ���Ⱦ�б�
	
	add_fbo(ptr);--���ӵ�����
	return fbo;
end

function fboobj_set_pos(fbo,x,y)
	local spr = fbo.spr;
	func_setPos(spr,x,y);
	cam_set_2dxy(fbo.cam2d,x,y);
end

--print(fbo.ptr);
