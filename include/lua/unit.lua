--��ʼ������(����ɶ�̬���������ļ�,������һ���༭���༭��Щ���ź�ƫ���������md2�ļ�)
local function f_split_init(md2)
	func_set_scale(md2,1/50);--��������һ�����ż�ֵ��ģ���ڵ�λ1����
	func_set_y(md2,0.5);	 --���õذ�y����ƫ��
	
	--print(md2);
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end

--����һ��������λ
function unit_create()
	local u = {
		p = nil;--��Դ����
	}
	--drfreak
	--bauul
	local url = "bauul";
	
	local md2=load_VBO_model(func_create_name(),string.format("\\resource\\md2\\%s.md2",url));
	setMaterial(md2,func_load(string.format("//resource//material//%s.mat",url)));
	setv(md2,FLAGS_VISIBLE);
	func_setRotateX(md2,PI/2)--��תһ������

	f_split_init(md2);
	
	----------------------------
	u.p = md2;
	return u;
end

