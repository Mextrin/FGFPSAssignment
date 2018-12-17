#include "FGLuaComponent.h"
#include "Lua/FGLuaContext.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Actor.h"
#include "Interfaces/FGLuaComponentInterface.h"

void UFGLuaComponent::BeginPlay()
{
	Super::BeginPlay();
	LuaContext = NewObject<UFGLuaContext>(this, TEXT("LuaContext"));
	if (bLoadFileOnBeginPlay)
	{
		LoadFile();
	}
}

void UFGLuaComponent::BeginDestroy()
{
	CloseLuaContext();

	Super::BeginDestroy();
}

void UFGLuaComponent::DestroyComponent(bool bPromoteChildren)
{
	CloseLuaContext();

	Super::DestroyComponent(bPromoteChildren);
}

void UFGLuaComponent::LoadFile()
{
	CloseLuaContext();
	LuaContext->LoadFile(Filename);
}

void UFGLuaComponent::CallFunction(const FString& FunctionName)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName));
}

void UFGLuaComponent::CallFunction_OneParamNumber(const FString& FunctionName, float Param1)
{
	check(LuaContext != nullptr);
	LuaContext->CallFunction(TCHAR_TO_ANSI(*FunctionName), Param1);
}

bool UFGLuaComponent::CallFunction_RetValueBool(const FString& FunctionName)
{
	check(LuaContext != nullptr);
	return LuaContext->CallFunction_RetValueBool(TCHAR_TO_ANSI(*FunctionName));
}

float UFGLuaComponent::CallFunction_RetValueNumber(const FString& FunctionName, float DefaultValue /*= 0.0f*/)
{
	check(LuaContext != nullptr);
	return LuaContext->CallFunction_RetValueNumber(TCHAR_TO_ANSI(*FunctionName), DefaultValue);
}

float UFGLuaComponent::GetNumber(const FString& Name) const
{
	check(LuaContext != nullptr);
	return LuaContext->GetNumber(TCHAR_TO_ANSI(*Name));
}

void UFGLuaComponent::SetNumber(float Number, const FString& Name)
{
	check(LuaContext != nullptr);
	LuaContext->SetNumber(Number, TCHAR_TO_ANSI(*Name));
}

UFGLuaComponent* UFGLuaComponent::GetLuaComponentFromActor(UObject* WorldContextObject, class AActor* TargetActor, bool bUseFindComponent /*= true*/)
{
	if (!TargetActor)
	{
		UKismetSystemLibrary::PrintString(WorldContextObject, TEXT("[GetLuaComponentFromActor] TargetActor is not valid."));
		return nullptr;
	}

	if (TargetActor->GetClass()->ImplementsInterface(UFGLuaComponentInterface::StaticClass()))
	{
		return IFGLuaComponentInterface::Execute_GetLuaComponent(TargetActor);
	}

	if (bUseFindComponent)
	{
		return TargetActor->FindComponentByClass<UFGLuaComponent>();
	}

	return nullptr;
}

struct lua_State* UFGLuaComponent::GetLuaState() const
{
	check(LuaContext != nullptr);
	return LuaContext->GetLuaState();
}

void UFGLuaComponent::CloseLuaContext()
{
	if (LuaContext)
	{
		LuaContext->Close();
	}
}
