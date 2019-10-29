local function f_onLoadTex(data,obj)
	print("next f_onLoadTex...",data,obj,obj.f_render);
	evt_once(obj,EVENT_ENGINE_RENDER_3D,obj.f_render,obj);
end

local function f_start(url,obj)
	
	local status = 0;
   	if(obj.cnt > 0) then
		local node = obj.list[obj.cnt-1];
		--evt_on(0,EVENT_ENGINE_TEX_LOAD_COMPLETE,f_start,obj);

		print("***********************************  ��ʼ����tex"..obj.cnt);
		
		load_tex(node.url);
		obj.cnt = obj.cnt - 1;
		
		func_clearTableItem(node);
		node = nil;
	else
		obj.callback(obj.param);
		func_clearTableItem(obj);
		obj = nil;
		status = 1;
	end
	return status;
end

local function f_render(data,obj)
	print("enter frame",obj);
	--evt_off(obj,EVENT_ENGINE_RENDER_3D,f_render);
	obj.f_render = f_render;
	evt_on(0,EVENT_ENGINE_TEX_LOAD_COMPLETE,f_onLoadTex,obj);
	f_start(nil,obj);
	
	
--[[	
	local status = f_start(nil,param);
	print(data,param,status);
	if(status==1) then
		evt_off(obj,EVENT_ENGINE_RENDER_3D,f_render);
	end
	--]]
end



--������Դ���֮��ص�
function resload(endCallBack,param,...)
	local obj = {}
	obj.list = {};
	obj.callback = endCallBack;
	obj.param = param;
	obj.cnt = 0;
	obj.f_render = nil;
	
	for index,value in ipairs({...}) do
		--print(index,value);
		local node = {};
		node.index = index - 1;
		node.url = value;
		obj.list[node.index] = node;
		obj.cnt = index;
	end
	--print(1);
	
	evt_once(obj,EVENT_ENGINE_RENDER_3D,f_render,obj);
	--evt_off(obj,EVENT_ENGINE_RENDER_3D,f_render);
	--f_start("",obj);
	--endCallBack(param);
end