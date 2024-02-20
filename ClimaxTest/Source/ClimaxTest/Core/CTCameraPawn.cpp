// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCameraPawn.h"

#include "../Components/CTUnitDirectorComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h"
#include "ClimaxTest/Data/CTCameraPawnData.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACTCameraPawn::ACTCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Have our own rootcomponent
	RootComp = CreateDefaultSubobject<USceneComponent>(FName(TEXT("Scene Component")));
	SetRootComponent(RootComp);

	//Spring arm for camera, useful when zooming
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName(TEXT("Spring Arm Component")));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1000.f;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(FName(TEXT("Camera Component")));
	CameraComponent->SetupAttachment(SpringArmComponent);

	UnitDirectorComponent = CreateDefaultSubobject<UCTUnitDirectorComponent>(FName(TEXT("Unit Director")));
}

// Called when the game starts or when spawned
void ACTCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	// Make sure that we have a valid PlayerController.
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultInputContext, 0);
		}
	}

	checkf(CameraPawnData, TEXT("Camera Pawn Data is null. Consider assigning one in the editor"));

	CameraTargetLocation = GetActorLocation();
	TargetZoom = CameraPawnData->StartingZoom;
}

// Called to bind functionality to input
void ACTCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveCameraAction, ETriggerEvent::Triggered, this, &ACTCameraPawn::MoveCamera);
		EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &ACTCameraPawn::ZoomCamera);
		EnhancedInputComponent->BindAction(MoveUnitAction, ETriggerEvent::Completed, UnitDirectorComponent, &UCTUnitDirectorComponent::MoveUnits);
		EnhancedInputComponent->BindAction(SelectUnitAction, ETriggerEvent::Completed, UnitDirectorComponent, &UCTUnitDirectorComponent::SelectUnit);
		EnhancedInputComponent->BindAction(MarqueeSelectUnitAction, ETriggerEvent::Started, UnitDirectorComponent, &UCTUnitDirectorComponent::MarqueeSelectUnits);
		EnhancedInputComponent->BindAction(MarqueeSelectUnitAction, ETriggerEvent::Completed, UnitDirectorComponent, &UCTUnitDirectorComponent::StopMarqueeSelectUnits);
	}
}

void ACTCameraPawn::EdgeScroll()
{
	//Calculation to get mouse position into a 0 to -1 range.
	FVector2D MousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	//Convert mouse position from screen space to world space
	const FVector2D ViewportSize = UWidgetLayoutLibrary::GetViewportSize(GetWorld());
	MousePosition = MousePosition * UWidgetLayoutLibrary::GetViewportScale(GetWorld());

	//Put it in Range
	MousePosition = FVector2D(MousePosition.X / ViewportSize.X, MousePosition.Y / ViewportSize.Y);

	FVector2D MoveAxis = FVector2D::Zero();

	//Move Camera Right/Left
	if (MousePosition.X > CameraPawnData->ScrollEdgeDistance && MousePosition.X <= 1.f)
	{
		MoveAxis.X = FMath::GetMappedRangeValueClamped(FVector2D(1.f - CameraPawnData->ScrollEdgeDistance, 1.f), FVector2D(0.f, 1.f), MousePosition.X);
	}
	else if (MousePosition.X < CameraPawnData->ScrollEdgeDistance && MousePosition.X > 0.f)
	{
		MoveAxis.X = -(FMath::GetMappedRangeValueClamped(FVector2D(0.f, CameraPawnData->ScrollEdgeDistance), FVector2D(1.f, 0.f), MousePosition.X));
	}

	//Move Camera Up/Down
	if (MousePosition.Y > CameraPawnData->ScrollEdgeDistance && MousePosition.Y <= 1.f)
	{
		MoveAxis.Y = -(FMath::GetMappedRangeValueClamped(FVector2D(1.f - CameraPawnData->ScrollEdgeDistance, 1.f), FVector2D(0.f, 1.f), MousePosition.Y));
	}
	else if (MousePosition.Y < CameraPawnData->ScrollEdgeDistance && MousePosition.Y > 0.f)
	{
		MoveAxis.Y = FMath::GetMappedRangeValueClamped(FVector2D(0.f, CameraPawnData->ScrollEdgeDistance), FVector2D(1.f, 0.f), MousePosition.Y);
	}

	
//	MoveAxis.Y = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(-1.f, 1.f), MousePosition.Y);

	MoveCamera(FInputActionValue(MoveAxis));
}

void ACTCameraPawn::MoveCamera(const FInputActionValue& Value)
{
	const FVector2D VectorValue = Value.Get<FVector2D>();
	if (VectorValue.IsNearlyZero(0.005f))
	{
		return;
	}
		
	UE_LOG(LogTemp, Log, TEXT("Moving: %s"), *Value.ToString());
	CameraTargetLocation = (RootComponent->GetForwardVector() * VectorValue.Y * CameraPawnData->MoveSpeed)+ (RootComponent->GetRightVector() * VectorValue.X * CameraPawnData->MoveSpeed) + CameraTargetLocation;
}

void ACTCameraPawn::ZoomCamera(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Zoom: %s"), *Value.ToString());
	const float Zoom = Value.Get<float>() * CameraPawnData->ZoomSpeed;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, CameraPawnData->MinZoom, CameraPawnData->MaxZoom);
}

// Called every frame
void ACTCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EdgeScroll();

	//Lerp camera movement to target location smoothly
	if (!(GetActorLocation().Equals(CameraTargetLocation, 1.f)))
	{
		const FVector InterpedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), CameraTargetLocation, DeltaTime, CameraPawnData->MoveSpeed);
		SetActorLocation(InterpedLocation);
	}

	if (!(FMath::IsNearlyEqual(SpringArmComponent->TargetArmLength, TargetZoom)))
	{
		const float InterpedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, TargetZoom, DeltaTime, CameraPawnData->ZoomSpeed);
		SpringArmComponent->TargetArmLength = InterpedZoom;
	}
	
}

