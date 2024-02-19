// Fill out your copyright notice in the Description page of Project Settings.


#include "CTCharacter.h"

// Sets default values
ACTCharacter::ACTCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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

