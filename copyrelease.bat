@echo off
if not exist L:\Lib goto error
copy .\Release\zLibdll.lib L:\Lib
copy .\Release\zLibdll.dll L:\Lib
goto done
:error
error: L:\Lib does not exist
goto done
:done