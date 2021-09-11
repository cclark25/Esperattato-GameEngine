#include "./lua-objects.h"

namespace Esperatto
{
    Table::Table()
    {
        this->data = std::shared_ptr<std::map<std::string, Field>>(new std::map<std::string, Field>);
    }
    void Table::insert(std::string str, Field f)
    {
        std::pair<std::string, Field> p = std::pair<std::string, Field>(str, f);
        this->data->insert(p);
    }

    Table::Table(const Table &source)
    {
        this->data = source.data;
    }

    Field::Internal::~Internal()
    {
        if (this->data == nullptr)
        {
            return;
        }

        switch (this->type)
        {
        case nil:
            break;
        case boolean:
            delete (bool *)this->data;
            break;
        case number:
            delete (lua_Number *)this->data;
            break;
        case string:
            delete (std::string *)this->data;
            break;
        case function:
            delete (LuableFunction *)this->data;
            break;
        // case userdata:
        //     delete (bool*) this->data;
        //     break;
        // case thread:
        //     delete (bool*) this->data;
        //     break;
        case table:
            delete (Table *)this->data;
            break;
        }
    }

    Field::Field(AllowedDataType type, void *data)
    {
        this->data = std::shared_ptr<Internal>(new Internal());
        this->data->type = type;
        this->data->data = data;
    }

    int compileTable(lua_State *state, Table definition)
    {
        lua_newtable(state);
        int top = lua_gettop(state);

        std::vector<std::map<std::string, Esperatto::Field>::iterator> tableFields;

        for (auto it = definition.data->begin(); it != definition.data->end(); ++it)
        {
            if(it->second.data->type == AllowedDataType::table){
                tableFields.push_back(it);
                continue;
            }

            const char *key = it->first.c_str();
            lua_pushstring(state, key);

            switch (it->second.data->type)
            {
            case nil:
                lua_pushnil(state);
                break;
            case boolean:
                lua_pushboolean(state, *((bool *)it->second.data->data));
                break;
            case number:
                lua_pushnumber(state, *((lua_Number *)it->second.data->data));
                break;
            case string:
                lua_pushstring(state, ((std::string *)it->second.data->data)->c_str());
                break;
            case function:
                // TODO: add support for function members.
                break;
            // case userdata:
            //     delete (bool*) this->data;
            //     break;
            // case thread:
            //     delete (bool*) this->data;
            //     break;
            
            }

            lua_settable(state, top);
        }

        return 1;
    }
}
