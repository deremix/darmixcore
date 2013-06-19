@echo off
color 0b
:panel
cls
echo.
echo ////////////////////////////////////////////////////
echo /// BlizzLikeCore dbc/maps/vmaps/mmaps extractor ///
echo ////////////////////////////////////////////////////
echo.
echo Extract dbc/maps [1]
echo Extract vmaps	 [2]
echo Extract mmaps	 [3]
echo Exit		 [0]
echo.
set /p opc=what is your option? 
cls
if "%opc%" == "1" goto 1_extract_dbc_maps
if "%opc%" == "2" goto 2_extract_vmaps
if "%opc%" == "3" goto 3_extract_mmaps
if "%opc%" == "0" goto exit
goto panel
:1_extract_dbc_maps
if exist dbc (del /S /Q dbc)
if exist maps (del /S /Q maps)
map_extractor.exe
echo.
echo Press any key to panel . . .
pause>nul
goto panel
:2_extract_vmaps
if exist buildings (del /S /Q buildings)
vmap_extractor.exe
if exist vmaps (del /S /Q vmaps) else (md vmaps)
vmap_assembler.exe buildings vmaps
echo.
echo Press any key to panel . . .
pause>nul
goto panel
:3_extract_mmaps
if exist mmaps (del /S /Q mmaps) else (md mmaps)
mmap_generator.exe
echo.
echo Press any key to panel . . .
pause>nul
goto panel
:exit