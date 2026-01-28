@echo on

echo celeritas_common_dir = %celeritas_common_dir%
echo %cd%
setx celeritas_common_dir "%cd%\..\src\common"

pause