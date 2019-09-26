--���item�����Ƿ�������������
local function f_checkItemOutside(sv,pos_y)
    if(pos_y   >= sv.sh) then
        return 1;--���³���
    end

    if(pos_y + sv.itemHeight <= 0 ) then
        return 2;--���ϳ���
    end

    return 0;
end

--��ǰ��2dcam���
function scrollView_get_cam(sv)
	return sv.fbo.cam2d;
end
--��������node�ڵ��index��ֵ
local function f_rebuildIndex(list,pnode,ptype)
    local cnt = 0;
    for key, value in pairs(list) do
        local node = value;
        cnt = cnt + 1;
        if(node~=pnode) then
            if(ptype == 1) then
                node.index = node.index + 1;
            else
                node.index = node.index - 1;
            end
           
        end
    end
    if(ptype == 1) then
        pnode.index = 0;
    else
        pnode.index = cnt-1;
    end
    pnode.ptype = ptype;
   -- print(list);
end

local function f_getPos(list,index)
 for key, value in pairs(list) do
        local node = value;
        if(node.index == index)then
            local spr = node.view;
            local x,y = func_get_sprite_xy(spr);
            return y;
        end
    end
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

local function f_pool_getItem(sv)
    local data = sv.pool;
    for key, value in pairs(data) do
            --local obj = value.obj;
            --local used = value.used;
            if(value.used == 0) then
                value.used = 1;
                return value;
            end
	end
    local node = sv.itemFunc();
    
    --print(tostring(node));

    sv.pool[tostring(node)] = node;
    
    return node;

    --[[
    local p = {};
    p.obj =  sv.itemFunc();
    p.used = 1;
    sv.pool[sv.poolindex] = p;
    --]]
--      print("new node",sv.poolindex);ret
--      sv.poolindex = sv.poolindex + 1;
--      return p.obj;

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
--���һ��������
local function f_add_scrollBar(sv)
	local sc;
	if(sv.dir == CONST_DIRECTION_VERTICAL) then
		sc = scrollBar_new(sv.x+sv.sw,sv.y,15,sv.maxSize);
	else
		print("��ʵ��");
	end
	sv.sc = sc;


--**************************************************

	
--**************************************************

   

    local temp = 0;
	
	local curp = 0;
	
	local curIndex = 0;
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
        
		local p = math.floor(oy/sv.itemHeight);
		local _sub = nil;
		if(p~=curp) then
			--print("******************************* change sub index %d",curp - p);
			_sub = curp - p;
			curp = p;
		end
		
		
		local iy= oy/sv.itemHeight-p;
	--	print("dh="..sv.dataHeight,"change",stat,"oy="..oy.." sh="..sv.sh..","..(oy%sv.itemHeight),oy/sv.itemHeight,p);
	
	
	--[[	
		local item = f_find(sv,1);
		resetv(item,FLAGS_DRAW_PLOYGON_LINE);
		--print("iy="..iy,item);
		local itemy = -iy*sv.itemHeight;
		sprite_setpos(item,0,itemy);--����λ�ڶ�����itemԪ����ͼ���������
		--]]
		
		local itemy = -iy*sv.itemHeight;
		if(_sub~=nil) then
			curIndex = curIndex-_sub;
		end
		for i=0,sv.needCnt-1,1 do
			--print(i)
			local _node = f_find_by_index(sv,i);
			local item = _node.view;
			sprite_setpos(item,0,itemy);
			itemy = itemy + sv.itemHeight;
			if(_sub~=nil) then
				_node.data = f_find_data_by_index(sv,i+curIndex);
				if(_node.data) then
					sv.itemRefreshFunc(_node);
				end
			end
		end
    end
	scrollBar_bind(sc,f_scHandle);	
end

--��ȡ�ù����������Ҫ���item����
local function f_need_cnt(sv)
     local a = sv.sh % sv.itemHeight;

    if(a > 0) then a = 1 end
	
    return (math.ceil(sv.sh/sv.itemHeight) + a)+2;
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
			    sv.itemRefreshFunc(_node);--refresh data
			    local w,h = func_get_sprite_size(itemView);
			    --print(w,h);
			    sv.itemHeight = h;
			    --***********************
			    --����
                sprite_setpos(itemView,0,cur_h);

                --sv.sprList[n]= itemView;
                --f_saveNode(sv,itemView,n);
				--print(n);
				
				_node.index = n;
				
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
    
	sv.dataHeight = cnt * sv.itemHeight;
    sv.datacnt = cnt;
end

--CONST_DIRECTION_HORIZONTAL = 0	--ˮƽ,��
--CONST_DIRECTION_VERTICAL   = 1 	--��ֱ,��
function scrollView_init(sw,sh,x,y,dir)
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
	dir = dir or CONST_DIRECTION_VERTICAL;
	
    sv.itemHeight = 64;

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
