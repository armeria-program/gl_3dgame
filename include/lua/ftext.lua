--���û�������С
function func_ftext_set_buffer(txt, v)
    return ftext(txt, "set_buffer",v);
end
-- �����ı�����
function func_ftext_create(w, h, fw, fh)
    local name = func_create_name();
    w = w or 64;
    h = h or 64;

    -- 12,11
--    fw = fw or 14;
--    fh = fh or 13;
    fw = fw or 16;
    fh = fh or 15;
    local p = ftext(nil, "create", name, string.format("%s,%s,%s,%s", w, h, fw, fh));
    func_ftext_set_buffer(p,64);--����Ԥ�ƻ�����
    return p;
end
-- �����ı���������
function func_ftext_setpos(txt, x, y)
    ftext(txt, "setpos", string.format("%s,%s", x or 0, y or 0));
end
----�����ı������е��ı�����
-- function func_ftext_setchar(txt,s,x,y)
--    x = x or 0;
--    y = y or 0;
--    return ftext(txt,"setchar",s,string.format("%s,%s",x,y));
-- end
-- ��������ַ�
function func_ftext_parse(txt, s)
    return ftext(txt, "parse", s);
end
-- ��ʾ����
function func_ftext_vis(txt, v)
    return ftext(txt, "vis", tostring(v));
end

-- ����
function func_fext_clear(txt)
    ftext(txt, "clear");
end
--��ȡ�ı�
function func_ftext_str(txt)
    return ftext(txt, "str");
end
--��ȡ�ı���w,h
function func_ftext_getsize(txt)
    return ftext(txt, "size");
end
-- ���������ı�����
function func_ftext_reset(txt, s)
--    print(string.len(s));
    func_fext_clear(txt);
    func_ftext_parse(txt, s);
end

-- ����
function func_fext_dispose(txt)
    ftext(txt, "dispose");
end

----�ָ�����ĺ�Ӣ�Ĵ洢��table��
-- function func_split_cn(s)
----    local b = string.byte(s);
--    local len = string.len(s);
--    print("len=",len);
--    for i=1,len,1 do
--        print( "["..string.sub(s,i-1,1).."]")
--    end
-- end




-- ��ʾ�����������ı�
-- a,b   ==> a��b
-- function func_ftext_set_much_chars(ft,s,x,y,hGap)
--    hGap = hGap or 16
--    x = x or 0;
--    y = y or 0;
--    local arr =   func_split(s,",");
----    local i = 0;
--    local w,h;
--    for key, value in pairs(arr) do		
----		i = i + 1

--        w,h = func_ftext_setchar(ft,value,x,y);
----        print(w,h);
--        x=x+w;
-- end


----    local len = string.len(s);
----    print(len);
----    for i=1,len,1 do
----        print( string.sub(s,i-1,i))
----    end

----    "[\\0-\127\194-\244][\128-\191]*"
----      "[\0-\x7F\xC2-\xF4][\x80-\xBF]*"
----    for ch in string.gmatch(s, "[\\0-\127\194-\244][\128-\191]*") do
----	    print(ch,#ch~=1)
----    end
-- end


-- FText = {
--    str,--�ı�����
--    txtWidth,--�ı�����Ŀ��
-- };
-- FText.__index = FText;