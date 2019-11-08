```
--������������������������������ԣ����Ҹ��������Եĳ�ʼֵ
Class = {x=0,y=0}
--����Ԫ�����������ģ����Ļ����ⲽ�����ܹؼ�
Class.__index = Class
--���췽�������췽���������������ģ�ϰ��������Ϊnew()
function Class:new(x,y)
     local self = {}  --��ʼ��self�����û����䣬��ô���������Ķ��������һ���ı䣬�������󶼻�ı�
     setmetatable(self, Class)  --��self��Ԫ���趨ΪClass
     self.x = x   --����ֵ��ʼ��
     self.y = y
     return self  --��������
end

--���ﶨ�������������
function Class:test()
 print(self.x,self.y)
end

function Class:plus()
 self.x = self.x + 1
 self.y = self.y + 1
end

--require 'Class'

--�������µ�����Z  
SubClass = {z = 0}  
--����Ԫ��ΪClass  
setmetatable(SubClass, Class)  
--���Ǻ��ඨ��һ�����������趨Ϊ����  
SubClass.__index = SubClass  
--�����ǹ��췽��
function SubClass:new(x,y,z)  
   local self = {}             --��ʼ����������  
   self = Class:new(x,y)       --�����������趨Ϊ���࣬�������൱���������Ե�super ���������Ϊ���ø���Ĺ��캯�� 
   setmetatable(self, SubClass)    --����������Ԫ���趨ΪSubClass��  
   self.z= z                   --�µ����Գ�ʼ�������û�н��ᰴ������=0  
   return self  
end  

--����һ���µķ���  
function SubClass:go()  
   self.x = self.x + 10  
end  

--�ض��常��ķ������൱��override
function SubClass:test()  
     print(self.x,self.y,self.z)  
end



local a = Class:new() -- ����ʵ��������Ķ��󣬲����ø����еķ���
a:plus()
a:test()

a = SubClass:new()    -- Ȼ��ʵ�����������
a:plus()            -- ���������Է��ʵ������еĳ�Ա�ͷ���
a:go()                -- ���������������е���������
a:test()            -- ������������д�ķ���
```