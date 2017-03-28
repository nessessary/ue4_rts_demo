// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TopDown.h"
#include "TopDownCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "MyActor_Spawner.h"

ATopDownCharacter::ATopDownCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->RelativeRotation = FRotator(-60.f, 0.f, 0.f);
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

										  // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}
	CursorToWorld->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	// ´´½¨Ñ¡Ôñ±ß¿ò
	SelectedBound = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectedBound"));
	SelectedBound->SetupAttachment(RootComponent);
	SelectedBound->SetDecalMaterial(DecalMaterialAsset.Object);
	SelectedBound->DecalSize = FVector(16.0f, 32.0f, 32.0f);
	SelectedBound->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	UE_LOG(LogTemp, Warning, TEXT("ATopDownCharacter::ATopDownCharacter"));
	//FVector newLocation = FVector(-180.000000, -350.000000, 171.000000);
	//AMyActor_Spawner* spawner = GWorld->SpawnActor<AMyActor_Spawner>(AMyActor_Spawner::StaticClass(), newLocation, FRotator::ZeroRotator);

	auto Con = GetController();
	// create Aicontroller
	if (GetWorld()) {
		PlayAI = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass(), GetActorLocation(), GetActorRotation());
		//PlayAI->Possess(this);
		//this->PossessedBy(PlayAI);
	}
}

void ATopDownCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	if (CursorToWorld != nullptr)
	{
		if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
		{
			if (UWorld* World = GetWorld())
			{
				FHitResult HitResult;
				FCollisionQueryParams Params;
				FVector StartLocation = TopDownCameraComponent->GetComponentLocation();
				FVector EndLocation = TopDownCameraComponent->GetComponentRotation().Vector() * 2000.0f;
				Params.AddIgnoredActor(this);
				World->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, Params);
				FQuat SurfaceRotation = HitResult.ImpactNormal.ToOrientationRotator().Quaternion();
				CursorToWorld->SetWorldLocationAndRotation(HitResult.Location, SurfaceRotation);
			}
		}
		else if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}

void ATopDownCharacter::Select(bool is_selected)
{
	if (is_selected) {
		FVector pos = GetActorLocation();
		float CollisionRadius = 0;
		float CollisionHalfHeight = 0;
		this->GetComponentsBoundingCylinder(CollisionRadius, CollisionHalfHeight);
		pos.Z -= CollisionHalfHeight;
		SelectedBound->SetHiddenInGame(false);
		FRotator CursorR = pos.Rotation();
		SelectedBound->SetWorldLocation(pos);
		//SelectedBound->SetWorldRotation(CursorR);
		is_mouse_selected = true;
	}
	else
	{
		SelectedBound->SetHiddenInGame(true);
		is_mouse_selected = false;
	}
}

bool ATopDownCharacter::IsMouseSelected() const
{
	return is_mouse_selected;
}


