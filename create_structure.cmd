@echo off
REM Включає затримку змінних
setlocal enabledelayedexpansion

REM === Створення структури папок ===
mkdir main\1\a
mkdir main\1\b
mkdir main\2\f
mkdir main\2\e
mkdir main\2\v
mkdir main\3\c
mkdir main\3\d
mkdir bin

REM === Створення файлів у a та b ===
echo Це файл a1 > main\1\a\a1.txt
echo Це файл b1 > main\1\b\b1.txt

REM === Перевірка: показує вміст папки перед копіюванням ===
echo Вміст main\1\a:
dir main\1\a
echo Вміст main\1\b:
dir main\1\b

REM === Копіювання файлів у bin ===
copy main\1\a\*.txt bin
copy main\1\b\*.txt bin

REM === Перевірка: що є в bin після копіювання ===
echo Вміст папки bin:
dir bin

REM === Виведення чисел від 1 до 100 з кроком 5 ===
for /L %%i in (1,5,100) do echo %%i

REM === Відкриття лише перших 10 .txt файлів на диску C ===
set count=0
for /R C:\ %%f in (*.txt) do (
    if !count! lss 10 (
        start "" "%%f"
        set /a count+=1
    ) else (
        goto :afteropen
    )
)
:afteropen

pause
