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
	GetLuaContext()->LoadFile(Filename);
}

void UFGLuaComponent::CallFunction(const FString& FunctionName)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName));
}

void UFGLuaComponent::CallFunction_OneParamNumber(const FString& FunctionName, float Param1)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName), Param1);
}

void UFGLuaComponent::CallFunction_OneParamString(const FString& FunctionName, const FString& Param1)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName), TCHAR_TO_ANSI(*Param1));
}

void UFGLuaComponent::CallFunction_TwoParamsNumber(const FString& FunctionName, float Param1, float Param2)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName), Param1, Param2);
}

void UFGLuaComponent::CallFunction_TwoParamsStringNumber(const FString& FunctionName, const FString& Param1, float Param2)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName), TCHAR_TO_ANSI(*Param1), Param2);
}

void UFGLuaComponent::CallFunction_ThreeParamsStringNumber(const FString& FunctionName, const FString& Param1, const FString& Param2, float Param3)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName), TCHAR_TO_ANSI(*Param1), TCHAR_TO_ANSI(*Param2), Param3);
}

void UFGLuaComponent::CallFunction_FourParamsStringNumber(const FString& FunctionName, const FString& Param1, const FString& Param2, float Param3, float Param4)
{
	GetLuaContext()->CallFunction(TCHAR_TO_ANSI(*FunctionName), TCHAR_TO_ANSI(*Param1), TCHAR_TO_ANSI(*Param2), Param3, Param4);
}

bool UFGLuaComponent::CallFunction_RetValueBool(const FString& FunctionName)
{
	return GetLuaContext()->CallFunction_RetValueBool(TCHAR_TO_ANSI(*FunctionName));
}

float UFGLuaComponent::CallFunction_RetValueNumber(const FString& FunctionName, float DefaultValue)
{
	return GetLuaContext()->CallFunction_RetValueNumber(TCHAR_TO_ANSI(*FunctionName), DefaultValue);
}

FString UFGLuaComponent::CallFunction_RetValueString(const FString& FunctionName, const FString& DefaultValue)
{
	if (const char* RetValue = GetLuaContext()->CallFunction_RetValueString(TCHAR_TO_ANSI(*FunctionName)))
	{
		return FString(RetValue);
	}

	return DefaultValue;
}

float UFGLuaComponent::GetNumber(const FString& Name, float DefaultValue) const
{
	return GetLuaContext()->GetNumber(TCHAR_TO_ANSI(*Name), DefaultValue);
}

void UFGLuaComponent::SetNumber(float Number, const FString& Name)
{
	GetLuaContext()->SetNumber(Number, TCHAR_TO_ANSI(*Name));
}

float UFGLuaComponent::GetBool(const FString& Name, bool bDefaultValue /*= false*/) const
{
	return GetLuaContext()->GetBool(TCHAR_TO_ANSI(*Name), bDefaultValue);
}

void UFGLuaComponent::SetBool(bool bValue, const FString& Name)
{
	GetLuaContext()->SetBool(bValue, TCHAR_TO_ANSI(*Name));
}

struct lua_State* UFGLuaComponent::GetLuaState() const
{
	return GetLuaContext()->GetLuaState();
}

void UFGLuaComponent::CloseLuaContext()
{
	if (LuaContext)
	{
		LuaContext->Close();
	}
}
