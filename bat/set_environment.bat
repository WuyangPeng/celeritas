@echo on

echo celeritas_dir = %celeritas_dir%
echo %cd%
setx celeritas_dir "%cd%\..\src"

pause