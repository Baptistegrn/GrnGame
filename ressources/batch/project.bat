@echo off

xcopy "%~dp0project_model" "%~2\project_model" /E /I /H /Y
ren "%~2\project_model" "%~1"
