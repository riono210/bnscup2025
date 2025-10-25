call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
msbuild ".\\"%1".vcxproj" /p:Configuration=Debug
cd /d "%~dp0App"
"%~dp0App\%1_debug.exe"