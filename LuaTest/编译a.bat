cd ..
call initroot.bat

cd LuaTest

@rem ���ӱ���include·��
set INCLUDE=%INCLUDE%%cd%\include\;

@rem ���ӱ���lib·��
set LIB=%LIB%%cd%\lib\;

cd out
del *.exe

cd ..

cl a.c
del *obj

copy a.exe out\a.exe
del *exe

cd out
a.exe

pause


