@echo off

:init
    set TKAPI=..\TKAPI
    set PTHREADS=..\pthreads
    set DSTDIR=debug,release


:copy
    for %%i in (%DSTDIR%) do (
        if exist %%i (
            replace %TKAPI%\%%i\tkbase.dll %%i /A
            replace %TKAPI%\%%i\tkbase.dll %%i /U
            
            replace %TKAPI%\%%i\tkw32.dll %%i /A
            replace %TKAPI%\%%i\tkw32.dll %%i /U

            replace %PTHREADS%\lib\pthreadvc2.dll %%i /A     
            replace %PTHREADS%\lib\pthreadvc2.dll %%i /U     
        )
    )


:ende
    echo Verarbeitung beendet
    pause
