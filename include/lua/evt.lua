--local evtlist = {};

local list = NStack:new();--�¼��б�
local function f_each(node,p)
	--print(d,p);

	--[[local evt = 
	{
		id = id;
		func = func;
		obj = obj;
        params = params;
		once = once
	}
--]]
	if(node and node.obj == p.obj and node.id == p.id and node.func == p.func) then
		p.find = true;
		func_print("�ظ����¼�,�¼�id = "..p.id);
		return true;
	end
end

local function f_find(node,p)
	if(node and node.obj == p.obj and node.id == p.id and node.func == p.func) then
		p.node = node;
		return true;
	end
end

--node and node.obj == p.obj and 
local function f_find_dispath(node,p)
	if(node.id == p.id) then
		p.node = node;
		return true;
	end
end

local function f_find_dispath_obj(node,p)
	if(node and node.obj == p.obj and node.id == p.id) then
		p.node = node;
		return true;
	end
end

--�Ƿ���table�ַ���
local function func_is_table_str(value)
	local s = tostring(value);
	return string.find(s,"table: ") ~= nil;
end
--[[
    params:Ĭ�ϲ����ݲ���

    local function f_resize(evtData,self)
        print(c_data);--����evt_dispatch�е�	local data;--����
    end
]]--
--��tableת��Ϊһ��Numberֵ(��ʵ��table�ĵ�ַ����)
local function f_cv(obj)
	if(func_is_table_str(obj)) then
		local n = func_get_address(obj);
		func_print("evt_on==>"..tostring(obj).." ת��ΪNumber:"..string.format("%0x",n));
		return n;
	end
	return obj;
end

function evt_on(obj,id,func,params,once)	
	
	obj = f_cv(obj);--is number
	
    if(func == nil) then
        func_error("func = nil");
    end

	--local str = string.format("���¼� evt id = %d , obj = %d",id,obj);
	local str = string.format("���¼� evt id=(%d),obj=(%s) params=(%s)",id,tostring(obj),tostring(params));
	func_print(str);


	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.func = func;
	findobj.find = false;
	
	list:for_each(f_each,findobj);
	if(findobj.find)then
		
	else
		local evt = 
		{
			id = id;
			func = func;
			obj = obj;
			params = params;
			once = once;
		}
		--local v = tonumber(f_cv(evt));
		list:push(evt);
		
		--func_print("add֮���¼�"..id.."����"..list:len());
	end
--	evtlist[evt] = evt;
	findobj = nil;
end

function evt_once(obj,id,func,params)
	evt_on(obj,id,func,params,true);
end

--�Ƿ��и��¼�
function evt_has(obj,id,func)
	--[[for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			return true
		end
	end
	return false--]]
	
	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.func = func;
	findobj.node = nil;
	
	list:for_each(f_find,findobj);
	
	if(findobj.node) then
		return true;
	end
    return false;
end
--�Ƴ��¼� �����ͷ��¼�����
function evt_off(obj,id,func)

--[[
	if(evt_has(obj,id,func)==false)then
		local str = string.format("evt_has�Ƴ�[%d]�¼�[%d]ʧ��! �¼�������",obj,id);
		func_print(str);
		return;
	end
--]]

	
	local ok = false;
	--[[
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			
			local str = string.format("�Ƴ��¼� evt_off==> evt.id = "..id..",obj = "..obj);
			func_print(str);
			
			

			evtlist[node]=nil;
			func_clearTableItem(node);
			node = nil;
			ok = true;
		end
	end
	--]]
	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.func = func;
	findobj.node = nil;
	
	list:for_each(f_find,findobj);
	if(findobj.node) then
		local str = string.format("�Ƴ��¼� evt_off==> evt.id = "..id..",obj = "..obj);
		func_print(str);
		
		list:del(findobj.node);
		--func_print("del֮����¼�"..id.."����"..list:len());
		ok = true;
		
	else
		func_error("�Ƴ��¼�"..id.."ʧ��!");
	end
	
	findobj = nil;
	
	--[[if(ok~=true) then
		func_print("�Ƴ��¼�"..id.."ʧ��!",0xff0000);
	end--]]
	
	return ok;
end


--ȫ���¼�
function evt_dispatch(...)
	 --id,data,obj
	local obj; --ָ��Ķ���
	local id;  --�¼�id
	local data;--����
--	obj ,id,data = f_parse({...});
	local arr = {...};
	obj = arr[1];
	id  = arr[2];
	data= arr[3];
	
	obj = f_cv(obj);

	
--	print("evt_dispatch",obj ,id,data );
	
	
		
--[[		
	for index,value in ipairs({...}) do
		--print(index,value);
		if(index == 1) then
			obj = value;
		elseif(index == 2) then
			id = value;
		elseif(index == 3) then
			data = value;
		end
	end--]]
	
--    if(id == 103)then
--        print("************");
--    end
	
	
	local findobj = {};
	findobj.obj = obj;
	findobj.id = id;
	findobj.node = nil;

	
	if (obj == 0) then
		--ȫ���¼�
		--[[for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.id == id) then
				node.func(data,node.params);
				if(node.once) then
					evt_off(obj,id,node.func);--obj,id,func
					--func_print("�Ƴ�ȫ���¼�"..id);
				end
			end
		end--]]
		list:for_each(f_find_dispath,findobj);
		local node = findobj.node;
		if(node)then
			node.func(data,node.params);
			if(node.once) then
				evt_off(obj,id,node.func);--obj,id,func
				--func_print("�Ƴ�ȫ���¼�"..id);
			end
		end
	else
		--print(id,data,obj);
	
		
		list:for_each(f_find_dispath_obj,findobj);

		local node = findobj.node;
		if(node)then
			node.func(data,node.params);--data:C�㴫�ݵĲ��� node.params:Lua�㴫�ݵĲ���
				
				if(node.once) then
					
					--local str = string.format("evt_dispatch֮���Ƴ��¼� evt id = %d  node = (%s) obj =(%d)  data=(%s) node.params=(%s)",id,tostring(node),obj,tostring(data),tostring(node.params));
					--func_print(str);
					
					evt_off(obj,id,node.func);--obj,id,func
				end
		end
	end
	--print(id);
end