#include "FGCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Engine/World.h"
#include "Weapon/FGWeapon.h"
#include "Weapon/FGWeaponSchematic.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/FGPlayerInteractorComponent.h"
#include "Components/FGCharacterMovementComponent.h"
#include "Components/FGLuaComponent.h"
#include "Lua/FGLuaContext.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AFGCharacter::AFGCharacter(const FObjectInitializer& ObjectInitializer) : 
	Super( ObjectInitializer.SetDefaultSubobjectClass<UFGCharacterMovementComponent>(ACharacter::CharacterMovementComponentName) )
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	LuaComponent = CreateDefaultSubobject<UFGLuaComponent>(TEXT("LuaComponent"));

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	Mesh1P->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
	
	InteractorComponent = ObjectInitializer.CreateDefaultSubobject<UFGPlayerInteractorComponent>(this, TEXT("Interactor"));
}

void AFGCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultTransform = Mesh1P->GetRelativeTransform();
}

void AFGCharacter::BeginDestroy()
{
	Super::BeginDestroy();
}

void AFGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float ADSSpeed = GetLuaContext()->CallFunction_RetValueNumber("GetADSSpeed");

	if (bDoADS)
	{
		Mesh1P->SetRelativeTransform( UKismetMathLibrary::TInterpTo(Mesh1P->GetRelativeTransform(), TargetTransform, DeltaSeconds, ADSSpeed) );
	}
	else
	{
		Mesh1P->SetRelativeTransform(UKismetMathLibrary::TInterpTo(Mesh1P->GetRelativeTransform(), DefaultTransform, DeltaSeconds, ADSSpeed));
	}

	GetLuaContext()->SetBool(IsCharacterMoving(), "IsMoving");
	LuaComponent->CallFunction_OneParamNumber(TEXT("Tick"), DeltaSeconds);
}

bool AFGCharacter::GetShootDirection_Implementation(FVector& StartLocation, FVector& ForwardDirection) const
{
	StartLocation = FirstPersonCameraComponent->GetComponentLocation();
	ForwardDirection = FirstPersonCameraComponent->GetForwardVector();

	return true;
}

bool AFGCharacter::GetSpreadMultiplier_Implementation(float& SpreadMultiplier) const
{
	SpreadMultiplier = GetLuaContext()->CallFunction_RetValueNumber("GetSpreadMultiplier", 1.0f);
	return true;
}

void AFGCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AFGCharacter::ADSPressed);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AFGCharacter::ADSReleased);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFGCharacter::SprintPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFGCharacter::SprintReleased);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFGCharacter::InteractPressed);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &AFGCharacter::InteractReleased);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFGCharacter::FirePressed);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFGCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFGCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFGCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFGCharacter::LookUpAtRate);
}

void AFGCharacter::EquipNewWeapon(UFGWeaponSchematic* NewWeaponSchematic)
{
	if(!NewWeaponSchematic || !NewWeaponSchematic->WeaponClass)
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

UFGLuaContext* AFGCharacter::GetLuaContext() const
{
	return GetLuaComponent()->GetLuaContext();
}

bool AFGCharacter::IsCharacterMoving() const
{
	return GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->Velocity.Size2D() > KINDA_SMALL_NUMBER;
}

void AFGCharacter::HandleWeaponFire()
{
	if (!WeaponSchematic)
	{
		return;
	}

	if (WeaponSchematic->ShootMontage)
	{
		if (UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance())
		{
			AnimInstance->Montage_Play(WeaponSchematic->ShootMontage, 1.0f);
		}
	}

	if (WeaponSchematic->ShootSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, WeaponSchematic->ShootSound, GetActorLocation());
	}
}

void AFGCharacter::SetWeaponADS()
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

void AFGCharacter::ADSPressed()
{
	bDoADS = true;
	GetLuaContext()->SetBool(true, "IsADS");
}

void AFGCharacter::ADSReleased()
{
	bDoADS = false;
	GetLuaContext()->SetBool(false, "IsADS");
}

void AFGCharacter::SprintPressed()
{
	LuaComponent->CallFunction(TEXT("StartSprint"));
}

void AFGCharacter::SprintReleased()
{
	LuaComponent->CallFunction(TEXT("StopSprint"));
}

void AFGCharacter::InteractPressed()
{
	check(InteractorComponent != nullptr);
	InteractorComponent->StartInteracting();
}

void AFGCharacter::InteractReleased()
{
	check(InteractorComponent != nullptr);
	InteractorComponent->StopInteracting();
}

void AFGCharacter::FirePressed()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Fire();
	}
}

void AFGCharacter::MoveForward(float Value)
{	
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFGCharacter::MoveRight(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFGCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFGCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
