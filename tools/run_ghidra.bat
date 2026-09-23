@echo off
rem Headless Ghidra import + analysis of the Sims2EP9.exe dump, then ExportRE.java writes re\*.tsv.
set JAVA_HOME=C:\Users\Nitropc\scoop\apps\temurin21-jdk\current
set PATH=C:\Users\Nitropc\scoop\apps\temurin21-jdk\current\bin;%PATH%
set R=C:\Users\Nitropc\code\mods\sims2
call "C:\Users\Nitropc\scoop\apps\ghidra\current\support\analyzeHeadless.bat" "%R%\re\ghidra" Sims2RE -import "%R%\dumps\Sims2EP9_1.18.0.187_24041f83_dump.exe" -overwrite -max-cpu 12 -scriptPath "%R%\tools\ghidra" -postScript ExportRE.java "%R%\re" -log "%R%\re\ghidra_analysis.log" -scriptlog "%R%\re\ghidra_script.log" > "%R%\re\ghidra_stdout.log" 2>&1
echo EXIT %ERRORLEVEL% >> "%R%\re\ghidra_stdout.log"
