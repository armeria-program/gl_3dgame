--������
local loadman = {
	isload=0,--�Ƿ��ڼ���
	list = {};--�б�
	cnt = 0;
	loadfunc=nil;
};

local function f_getnode()
	if(loadman.cnt == 0) then
		return nil;
	else
		loadman.cnt = loadman.cnt - 1;
		return loadman.list[loadman.cnt];
	end
end

local function f_callback(data,param)
	local node = param;
	
	--print("�������",node.callback,node.res);
	
	if(node.callback) then
		--print("�ص�",node.p);
		node.callback(node.p);
	end
	func_clearTableItem(node);
	local n =  f_getnode();
	
	if(n) then
		loadman.loadfunc(n);
	else
		loadman.isload = 0;
	end
end
local function f_load(node)
	--print("��ʼ����,����״̬:",loadman.isload);
	local r =  load_tex(node.res,JEngine:getIns():get_atals());
	evt_once(r,EVENT_ENGINE_TEX_LOAD_COMPLETE,f_callback,node);
    load_tex_start(r);
end

--��ʼ������Դ
--callback:�ص�����,p:����
function loadtexs(res,callback,p)
	local node = {
		res = res;
		callback = callback;
		p = p;--����
	};
	loadman.loadfunc = f_load;
	if(loadman.isload == 1) then
		loadman.list[loadman.cnt] = node;
		loadman.cnt = loadman.cnt + 1;
		func_print(string.format("%s[%s]","�������м�����Ŀ,�ȴ��������֮�� �첽����",res));
	else
		loadman.isload = 1;
		f_load(node);
	end
end