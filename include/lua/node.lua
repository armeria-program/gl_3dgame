function node_new()
	local a = {
		node,
		material,
	};	
	return a;
end

function node_loadModel(avatar,url)
	url = url or "\\resource\\obj\\box.obj";--"\\resource\\md2\\bauul.md2";box
	local name = func_create_name();
	--print(name,url);
	avatar.node = func_ex_loadVBO(name,url);
	setv(avatar.node,FLAGS_VISIBLE);
end

function node_loadMaterial(avatar,url)
	url = url or "//resource//material//triangle.mat";
	avatar.material = func_load(url);
	setMaterial(avatar.node,avatar.material);
end

function node_getNode(avatar)
	return avatar.node;
end

function node_setScale(n,value)
	func_set_scale(n.node,value);
end

function node_setRX(n,value)
	   func_setRotateX(n.node,value)
end

function node_setRY(n,value)
	   func_setRotateY(n.node,value)
end

function node_setRZ(n,value)
	   func_setRotateZ(n.node,value)
end
--��ȡ�ڵ������
function node_getName(n)
	return func_get_name(n.node);
end
--��ȡ�ڵ����ڵ�cam�ľ���ռ�
function node_setCam(n,value)
	--change_attr(o,"setcam",tostring(value));
	set_cam(n.node,value);
end

--**************************************************************************
--��ʼ������(����ɶ�̬���������ļ�,������һ���༭���༭��Щ���ź�ƫ���������md2�ļ�)
local function f_split_init(md2)
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end


local function add_small_node(parent)
   local cam = get_cam(parent);
    local sprite = sprite_create(nil,0,0,64,64,0,1,cam);
--	setv(sprite,FLAGS_DRAW_PLOYGON_LINE);
    func_setIcon(sprite, "smallbtn.png");
    func_addchild(parent,sprite);
    engine_addNode(sprite);
end









function node_fbo(avtar)
	
	local fbo = fboobj_init(256,256);
	fboobj_set_pos(fbo,220,0);
	
	
	
	local n = node_new();

	local useAvtar = avtar or 0;
	local e = engine_get();
	if(useAvtar == 1) then
    --����һ����ɫģ��
		node_loadModel(n,"\\resource\\md2\\bauul.md2");  node_setScale(n,0.02);
		node_loadMaterial(n,"//resource//material//bauul.mat");
	else
		node_loadModel(n);
		node_loadMaterial(n,"//resource//material//triangle.mat");--bauul.mat
		setv(node_getNode(n),FLAGS_DRAW_PLOYGON_LINE);
		setv(node_getNode(n),FLAGS_REVERSE_FACE);--��ת������
	end

	
	node_setRX(n,PI/2);
	f_split_init(n.node);
		--setv(node_getNode(n),FLAGS_REVERSE_FACE);--��ת������
	
	--print("name="..node_getName(n));
	node_setCam(n,fbo.cam3d);
	
    engine_addNode(n.node);
	
	
	local ch = 500;--�����ĸ߶�
    local sprite = sprite_create(nil,0,0,100,ch,0,1,fbo.cam2d);
	setv(sprite,FLAGS_DRAW_PLOYGON_LINE);
    func_setIcon(sprite, "smallbtn.png");
    
	
	
	
	
    --setv(sprite,FLAGS_VISIBLE);
    local z = engine_newPosZ();
    --print(z);
    func_sprite_set_z(sprite,z);
	engine_addNode(sprite);
	

    --add_small_node(sprite);
    --setv(sprite,FLAGS_REVERSE_FACE);--��ת������ 
    --setv(sprite,FLAGS_DISABLE_CULL_FACE);
    
    --engine_addNode(sprite);--���ӵ�����̨
	
	
	
	
	local cy = 0;--��ǰ������
	local ox = 1;
	local h = fbo.size;
	--����
    local function f_scHandle(sc)
		
		if(h >= ch) then
			
			return;
		end
		local v = sc.value;
        --print(v);
        func_setPos(sprite,0,(h-ch)*v);--���о�����������Ĺ�����
    end
    local sc = scrollBar_new(200,0,15,256);
    scrollBar_bind(sc,f_scHandle);



--[[	
	--################
	local sprite1 = sprite_create("defaultname2",20,10,100,50,0,1);
    func_setIcon(sprite1, "smallbtn.png");
	engine_addNode(sprite1);
--]]
	
end