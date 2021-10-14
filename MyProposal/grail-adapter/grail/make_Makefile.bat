@echo off
@echo "please choose you environment:"
@echo "a= Watcom"
@echo "b= borland"
@echo "c= VC60"
@echo "d= Use old Makefile"

:CHOOSE
set SYS=
set /p SYS= system type:

if %SYS%==a goto WAT
if %SYS%==b goto BOR
if %SYS%==c goto VC
if %SYS%==d goto ORI

goto ELSE
 
:WAT
echo "system set as Watcom"
del Makefile
copy Makefiles\Makefile_WAT  Makefile 
echo "Watcom Makefile is generated"
@pause
@exit

:BOR
echo "system set as Borland"
del Makefile
copy Makefiles\Makefile_BOR  Makefile
echo "Borland Makefile is generated"
@pause
@exit

:VC
echo "system set as VC60"
del Makefile
copy Makefiles\Makefile_VC60  Makefile
echo "VC60 Makefile is generated"
@pause
@exit

:ORI
echo "Using original Makefile"
del Makefile
copy Makefiles\Makefile_Original  Makefile
echo "Original Makefile is used"
@pause
@exit


:ElSE
echo "wrong input, choose again please"

@goto CHOOSE
 
