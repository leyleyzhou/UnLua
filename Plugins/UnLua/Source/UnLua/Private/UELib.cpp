﻿// Tencent is pleased to support the open source community by making UnLua available.
// 
// Copyright (C) 2019 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the MIT License (the "License"); 
// you may not use this file except in compliance with the License. You may obtain a copy of the License at
//
// http://opensource.org/licenses/MIT
//
// Unless required by applicable law or agreed to in writing, 
// software distributed under the License is distributed on an "AS IS" BASIS, 
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
// See the License for the specific language governing permissions and limitations under the License.

#include "lauxlib.h"
#include "UELib.h"

#include "LuaCore.h"

const char* REGISTRY_KEY = "UnLua_UELib";

static void LoadUEType(const char* InName)
{
    FString Name = UTF8_TO_TCHAR(InName);

    // find candidates in memory
    UField* Ret = FindObject<UClass>(ANY_PACKAGE, *Name);
    if (!Ret)
        Ret = FindObject<UScriptStruct>(ANY_PACKAGE, *Name);
    if (!Ret)
        Ret = FindObject<UEnum>(ANY_PACKAGE, *Name);

    // load candidates if not found
    if (!Ret)
        Ret = LoadObject<UClass>(nullptr, *Name);
    if (!Ret)
        Ret = LoadObject<UScriptStruct>(nullptr, *Name);
    if (!Ret)
        Ret = LoadObject<UEnum>(nullptr, *Name);
}

static int Index(lua_State* L)
{
    const int32 Type = lua_type(L, 2);
    if (Type != LUA_TSTRING)
        return 0;

    const char* Name = lua_tostring(L, 2);
    const char Prefix = Name[0];
    // LoadUEType(Name + 1);

    if (Prefix == 'U' || Prefix == 'A' || Prefix == 'F')
    {
        RegisterClass(L, Name);
    }
    else if (Prefix == 'E')
    {
        RegisterEnum(L, Name);
    }
    lua_rawget(L, 1);
    return 1;
}

int UnLua::UELib::Open(lua_State* L)
{
    lua_newtable(L);
    lua_pushstring(L, "__index");
    lua_pushcfunction(L, Index);
    lua_rawset(L, -3);

    lua_pushvalue(L, -1);
    lua_setmetatable(L, -2);

    lua_pushvalue(L, -1);
    lua_pushstring(L, REGISTRY_KEY);
    lua_rawset(L, LUA_REGISTRYINDEX);

    lua_pushvalue(L, -1);
    lua_setglobal(L, "UE");
    
    lua_pushboolean(L, true);
    lua_setglobal(L, "WITH_UE4_NAMESPACE");

    return 1;
}