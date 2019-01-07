#include "FGPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/FGPlayerInteractorComponent.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/FGPlayerInteractorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Lua/FGLuaContext.h"
#include "Weapon/FGWeapon.h"
#include "Weapon/FGWeaponSchematic.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"

AFGPlayerCharacter::AFGPlayerCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super(ObjectInitializer)
{
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this,TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	InteractorComponent = ObjectInitializer.CreateDefaultSubobject<UFGPlayerInteractorComponent>(this, TEXT("Interactor"));
}

void AFGPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultTransform = Mesh1P->GetRelativeTransform();
}

void AFGPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float ADSSpeed = GetLuaContext()->CallFunction_RetValueNumber("GetADSSpeed");

	if (bDoADS)
	{
		Mesh1P->SetRelativeTransform(UKismetMathLibrary::TInterpTo(Mesh1P->GetRelativeTransform(), TargetTransform, DeltaSeconds, ADSSpeed));
	}
	else
	{
		Mesh1P->SetRelativeTransform(UKismetMathLibrary::TInterpTo(Mesh1P->GetRelativeTransform(), DefaultTransform, DeltaSeconds, ADSSpeed));
	}
}

bool AFGPlayerCharacter::GetShootDirection_Implementation(FVector& StartLocation, FVector& ForwardDirection) const
{
	StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	ForwardDirection = FirstPersonCameraComponent->GetForwardVector();

	return true;
}

bool AFGPlayerCharacter::GetSpreadMultiplier_Implementation(float& SpreadMultiplier) const
{
	SpreadMultiplier = GetLuaContext()->CallFunction_RetValueNumber("GetSpreadMultiplier", 1.0f);
	return true;
}

void AFGPlayerCharacter::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAction("ADS", IE_Pressed, this, &AFGPlayerCharacter::ADSPressed);
	InputComponent->BindAction("ADS", IE_Released, this, &AFGPlayerCharacter::ADSReleased);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &AFGPlayerCharacter::SprintPressed);
	InputComponent->BindAction("Sprint", IE_Released, this, &AFGPlayerCharacter::SprintReleased);

	InputComponent->BindAction("Interact", IE_Pressed, this, &AFGPlayerCharacter::InteractPressed);
	InputComponent->BindAction("Interact", IE_Released, this, &AFGPlayerCharacter::InteractReleased);

	InputComponent->BindAction("Fire", IE_Pressed, this, &AFGPlayerCharacter::FirePressed);

	InputComponent->BindAction("Reload", IE_Pressed, this, &AFGPlayerCharacter::ReloadPressed);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AFGPlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFGPlayerCharacter::MoveRight);

	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AFGPlayerCharacter::EquipNewWeapon(class UFGWeaponSchematic* NewWeaponSchematic, FName SocketName)
{
	if (!NewWeaponSchematic || !NewWeaponSchematic->WeaponClass)
	{
		return;
	}

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = this;
	Params.Owner = this;

	AFGWeapon* NewWeapon = GetWorld()->SpawnActor<AFGWeapon>(NewWeaponSchematic->WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, Params);

	if (NewWeapon)
	{
		CurrentWeapon = NewWeapon;
		CurrentWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), TEXT("GripPoint"));
		CurrentWeapon->OnFire.AddDynamic(this, &AFGCharacter::HandleWeaponFire);
		WeaponSchematic = NewWeaponSchematic;
		SetWeaponADS();
	}
}

void AFGPlayerCharacter::HandleWeaponFire()
{
	Super::HandleWeaponFire();

	if (!WeaponSchematic)
	{
		return;
	}

	if (GetWeaponSchemtic()->ShootMontage)
	{
		if (UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance())
		{
			AnimInstance->Montage_Play(GetWeaponSchemtic()->ShootMontage);
		}
	}
}

void AFGPlayerCharacter::SetWeaponADS()
{
	if (!CurrentWeapon)
	{
		return;
	}

	const FVector CameraDifference = FirstPersonCameraComponent->RelativeLocation - Mesh1P->GetSocketTransform(TEXT("GripPoint"), RTS_Actor).GetLocation();
	const FVector MeshOffset = CameraDifference + Mesh1P->RelativeLocation;
	const FVector TargetRelativeLocation = MeshOffset - CurrentWeapon->GetRootComponent()->GetSocketTransform(TEXT("Sight"), RTS_ParentBoneSpace).GetLocation();
	TargetTransform.SetComponents(Mesh1P->RelativeRotation.Quaternion(), TargetRelativeLocation, FVector::OneVector);
}

void AFGPlayerCharacter::ADSPressed()
{
	bDoADS = true;
	GetLuaContext()->SetBool(true, "IsADS");
}

void AFGPlayerCharacter::ADSReleased()
{
	bDoADS = false;
	GetLuaContext()->SetBool(false, "IsADS");
}

void AFGPlayerCharacter::SprintPressed()
{
	GetLuaContext()->CallFunction("StartSprint");
}

void AFGPlayerCharacter::SprintReleased()
{
	GetLuaContext()->CallFunction("StopSprint");
}

void AFGPlayerCharacter::InteractPressed()
{
	GetInteractorComponent()->StartInteracting();
}

void AFGPlayerCharacter::InteractReleased()
{
	GetInteractorComponent()->StopInteracting();
}

void AFGPlayerCharacter::FirePressed()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void AFGPlayerCharacter::ReloadPressed()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Reload();
	}
}

void AFGPlayerCharacter::MoveForward(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFGPlayerCharacter::MoveRight(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}
