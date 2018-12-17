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
	bool CallFunction_RetValueBool(const FString& FunctionName);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	float CallFunction_RetValueNumber(const FString& FunctionName, float DefaultValue = 0.0f);

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	float GetNumber(const FString& Name) const;

	UFUNCTION(BlueprintCallable, Category = "FG|Lua")
	void SetNumber(float Number, const FString& Name);

	UFUNCTION(BlueprintPure, Category = "FG|Lua", meta = (WorldContext = "WorldContextObject"))
	static UFGLuaComponent* GetLuaComponentFromActor(UObject* WorldContextObject, class AActor* TargetActor, bool bUseFindComponent = true);

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
