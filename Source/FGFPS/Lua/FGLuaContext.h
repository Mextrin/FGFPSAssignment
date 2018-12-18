#pragma once

#include "FGLuaContext.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LuaLog, Log, All);

UCLASS()
class FGFPS_API UFGLuaContext : public UObject
{
	GENERATED_BODY()
public:
	void LoadFile(const FString& Filename);
	void Close();

	void CallFunction(const char* FunctionName);
	void CallFunction(const char * FunctionName, float Param1);
	void CallFunction(const char * FunctionName, float Param1, float Param2);
	void CallFunction(const char * FunctionName, const char* Param1);
	void CallFunction(const char * FunctionName, const char* Param1, float Param2);
	void CallFunction(const char * FunctionName, const char* Param1, float Param2, float Param3);
	void CallFunction(const char * FunctionName, const char* Param1, const char* Param2, float Param3);
	void CallFunction(const char * FunctionName, const char* Param1, const char* Param2, float Param3, float Param4);

	bool CallFunction_RetValueBool(const char * FunctionName);
	float CallFunction_RetValueNumber(const char * FunctionName, float DefaultValue = 0.0f);
	const char* CallFunction_RetValueString(const char* FunctionName);

	void SetBool(bool bValue, const char* Name);
	bool GetBool(const char* Name, bool bDefaultValue = false);

	void SetNumber(float Number, const char* Name);
	float GetNumber(const char* Name, float DefaultValue = 0.0f);

	bool DoesFunctionExist(const char* FunctionName) const;

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")	
	static void ReloadScripts();

	FORCEINLINE bool IsLoaded() const { return LuaState != nullptr; };

	FORCEINLINE_DEBUGGABLE struct lua_State* GetLuaState() const { check(LuaState != nullptr && "LuaState has not been initialized!"); return LuaState; }

private:

	bool DoesFunctionExist_Internal(const char* FunctionName) const;
	
	static TArray<UFGLuaContext*> GlobalContexts;

	void Reload();

	bool LuaError(int ErrorCode);

	FString Fullpath;

	struct lua_State* LuaState = nullptr;
};
