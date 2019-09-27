local function f_pool_getItem(sv)
    local data = sv.pool;
    for key, value in pairs(data) do
            if(value.used == 0) then
                value.used = 1;
                return value;
            end
	end
    local node = sv.itemFunc();
    sv.pool[tostring(node)] = node;    
    return node;
end
--��ȡ��ǰitemRender�ĸ߶�,�����nilȡ��ʼ����ʱ���gapֵ��Ϊ�߶�
local function f_getItemHeight(sv)
	if(sv.itemHeight==nil) then
		local node = f_pool_getItem(sv);
		node.used = 0;
		local w,h = func_get_sprite_size(node.view);
		sv.itemHeight = h;
	end
	return sv.itemHeight;
end

--���item�����Ƿ�������������
local function f_checkItemOutside(sv,pos_y)
    if(pos_y   >= sv.sh) then
        return 1;--���³���
    end

    if(pos_y + f_getItemHeight(sv) <= 0 ) then
        return 2;--���ϳ���
    end

    return 0;
end

--��ǰ��2dcam���
function scrollView_get_cam(sv)
	return sv.fbo.cam2d;
end

local function f_move_node(sv,node,offsety,changeValue)
    local view = node.view;
    local w,h = func_get_sprite_size(view);
    local x,y = func_get_sprite_xy(view);
    --if(y - changeValue <0) then
    --    sprite_setpos(view,0,0);
    --else
        sprite_setpos(view,0,y-changeValue);
    --end
end


local function f_find(sv,f)
	local data = sv.pool;
	
    for key, value in pairs(data) do
        local node = value;
		if(node.data == f)then
			return node.view;
		end
		
	end
end

local function f_find_by_index(sv,index)
	for key, node in pairs(sv.pool) do
		if(node.index == index)then
			return node;
		end
	end
end

function scrollView_set_pos(sv,x,y)
	sv.x = x;
	sv.y = y;
	fboobj_set_pos(sv.fbo,x,y);
end



local function f_pool_recycs(sv,targetObj)
	local data = sv.pool;
    for key, value in pairs(data) do
          --  local obj = value.obj;
         --   local used = value.used;
			
			--print('key=['..key..']'..'value['..value..']');
	        --print(value,obj,targetObj);		
			
            if(value == targetObj) then
                value.used = 0;
				print("recycs");
                return;
            end
	end
end

--�ͷŶ�����е����нڵ�
local function f_pool_recycs_all(sv)
    local data = sv.pool;
    for key, value in pairs(data) do
       -- local obj = value.obj;
       -- local used = value.used;
        value.used = 0;
	end
end
local function f_find_data_by_index(sv,index)
	local n = 0;
	for key, value in pairs(sv.dataList) do  
--		print(key,value);
		if(n == index) then
			return value;
		end
		n=n+1;
	end
	return nil;
end

local function f_find_node_by_data(sv,data)
	--[[for key, value in pairs(sv.dataList) do  
		if(value == data) then
			return sv.dataList[key];
		end
	end--]]
	
	for key, node in pairs(sv.pool) do
		if(node.data == data)then
			return node;
		end
	end
end

--���һ��������
local function f_add_scrollBar(sv)
	local sc;
	if(sv.dir == CONST_DIRECTION_VERTICAL) then
		sc = scrollBar_new(sv.x+sv.sw,sv.y,15,sv.maxSize);
	else
		print("��ʵ��");
	end
	sv.sc = sc;
   

    local temp = 0;
	
	local curp = 0;
	
	
	local curIndex = 0;
	
	local _startIndex = 0;--���µ���ʼ����
	
	--print(f_need_cnt);
	--����
    local function f_scHandle(_sc)
