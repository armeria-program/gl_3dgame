dofile("..\\include\\lua\\func.lua");

dofile("..\\include\\lua\\UIlib.lua");--button
dofile("..\\include\\lua\\UScrollBar.lua")


--[[
local function myFun()
	
	print("myFun");
end
--]]

--����һ���ı�
function createTxt(x,y)
	return tf_create(128,x,y,1.0,1.0,1.0);
end


--���н���
checkBox={}

function checkBoxClick(name)
	local ck = instance[name];
	checkBox.clickEvent(ck);
	if(ck.callBack) then
		ck.callBack(ck)
	end	
end
--ΪcheckBox����һ���ı�
local function setCKlabel(ck,label)
	--���ı�������,û�оʹ����ı�����
	if(ck.tf==nil) then
		ck.tf,tfHeight=tf_create(128,	0,0,	1.0,1.0,1.0);
		tf_setPos(ck.tf,ck.posx+ck.width,ck.posy + (ck.height - tfHeight)/2);
	end
	
	tf_setText(ck.tf,label);
end
--��ʼ��һ��checkBox
function checkBox.new(url1,url2,x,y,w,h,callBack,label)
	local ck = 
	{
		select=false,
		--pname=name;
		name,
		icon1=nil,
		icon2=nil,
		posx=x,
		posy=y,
		width=w,
		height=h,
		spritePtr=nil,
		tf=nil,
		callBack
	};
	
	ck.callBack = callBack;
	ck.name = tableToInt(ck);
	instance[ck.name]=ck;
	
	local sprPtr=sprite_create(ck.name,x,y,w,h,"checkBoxClick");
	ck.spritePtr = sprPtr;
	ck.icon1 = url1;
	ck.icon2 = url2;

	local _x,_y,_width,_height=sprite_texName(sprPtr,url1);
	--setv(ck.spritePtr,flags.FLAGS_BUTTON_EFFECT);
	
	if(label) then
		setCKlabel(ck,label);
		--print(label);
	end
	return ck;
end



--����checkBox��ͼ
local function updateTextureByState(ck)
	local icon = nil;
	if ck.select then
		icon = ck.icon2;
	else
		icon = ck.icon1;
	end	
	sprite_texName(ck.spritePtr,icon);
end;

--������¼�
function checkBox.clickEvent(ck)
	ck.select = not ck.select;--ȡ��
	updateTextureByState(ck);
end

function checkBox.setSelect(ck,value)
	ck.select = value;
	updateTextureByState(ck);
end

--����checkBox
function checkBox.dispose(ck)
	ptr_remove(ck.spritePtr);
	ck.spritePtr=nil;
	ck.icon1=nil;
	ck.icon2=nil;
	
	if(ck.tf) then
		tf_dispose(ck.tf);
	end
	
	instance[ck.name]=nil;
end

-------------------------------------------------------

dofile("..\\include\\lua\\button.lua");

--�л�״̬
function switchFlags(_o,_flag)
	if(_o==nil) then
		print("switchFlags	[_o] is nil");
		return;
	end;
	
	if(_flag == nil)then
		print("flag is nil ");
	else
	
		if	getv(_o,_flag) == 1	then
			resetv(_o,_flag);
		else
			setv(_o,_flag);
		end
		
		--print('����'..string.format("%#x",_o).."���ñ�ʾ".._flag.."״̬Ϊ:"..getv(_o,_flag) );
	end
end

-------------------------------------------------------
--ui.scroll.ScrollBar
--������
dofile("..\\include\\lua\\ScrollBar.lua");





--------------------------------------------------------------------------------------
local nameKey = -1;
--����һ������
function getName()
	nameKey=nameKey+1;
	return "instance_"..tostring(nameKey);
end

function checkPtr(str,o)
	local ptr = tonumber(o);
	if(ptr==0) then
		print(str.."  checkPtr ����ʧ��!");
	else
		return o;
	end
	
	return nil;
end

--ʮ����תʮ������
function _10to16(v)
	return	string.format("%#x",v)
end

--λ�ƿ�����,4����ͷ�İ�ť
function arrowControl(x,y,arrowbtnClick)
	local w = 32;
	local arrow1 = button.new("arrow3.png",x+w,y,w,w,arrowbtnClick);
	local arrow2 = button.new("arrow3.png",x+2*w,y+w,w,w,arrowbtnClick);
	
	change_attr(arrow2.ptr,"sprite_rotateZ",tostring(-PI/2));
	
	local arrow3 = button.new("arrow3.png",x+w,y+w*2,w,w,arrowbtnClick);
	change_attr(arrow3.ptr,"sprite_rotateZ",tostring(-PI));
	
	local arrow4 = button.new("arrow3.png",x,y+w,w,w,arrowbtnClick);
	change_attr(arrow4.ptr,"sprite_rotateZ",tostring(-PI*1.5));
	
	button.setId(arrow1,1);
	button.setId(arrow2,2);
	button.setId(arrow3,3);
	button.setId(arrow4,4);
end

--��ȡ��ǰʱ�� ��ȷ������
function get_longTime()
	local _t = get_attr(nil,"get_longTime");
	return  _t;
