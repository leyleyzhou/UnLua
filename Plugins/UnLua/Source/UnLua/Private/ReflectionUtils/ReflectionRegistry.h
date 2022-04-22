// Tencent is pleased to support the open source community by making UnLua available.
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

#include "EnumDesc.h"
#include "ClassDesc.h"
#include "FunctionDesc.h"

#define ENABLE_CALL_OVERRIDDEN_FUNCTION 1           // option to call overridden UFunction

/**
 * Descriptor types
 */
enum EDescType
{   
	DESC_NONE = 0,
	DESC_CLASS = 1,
	DESC_FUNCTION = 2,
	DESC_PROPERTY = 3,
	DESC_FIELD = 4,
	DESC_ENUM = 5,
};

/**
 * Reflection registry
 */
class UNLUA_API FReflectionRegistry
{
public:
    FReflectionRegistry();
    ~FReflectionRegistry();

    void Cleanup();

    // all other place should use this to found desc!
    FEnumDesc* FindEnum(const char* InName);

    bool UnRegisterEnum(const FEnumDesc* EnumDesc);
    FEnumDesc* RegisterEnum(const char* InName);
    FEnumDesc* RegisterEnum(UEnum *InEnum);

    bool UnRegisterFunction(UFunction *InFunction);
#if ENABLE_CALL_OVERRIDDEN_FUNCTION
    bool AddOverriddenFunction(UFunction *NewFunc, UFunction *OverriddenFunc);
    UFunction* RemoveOverriddenFunction(UFunction *NewFunc);
    UFunction* FindOverriddenFunction(UFunction *NewFunc);
#endif

	void RemoveFromDescSet(void* Desc);
	bool IsDescValid(void* Desc, EDescType type);
    bool IsDescValidWithObjectCheck(void* Desc, EDescType type);

private:
    FDelegateHandle PostGarbageCollectHandle;
    void PostGarbageCollect();
    
    TMap<FName, FEnumDesc*> Enums;
    TMap<TWeakObjectPtr<UFunction>, TSharedPtr<FFunctionDesc>> Functions;
};

extern UNLUA_API FReflectionRegistry GReflectionRegistry;
