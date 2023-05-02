# Better Swim Prevent
A low level mouse hook to detect and prevent auto clickers, and preforms basic scans for running cheat processes
# How it works
Two low level hooks are made, one to hook mouse input and prevent double clicks, the other to read raw mouse input data and determine the source (if it came from a real hardware device or not)
<br>
All this currently does is prevent double clicking and log clicks in a basic console, as well as say if the mouse name, mouse info, and mouse HID is valid. Also does two basic checks on all running processes at the start for testing purposes.
# To Do
DLL enumeration checks on minecraft.exe
<br>
Window and sub window enumeration checks
<br>
Anti debugger checks
<br>
Scanning memory locations such as reach and setting them back to default on minecraft.exe (this will be hard)
<br>
Better logging, something upgraded from just a console program
<br>
A lot of testing, try to find as many ways to bypass what I currently have
# Disclaimer
This is just for education purposes right now, I personally have no bias for or against cheat devs/anti cheat devs, as I enjoy making cheats and anti cheats my self :D
# Requires C++ Redistributable
[Download here](https://aka.ms/vs/17/release/vc_redist.x64.exe)
