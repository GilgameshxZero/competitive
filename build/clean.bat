@ECHO OFF

FOR /d /r ..\ %%d IN (bin) DO @IF EXIST "%%d" rmdir /s /q "%%d"
FOR /d /r ..\ %%d IN (obj) DO @IF EXIST "%%d" rmdir /s /q "%%d"
FOR /d /r ..\ %%d IN (.vs) DO @IF EXIST "%%d" rmdir /s /q "%%d"
