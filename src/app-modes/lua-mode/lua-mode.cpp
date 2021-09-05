#include <string>
#include <iostream>
#include "../../../BUILD/object_files/lua5.4/include/lua.hpp"

#include "../../../BUILD/object_files/lua5.4/include/lua.h"
#include "../../../BUILD/object_files/lua5.4/include/lualib.h"
#include "../../../BUILD/object_files/lua5.4/include/lauxlib.h"

using namespace std;
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cerr << "Invalid usage. Usage is: "
             << argv[0]
             << " <path to lua entry file>"
             << endl;
        return 1;
    }

    string luaPath = argv[1];

    cout << "Targeted lua file: " << luaPath << endl;

    lua_State *state = luaL_newstate();
    luaL_openlibs(state);
    int result = luaL_loadfile(state, luaPath.c_str());
    if (result != LUA_OK)
    {
        const char *message = lua_tostring(state, -1);
        puts(message);
        lua_pop(state, 1);
    }
    result = lua_pcall(state, 0, LUA_MULTRET, 0);

    lua_close(state);

    return 0;
}