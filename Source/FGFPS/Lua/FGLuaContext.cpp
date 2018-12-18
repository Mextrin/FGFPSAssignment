#include "FGLuaContext.h"
#include "lua.hpp"
#include "Paths.h"
#include "Engine/Engine.h"

#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LuaLog);

TArray<UFGLuaContext*> UFGLuaContext::GlobalContexts;

void UFGLuaContext::Reload()
{
	int ErrorCode = luaL_loadfile(GetLuaState(), TCHAR_TO_ANSI(*Fullpath));
	LuaError(ErrorCode);
	ErrorCode = lua_pcall(GetLuaState(), 0, 0, 0);
	LuaError(ErrorCode);

	UKismetSystemLibrary::PrintString(this, TEXT("[LuaContext] Reloading ") + Fullpath );
}

static void DebugPrint(const char* DebugString)
{
	const TCHAR* DebugStringToPrint = ANSI_TO_TCHAR(DebugString);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green, DebugStringToPrint);
	}

	UE_LOG(LuaLog, Log, TEXT("%s"), DebugStringToPrint);
}

static int LuaDebugPrint(lua_State* LuaState)
{
	const char* DebugString = lua_tostring(LuaState, 1);
	lua_pop(LuaState, 1);
	DebugPrint(DebugString);
	return 0;
}

void UFGLuaContext::LoadFile(const FString& Filename)
{
	check(!Filename.IsEmpty() && "No filename was specified when attempting to load a file.");
	Close();

	LuaState = luaL_newstate();
	luaL_openlibs(GetLuaState()); // print, math etc

	Fullpath = FPaths::GameSourceDir() + TEXT("LuaSource/") + Filename + TEXT(".lua");
	int ErrorCode = luaL_loadfile(GetLuaState(), TCHAR_TO_ANSI(*Fullpath));
	LuaError(ErrorCode);
	ErrorCode = lua_pcall(GetLuaState(), 0, 0, 0);
	LuaError(ErrorCode);

	lua_newtable(GetLuaState());
	lua_pushcfunction(GetLuaState(), LuaDebugPrint);
	lua_setfield(GetLuaState(), -2, "print");
	lua_setglobal(GetLuaState(), "ue4");

	GlobalContexts.Add(this);
}

void UFGLuaContext::Close()
{
	if (LuaState != nullptr)
	{
		lua_close(LuaState);
		LuaState = nullptr;
		GlobalContexts.Remove(this);
	}
}

void UFGLuaContext::CallFunction(const char* FunctionName)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	int ErrorCode = lua_pcall(GetLuaState(), 0, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, float Param1)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushnumber(GetLuaState(), Param1);
	int ErrorCode = lua_pcall(GetLuaState(), 1, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, const char* Param1)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushlstring(GetLuaState(), Param1, strlen(Param1));
	int ErrorCode = lua_pcall(GetLuaState(), 1, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, const char* Param1, float Param2)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushlstring(GetLuaState(), Param1, strlen(Param1));
	lua_pushnumber(GetLuaState(), Param2);
	int ErrorCode = lua_pcall(GetLuaState(), 2, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, const char* Param1, float Param2, float Param3)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushlstring(GetLuaState(), Param1, strlen(Param1));
	lua_pushnumber(GetLuaState(), Param2);
	lua_pushnumber(GetLuaState(), Param3);
	int ErrorCode = lua_pcall(GetLuaState(), 3, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, const char* Param1, const char* Param2, float Param3)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushlstring(GetLuaState(), Param1, strlen(Param1));
	lua_pushlstring(GetLuaState(), Param2, strlen(Param2));
	lua_pushnumber(GetLuaState(), Param3);
	int ErrorCode = lua_pcall(GetLuaState(), 3, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, const char* Param1, const char* Param2, float Param3, float Param4)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushlstring(GetLuaState(), Param1, strlen(Param1));
	lua_pushlstring(GetLuaState(), Param2, strlen(Param2));
	lua_pushnumber(GetLuaState(), Param3);
	lua_pushnumber(GetLuaState(), Param4);
	int ErrorCode = lua_pcall(GetLuaState(), 4, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, float Param1, float Param2)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	lua_pushnumber(GetLuaState(), Param1);
	lua_pushnumber(GetLuaState(), Param2);
	int ErrorCode = lua_pcall(GetLuaState(), 2, 0, 0);
	LuaError(ErrorCode);
}

