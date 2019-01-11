@ECHO OFF
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::
:: Copyright (C) 2018 Pelagicore AG
:: Contact: https://www.qt.io/licensing/
::
:: This file is part of the QtIvi module of the Qt Toolkit.
::
:: $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
:: Commercial License Usage
:: Licensees holding valid commercial Qt Automotive Suite licenses may use
:: this file in accordance with the commercial license agreement provided
:: with the Software or, alternatively, in accordance with the terms
:: contained in a written agreement between you and The Qt Company.  For
:: licensing terms and conditions see https://www.qt.io/terms-conditions.
:: For further information use the contact form at https://www.qt.io/contact-us.
::
:: GNU General Public License Usage
:: Alternatively, this file may be used under the terms of the GNU
:: General Public License version 3 as published by the Free Software
:: Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
:: included in the packaging of this file. Please review the following
:: information to ensure the GNU General Public License requirements will
:: be met: https://www.gnu.org/licenses/gpl-3.0.html.
::
:: $QT_END_LICENSE$
::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SET SCRIPT=%~dp0

IF %1%=="" (
    call:usage
)

SET VIRTUALENV=%1%
IF NOT EXIST %VIRTUALENV% (
    call:usage
)

IF NOT EXIST %VIRTUALENV%\Lib (
    call:usage
)

SET VIRTUALENV_LIB=%VIRTUALENV%\lib
SET LIB_FOLDER=%VIRTUALENV_LIB%
FOR %%F in (%VIRTUALENV_LIB%\python*) DO (
    IF EXIST %%~fF (
        SET LIB_FOLDER=%%~fF
        SET PYTHON_VERSION=%%~nF
    )
)

IF NOT EXIST %LIB_FOLDER%\orig-prefix.txt (
    echo "orig-prefix.txt doesn't exist";
    exit 1
)

SET /p ORIG_PREFIX=<%LIB_FOLDER%\orig-prefix.txt
SET ORIG_LIB=%ORIG_PREFIX%\lib\%PYTHON_VERSION%
IF NOT EXIST "%ORIG_LIB%" (
    echo "%ORIG_LIB% doesn't exist"
    exit 1
)

echo "copying files from %ORIG_LIB% to %VIRTUALENV_LIB%"
FOR /f %%i in (%SCRIPT%\deploy-virtualenv-files.txt) DO (
    IF EXIST "%ORIG_LIB%%%i\" (
        IF NOT EXIST %VIRTUALENV_LIB%\%%i mkdir %VIRTUALENV_LIB%\%%i
        xcopy "%ORIG_LIB%%%i" "%VIRTUALENV_LIB%\%%i" /E /Q /H /Y >NUL 2>&1
    ) else (
        xcopy "%ORIG_LIB%%%i" "%VIRTUALENV_LIB%" /H /Q /Y >NUL 2>&1
    )
)

IF EXIST %ORIG_PREFIX%\DLLs\ (
    IF NOT EXIST %VIRTUALENV%\DLLs mkdir %VIRTUALENV%\DLLs
    xcopy "%ORIG_PREFIX%\DLLs" "%VIRTUALENV%\DLLs" /E /Q /H /Y >NUL 2>&1
)

echo "done"

EXIT /B %ERRORLEVEL%

:: Functions

:usage
echo "deploy-virtualenv.bat <virtualenv>"
EXIT 1


