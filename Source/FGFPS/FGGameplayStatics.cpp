#include "FGGameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Interfaces/FGLuaComponentInterface.h"
#include "Components/FGLuaComponent.h"
#include "GameFramework/Actor.h"

UFGLuaComponent* UFGGameplayStatics::GetLuaComponentFromActor(UObject* WorldContextObject, class AActor* TargetActor, bool bUseFindComponent /*= true*/)
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

