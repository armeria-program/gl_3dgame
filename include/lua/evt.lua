local evtlist = {};

--[[
    params:Ĭ�ϲ����ݲ���

    local function f_resize(evtData,self)
        print(c_data);--����evt_dispatch�е�	local data;--����
    end
]]--

function evt_on(obj,id,func,params,once)	
	
	if(func_is_table_str(obj)) then
		func_print("evt_on==>"..tostring(obj).." ת��Ϊnumber");
		obj = func_get_address(obj);
	end
	
	
    if(func == nil) then
        func_error("func = nil");
    end

	--local str = string.format("���¼� evt id = %d , obj = %d",id,obj);
	local str = string.format("���¼� evt id=(%d),obj=(%s) params=(%s)",id,tostring(obj),tostring(params));
	func_print(str);

	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.obj == obj and node.id == id and node.func == func) then
			--node.func(data);
			--�ظ����¼�
			
			func_print("�ظ����¼�");
			return;
		end
	end	
		
	local evt = 
	{
		id = id;
		func = func;
		obj = obj;
        params = params;
		once = once
	}
	evtlist[evt] = evt;
end

function evt_once(obj,id,func,params)
	evt_on(obj,id,func,params,true);
end


--�Ƴ��¼� �����ͷ��¼�����
function evt_off(obj,id,func)
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			node.id = nil
			node.func = nil;
			node.obj = nil;
            node.params = nil;
			evtlist[node]=nil;
		end
	end
end
--�Ƿ��и��¼�
function evt_has(obj,id,func)
	for k, v in pairs(evtlist) do
		local node = evtlist[k];
		if(node and node.id == id and node.func == func and node.obj == obj) then
			return true
		end
	end
	return false
end

--ȫ���¼�
function evt_dispatch(...)
	 --id,data,obj
	local obj; --ָ��Ķ���
	local id;  --�¼�id
	local data;--����
	
	for index,value in ipairs({...}) do
		--print(index,value);
		if(index == 1) then
			obj = value;
		elseif(index == 2) then
			id = value;
		elseif(index == 3) then
			data = value;
		end
	end
	
--    if(id == 103)then
--        print("************");
--    end

	if (obj == 0) then
		--ȫ���¼�
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.id == id) then
				node.func(data,node.params);
				if(node.once) then
					evt_off(obj,id,node.func);--obj,id,func
					func_print("�Ƴ�ȫ���¼�"..id);
				end
			end
		end
	else
		--print(id,data,obj);
		for k, v in pairs(evtlist) do
			local node = evtlist[k];
			if(node and node.obj == obj--[[  �����¼��Ķ������ж�--]] and node.id == id) then
				
				node.func(data,node.params);
				
				if(node.once) then
					
					local str = string.format("evt_dispatch֮���Ƴ��¼� evt id = %d  node = (%s) obj =(%d)  data=(%s) node.params=(%s)",id,tostring(node),obj,tostring(data),tostring(node.params));
					func_print(str);
					
					evt_off(obj,id,node.func);--obj,id,func
				end
			end
		end
	end
	--print(id);
end




