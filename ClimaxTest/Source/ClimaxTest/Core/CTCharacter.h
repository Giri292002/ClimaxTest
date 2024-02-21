// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../AI/CTUnitInterface.h"
#include "CTCharacter.generated.h"

class ACTAIController;

UCLASS()
class CLIMAXTEST_API ACTCharacter : public ACharacter, public ICTUnitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACTCharacter();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CT | Components")
	UDecalComponent* SelectedDecal;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//--- ICTUnitInterface Implementation Begin ---
	virtual void Select_Implementation() override;
	virtual void Deselect_Implementation() override;
	//--- ICTUnitInterface Implementation End ---

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY()
	ACTAIController* CTAIController;
};
