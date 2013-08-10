"C:\Installs\flex\flex" -osimple2_l.cpp simple2.l

cd C:\Installs\GnuWin32\bin\
"C:\Installs\GnuWin32\bin\bison" -v -d --debug -oC:\Dev\PhysicsWorld\skeleton\simple2_y.cpp C:\Dev\PhysicsWorld\skeleton\simple2.y
cd C:\Dev\PhysicsWorld\skeleton

gcc -osimple2.exe simple2_l.cpp simple2_y.cpp simple2main.cpp
