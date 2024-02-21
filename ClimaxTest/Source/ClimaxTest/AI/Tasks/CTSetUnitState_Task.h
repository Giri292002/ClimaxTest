// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../CTUnitStates.h"
#include "CTSetUnitState_Task.generated.h"

/**
 * 
 */
UCLASS()
class CLIMAXTEST_API UCTSetUnitState_Task : public UBTTaskNode
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "CT | AI")
	ECTUnitState NewUnitState;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
