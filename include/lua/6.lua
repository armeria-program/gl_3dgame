dofile("..\\include\\lua\\core.lua");
func_print('��Ԫ����ת����',0xff0000)



local animsc = scrollBar_new(0,0)
scrollBar_setRange(animsc,0,1)
local tf = scrollBar_add_text(animsc,'')

scrollBar_bind(animsc,
	function(sc)
		test_unit_01(sc.value)
		--print(sc.value)
		tf_setText(tf,sc.value)
	end
)


local _scale = 1
--С����
local obj1 = func_loadobj('quad',nil,'myObj1',false)--quad
setv(obj1,FLAGS_RAY)		
setv(obj1,FLAGS_DRAW_RAY_COLLISION)
setv(obj1,FLAGS_DRAW_PLOYGON_LINE)



func_set_scale(obj1,_scale)

--local md5file = func_loadmd5('wolf',0.02,"\\resource\\texture\\wolf.tga")

--����һ��objģ��
--VBOû����Ⱦ������
local box = func_loadobj('arrow',nil,'myBox',false)--'box' 'torus' 'teapot' 'arrow'
--setv(box,FLAGS_RAY)					--����Ϊ��ʰȡ״̬
setv(box,FLAGS_DRAW_RAY_COLLISION)
setv(box,FLAGS_DRAW_PLOYGON_LINE)




--�����ذ�
local _floor = func_loadobj('box',nil,'_floor',false);
func_set_scale(_floor,20);
func_set_y(_floor,-10);
setv(_floor,FLAGS_RAY)
setv(_floor,FLAGS_DRAW_RAY_COLLISION)
setv(_floor,FLAGS_DRAW_PLOYGON_LINE)

--����һ����ɫģ��
local horse=func_loadmd2('bauul','bauul.tga','vbo','_horse')--'horse'
func_setRotateX(horse,PI/2)--��תһ������
func_set_scale(horse,0.1)
func_set_x(horse,-5)
func_set_z(horse,-5)

local _target = func_loadobj('box',nil,'_target',false);
setv(_target,FLAGS_DRAW_PLOYGON_LINE)



func_set_camera_pos(0,-5,-20)
