// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CTUnitInterface.h"
#include "CTUnitStates.h"
#include "CTAIController.generated.h"

UCLASS()
class CLIMAXTEST_API ACTAIController : public AAIController, public ICTUnitInterface
{
	GENERATED_BODY()

public:
	ACTAIController(const FObjectInitializer& ObjectInitializer);

	//--- ICTUnitInterface Implementation Begin ---
	virtual void Move_Command_Implementation(FVector& MoveToLocation) override;
	virtual void Wait_Command_Implementation() override;
	virtual FVector GetMoveToLocation_Implementation() override;
	virtual void SetUnitState_Implementation(ECTUnitState NewUnitState) override;
	virtual ECTUnitState GetUnitState_Implementation() override;
	//--- ICTUnitInterface Implementation End ---

protected:
	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* BehaviorTree;

	virtual void OnPossess(APawn* InPawn) override;

private:
	UPROPERTY()
	FVector MoveToCommandLocation;

	UPROPERTY()
	ECTUnitState UnitState;
};
