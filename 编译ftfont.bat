echo %time%
@rem ��Ŀ��vs2005����

call initroot.bat

@rem ============================================================================================

@rem ���ӱ���include·��
set INCLUDE=%INCLUDE%%cd%\lib_freetype2.9.1\include\;%cd%\common\include\


@rem ���ӱ���lib·��
set LIB=%LIB%%cd%\lib_freetype2.9.1\;

cd ftfont

cl -c *.c /Ox


link -dll -out:ftfont.dll *.obj

::��ӡ�����ĺ���
dumpbin /exports ftfont.dll  

copy ftfont.dll ..\dll\ftfont.dll 
copy ftfont.dll ..\Test\Debug\ftfont.dll 
copy ftfont.lib ..\lib\ftfont.lib 


::@echo off
::��ȡtxt��doc��xls�ļ��ܴ�С
for /f "delims=" %%a in ('dir /b *.dll') do set /a "s+=%%~za/1024"
echo,%s% kb

del *.obj *.exp *.lib *.dll

::pause 