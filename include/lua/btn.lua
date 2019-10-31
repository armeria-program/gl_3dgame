--EVENT_BUTTON_CLICK = 10000;--����¼�


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
		--ptr_remove(btn.label);
        fext_dispose(btn.label);
		btn.label = nil
	end
end

local function f_loadend(n)
	--print("f_loadend  "..n.url);
	sprite_set_9grid(n.sprite,n.url,3,3,3,3);
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

	local sprite = sprite_create_typical(name,x,y,w,h);
	engine_addNode(sprite);
	loadtexs(url,f_loadend, { sprite=sprite;url=url});
    	
	
	btn.sprite =  sprite;
	btn_scaleStyle(btn);
	
	return btn
end
--��ȡbtn��λ�ü���ߴ�
function btn_get_rect(btn)
    local x,y = get_attr(btn.sprite,"spritePos");
	local w,h = get_attr(btn.sprite,"spriteSize");
    return x,y,w,h;
end
--��ȡ��ť������(Sprite)
function btn_get_container(btn)
	return btn.sprite;
end

--[[
	Ϊ��ť��һ������ص�
	clickEvt(point)		point Ϊbtn����
--]]
function btn_bindClick(btn,clickEvt)
	btn.clickEvtCallBack = clickEvt;
	evt_on(btn.sprite,EVENT_ENGINE_SPRITE_CLICK,clickEvt);
end

--[[
	Ϊ��ť��һ������ص�
	clickEvt(point)		point Ϊbtn����
--]]
function btn_bindDown(btn,c)
	btn.mouseDownCallBack = c;
	evt_on(btn.sprite,EVENT_ENGINE_SPRITE_CLICK_DOWN,c);
end
--�Ƴ��¼�
local function f_remove_evt(btn)
	evt_off(btn.sprite,EVENT_ENGINE_SPRITE_CLICK,btn.clickEvtCallBack);
	evt_off(btn.sprite,EVENT_ENGINE_SPRITE_CLICK_DOWN,btn.mouseDownCallBack);
end
--[[
	���ٰ�ť����
--]]
function btn_dispose(btn)
	
	f_delLabel(btn)
	
	ptr_remove(btn.sprite)
	btn.sprite = nil
	
	btn.mouseDownCallBack = nil
	--btn.clickEvtCallBack = nil
	f_remove_evt(btn);


	func_tableDel(btn)
	
end
--[[
	���ð�ť���µ�����״̬
--]]
function btn_scaleStyle(btn)
	setv(btn.sprite,FLAGS_BUTTON_EFFECT);
end
--����label������
local function f_reset_label_pos(btn)
    local w,h = ftext_getsize(btn.label);
    local x,y = func_get_sprite_xy(btn.sprite);--get_attr(btn.sprite,"spritePos")
    local sw,sh=func_get_sprite_size(btn.sprite);
    ftext_setpos(btn.label,x+(sw-w)/2,y+(sh-h)/2);

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
			
--			local x,y = get_attr(btn.sprite,"spritePos")

--			btn.label=tf_create(128,x,y,r,g,b);
            btn.label = ftext_create();
--            ftext_setpos(btn.label,x,y);
		end
--		tf_setText(btn.label,str);
        ftext_reset(btn.label,str);
        
        f_reset_label_pos(btn);
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
		if(btn.label) then
--			setv(btn.label,f);
            ftext_vis(btn.label,1);
		end
		setv(btn.sprite,f);
	else
		if(btn.label) then
--			resetv(btn.label,f);
            ftext_vis(btn.label,0);
		end
		resetv(btn.sprite,f);
	end
end

--[[
	���ð�ť��������
--]]
function btn_pos(btn,x,y)
	func_setPos(btn.sprite,x,y)
	
	if(btn.label) then
--		func_setPos(btn.label,x,y)
--        ftext_setpos(btn.label,x,y);
         f_reset_label_pos(btn);
	end
end
--��ť����Ϊ�߿���Ⱦģʽ
--function btn_set_line(btn)
--    setv( btn.sprite,FLAGS_DRAW_PLOYGON_LINE);
--end
---------------------------------------------------