end

----------------------------------

--����һ��obj ʹ��vboģʽ
function load_objvbo_byName(objName,tex)
	local name =getName();
	local o=load_VBO_model(name,"\\resource\\obj\\"..objName..".obj");
	setv(o,flags.FLAGS_VISIBLE);
	local m=createMaterial("vboDiffuse","\\resource\\texture\\"..tex);
	setMaterial(o,m);
	return o;
end

--����md2ģ��
function load_md2vbo_byName(objName,tex)
	
	local cur = func_getTime()
	local name =getName();
	
	if( objName == nil) then objName = "purgatori" end
		
	if( tex == nil) then tex = "purgatori.bmp" end
	
	local o=load_VBO_model(name,"\\resource\\md2\\"..objName..".md2");
	setv(o,flags.FLAGS_VISIBLE);
	
	local m=createMaterial("vboDiffuse","\\resource\\texture\\"..tex);
	setMaterial(o,m);
	
	print("["..objName..".md2,"..tex.."]".."��ʱ("..tostring((func_getTime() - cur) / 1000)..")seconds")
	return o;
end

--����һ������objģ��
function LoadRoutineObj(url,img)
	if(url == nil) then url = "torus" 		end--torus,teapot

	if(img == nil)then img = "default.bmp"	end
	
	local name = getName();
	local o=load_model(name,"\\resource\\obj\\"..url..".obj",0,0,0,1.0);
	
	local material=createMaterial("diffuse","\\resource\\texture\\"..img);
	setMaterial(o,material);
	
	setv(o,flags.FLAGS_VISIBLE);
	
	print("[����ģ��:"..url..".obj ʹ����ͼ:"..img.."]")
	return o;
end

--�÷�VBOģʽ����һ��md5ģ��,����������
function LoadLegacyWolfMd5(name,scale)	
	if(name == nil) then name = "wolf" end
	if(scale == nil) then scale = 1.0 end
	local md5=load_model(name,"\\resource\\md5\\wolf\\body.md5mesh",0,0,0,scale);	
	local mat1=createMaterial("diffuse","\\resource\\md5\\wolf\\body.bmp","");
	setMaterial(md5,mat1);
	md5_loadAnim(md5, "\\resource\\md5\\wolf\\walk.md5anim","walk");
	md5_loadAnim(md5, "\\resource\\md5\\wolf\\stand.md5anim","stand");
	setanim(md5,"walk");
	setv(md5,flags.FLAGS_VISIBLE);
	return md5;
end

--����һ��md2����ģ�ͽ���������
--function load_md2vbo_TestModel()
--	load_md2vbo_byName("horse","horse.bmp")
--end


--����listbox���
dofile("..\\include\\lua\\listbox.lua");

--����scrollBar���
--dofile("..\\include\\lua\\UScrollBar.lua");




--------------------------------------------------------------

--��ʼ����ʾ
local function f_setObjFlags(o)
	setv(o,flags.FLAGS_VISIBLE);
	--	setv(o,flags.FLAGS_GLSL_OUTLINE);
	--	setv(o,flags.FLAGS_DRAW_PLOYGON_LINE);--�߿���Ⱦ
	--	setv(o,flags.FLAGS_DRAW_NORMAL);
	
	
	--����ʰȡ
	---[[
	setv(o,flags.FLAGS_RAY);
	setv(o,flags.FLAGS_DRAW_RAY_COLLISION);
	setv(o,flags.FLAGS_RENDER_BOUND_BOX);
	--]]
	
end
--����һ��objģ��
local function f_loadObj(url,glsl)
	--math.randomseed(tostring(os.time()):reverse():sub(1, 7)) --����ʱ������
	--local 	name = "obj"..nameKey;
	
	--local name = "obj"..tostring(t);--math.random(1,100);--tostring(i)..'_'..os.time();
	
	local name = getName();
	local	o=load_model(name,"\\resource\\obj\\"..url..".obj",0,0,0,1.0);
	local	mat1=createMaterial(glsl,"\\resource\\texture\\e1.bmp","\\resource\\texture\\e2.bmp");--��������
	setMaterial(o,mat1);
	f_setObjFlags(o);
	
	--print(os.date("%c").."����:"..name);
	return o;
end
--�������е���ɫ���Ĳ��
function loadAllShader()
	
	local shaders={}

	shaders[0] = "diffuse";
	shaders[1] = "bump";
	shaders[2] = "diffuseSpecular";
	shaders[3] = "multTex";
	shaders[4] = "line";
	shaders[5] = "normalMap";
	shaders[6] = "toon";
	shaders[7] = "sprite";
	local tCount = #shaders;

	--print("length = "..tCount);
	
	local x = 0;
	for i=0,tCount,1 do

		--obj1=loadObj("teapot",shaders[i]);--triangle3
		if(shaders[i]) then
			print("loadAllShader  "..shaders[i]);
			x=x-50;
			
			local obj1=f_loadObj("teapot",shaders[i]);--triangle3
			change_attr(obj1,"setModelPos",tostring(x)..",0,-300");
		end
	end
	shaders = nil;
end
