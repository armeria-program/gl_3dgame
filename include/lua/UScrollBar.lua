--#######################################################
--	ScrollBar���
--
--
--#######################################################

--Ĭ�Ͽ��
local defaultBg_width = 100
local defaultBg_height = 14
--С��ť���
local barSize = defaultBg_height
--������׺
local BgSuffix = '_bg'


local dragDirection = CONST_DIRECTION_HORIZONTAL --ˮƽ

--����һ����ť�ṹ��
local function f_create()
	return {
		--��������
		type = "ScrollBar",
		
		--������ťʵ��(Sprite)
		btn = nil,
		
		bg = nil,
		
		--����������(Ĭ������һ����ֵ)
		s = 0.0,
		e = 1.0,
		
		callBack = nil,
		
		--����ֵ
		value = nil,
	};
end

--���ðٷֱ�
local function 
f_ScDragMove(sc,progress)
	
	if(sc == nil) then
		print('f_ScDragMove sc = nil')
		return
	end	
	
	local width = progress * (sc.e - sc.s)
	
	sc.value = sc.s + width
	
	if(sc.callBack) then
		sc.callBack(sc)
	else
		print(sc.callBack)--�ص�Ϊnil
	end
end
--��ȡС��ť����ק����
local function 
f_Sprite_DragDirection(littleName)
	local scrollbtn = func_find_obj(littleName)
	return get_attr(scrollbtn,"dragDirection")
end


--����С��ťλ��(����)
--[[
	bgName:������(Sprite)�󱳾���
	littleName:С��ť(Sprite)��
	DragDirection:��ק�ķ���
--]]
local function 
f_SetBarPostion(bgName,littleName)
	
	--�����ķ���
	local dragDirection=f_Sprite_DragDirection(littleName)
	
	--����sprite����
	local sprite = func_find_obj(bgName)--func_getTable(bgName)
	
	--�����ֲ�����
	local local_x,local_y = get_attr(sprite,"spriteLocalXY")
	
	--������������
	local bx,by = get_attr(sprite,"spritePos")
	
	--�������Ŀ��
	local sprite_w,sprite_h = get_attr(sprite,"spriteSize")
	
	--С��ť����
	local scrollbtn = func_find_obj(littleName)
	
	--С��ť�Ŀ��
	local sc_w,sc_h = get_attr(scrollbtn,"spriteSize")
		
	--local cur_x = --GetBarX(x,sc_w,w)
	
	
	if(dragDirection == CONST_DIRECTION_HORIZONTAL) then
		local v = local_x
		
		local target_x = bx + v 
		
		--print("����С��ť��λ��:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_x > sprite_w  - sc_w + bx) then
			target_x = sprite_w - sc_w + bx
			--print('����!!!')
		end
		
		sprite_setpos(scrollbtn,target_x,by)
		return v / sprite_w
	else

		local v = local_y
		
		local target_y = by + v 
		
		--print("����С��ť��λ��:"..tostring(target_x)..","..tostring(by)..",sc_w="..sc_w..',sc_h='..sc_h..',sprite_w='..sprite_w..',sprite_h='..sprite_h)

		if (target_y > sprite_h  - sc_h + by) then
			target_y = sprite_h - sc_h + by
			--print('����!!!')
		end
		
		sprite_setpos(scrollbtn,bx,target_y)
		return v / sprite_h
	end
end
--[[
	����������ϵ�С��ť
--]]
function 
f_ScrollBarBG_Click2(name)
	
	--�󱳾�
	local sprite = func_find_obj(name)
	--print('########### '..name)
	--print(sprite)
	
	local x,y = get_attr(sprite,"spriteLocalXY")
	local w,h = get_attr(sprite,"spriteSize")
	
	
	--С��ť
	local scName=string.sub(name,0,-string.len(BgSuffix)-1)
	local sc = func_getTable(scName)
	
	--��ק
	local v =  f_SetBarPostion(name,scName)
	f_ScDragMove(sc,v)
end

function 
f_LuaDragMove2(name,progress) 
	f_ScDragMove(func_getTable(name),progress)
end

function 
scrollBar_new(x,y)
	local sc = f_create()
	
	if(x == nil) then x = 0	end
	if(y == nil) then y = 0 end
	func_tableSave(sc)
	
	local name = func_getTableName(sc)--��ȡbtn������

	--����
	local bg = sprite_create(string.format("%s%s",name,BgSuffix),x,y,defaultBg_width,defaultBg_height,'f_ScrollBarBG_Click2')
	func_setIcon(bg,"gundi.png")
	
	--����С��ť
	local btn=sprite_create(name,x,y,barSize,barSize,"","f_LuaDragMove2","",dragDirection);

	--���ÿ���ק��Χ
	sprite_setDragScope(btn,0,0,defaultBg_width,defaultBg_height);
	
	--��sc��ֵ
	sc.bg = bg
	sc.btn = btn
	return sc
end

--ɾ��
function scrollBar_del(sc)
	ptr_remove(sc.btn)
	ptr_remove(sc.bg)
	func_tableDel(sc)
end

--��һ���ص�����
function 
scrollBar_bind(sc,call)
	sc.callBack = call
end

--��������
function 
scrollBar_setRange(sc,s,e)
	sc.s = s
	sc.e = e
end

--��ȡ�����Ϣ
function 
scrollBar_getInfo(sc)
	local bg = sc.bg
	local x,y = get_attr(bg,"spritePos")
	local bgw,bgh = get_attr(bg,"spriteSize")
	return x,y,bgw,bgh
end

--��ʾһ���ı�
local function 
f_showTF(x,y,label)
	local tf = tf_create(128,x,y);
	tf_setText(tf,label);
	return tf
end

--���һ��text
function 
scrollBar_add_text(sc,label)
	local x,y,sw,sh=scrollBar_getInfo(sc)
	return f_showTF(x + sw,y,label)
end