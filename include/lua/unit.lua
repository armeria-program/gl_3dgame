--��ʼ������(����ɶ�̬���������ļ�,������һ���༭���༭��Щ���ź�ƫ���������md2�ļ�)
local function f_split_init(md2)
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end
--[[
--����һ��box
local function f_load_box(vbo)

	--vbo = false;--�Ƿ�ʹ��vboģʽ
	
	local obj
	local url = "\\resource\\obj\\o1.obj";--tri
	if(vbo) then
		local name = func_create_name();
		--print("name="..name);
		obj=load_VBO_model(name,url);--box	arrow
        local mat = func_load("//resource//material//triangle.mat");
		setMaterial(obj,mat);		
	else
		obj =load_model(func_create_name(), "\\resource\\obj\\box.obj");
		setMaterial(obj,func_load("//resource//material//diffuse.mat"));
	end
	
	setv(obj,FLAGS_DRAW_PLOYGON_LINE)--�߿�
	setv(obj,FLAGS_DISABLE_CULL_FACE);--����˫�涼����Ⱦ
	setv(obj,FLAGS_REVERSE_FACE);
	setv(obj,FLAGS_VISIBLE);
	return obj
end
--]]
Unit = {}
--print(Unit)
local function f_removeEvt(obj)
	--print(obj,obj.p);
	evt_off(obj.p,EVENT_ENGINE_BASE_UPDATE,obj.update);
	evt_off(obj.p,EVENT_ENGINE_BASE_END,obj.endCall);
end;

--	_key = "DEBUG"��ʱ����ʾһ������ģ��
function Unit:create(_key)
	
	--����tostring����,��������Ϣ
	
	self.__tostring = function(t)
		return string.format("Ԫ�� = %s p = %0x speed = %.3f",tostring(self),t.p,t.speed);
	end;
	
	local new_sharp = { 
		p = nil;--��ɫ���
		
		speed;--�ƶ��ٶ�
		
		update =function(data)
					--print(data,self,self.p);
					func_update_mat4x4(self.p);
					
					--func_update_mat4x4(self.box);
					--print("p = "+ p);
				end;
		
		endCall=function(data)
					
					--print("end %d:%d"..func_get_longTime().."\t"..self)
					f_removeEvt(self);
					
					func_set_anim(self.p,"stand");
					--print("�ƶ�����!!!");
				end;

		
        --����һ��������λ,Ĭ����1����λ������
        --����һ��vboģ��
        loadvbo = function(modelURL,maturl)
            local md2=load_VBO_model(func_create_name(),modelURL);
            setMaterial(md2,func_load(maturl));
	        setv(md2,FLAGS_VISIBLE);
            f_split_init(md2);
            setv(md2,FLAGS_DISABLE_CULL_FACE)--ȡ��˫����Ⱦ
            self.p = md2;
        end;

        --����һ�����Ե�������
        loadbox = function(vbo,url)
            vbo = vbo or true;
            local obj
	        url = url or "\\resource\\obj\\o1.obj";--tri
	        if(vbo) then
		        local name = func_create_name();
		        --print("name="..name);
		        obj=load_VBO_model(name,url);--box	arrow
                local mat = func_load("//resource//material//triangle.mat");
		        setMaterial(obj,mat);		
	        else
		        obj =load_model(func_create_name(), "\\resource\\obj\\box.obj");
		        setMaterial(obj,func_load("//resource//material//diffuse.mat"));
	        end
	
	        setv(obj,FLAGS_DRAW_PLOYGON_LINE)--�߿�
	        setv(obj,FLAGS_DISABLE_CULL_FACE);--����˫�涼����Ⱦ
	        setv(obj,FLAGS_REVERSE_FACE);
	        setv(obj,FLAGS_VISIBLE);
            self.p = obj;
        end;

        --���ö��������
        position=function(x,y,z)
            func_set_position(self.p,x,y,z);
        end;

        --�ƶ�һ����Ҫ�ĺ�����
        set_speed=function(value)
	        self.speed = value;
        end;
        move=function(x,y,z)
	        local o = self.p;
	        if(o==nil) then
                func_error("unit.p = nil");
                return;
            end
            if(self.speed==nil) then
                func_error("unit.speed = nil");
                return;
            end
	        local px,py,pz = func_get_xyz(o);
	
	        y = py;
	        --print(self.offset_y);
	        local distance = vec_distance(px,py,pz,x,y,z);--����ˮƽ����
	
	        --print(string.format("@@@ %.3f %.3f %.3f=>%.3f %.3f %.3f",px,py,pz,x,y,z));
	        --local func = self.update;
	        --print(self,func,o);
	        --self.update("aaaaa");
	        func_look_at(o,x,y,z);--ת��Ŀ������
	        --func_look_at(self.box,x,y,z)
	
        --	evt_off(o,EVENT_ENGINE_BASE_UPDATE,self.update);
        --	evt_off(o,EVENT_ENGINE_BASE_END,self.endCall);
	
	        func_set_anim(self.p,"run");
	
	        --self.removeEvt();
	        f_removeEvt(self);
	        --evt_on(o,EVENT_ENGINE_BASE_UPDATE,self.update);
	        evt_on(o,EVENT_ENGINE_BASE_END,self.endCall);
	
	        --print(string.format("��Ҫ���ߵľ��� = %.3f",distance));
	
	        --func_move(o,distance * 1000,x,y,z);--(һ����λ������1����ٶ�);
	
	        --print("��Ҫ��ʱ��:"..(distance * self.speed));
	       
	        func_move(o,distance * self.speed,x,y,z);
	        --func_move(self.box,distance * self.speed,x,y,z);
        end
	}
    self.__index = self  --�ڣ�self == Sharp
    setmetatable(new_sharp, self)  --��
	
	--��ʼ��
	
	--new_sharp.init();
	
    return new_sharp;
end


function Unit:scale(value)
    func_set_scale(self.p,value);
end
function Unit:rx(v)
    func_setRotateX(self.p,v)
end
function Unit:x(v)
    func_set_x(self.p,v);
end
function Unit:y(v)
    func_set_y(self.p,v);
end
function Unit:z(v)
    func_set_z(self.p,v);
end

function Unit:get_ptr()
	return self.p;
end	


--��һ���ٶ�ת��Ŀ��
function Unit:rotateTo(x,y,z,time)
    --local s = string.format("%.3f\t%.3f\t%.3f\ttime=%.3f\n",x,y,z,time);
    --print(s);
    func_look_at(self.p,x,y,z,time);
end

function Unit:set_anim(ani)
	func_set_anim(self.p,ani);
end

