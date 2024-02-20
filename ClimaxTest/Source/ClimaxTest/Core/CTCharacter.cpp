// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCharacter.h"

#include "../AI/CTAIController.h"
#include "Components/DecalComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ACTCharacter::ACTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ACTAIController::StaticClass();

	SelectedDecal = CreateDefaultSubobject<UDecalComponent>(FName(TEXT("Selected Decal")));
	SelectedDecal->SetupAttachment(GetMesh());

	GetMesh()->CustomDepthStencilValue = 1;
}

// Called when the game starts or when spawned
void ACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	SelectedDecal->SetVisibility(false);
	
}

void ACTCharacter::Select_Implementation()
{
	SelectedDecal->SetVisibility(true);
	GetMesh()->SetRenderCustomDepth(true);
}

void ACTCharacter::Deselect_Implementation()
{
	SelectedDecal->SetVisibility(false);
	GetMesh()->SetRenderCustomDepth(false);
}

// Called every frame
void ACTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

