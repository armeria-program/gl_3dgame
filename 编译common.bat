echo %time%
@rem ��Ŀ��vs2005����

call initroot.bat

@rem ============================================================================================

@rem ���ӱ���include·��
set INCLUDE=%INCLUDE%%cd%\common\include\;


@rem ���ӱ���lib·��
::set LIB=%LIB%%cd%\lib\;

cd common\src\

cl -c *.c /Ox
::cl  common.c array.c byte.c evt.c gettime.c list.c psv.c ring.c str.c tl_malloc.c tools.c tween.c vmath.c xml.c /O1

link -dll -out:common.dll *.obj

::��ӡ�����ĺ���
::dumpbin /exports common.dll  

copy common.dll ..\..\dll\common.dll 
copy common.dll ..\..\Test\Debug\common.dll 
copy common.lib ..\..\lib\common.lib 


::@echo off
::��ȡtxt��doc��xls�ļ��ܴ�С
for /f "delims=" %%a in ('dir /b *.dll') do set /a "s+=%%~za/1024"
echo,%s% kb

del *.obj *.exp *.lib *.dll


cd..\..\
echo %CD%
::pause 