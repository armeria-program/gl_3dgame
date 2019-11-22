function xml_load(url)
	return core_xml("load",url);
end

function xml_load_str(str)
	return core_xml("loadstr",str);
end

--xml�еĽڵ�ĸ���
function xml_get_length(xml)
	return	core_xml("get_node_cnt",xml);
end

function xml_get_node_by_index(xml,index)
	return	core_xml("getNodeByIndex",xml,index);
end

function xml_get_node(xml,k,v)
	return	core_xml("getrow",xml,k,v);
end

--��ȡ�ڵ��е���k��ֵ(string),���û��ȡ����key��Ӧ��ֵ�ͷ���nil
function xml_get_str(node,k,buffersize)
	buffersize = buffersize or 128;
	local str = core_xml("getstr",node,k,buffersize);
	return	str;
end

function xml_get_float(node,k)
	return core_xml("getfloat",node,k)
end

function xml_del(xml)
	core_xml("del",xml);
end

