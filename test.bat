echo %time%
@rem ��Ŀ��vs2005����

call initroot.bat

@rem ============================================================================================


@rem ���ӱ���include·��
set INCLUDE=%INCLUDE%%cd%\include\;

@rem ���ӱ���lib·��
set LIB=%LIB%%cd%\lib\;


set loaclfile=%cd%\dll\test.exe
if exist "%cd%\dll\test.exe" (del %loaclfile%) else (echo �������ļ�%loaclfile%)

cd src

cl test.c tlgl.c ex.c tools.c tmat.c md5.c jgl.c gettime.c sprite.c atlas.c obj_vbo.c anim.c node.c collide.c animtor.c obj.c md2.c tl_malloc.c test_unit_01.c byte.c xml.c psv.c list.c text.c vmath.c array.c bmp.c evt.c

link *.obj
echo %time%
del *.obj

copy test.exe ..\dll\test.exe
del test.exe 
cd ..\dll\ 

test.exe 
 
cd ..

cd include

del *.exe

cd ..\src
del *.exe




