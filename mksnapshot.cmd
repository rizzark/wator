@echo off

set ARCHIVE=wator.zip 
set EXCLUDES=*.obj *.sbr *.pdb *.idb *.ilk *.pch *.lib *.aps *.res *.bsc *.exe *.dll *.ncb *.zip

if exist %ARCHIVE% del %ARCHIVE%


zip -r9 %ARCHIVE% o:\vc\320\wator\*.* -x %EXCLUDES% 
zip -r9 %ARCHIVE% o:\vc\320\libnomfc\*.* -x %EXCLUDES%
zip -r9 %ARCHIVE% o:\vc\320\wator\release\*.* -i *.exe *.pdb

pause





