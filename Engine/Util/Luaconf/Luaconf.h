#pragma once
#include <string>
#include <fstream>
#include <iostream>

#include <lua/lua.hpp>

#include "LValue.h"

namespace z {
namespace luaconf {

namespace detail {

bool ParseValue(lua_State *L, int index, Value &value);

inline bool ParseTable(lua_State *L, Value &table) {
	lua_pushnil(L); // nil | table
	while (lua_next(L, -2)) {
		// value | key | table
		Value key, value;
		if (ParseValue(L, -2, key)) {
			if (key.Type() == ValueType::TYPE_STRING) {
				// table -> object
				if (table.Type() == ValueType::TYPE_NONE)
					table = object_t{};
				if (table.Type() != ValueType::TYPE_OBJECT)
					return false;
			} else if (key.Type() == ValueType::TYPE_INT) {
				// table -> array
				if (table.Type() == ValueType::TYPE_NONE)
					table = array_t{};
				if (table.Type() != ValueType::TYPE_ARRAY)
					return false;
			} else {
				return false;
			}
		}
		
		if (!ParseValue(L, -1, value)) {
			return false;
		}

		if (table.Type() == ValueType::TYPE_OBJECT) {
			// table[key.] = value;
			std::string key_str;
			key.GetValSafety(key_str);
			table[key_str] = value;
		} else if (table.Type() == ValueType::TYPE_ARRAY) {
			int key_int;
			key.GetValSafety(key_int);
			table[key_int - 1] = value;
		} else {
			return false; 
		}
		lua_pop(L, 1);
	}
	return true;
}

inline bool ParseValue(lua_State *L, int index, Value &value) {
	int ltype = lua_type(L, index);
	switch (ltype) {
	case LUA_TTABLE:
		return ParseTable(L, value);
	case LUA_TBOOLEAN:
		value = lua_toboolean(L, index) ? true : false;
		break;
	case LUA_TNUMBER:
		{
			double v = lua_tonumber(L, index);
			if (std::abs(int(v) - v) < 1e-6) {
				value = (int)v;
			} else {
				value = v;
			}
		}
		break;
	case LUA_TSTRING:
		value = std::string(lua_tostring(L, index));
		break;
	case LUA_TNIL:
		value = null_t{};
		break;
	default:
		// not supported
		return false;
	}
	return true;
}
}


inline bool Parse(std::string const& code, Value& value) {
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	
	if (0 == luaL_dostring(L, code.c_str())) {
		bool ret = detail::ParseValue(L, -1, value);
		lua_close(L);
		return ret;
	}
	const char *err = lua_tostring(L, -1);
	std::cout << std::string(err) << std::endl;
	lua_close(L);
	return false;
}

inline bool ParseFile(std::string const& filepath, Value &value) {
	std::ifstream st(filepath);
	if (!st.is_open()) {
		return false;
	}
	std::string str((std::istreambuf_iterator<char>(st)), std::istreambuf_iterator<char>());
	return Parse(str, value);

}

}
}