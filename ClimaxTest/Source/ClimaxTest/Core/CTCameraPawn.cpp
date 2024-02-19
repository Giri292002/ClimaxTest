// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCameraPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
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

	MoveSpeed = 20.f;
	ZoomSpeed = 100.f;
	MinZoom = 500.f;
	MaxZoom = 1000.f;
	TargetZoom = 1000.f;
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

	CameraTargetLocation = GetActorLocation();
}

// Called to bind functionality to input
void ACTCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACTCameraPawn::Move);
		EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &ACTCameraPawn::Zoom);
	}
}

void ACTCameraPawn::Move(const FInputActionValue& Value)
{
	const FVector2D VectorValue = Value.Get<FVector2D>();
	if (VectorValue.IsNearlyZero())
	{
		return;
	}
		
	UE_LOG(LogTemp, Log, TEXT("Moving: %s"), *Value.ToString());
	CameraTargetLocation = (RootComponent->GetForwardVector() * VectorValue.Y * MoveSpeed)+ (RootComponent->GetRightVector() * VectorValue.X * MoveSpeed) + CameraTargetLocation;
}

void ACTCameraPawn::Zoom(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Log, TEXT("Zoom: %s"), *Value.ToString());
	const float Zoom = Value.Get<float>() * ZoomSpeed;
	TargetZoom = FMath::Clamp(Zoom + TargetZoom, MinZoom, MaxZoom);
}


// Called every frame
void ACTCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Lerp camera movement to target location smoothly
	const FVector InterpedLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), CameraTargetLocation, DeltaTime, MoveSpeed);
	SetActorLocation(InterpedLocation);

	const float InterpedZoom = UKismetMathLibrary::FInterpTo(SpringArmComponent->TargetArmLength, TargetZoom, DeltaTime, ZoomSpeed);
	SpringArmComponent->TargetArmLength = InterpedZoom;
}

