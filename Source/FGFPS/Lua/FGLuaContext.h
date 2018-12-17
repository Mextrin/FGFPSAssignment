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

	bool CallFunction_RetValueBool(const char * FunctionName);
	float CallFunction_RetValueNumber(const char * FunctionName, float DefaultValue = 0.0f);

	void SetBool(bool bValue, const char* Name);

	void SetNumber(float Number, const char* Name);
	float GetNumber(const char* Name);

	bool DoesFunctionExist(const char* FunctionName);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")	
	static void ReloadScripts();

	FORCEINLINE struct lua_State* GetLuaState() const { return LuaState; }

private:
	static TArray<UFGLuaContext*> GlobalContexts;

	void Reload();

	bool LuaError(int ErrorCode);

	FString Fullpath;

	struct lua_State* LuaState = nullptr;
};
