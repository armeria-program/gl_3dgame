--ģ�Ϳ��ƽ������,ֻҪbind����������ģ�Ͷ��ܿ���
--��ǰ��ģ��
local curModel

local nameTF	--ѡ��Ķ�����Ϣ
local fpsTF
local setframeTF
local frameSC
local scaleTF


------------------------------------------------------

local function f_getModel()
	return curModel
end
local function f_callBack_x(sc)	
	func_setRotateX(f_getModel(),sc.value)
end
local function f_callBack_y(sc)
	func_setRotateY(f_getModel(),sc.value)
end
local function f_callBack_z(sc)
	func_setRotateZ(f_getModel(),sc.value)
end

--#################################
--��ʾһ���ı�
local function f_showTF(x,y,label)
	local tf = tf_create(128,x,y);
	tf_setText(tf,label);
	return tf
end


local function f_addTf(sc,label)
	local x,y,sw,sh=scrollBar_getInfo(sc)
	return f_showTF(x + sw,y,label)
end
local function f_fps(sc)
	local v = sc.value
	--print("����ģ��("..string.format("%#x",tonumber(model))..') FPS = '..	string.format("%0.2f", v))

	--local frame = '[frame = '..get_attr(model,"getCurFrame")..']';
	tf_setText(fpsTF,"fps:" .. string.format("%0.0f", v));
	
	change_attr(f_getModel(),"fps",tostring(v))
end

local function fployline(b)
	btn_label(b,"ployline:"..tostring(func_changeFlags( f_getModel(),FLAGS_DRAW_PLOYGON_LINE)))
end
local function fskeleton(b)
	local o = f_getModel()
	local suffix = func_get_suffix(o)
	if (suffix == SUFFIX_MD5MESH) then
		--print(func_get_suffix(o))
		btn_label(b,"skeleton:"..tostring(func_changeFlags( o,FLAGS_RENDER_DRAWSKELETON)))
	else
		func_error(string.format("%s type = %s","��������",suffix))
	end
end
local function fcull(b)	
	local o = f_getModel()
	
	--print(getv(o,FLAGS_DISABLE_CULL_FACE))
	--if(getv(o,FLAGS_DISABLE_CULL_FACE) == 1) then
		btn_label(b,"cull face:"..tostring(func_changeFlags( o,FLAGS_DISABLE_CULL_FACE)))
	--else
	--	func_error(string.format("δ����״̬ %d",FLAGS_DISABLE_CULL_FACE))
	--end
end
local function f_frame(sc)
	local value = sc.value
	local v = math.ceil(value)
	if(v < 0) then
		--func_error("������������",modelUI_bind)
		--assert(nil,'�����˻�������')
		--return
	end
	change_attr(f_getModel(),"setframe",v)
	--tf_setText(setframe_tf,"setframe:" .. string.format("%0.0f", v)  ..frame);
	--local frame = get_attr(model,"getCurFrame")
	
	local str =  string.format()
	
	
	tf_setText(setframeTF,'setframe '..v..','..tostring(get_attr(f_getModel(),"getCurFrame")))


end

local function f_gc()
	func_gc()
end

local function f_scale(sc)
	local v = sc.value
	local o = f_getModel()
	func_set_scale(o,v)
	tf_setText(scaleTF,'scale '..string.format('%.2f',v))
end
function  modelUI_initBtns()
	local y = 20
	local btnHeight = 20
	
	
	-------------------------------------
	--�߿�ģʽ���л�
	local btn = btn_create(150,y,100)
	btn_label(btn,"ployline")
	btn_bindClick(btn,fployline)

	-------------------------------------
	--�л���ʾ����
	local btn_skeleon = btn_create(150,y + btnHeight,100)
	btn_label(btn_skeleon,"skeleton")
	btn_bindClick(btn_skeleon,fskeleton)

	--------------------------------------
	--����˫����Ⱦ
	local btn_cullFace = btn_create(150,y + btnHeight*2)
	btn_label(btn_cullFace,"cull face")
	btn_bindClick(btn_cullFace,fcull)
	
	--gc��ť
	local gc_btn = btn_create(150,y + btnHeight*3)
	btn_label(gc_btn,"gc")
	btn_bindClick(gc_btn,f_gc)

end

--btn_create(100,60)
function modelUI_init()
	
	--curModel = model
	nameTF = f_showTF(0,0,"")
	
	--��תX��
	local sc = scrollBar_new(0,15)
	scrollBar_bind(sc,f_callBack_x)
	scrollBar_setRange(sc,0,PI)
	f_addTf(sc,"rotateX")
	
	--print(w..','..h)
	--��תY��
	local sc = scrollBar_new(0,30)
	scrollBar_bind(sc,f_callBack_y)
	scrollBar_setRange(sc,0,PI)
	f_addTf(sc,"rotateY")
	
	--��תZ��
	local sc = scrollBar_new(0,45)
	scrollBar_bind(sc,f_callBack_z)
	scrollBar_setRange(sc,0,PI)
	f_addTf(sc,"rotateZ")

	--####################################################################
	--����fps ֡��

	local sc = scrollBar_new(0,60)
	scrollBar_bind(sc,f_fps)
	scrollBar_setRange(sc,0,600)
	fpsTF = f_addTf(sc,"fps")
	
	
	
	--####################################################################
	--setframe���õ�ǰ�ؼ�֡
	frameSC = scrollBar_new(0,75)

	scrollBar_bind(frameSC,f_frame)
	--scrollBar_setRange(sc,-1,get_attr(model,"frameCount")-1)
	setframeTF = f_addTf(frameSC,"setframe")
	--####################################################################
	
	--scale������
	local scale_sc =  scrollBar_new(0,90)
	scrollBar_bind(scale_sc,f_scale)
	scrollBar_setRange(scale_sc,0,1.0)
	scaleTF = f_addTf(scale_sc,"scale")
	
	
	--func_error("***s",modelUI_bind)
	
	--scrollBar_del(sc)
	--sc = nil
	
	--####################################################################
	--��Ե��lineWidth����
	local line_sc =  scrollBar_new(0,115)
	local lineTF = f_addTf(line_sc,"line")
	scrollBar_bind(line_sc,
		function(sc)
		
			local v = sc.value
			local o = f_getModel()
			--func_set_scale(o,v)
			func_changeFlags(o,FLAGS_GLSL_OUTLINE)--֧�ֱ�Ե��Ⱦ
			func_set_glsl_parms(o,'lineWidth',v)
			tf_setText(lineTF,'line '..string.format('%.2f',v))
		end
	
	)
	scrollBar_setRange(line_sc,0.01,1.0)
	--####################################################################
	
	--��Ԫ��
	local quat_sc = scrollBar_new(0,140)
	local quatTF = f_addTf(quat_sc,"quat")
		scrollBar_bind(quat_sc,		
		function(sc)
			local v = sc.value
			print(v)
			
			local o = f_getModel()
			
			if(o) then
				test_unit_02(func_get_name(o),v)
			end
		end
	)
	scrollBar_setRange(quat_sc,0.0,1.0)
end

function moduleUI_bind(model)
	curModel = model
	
	--func_set_box_color(model,0,1,0)--Ĭ�����ó���ɫ���߿�
	
	print(string.format("moduleUI_bind: frame=%d\n",get_attr(model,"frameCount")))
	scrollBar_setRange(frameSC,-1,get_attr(model,"frameCount")-1)
	
	local str = string.format(
		"%#x,%s,%s,%s",
		
		tonumber(model),
		func_get_name(model),
		func_get_suffix(model),
		func_get_type_str(model)
	)
	tf_setText(nameTF,str)
end


modelUI_initBtns()
