@echo off
REM MERS Compiler Wrapper - Makes "my input.my" work like "gcc"
REM Usage: my input.my [options]

setlocal enabledelayedexpansion

if "%1"=="" (
    "%~dp0build\compiler.exe" -h
    exit /b 1
)

REM Forward all arguments to the compiler
"%~dp0build\compiler.exe" %*