bool UFGLuaContext::CallFunction_RetValueBool(const char * FunctionName)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return false;
	}

	bool RetValue = false;

	lua_getglobal(GetLuaState(), FunctionName);
	int ErrorCode = lua_pcall(GetLuaState(), 0, 1, 0);
	LuaError(ErrorCode);

	if (lua_isboolean(GetLuaState(), -1))
	{
		RetValue = lua_toboolean(GetLuaState(), -1);
		lua_pop(GetLuaState(), 1);
	}

	return RetValue;
}

float UFGLuaContext::CallFunction_RetValueNumber(const char* FunctionName, float DefaultValue) 
{
	float RetValue = DefaultValue;

	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return RetValue;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	int ErrorCode = lua_pcall(GetLuaState(), 0, 1, 0);
	LuaError(ErrorCode);

	if (lua_isnumber(GetLuaState(), -1))
	{
		RetValue = lua_tonumber(GetLuaState(), -1);
		lua_pop(GetLuaState(), 1);
	}

	return RetValue;
}

const char* UFGLuaContext::CallFunction_RetValueString(const char* FunctionName)
{
	if (!DoesFunctionExist_Internal(FunctionName))
	{
		return nullptr;
	}

	lua_getglobal(GetLuaState(), FunctionName);
	int ErrorCode = lua_pcall(GetLuaState(), 0, 1, 0);
	LuaError(ErrorCode);

	if (lua_isstring(LuaState, -1))
	{
		const char* RetValue = lua_tostring(GetLuaState(), -1);
		lua_pop(GetLuaState(), 1);
		return RetValue;
	}

	return nullptr;
}

void UFGLuaContext::SetBool(bool bValue, const char* Name)
{
	check(LuaState != nullptr);

	lua_pushboolean(GetLuaState(), bValue);
	lua_setglobal(GetLuaState(), Name);
}

bool UFGLuaContext::GetBool(const char* Name, bool bDefaultValue)
{
	bool bValue = bDefaultValue;
	lua_getglobal(GetLuaState(), Name);
	if (lua_isboolean(GetLuaState(), -1))
	{
		bValue = lua_toboolean(GetLuaState(), -1);
	}
	else
	{
		const FString ErrorString = TEXT("[LuaContext]: ") + FString(ANSI_TO_TCHAR(Name)) + TEXT(" is not a bool.");
		DebugPrint(TCHAR_TO_ANSI(*ErrorString));
	}

	lua_pop(LuaState, 1);

	return bValue;
}

void UFGLuaContext::SetNumber(float Number, const char* Name)
{

	lua_pushnumber(GetLuaState(), Number);
	lua_setglobal(LuaState, Name);
}

float UFGLuaContext::GetNumber(const char* Name, float DefaultValue)
{
	float Value = DefaultValue;
	lua_getglobal(GetLuaState(), Name);
	if (lua_isnumber(GetLuaState(), -1))
	{
		Value = lua_tonumber(GetLuaState(), -1);
	}
	else
	{
		const FString ErrorString = TEXT("[LuaContext]: ") + FString(ANSI_TO_TCHAR(Name)) + TEXT(" is not a number.");
		DebugPrint(TCHAR_TO_ANSI(*ErrorString));
	}

	lua_pop(LuaState, 1);

	return Value;
}

bool UFGLuaContext::DoesFunctionExist(const char* FunctionName) const
{
	bool bExists = false;
	lua_getglobal(GetLuaState(), FunctionName);

	if (lua_isfunction(GetLuaState(), -1))
	{
		bExists = true;
	}

	lua_pop(GetLuaState(), 1);

	return bExists;
}

bool UFGLuaContext::DoesFunctionExist_Internal(const char* FunctionName) const
{
	bool bExists = false;
	lua_getglobal(GetLuaState(), FunctionName);

	if (lua_isfunction(GetLuaState(), -1))
	{
		bExists = true;
	}
	else
	{
		const FString ErrorString = TEXT("[FGLuaContext] Unable to find function: ") + FString(ANSI_TO_TCHAR(FunctionName)) + TEXT(" in ") + Fullpath;
		DebugPrint(TCHAR_TO_ANSI(*ErrorString));
	}

	lua_pop(GetLuaState(), 1);

	return bExists;
}

void UFGLuaContext::ReloadScripts()
{
	for (UFGLuaContext* LuaContext : GlobalContexts)
	{
		LuaContext->Reload();
	}
}

bool UFGLuaContext::LuaError(int ErrorCode)
{
	if (ErrorCode != 0)
	{
		const char* ErrorString = lua_tostring(GetLuaState(), -1);
		lua_pop(GetLuaState(), 1);
		DebugPrint(ErrorString);
		return true;
	}

	return false;
}
