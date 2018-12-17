#include "FGInteractorComponent.h"
#include "FGInteractableComponent.h"
#include "UI/FGInteractionWidget.h"
#include "Kismet/KismetSystemLibrary.h"

UFGInteractorComponent::UFGInteractorComponent(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UFGInteractorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector StartLocation = FVector::ZeroVector;
	FVector ForwardDirection = FVector::ForwardVector;
	GetLookDirection(StartLocation, ForwardDirection);

	TArray<AActor*> IgnoreActors{ GetOwner() };
	FHitResult Hit;

	UFGInteractableComponent* OldInteractable = CurrentInteractable;
	UFGInteractableComponent* NewInteractable = nullptr;

	const FVector EndLocation = StartLocation + (ForwardDirection * InteractionLength);

	if (UKismetSystemLibrary::LineTraceSingleByProfile(GetWorld(), StartLocation, EndLocation, TEXT("Interaction"), false, IgnoreActors, 
		EDrawDebugTrace::ForDuration, Hit, true, FLinearColor::Blue, FLinearColor::Black, DeltaTime))
	{
		if (UFGInteractableComponent* Interactable = Cast<UFGInteractableComponent>(Hit.Component))
		{
			CurrentInteractable = Interactable;
			NewInteractable = Interactable;
		}
	}

	if (OldInteractable && (OldInteractable != NewInteractable || !NewInteractable))
	{
		bIsInteracting = false;
		if (InteractionWidget)
		{
			InteractionWidget->RemoveFromParent();
			InteractionWidget = nullptr;
		}
	}

	if (!OldInteractable && NewInteractable && NewInteractable->GetInteractionWidget())
	{
		InteractionWidget = CreateWidget<UFGInteractionWidget>(GetWorld(), NewInteractable->GetInteractionWidget());
		InteractionWidget->AddToViewport();
	}

	if (bIsInteracting && CurrentInteractable)
	{
		if (CurrentInteractable->Interact(this))
		{
			bIsInteracting = false;
			if (InteractionWidget)
			{
				InteractionWidget->OnSetIsInteracting(false);
			}
		}

		if (InteractionWidget)
		{
			InteractionWidget->OnSetProgress(CurrentInteractable->GetProgress());
		}
	}

	CurrentInteractable = NewInteractable;
}

void UFGInteractorComponent::StartInteracting()
{
	if (!bIsInteracting && CurrentInteractable)
	{
		bIsInteracting = true;
		CurrentInteractable->StartInteraction();
	}

	if (InteractionWidget)
	{
		InteractionWidget->OnSetIsInteracting(true);
	}
}

void UFGInteractorComponent::StopInteracting()
{
	bIsInteracting = false;

	if (CurrentInteractable)
	{
		CurrentInteractable->StopInteraction();
	}

	if (InteractionWidget)
	{
		InteractionWidget->OnSetIsInteracting(false);
	}
}

void UFGInteractorComponent::GetLookDirection(FVector& StartLocation, FVector& ForwardDirection) const
{
	StartLocation = FVector::ZeroVector;
	ForwardDirection = FVector::ForwardVector;
	check(false && "Abstract method not implemented");
}

