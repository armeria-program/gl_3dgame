function scrollView_set_pos(sv,x,y)
	sv.x = x;
	sv.y = y;
	fboobj_set_pos(sv.fbo,x,y);
end

--��ǰ��2dcam���
function scrollView_get_cam(sv)
	return sv.fbo.cam2d;
end

local function f_offset_xy(sv,oy)
	local cur_h = oy;
	local data = sv.sprList;
	for key, value in pairs(data) do      
		--print('key=['..key..']'..'value['..value..']')
		--print(value);
		
		local spr = value;
		sprite_setpos(spr,0,cur_h);
		local w,h = func_get_sprite_size(spr);
		cur_h = cur_h + h;
		
--[[		
		local itemView = sv.itemFunc();
		sv.sprList[n]= itemView;
		
		local w,h = func_get_sprite_size(itemView);
		--print(w,h);
		
		
		--***********************
		--����
		sprite_setpos(itemView,0,cur_h)
		n = n + 1;
		cur_h = cur_h + h;
		--***********************
		--]]
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
	
	--����
    local function f_scHandle(_sc)
	---[[	
	
		--*************************����
		if(sv.sh >= sv.dataHeight) then
			return;
		end
		local v = _sc.value;
		
		local oy = (sv.sh - sv.dataHeight)*v;
        --print(oy);
		f_offset_xy(sv,oy);
        --func_setPos(sprite,0,(h-ch)*v);--���о�����������Ĺ�����
		--]]
		--**************************
    end
	scrollBar_bind(sc,f_scHandle);	
end

--�����������srollView�е�����
function scrollView_set_data(sv,data)
	sv.dataList = data;
	--local cam = f_get_cam(sv);
	if(sv.sprList==nil) then
		sv.sprList = {}
	end
	
	local cur_h = 0;
	local n = 0;
	if(data) then
		for key, value in pairs(data) do      
			--print('key=['..key..']'..'value['..value..']')
			print(value);
			local itemView = sv.itemFunc();
			sv.sprList[n]= itemView;
			
			local w,h = func_get_sprite_size(itemView);
			--print(w,h);
			
			
			--***********************
			--����
			sprite_setpos(itemView,0,cur_h)
			n = n + 1;
			cur_h = cur_h + h;
			--***********************
		end
	end
	
	sv.dataHeight = cur_h;
end

--CONST_DIRECTION_HORIZONTAL = 0	--ˮƽ,��
--CONST_DIRECTION_VERTICAL   = 1 	--��ֱ,��
function scrollView_init(sw,sh,x,y,dir)
    local sv = {
		fbo,--FBO���
		sc,--���������
		
		sprList,--sprite�б�
		dataList,--�����б�
		itemFunc,--item�б�Ĵ�������

		
		x,
		y,
		sw,--��
		sh,--��
		dir,
		maxSize,
		
		dataHeight,--�����ܸ߶ȣ���dataList�й�ϵ
    };
	dir = dir or CONST_DIRECTION_VERTICAL;
	
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
