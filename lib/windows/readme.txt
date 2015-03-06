dumpbin /exports lua_shared.dll > lua_shared.def

then setup the def file with the format:
EXPORTS
FUNC1
FUNC2
...

lib /machine:x86 /def:lua_shared.def /out:lua_shared.lib