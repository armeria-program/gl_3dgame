--�����ı�����
function func_ftext_create(name)
    return ftext(nil,"create",name);
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

FText = {
    str,--�ı�����
};