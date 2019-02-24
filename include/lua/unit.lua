local m_name = "triangle"
m_name = "bauul"
m_name = "gobin"

--��ʼ������(����ɶ�̬���������ļ�,������һ���༭���༭��Щ���ź�ƫ���������md2�ļ�)
local function f_split_init(md2)
	
	if(m_name == "bauul" or m_name == "gobin")  then
		func_set_scale(md2,1/50);--��������һ�����ż�ֵ��ģ���ڵ�λ1����
		--func_set_y(md2,0.5);	 --���õذ�y����ƫ��
		func_setRotateX(md2,PI/2)--��תһ������
	end
	
	
	--print(md2);
	func_anim_push(md2,"stand",0,39);
	func_anim_push(md2,"run",40,45);
	func_anim_push(md2,"jump",66,71);
	func_set_anim(md2,"stand");
	func_play(md2);
end

--����һ��box
local function f_load_box()
	local obj=load_VBO_model(func_create_name(),"\\resource\\obj\\box.obj");
	setMaterial(obj,func_load("//resource//material//triangle.mat"));
	setv(obj,FLAGS_DRAW_PLOYGON_LINE)--�߿�
	--setv(obj,FLAGS_DISABLE_CULL_FACE);
	setv(obj,FLAGS_REVERSE_FACE);
	setv(obj,FLAGS_VISIBLE);
	return obj;
end

Unit = {}

local function f_removeEvt(obj)
	--print(obj,obj.p);
	evt_off(obj.p,EVENT_ENGINE_BASE_UPDATE,obj.update);
	evt_off(obj.p,EVENT_ENGINE_BASE_END,obj.endCall);
end;

function Unit:create(_key)
    local new_sharp = { 
		p = nil;--��ɫ���
		
		speed = 1000;--�ƶ��ٶ�
		
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
		init =  function ()
					--local u = {
					--	p = nil;--��Դ����
					--}
					--drfreak
					--bauul
					--triangle
					
					---[[
					
					if(_key ~= "box") then
						local url = m_name;
						local modelURL = string.format("\\resource\\md2\\%s.md2",url);
						local md2=load_VBO_model(func_create_name(),modelURL);
						--local md2 = load_model(func_create_name(),modelURL,0,0,0,1.0);
						setMaterial(md2,func_load(string.format("//resource//material//%s.mat",url)));
						setv(md2,FLAGS_VISIBLE);
						
						
						f_split_init(md2);
						--setv(md2,FLAGS_RAY);
						--setv(md2,FLAGS_DRAW_RAY_COLLISION);
						setv(md2,FLAGS_DISABLE_CULL_FACE)--ȡ��˫����Ⱦ
						----------------------------
						self.p = md2;
						
						--self.box = f_load_box();
						--]]
					else
						self.p = f_load_box();
					end
					
					func_set_y(self.p,0.5);--	y��ƫ��0.5
				end;
	}
    self.__index = self  --�ڣ�self == Sharp
    setmetatable(new_sharp, self)  --��
	
	--��ʼ��
	new_sharp.init();
	
    return new_sharp;
end
--�ƶ�һ����Ҫ�ĺ�����
function Unit:set_speed(value)
	self.speed = value;
end

function Unit:move(x,y,z)
	local o = self.p;
	
	local px,py,pz = func_get_xyz(o);
	
	y = py;
	
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
	evt_on(o,EVENT_ENGINE_BASE_UPDATE,self.update);
	evt_on(o,EVENT_ENGINE_BASE_END,self.endCall);
	
	--print(string.format("��Ҫ���ߵľ��� = %.3f",distance));
	
	--func_move(o,distance * 1000,x,y,z);--(һ����λ������1����ٶ�);
	func_move(o,distance * self.speed,x,y,z);
	--func_move(self.box,distance * self.speed,x,y,z);
end

function Unit:set_anim(ani)
	func_set_anim(self.p,ani);
end