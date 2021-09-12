#ifndef LUA_OBJECT_HELPERS
#define LUA_OBJECT_HELPERS

#include <map>
#include <string>
#include <functional>
#include <memory>
#include <iostream>
#include "../../../BUILD/object_files/lua5.4/include/lua.hpp"

#define NumberField(num) (Esperatto::Field(Esperatto::AllowedDataType::number, new lua_Number(num)))
#define StringField(str) (Esperatto::Field(Esperatto::AllowedDataType::string, new std::string(str)))
#define NilField() (Esperatto::Field(Esperatto::AllowedDataType::nil, nullptr))
#define BooleanField(b) (Esperatto::Field(Esperatto::AllowedDataType::boolean, new bool(b)))
#define TableField(t) (Esperatto::Field(Esperatto::AllowedDataType::table, new Table(t)))

namespace Esperatto
{
    class Table;
    typedef int(*LuableFunction)(lua_State*) ;
    // typedef std::function<int(lua_State *)> LuableFunction;
    enum AllowedDataType
    {
        nil,
        boolean,
        number,
        string,
        function,
        // userdata,
        // thread,
        table
    };

    class Field
    {
    protected:
        class Internal
        {
        public:
            AllowedDataType type;
            void *data = nullptr;
            friend class Field;
            ~Internal();
        };

    public:
        std::shared_ptr<Internal> data;

        Field(AllowedDataType type, void *data);

        friend class Table;
    };

    class Table
    {
    private:
    public:
        std::shared_ptr<std::map<std::string, Field>> data;
        Table();
        Table(const Table &source);
        void insert(std::string, Field);

        friend struct Field;
    };

    int compileTable(lua_State *state, Table definition);
}

#endif