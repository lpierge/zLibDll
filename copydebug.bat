@echo off
if not exist L:\Lib goto error
copy .\Debug\zLibdll.d.lib L:\Lib
copy .\Debug\zLibdll.d.dll L:\Lib
goto done
:error
error: L:\Lib does not exist
goto done
:done