// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "CTUnitFormation.generated.h"

class ACTCharacter;
class EnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCTFormationData;

UENUM(BlueprintType)
enum class EUnitFormationShape : uint8
{
	Default = 0,
	Circle = 1,
	Grid = 2,
	Single = 3,
	VerticalLine = 4,
	HorizontalLine = 5
};

UCLASS()
class CLIMAXTEST_API ACTUnitFormation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACTUnitFormation();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "CT | AI | Formations")
	TMap<EUnitFormationShape, UCTFormationData*> FormationQueries;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void MoveUnitsInFormation(TSet<ACTCharacter*> InSelectedUnits, EUnitFormationShape FormationShape);

	void OnQueryCompleted(TSharedPtr<FEnvQueryResult> Result);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<ACTCharacter*> SelectedUnits;

};
