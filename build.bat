cl /Zi src\dll.cpp /LD /Fedll /link /DEF:src\dll.def
cl dll.lib src\exe.cpp /Zi
