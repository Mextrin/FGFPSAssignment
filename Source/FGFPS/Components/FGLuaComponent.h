#pragma once

#include "Components/ActorComponent.h"
#include "FGLuaComponent.generated.h"

UCLASS(BlueprintType, meta = (BlueprintSpawnableComponent))
class FGFPS_API UFGLuaComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

	virtual void DestroyComponent(bool bPromoteChildren) override;

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void LoadFile();

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction(const FString& FunctionName);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction_OneParamNumber(const FString& FunctionName, float Param1);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction_OneParamString(const FString& FunctionName, const FString& Param1);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction_TwoParamsNumber(const FString& FunctionName, float Param1, float Param2);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction_TwoParamsStringNumber(const FString& FunctionName, const FString& Param1, float Param2);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction_ThreeParamsStringNumber(const FString& FunctionName, const FString& Param1, const FString& Param2, float Param3);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void CallFunction_FourParamsStringNumber(const FString& FunctionName, const FString& Param1, const FString& Param2, float Param3, float Param4);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	bool CallFunction_RetValueBool(const FString& FunctionName);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	float CallFunction_RetValueNumber(const FString& FunctionName, float DefaultValue = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	FString CallFunction_RetValueString(const FString& FunctionName, const FString& DefaultValue = TEXT("None"));

	UFUNCTION(BlueprintPure, Category = "FG|Lua")
	float GetNumber(const FString& Name, float DefaultValue = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void SetNumber(float Number, const FString& Name);

	UFUNCTION(BlueprintPure, Category = "FG|Lua")
	float GetBool(const FString& Name, bool bDefaultValue = false) const;

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void SetBool(bool bValue, const FString& Name);

	struct lua_State* GetLuaState() const;

	FORCEINLINE class UFGLuaContext* GetLuaContext() const { check(LuaContext != nullptr); return LuaContext; }

private:
	void CloseLuaContext();

	UPROPERTY(EditAnywhere, Category = Lua)
	FString Filename;

	UPROPERTY(Transient)
	class UFGLuaContext* LuaContext = nullptr;

	UPROPERTY(EditAnywhere, Category = Lua)
	bool bLoadFileOnBeginPlay = true;
};
