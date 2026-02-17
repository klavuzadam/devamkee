@echo off
    for /R "." %%f in (*.data) do (
	echo processing %%~pf%%~nf
	PackMakerLite --nolog -u "%%~pf%%~nf"
)
pause
