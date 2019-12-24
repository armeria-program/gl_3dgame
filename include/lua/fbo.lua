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
--��ʼ��FBO����
local function fboobj_init(tw,th)
    local fbo = {};
    local ptr,cam3d,cam2d,tex = fbo_init(tw,th);--����fbo����
	
	fbo.ptr = ptr;

	fbo.cam3d = cam3d;
	
	fbo.cam2d = cam2d;

	fbo.tex = tex;
	
    fbo.size = tw;
	
	fbo.spr = f_createSprite(tw,tex);
	
	JEngine:getIns():add(fbo.spr);--��spr��ӵ���Ⱦ�б�
	
	add_fbo(ptr);--��ӵ������fbolist��
		
	return fbo;
end

--����fbo����
local function fboobj_dispose(fbo)
	fbo_dispose(fbo.ptr);
	remove_fbo(fbo.ptr);--��fbo�������fbolist�Ƴ�

	ptr_remove(fbo.spr);
end

local function fboobj_set_pos(fbo,x,y)
	local spr = fbo.spr;
	func_setPos(spr,x,y);
	cam_set_2dxy(fbo.cam2d,x,y);
end

--print(fbo.ptr);
--**************************************************
FboRender = {
	ptr,cam3d,cam2d,tex,spr
}
FboRender.__index = FboRender;
setmetatable(FboRender, Base);

function FboRender:new(tw,th)
	local fbo = Base:new();
	setmetatable(fbo, FboRender);
	w = w or 128;
	h = h or 128;
	local ptr,cam3d,cam2d,tex = fbo_init(tw,th);--����fbo����
	fbo.ptr = ptr;
	fbo.cam3d = cam3d;
	fbo.cam2d = cam2d;
	fbo.tex = tex;
	
	fbo.spr = f_createSprite(tw,tex);
	
	JEngine:getIns():add(fbo.spr);--��spr��ӵ���Ⱦ�б�
	
	add_fbo(ptr);--��ӵ������fbolist��
	return fbo;
end

function FboRender:dispose()
--����Ҫ����ӽ�������Ⱦ����ɾ����

	fbo_dispose(self.ptr);
	remove_fbo(self.ptr);--��fbo�������fbolist�Ƴ�
	ptr_remove(self.spr);--����fboռ�õ�sprite����
end

function FboRender:get_cam3d()
	return self.cam3d;
end
function FboRender:get_cam2d()
	return self.cam2d;
end
function FboRender:set_pos(x,y)
	local fbo = self;
	local spr = fbo.spr;
	func_setPos(spr,x,y);
	cam_set_2dxy(fbo.cam2d,x,y);
end