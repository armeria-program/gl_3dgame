

--#######################################################
--	��ť���
--
--
--#######################################################
--����һ����ť�ṹ��
local function f_createBtn()
	return {
		--��������
		type = "Button",
		--�����е�Sprite����
		sprite=nil,
		
		--����ص�����
		clickEvtCallBack=nil,
	
		--��갴��ȥ�ص�
		mouseDownCallBack = nil,
	 
		--textField
		label=nil,
		
		--id = 0,	--��ťid
		
		data = nil,
	};
end
local function f_delLabel(btn)
	if(btn.label) then
		ptr_remove(btn.label);
		btn.label = nil
	end
end
--[[
	�˽ӿڻ��������϶˵���,����Ҫ��Ϊ��������
	��ť�ص�
	spriteName��Sprite���õ�sprite.name,��C����Sprite��name��lua����btn������0EA02968
--]]
function btnCallBackEvt(spriteName)
	
	--print('C�е�spriteName='..spriteName..',Lua�е�btn����='..spriteName)
	
	local btn = func_getTable(spriteName)--btn����
	
	--��ť�ص�
	--ex_callParmLuaFun((const char*)sprite->callLuaFunName,b->name);
	if(btn.clickEvtCallBack)	then	btn.clickEvtCallBack(btn)	end
end


function btnMouseDownEvt(spriteName)
	local btn = func_getTable(spriteName)--btn����
	
	if(btn.mouseDownCallBack) then
		btn.mouseDownCallBack(btn);
	end
end


--[[
	����һ����ť
	x,yĬ��ֵΪ0,0
--]]
function btn_create(x,y,w,h,url)
	local btn = f_createBtn()
	--local w = 100
	--local h = 100
	--btn.id = 123
	
	--���水ť������
	func_tableSave(btn)

	--func_printTable(btn)
	
	local name = func_getTableName(btn)--��ȡbtn������

	---------------------------------------
	--Ĭ�ϲ�������
	if(url == nil ) then url = "smallbtn.png" end
	if(x == nil) then x = 0	end
	if(y == nil) then y = 0 	end
	if(w == nil) then w = 80	end
	if(h == nil) then h = 18	end
	---------------------------------------
	local sprite = sprite_create(name,x,y,w,h,"btnCallBackEvt",nil,"btnMouseDownEvt");
	
	func_setIcon(sprite,url)
	
	btn.sprite =  sprite

	return btn
end

--[[
	Ϊ��ť��һ������ص�
	clickEvt(point)		point Ϊbtn����
--]]
function btn_bindClick(btn,clickEvt)
	btn.clickEvtCallBack = clickEvt;
end

--[[
	Ϊ��ť��һ������ص�
	clickEvt(point)		point Ϊbtn����
--]]
function btn_bindDown(btn,c)
	btn.mouseDownCallBack = c;
end

--[[
	���ٰ�ť����
--]]
function btn_dispose(btn)
	
	f_delLabel(btn)
	
	ptr_remove(btn.sprite)
	btn.sprite = nil
	
	btn.mouseDownCallBack = nil
	btn.clickEvtCallBack = nil

	func_tableDel(btn)
	
end
--[[
	���ð�ť���µ�����״̬
--]]
function btn_scaleStyle(btn)
	setv(btn.sprite,FLAGS_BUTTON_EFFECT);
end
--[[
	���ð�ťlabel
--]]
function btn_label(btn,str,r,g,b)
	if(str == nil) then
		
		--ɾ���ı�
		f_delLabel(btn)
	else
	
		if(btn.label == nil) then
			if(r == nil) then r = 1.0	end
			if(g == nil) then g = 1.0	end
			if(b == nil) then b = 1.0	end
			
			local x,y = get_attr(btn.sprite,"spritePos")

			btn.label=tf_create(128,x,y,r,g,b);
		end
		tf_setText(btn.label,str);
	end
end
--����btn��name
function btn_set_data(btn,data)
	btn.data = data
end

function btn_get_data(btn)
	return btn.data
end

--[[
	���ð�ť������ʾ
--]]
function btn_visible(btn,vis)
	local f = FLAGS_VISIBLE
	if(vis) then
		setv(btn.label,f);
		setv(btn.sprite,f);
	else
		resetv(btn.label,f);
		resetv(btn.sprite,f);
	end
end

--[[
	���ð�ť��������
--]]
function btn_pos(btn,x,y)
	func_setPos(btn.sprite,x,y)
	
	if(btn.label) then
		func_setPos(btn.label,x,y)
	end
end
---------------------------------------------------