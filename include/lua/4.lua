--����listbox���

dofile("..\\include\\lua\\core.lua");

local list =  listbox_new(10,30)

listbox_add(list,"gc")
listbox_add(list,"delete")

listbox_bind(list,
	function(_l)
		local s = string.format('index = %d\tlabel = [%s]',
			listbox_get_index(_l),
			listbox_get_label(_l))
		
		print(s)
		
		local label = listbox_get_label(_l)
			
		if(label == 'delete') then 
			listbox_del(_l) --ɾ�����
		end
		
		if(label== 'gc') then func_gc() end--gc
		
	end
)


