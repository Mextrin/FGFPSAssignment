#include "FGLuaContext.h"
#include "lua.hpp"
#include "Paths.h"
#include "Engine/Engine.h"

#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LuaLog);

TArray<UFGLuaContext*> UFGLuaContext::GlobalContexts;

void UFGLuaContext::Reload()
{
	check(LuaState != nullptr);
	int ErrorCode = luaL_loadfile(LuaState, TCHAR_TO_ANSI(*Fullpath));
	LuaError(ErrorCode);
	ErrorCode = lua_pcall(LuaState, 0, 0, 0);
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
	check(!Filename.IsEmpty());
	Close();

	LuaState = luaL_newstate();
	luaL_openlibs(LuaState); // print, math etc

	Fullpath = FPaths::GameSourceDir() + TEXT("LuaSource/") + Filename + TEXT(".lua");
	int ErrorCode = luaL_loadfile(LuaState, TCHAR_TO_ANSI(*Fullpath));
	LuaError(ErrorCode);

	lua_newtable(LuaState);
	lua_pushcfunction(LuaState, LuaDebugPrint);
	lua_setfield(LuaState, -2, "print");
	lua_setglobal(LuaState, "ue4");
	
	ErrorCode = lua_pcall(LuaState, 0, 0, 0);
	LuaError(ErrorCode);

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
	if (!DoesFunctionExist(FunctionName))
	{
		return;
	}

	lua_getglobal(LuaState, FunctionName);
	int ErrorCode = lua_pcall(LuaState, 0, 0, 0);
	LuaError(ErrorCode);
}

void UFGLuaContext::CallFunction(const char * FunctionName, float Param1)
{
	if (!DoesFunctionExist(FunctionName))
	{
		return;
	}

	lua_getglobal(LuaState, FunctionName);
	lua_pushnumber(LuaState, Param1);
	int ErrorCode = lua_pcall(LuaState, 1, 0, 0);
	LuaError(ErrorCode);
}

bool UFGLuaContext::CallFunction_RetValueBool(const char * FunctionName)
{
	if (!DoesFunctionExist(FunctionName))
	{
		return false;
	}

	bool RetValue = false;

	lua_getglobal(LuaState, FunctionName);
	int ErrorCode = lua_pcall(LuaState, 0, 1, 0);
	LuaError(ErrorCode);

	if (lua_isboolean(LuaState, -1))
	{
		RetValue = lua_toboolean(LuaState, -1);
		lua_pop(LuaState, 1);
	}

	return RetValue;
}

float UFGLuaContext::CallFunction_RetValueNumber(const char* FunctionName, float DefaultValue) 
{
	float RetValue = DefaultValue;

	if (!DoesFunctionExist(FunctionName))
	{
		return RetValue;
	}

	lua_getglobal(LuaState, FunctionName);
	int ErrorCode = lua_pcall(LuaState, 0, 1, 0);
	LuaError(ErrorCode);

	if (lua_isnumber(LuaState, -1))
	{
		RetValue = lua_tonumber(LuaState, -1);
		lua_pop(LuaState, 1);
	}

	return RetValue;
}

void UFGLuaContext::SetBool(bool bValue, const char* Name)
{
	check(LuaState != nullptr);

	lua_pushboolean(LuaState, bValue);
	lua_setglobal(LuaState, Name);
}

void UFGLuaContext::SetNumber(float Number, const char* Name)
{
	check(LuaState != nullptr);

	lua_pushnumber(LuaState, Number);
	lua_setglobal(LuaState, Name);
}

float UFGLuaContext::GetNumber(const char* Name)
{
	check(LuaState != nullptr);

	float Value = 0.0f;
	lua_getglobal(LuaState, Name);
	if (lua_isnumber(LuaState, -1))
	{
		Value = lua_tonumber(LuaState, -1);
	}
	else
	{
		const FString ErrorString = TEXT("[LuaContext]: ") + FString(ANSI_TO_TCHAR(Name)) + TEXT(" is not a number.");
		DebugPrint(TCHAR_TO_ANSI(*ErrorString));
	}

	lua_pop(LuaState, 1);

	return Value;
}

bool UFGLuaContext::DoesFunctionExist(const char* FunctionName)
{
	check(LuaState != nullptr); // formality

	bool bExists = false;
	lua_getglobal(LuaState, FunctionName);

	if (lua_isfunction(LuaState, -1))
	{
		bExists = true;
	}
	else
	{
		const FString ErrorString = TEXT("[FGLuaContext] Unable to find function: ") + FString(ANSI_TO_TCHAR(FunctionName)) + TEXT(" in ") + Fullpath;
		DebugPrint(TCHAR_TO_ANSI(*ErrorString));
	}

	lua_pop(LuaState, 1);

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
		const char* ErrorString = lua_tostring(LuaState, -1);
		lua_pop(LuaState, 1);
		DebugPrint(ErrorString);
		return true;
	}

	return false;
}
