--�����ı�����
function func_ftext_create(name,fw,fh)
    fw = fw or 12;
    fh = fh or 11;
    return ftext(nil,"create",name,string.format("%s,%s",fw,fh));
end
--�����ı���������
function func_ftext_setpos(txt,x,y)
    ftext(txt,"setpos",string.format("%s,%s",x,y));
end
--�����ı������е��ı�����
function func_ftext_setchar(txt,s,x,y)
    x = x or 0;
    y = y or 0;
    return ftext(txt,"setchar",s,string.format("%s,%s",x,y));
end
--��ʾ�����������ı�
function func_ftext_set_much_chars(ft,s,x,y,hGap)
    hGap = hGap or 16
    x = x or 0;
    y = y or 0;
    local arr =   func_split(s,",");
    local i = 0;
    local w,h;
    for key, value in pairs(arr) do		
		i = i + 1
		
        w,h = func_ftext_setchar(ft,value,x,y);
        --print(w,h);
        x=x+w;
	end


--    local len = string.len(s);
--    print(len);
--    for i=1,len,1 do
--        print( string.sub(s,i-1,i))
--    end

--    "[\\0-\127\194-\244][\128-\191]*"
--      "[\0-\x7F\xC2-\xF4][\x80-\xBF]*"
--    for ch in string.gmatch(s, "[\\0-\127\194-\244][\128-\191]*") do
--	    print(ch,#ch~=1)
--    end
end


FText = {
    str,--�ı�����
};