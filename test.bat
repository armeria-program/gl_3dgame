::echo off
echo %time%��ʼ����
@rem ��Ŀ��vs2005����

call initroot.bat

@rem ============================================================================================


@rem ���ӱ���include·��
set INCLUDE=%INCLUDE%%cd%\include\;%cd%\common\include\;%cd%\ftfont\;

set commonpath=%cd%\common\src

@rem ���ӱ���lib·��
set LIB=%LIB%%cd%\lib\;


set loaclfile=%cd%\dll\test.exe
if exist "%cd%\dll\test.exe" (del %loaclfile%) else (echo �������ļ�%loaclfile%)

cd src

echo _________ ����%CD%\.*obj,����%commonpath%\*.c _________

cl *.c %commonpath%\*.c /Ox

echo _________ ����link.*obj _________
::pause

link -out:test.exe *.obj
echo %time%��������
del *.obj

copy test.exe ..\dll\test.exe
del *.exe 
cd ..\dll\ 

for /f "delims=" %%a in ('dir /b *.exe') do set /a "s+=%%~za/1024"
echo,%s% kb

test.exe 
 
cd ..

cd include

del *.exe

cd ..\src
del *.exe

::pause