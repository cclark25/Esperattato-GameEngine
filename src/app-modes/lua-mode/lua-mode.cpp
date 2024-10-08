#include <string>
#include <iostream>
#include "../../library/core/Game.h"
#include "../../../BUILD/object_files/lua5.4/include/lua.hpp"

#include "../../../BUILD/object_files/lua5.4/include/lua.h"
#include "../../../BUILD/object_files/lua5.4/include/lualib.h"
#include "../../../BUILD/object_files/lua5.4/include/lauxlib.h"
#include "../../library/Luable/lua-objects.h"

#define ADD_TO_LUA(state, name)     \
    lua_pushcfunction(state, name); \
    lua_setglobal(state, #name);

using namespace Esperatto;

Configuration config = Configuration({1,
                                      60,
                                      256,
                                      224,
                                      7.0,
                                      6.0, al_map_rgb(128, 128, 128)});

Esperatto::Game game = Esperatto::Game(config);

int repeatString(lua_State *state)
{
    int paramCount = lua_gettop(state);
    if (paramCount != 2)
    {
        throw "error";
    }

    int count = (int)lua_tonumber(state, -1);
    std::string baseString = lua_tostring(state, -2);

    std::string resultString = "";

    for (int i = 0; i < count; i++)
    {
        resultString += baseString;
    }

    lua_pushstring(state, resultString.c_str());

    return 1;
}

int createSquare(lua_State *state)
{
    int paramCount = lua_gettop(state);
    if (paramCount != 2)
    {
        throw "error";
    }

    double y = (double)lua_tonumber(state, -1);
    double x = (double)lua_tonumber(state, -2);

    CollisionSquareNode *second = new CollisionSquareNode(
        Coordinates({0, 0}), Coordinates({50, -50}), true);

    auto b = new Image("test/Test_Files/red-box.png");
    b->setZIndexInParent(-1);
    second->addChild(*b);
    game.rootNode.addChild(*second);
    second->setPositionInParent(x, y);
    std::cout << "Square created at: (" << x << ", " << y << ")" << std::endl;

    return 0;
}

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

    std::string luaPath = argv[1];

    cout << "Targeted lua file: " << luaPath << endl;

    lua_State *state = luaL_newstate();
    luaL_openlibs(state);
    
    ADD_TO_LUA(state, repeatString);
    ADD_TO_LUA(state, createSquare);
    
    game.compileTable(state);
    lua_setglobal(state, "Game");

    int result = luaL_loadfile(state, luaPath.c_str());
    if (result != LUA_OK)
    {
        const char *message = lua_tostring(state, -1);
        puts(message);
        lua_pop(state, 1);
    }
    result = lua_pcall(state, 0, LUA_MULTRET, 0);
    const char *message = lua_tostring(state, -1);
    cout << "Lua Script Response Code: " << result << endl;
    if (message != nullptr)
        puts(message);
    else
    {
    }

    lua_close(state);

    cout << "About to start game." << endl;
    game.StartGame();

    return 0;
}