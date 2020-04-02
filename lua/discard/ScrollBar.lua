--������
ScrollBar = {}
--function ScrollBar.showProgress(sc,)
	--if(sc.progressTf) then
	--	tf_setText(sc.progressTf,tostring(sc.progressValue));
	--end
--end

--�Ƿ�������
local function IsIntegral(sc)
	return sc.isIntegral == 1
end

--�����ı�
local function SetProgressTf(sc)
	if(sc.progressTf) then
		
		local str = str_cut_decimal(sc.progressValue,2)--ȡС������λ
		
		--����Ϊ����
		if(IsIntegral(sc)) then
			str = tl_getIntegralPart(tonumber(sc.progressValue));
		end
		tf_setText(sc.progressTf,str);
	end
end
--���ðٷֱ�
local function ScDragMove(sc,progress)
	
	if(sc == nil) then
		return
	end	
	
	local width = progress * (sc.e - sc.s)
	
	sc.progressValue = sc.s + width
	
	SetProgressTf(sc)
	
	sc.callBackDragMove(sc)--�ٷֱȻص�
	
end

function LuaDragMove(name,progress) 
	ScDragMove(instance[name],progress)
end

--���û���������ֵ
function ScrollBar.setProportion(sc,s,e)
	--sc.proportion = value;
	sc.s = s;
	sc.e = e;
end

--����������ʾΪ����
function ScrollBar.showInt(sc)
	sc.isIntegral = 1;
end

--�����ĺ�׺
local function GetLittleSuffix()
	return "_bg"
end

--��ȡС��ť����ק����
local function 
f_Sprite_DragDirection(littleName)
	local scrollbtn = xfun.FindNode(littleName)
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
	local sprite = func_getInstance(bgName)
	
	--�����ֲ�����
	local local_x,local_y = get_attr(sprite,"spriteLocalXY")
	
	--������������
	local bx,by = get_attr(sprite,"spritePos")
	
	--�������Ŀ��
	local sprite_w,sprite_h = get_attr(sprite,"spriteSize")
	
	--С��ť����
	local scrollbtn = xfun.FindNode(littleName)
	
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
function ScrollBarBG_Click(name)
	
	--�󱳾�
	local sprite = func_getInstance(name)
	local x,y = get_attr(sprite,"spriteLocalXY")
	local w,h = get_attr(sprite,"spriteSize")
	
	
	--С��ť
	local scName=string.sub(name,0,-string.len(GetLittleSuffix())-1)
	local sc = instance[scName]
	
	--��ק
	local v =  f_SetBarPostion(name,scName)
	ScDragMove(sc,v)
end

local function CreateScrollBg(spName,x,y,bgWidth,bgHeight,bgImg)
	--local spName = name.."_bg"
	local bg=sprite_create(spName,x,y,bgWidth,bgHeight,"ScrollBarBG_Click");
	--setv(bg,flags.FLAGS_DRAW_PLOYGON_LINE);--����Ϊ�߿���Ⱦ
	--func_bindInstance(bg)
	instance[spName] = bg
	
	func_getInstance(spName)
	--local bgSprite = func_getInstance(spName)
	
	sprite_texName(bg,bgImg);
	
	return bg
end

local function createScrollBar()
	local sc = 
	{
		isIntegral = nil,
		btn = nil,
		bg = nil,
		tf = nil,
		progressTf = nil,
		
		progressValue=0,
		callBackDragMove=nil,
		
		--proportion = 1.0;--����1~����
		--����������
		s = 0.0,
		e = 1.0,
	}
	return sc
end
--[[

	dragDirection = 0;	--0:�����϶�����,1�����϶�����
	label :��ʾ���ı�
	bProgress = true ��ʾ�������ı�,false ���ؽ������ı�
--]]
function ScrollBar.new(
	x,y,
	barImg,
	barWidth,
	barHeight,
	bgImg,
	bgWidth,
	bgHeight,
	callBackProgress,
	dragDirection,
	label,
	bProgress)
	
	local sc = createScrollBar()

	sc.callBackDragMove = callBackProgress
	
	local name = tableToInt(sc);--tostring(sc);
	instance[name] = sc;
	
	if(label) then
		sc.tf=tf_create(128,x+bgWidth,y,1.0,1.0,1.0);
		tf_setText(sc.tf,label);
	end
	
	if(bProgress) then
		sc.progressTf=tf_create(128,x+bgWidth + 50,y,1.0,1.0,1.0);
	end
	
	--����
	local bg= CreateScrollBg(name..GetLittleSuffix(),x,y,bgWidth,bgHeight,bgImg)
	

	--����С��ť  
	local btn=sprite_create(name,x,y,barWidth,barWidth,"","LuaDragMove","",dragDirection);
	
	sprite_texName(btn,barImg);
	--���ÿ���ק
	sprite_setDragScope(btn,0,0,bgWidth,bgHeight);
	
	sc.btn = btn;
	sc.bg = bg;
	
	--print("***********************"..instance[name].progressValue.."**("..name..")");
	
	return sc;
end

--����һ��Ĭ�ϵ���ʽ
--bProgress:�Ƿ���ʾһ�������ı�
function ScrollBar.CreateHorizontal(x,y,callBackProgress,label,bProgress)
	return	ScrollBar.new(x,y,"gunbar.png",10,10,"gundi.png",100,10,callBackProgress,CONST_DIRECTION_HORIZONTAL,label,bProgress);
end

function ScrollBar.CreateVertucal(x,y,callBackProgress,label,bProgress)
	return	ScrollBar.new(x,y,"gunbar.png",10,10,"gundi.png",10,100,callBackProgress,CONST_DIRECTION_VERTICAL,label,bProgress);
end

--����
function ScrollBar.dispose(sc)
	ptr_remove(sc.btn);
	ptr_remove(sc.bg);
	ptr_remove(sc.progressTf);
	ptr_remove(sc.tf);
end


