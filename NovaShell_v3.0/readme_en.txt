===================================================================
          NOVASHELL ENTERPRISE v5.0 - PROJECT GUIDE
===================================================================

1. ABOUT THE PROJECT:
   NovaShell is a professional, enterprise-grade Windows administration 
   console written in pure C++ using Win32 API, SCM, IP Helper, and Winsock2. 
   Features a "hacker-style" interface and multi-language support (RU, UK, EN).

2. QUICK START & BUILD (via MSYS2 / MinGW x64):
   Open a terminal in the project folder and run:
   g++ main.cpp -o novashell.exe -lpsapi -liphlpapi -lws2_32

3. PROJECT FILE STRUCTURE:
   - main.cpp          -> Main source code for the NovaShell core and UI.
   - novashell.exe     -> Compiled executable utility file.
   - config.ini        -> Configuration file for auto-running applications.
   - novashell.log     -> Automatic system action log.
   - help_*.txt        -> Detailed built-in command instructions.
   - readme_*.txt      -> Developer guides in multiple languages.

4. SYSTEM REQUIREMENTS:
   - OS: Windows 10 / 11 (x64)
   - Permissions: Running as Administrator is recommended for managing 
     Windows services (the 'srv' commands).
===================================================================