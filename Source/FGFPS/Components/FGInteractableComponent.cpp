#include "FGInteractableComponent.h"
#include "FGInteractorComponent.h"

// For now
#include "Kismet/KismetSystemLibrary.h"

bool UFGInteractableComponent::Interact(UFGInteractorComponent* Interactor)
{
	InteractionProgress = FMath::Min(InteractionProgress + Interactor->GetInteractionSpeed() * GetWorld()->GetDeltaSeconds(), InteractionTime);

	//Temp
	UKismetSystemLibrary::PrintString(GetWorld(), FString::SanitizeFloat(GetProgress()), true, true, 
		FLinearColor::Green, GetWorld()->GetDeltaSeconds());

	if (InteractionProgress >= InteractionTime)
	{
		OnInteractionSuccessful.Broadcast(this, Interactor);
		return true;
	}

	return false;
}

void UFGInteractableComponent::StartInteraction()
{
	InteractionProgress = 0.0f;
}

void UFGInteractableComponent::StopInteraction()
{

}