--         print("���ݳ���",sv.dataHeight);
	---[[	
	
		--*************************����
		if(sv.sh >= sv.dataHeight) then
			return;
		end
		local v = _sc.value;
		
		local oy = (sv.dataHeight-sv.sh)*v;
        local stat = oy - temp;

        temp = oy;
        
		local itemH = f_getItemHeight(sv);
		
		local p = math.floor(oy/itemH);
		local _sub = nil;
		if(p~=curp) then
			--print("******************************* change sub index %d",curp - p);
			_sub = curp - p;
			curp = p;
		end
		
		
		local iy= oy/itemH-p;
	--	print("dh="..sv.dataHeight,"change",stat,"oy="..oy.." sh="..sv.sh..","..(oy%sv.itemHeight),oy/sv.itemHeight,p);
	
		local itemy = -iy*itemH ;
		if(_sub~=nil) then
			curIndex = curIndex-_sub;
		end
		
		local _posList = {};--�����б�
		for i=0,sv.needCnt-1,1 do
			_posList[i] = itemy;
			itemy = itemy + itemH;
		end
		
		local newList = {};
		local m = 0;
		f_pool_recycs_all(sv);
		for n = curIndex,curIndex + sv.needCnt - 1 do
			
			local _td = f_find_data_by_index(sv,n);
			
			if(n >= _startIndex and n <= _startIndex + sv.needCnt - 1) then
				--print("**************** same data index = ",n,"data",_td);
				local node = f_find_node_by_data(sv,_td);
				node.used = 1;
				sprite_setpos(node.view,0,_posList[m]);				
			else
				--print("need ==> index = ",n,"data",_td);
				newList[m]=_td;
			end
			m = m + 1;
		end
		--print("*********************************");
		for key, value in pairs(newList) do
			local node = f_pool_getItem(sv);
			node.data = value;
			sprite_setpos(node.view,0,_posList[key]);
			sv.itemRefreshFunc(node);
		end
		
		_startIndex = curIndex;
		--********************************************************************
		
		--print(sv.dataList);
--		print(string.format("��ʼ����Ϊ%d,�ڵ�����Ϊ%d",_startIndex,sv.needCnt));
		
		--���˼·:�Ա���ʼ���ݺ͵�ǰ�仯������,���������Ҫrefresh��itemRender�ڵ�����ˢ����������,ƫ�Ʋ���Ҫ����ˢ���������ݵĽڵ�
		--������Ҫ�����б���
		
		
    end
	scrollBar_bind(sc,f_scHandle);	
end

--��ȡ�ù����������Ҫ���item����
local function f_need_cnt(sv)
	local h = f_getItemHeight(sv);
    local a = sv.sh % h;

    if(a > 0) then a = 1 end
	
    return (math.ceil(sv.sh/h) + a)+2;
end

--�����������srollView�е�����
function scrollView_set_data(sv,data)
	sv.dataList = data;
    local _realNeedCnt = f_need_cnt(sv);
    sv.needCnt = _realNeedCnt;
    f_pool_recycs_all(sv);


	local cur_h = 0;
	local n = 0;
    local cnt = 0;
	local h =  f_getItemHeight(sv);
	if(data) then
		for key, value in pairs(data) do      
			--print('key=['..key..']'..'value['..value..']')
			--print(value);
            if(f_checkItemOutside(sv,cur_h)~=0 and (cnt+1>_realNeedCnt)) then
                --print(value,"�����ɼ�����,��Ⱦ�޳�!,��ǰ�Ѿ������Ľڵ���;",cnt+1,"��������Ⱦ�ڵ���:",_realNeedCnt);
            else
                local _node = f_pool_getItem(sv);
                _node.data = value;
				
                local itemView = _node.view;
			    
			    --***********************
			    --����
                sprite_setpos(itemView,0,cur_h);
				
				_node.index = n;
				
				--print("��������",n);
				
				sv.itemRefreshFunc(_node);--refresh data
				
                n = n + 1;
			    cur_h = cur_h + h;
				
--				print(string.format("n = %d",n));
			    --***********************
            end
            cnt = cnt + 1;	
		end
	end
	
	
--	print(string.format("cnt=%d,n=%d",cnt,n));
	
--[[	if(n + 1 <= cnt) then
	local _node = f_pool_getItem(sv);
    _node.data = 5;
	
	end--]]
    
	sv.dataHeight = cnt * h;
    sv.datacnt = cnt;
end

--CONST_DIRECTION_HORIZONTAL = 0	--ˮƽ,��
--CONST_DIRECTION_VERTICAL   = 1 	--��ֱ,��

--gap �Զ���ļ��,Ĭ����ȡitemRender�������ĸ߶�
function scrollView_init(sw,sh,x,y,gap)
    local sv = {
		fbo,--FBO���
		sc,--���������
		
--		sprList,--sprite�б�
		dataList,--�����б�
        datacnt, --�����б���
		itemFunc,--item�б�Ĵ�������
        itemRefreshFunc,--��������,ˢ����ͼ�ķ���

        needCnt,--��Ҫ��Ԫ�ؽڵ���
		
		x,
		y,
		sw,--��������Ŀ�
		sh,--��������ĸ�
		dir,
		maxSize,
		
		dataHeight,--�����ܸ߶ȣ���dataList�й�ϵ

        itemHeight,--����item��Ⱦ�ڵ�ĸ߶�
		
        pool = {},--�����
  --      poolindex = 0;
    };
	local dir = CONST_DIRECTION_VERTICAL;
	if(gap) then
		sv.itemHeight = gap;
	end
	local maxSize = sw;

	if(sw < sh) then
		maxSize = sh;
	end
---[[
	sv.maxSize = maxSize;
	
	sv.sw = sw;
	sv.sh = sh;
	
	sv.dir = dir;
	
	sv.fbo = fboobj_init(maxSize,maxSize);
    
	x = x or 0;
	y = y or 0;
	sv.x =x;
    sv.y =y;
	f_add_scrollBar(sv);
    ---[[
	scrollView_set_pos(sv,x,y);
	--]]
	return sv;
end
