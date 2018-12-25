function btnCallBack(name)
	local btn = instance[name];
	if(btn.clickEvtCallBack)	then	btn.clickEvtCallBack(btn)	end
end
function btnMouseDown(name)
	local btn = instance[name];
	--print(name);
	if(btn.mouseDownEvt) then
		btn.mouseDownEvt(btn);
	end
end
--��ť
button = {}
function button.new(url,x,y,w,h,clickEvt,btnLabel,mouseDownEvt)
	local btn = 
	{
		ptr=nil,--Sprite����
		tf=nil,
		clickEvtCallBack=nil,
		
		id = 0,	--��ťid
		mouseDownEvt=nil,
	};
	
	btn.clickEvtCallBack = clickEvt;
	local name = tableToInt(btn);--tostring(sc);
	instance[name] = btn;
	
	local mouseDownEvtStr = nil;
	if(mouseDownEvt==nil) then
		btn.mouseDownEvt = nil;
	else 
		mouseDownEvtStr = "btnMouseDown";
		btn.mouseDownEvt = mouseDownEvt;
	end
	
	btn.ptr=sprite_create(name,x,y,w,h,"btnCallBack",nil,mouseDownEvtStr);
	sprite_texName(btn.ptr,url);
	
	--���ð�ť���������Ч
	setv(btn.ptr,flags.FLAGS_BUTTON_EFFECT);
	
	--������ť�ı�
	if btnLabel == "" or btnLabel == nil then
		
	else
		--print("btn tf_create");
		btn.tf=tf_create(128,x,y,1.0,1.0,1.0);
		tf_setText(btn.tf,btnLabel);
	end
	
	--setv(btn.ptr,flags.FLAGS_DRAG);--����Ϊ������ק״̬
	
	return btn;
	
end

--���ð�ť���ı�
function button.setLabel(btn,label)
	tf_setText(btn.tf,label);
end

--���ð�ťid
function button.setId(btn,id)
	btn.id = id;
end

--��ȡ��ťid
function button.getId(btn)
	return btn.id;
end
--���ٰ�ť����
function button.dispose(btn)
	ptr_remove(btn.ptr);
	ptr_remove(btn.tf);
end
--[[
	����һ��ͨ����ʽ��ť
	local btn1 = button.createStyle1Btn(0,0,btnClick,"AShjs");
	local btn1 = button.createStyle1Btn(0,0,nil,"AShjs");
	local btn1 = button.createStyle1Btn(0,0,btnClick,);
]]--
function button.createStyle1Btn(x,y,clickEvt,label)
	return button.new("smallbtn.png",x,y,60,20,clickEvt,label)
end