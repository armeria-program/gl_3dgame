@rem 项目用vs2005编译

call initroot.bat

@rem ============================================================================================


@rem 增加本地include路径
set INCLUDE=%INCLUDE%%cd%\include\;

@rem 增加本地lib路径
set LIB=%LIB%%cd%\lib\;


set loaclfile=%cd%\dll\test.exe
if exist "%cd%\dll\test.exe" (del %loaclfile%) else (echo 不存在文件%loaclfile%)

cd src

cl test.c tlgl.c ex.c tools.c tmat.c md5.c jgl.c gettime.c sprite.c atlas.c obj_vbo.c anim.c node.c collide.c animtor.c obj.c tl_malloc.c test_unit_01.c

link *.obj

del *.obj

copy test.exe ..\dll\test.exe
del test.exe 
cd ..\dll\ 
test.exe 
 
cd ..

cd include

del *.exe



