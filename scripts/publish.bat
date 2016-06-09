@echo off
@echo publushing ...

set INCLUDE_DIR=./../include/
set LIB_DIR=./../lib/
set PUBLISH_INCLUDE_DIR=%EVL_PUBLISH_ROOT%engine\evl_net\include\
set PUBLISH_LIB_DIR=%EVL_PUBLISH_ROOT%engine\evl_net\lib\

@echo copying include files from %INCLUDE_DIR% to %PUBLISH_INCLUDE_DIR%
robocopy /E %INCLUDE_DIR% %PUBLISH_INCLUDE_DIR%
@echo copying include files ... OK

@echo copying libraries ...
move %LIB_DIR%*.lib %PUBLISH_LIB_DIR%
move %LIB_DIR%*.pdb %PUBLISH_LIB_DIR%
move %LIB_DIR%*.dll %PUBLISH_LIB_DIR%
@echo copying libraries ... OK

@echo publushing ... OK

cleanup.bat