@echo off
echo [��ǰҪ������ļ���:(%cd%\%f%.c)]

@rem opengl��ť��������
@rem һ��С�Ľ����ܳ���,���ڲ���.
@rem ��Ŀ��vs2005����

@rem �洢��ǰ·��
set cur=%cd%

cd..

set p=dll\%f%.exe

IF exist %p% (del %p%) ELSE (echo [%p%�ļ�������!])

call initroot.bat

@rem ���ӱ���include·��
set INCLUDE=%INCLUDE%%cd%\include\;%cur%\;

@rem ���ӱ���lib·��
set LIB=%LIB%%cd%\lib\;

cd include

@rem jgl.c gettime.c 
cl %cur%\%f%.c  tools.c gettime.c jgl.c

link *.obj
copy %f%.exe ..\dll\%f%.exe
del *.obj *.exe
..\dll\%f%.exe
pause