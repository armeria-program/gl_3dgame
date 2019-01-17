--md2��������������
dofile("..\\include\\lua\\core.lua")

print('md2��������')
local function 
f_load(line)
	--load md2
	local name = "horse";
	--local md2=load_md2vbo_byName(name,func_get_default_tex())
	
	local tex = func_get_default_tex()
	
	local md2 = func_loadmd2(name,tex)
	
	setv(md2,FLAGS_RAY);
	if(line)then
		setv(md2,FLAGS_DRAW_PLOYGON_LINE);--�����߿���Ⱦ
	end
	setv(md2,FLAGS_DRAW_RAY_COLLISION);
	change_attr(md2,"collide_load","\\resource\\md2\\"..name..".md2");
	
	--������ת��
	func_setRotateX(md2,PI/2);
	
	--	"stand,0,39@run,40,45@attack,46,53@pain,54,65@jump,66,71@filp,72,83@salute,84,94@taunt,95,111@dead,178,197@"
	--�и�һ������
	change_attr(md2,"animtor_push","stand","0,39");
	change_attr(md2,"animtor_push","run","40,45");
	change_attr(md2,"animtor_push","jump","66,71");
	
	--���ó�ʼ����
	change_attr(md2,"animtor_setcur","jump");
	
	--change_attr(md2,"animtor_pause");--��ͣ
	change_attr(md2,"animtor_play");--����
	
	return md2;
end

--����������
local function 
f_anim_control_ui(m)
	local animsc = scrollBar_new(0,0)
	scrollBar_setRange(animsc,0,197)
	local animTF = scrollBar_add_text(animsc,'anim')
	scrollBar_bind(animsc,
		function(sc)
			local v = string.format('%.0f',sc.value)
			func_play_anim(m,v,v)
			--func_setRotateX(func_find_obj(ModelName),sc.value)
			tf_setText(animTF,'anim '..v)
		end
	)
	
	local list =  listbox_new(0,15)
	listbox_add(list,"stand")
	listbox_add(list,"run")
	listbox_add(list,"jump")
	listbox_bind(list,
		function(_l)
			
			local label = listbox_get_label(_l)
			func_set_anim(m,label)
			--string.format('%s vbo=%s',label,tostring(func_is_vbo(obj)) )
			listbox_set_label(_l,label)

		end
	)
	listbox_select(list,0)--Ĭ��ѡ0������
end
---------------------------------------------------------------------------
func_set_camera_pos(0,0,-30)
f_anim_control_ui(f_load(true))