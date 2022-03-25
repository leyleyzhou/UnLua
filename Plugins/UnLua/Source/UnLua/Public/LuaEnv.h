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

#pragma once

#include "UnLuaManager.h"
#include "lua.h"
#include "HAL/Platform.h"

namespace UnLua
{
    class UNLUA_API FLuaEnv
        : public FUObjectArray::FUObjectCreateListener,
          public FUObjectArray::FUObjectDeleteListener
    {
    public:
        FLuaEnv();

        virtual ~FLuaEnv();

        // TODO: refactor: move this to constructor
        void Initialize();

        virtual void NotifyUObjectCreated(const UObjectBase* Object, int32 Index) override;

        virtual void NotifyUObjectDeleted(const UObjectBase* Object, int32 Index) override;

        virtual void OnUObjectArrayShutdown() override;

        virtual bool TryBind(UObject* Object);

        virtual bool DoString(const FString& Chunk, const FString& ChunkName = "chunk");

        virtual void GC();

        lua_State* GetMainState() const { return L; }

        UUnLuaManager* GetManager() const { return Manager; }

        void AddBuiltInLoader(const FString Name, lua_CFunction Loader);

    protected:
        lua_State* L;

        static int LoadFromBuiltinLibs(lua_State* L);

        static int LoadFromCustomLoader(lua_State* L);

        static int LoadFromFileSystem(lua_State* L);

        static void* DefaultLuaAllocator(void* ud, void* ptr, size_t osize, size_t nsize);

        virtual lua_Alloc GetLuaAllocator() const;

    private:
        static TMap<lua_State*, FLuaEnv*> AllEnvs;
        TMap<FString, lua_CFunction> BuiltinLoaders;
        TArray<FWeakObjectPtr> Candidates; // binding candidates during async loading
        FCriticalSection CandidatesLock;
        UUnLuaManager* Manager;
        FDelegateHandle OnAsyncLoadingFlushUpdateHandle;

        void AddSearcher(lua_CFunction Searcher, int Index) const;

        void OnAsyncLoadingFlushUpdate();

        void RegisterDelegates();

        void UnRegisterDelegates();
    };
}