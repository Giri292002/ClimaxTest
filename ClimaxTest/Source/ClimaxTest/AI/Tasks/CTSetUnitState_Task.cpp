// Fill out your copyright notice in the Description page of Project Settings.


#include "CTSetUnitState_Task.h"
#include "../CTUnitInterface.h"
#include "../CTAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

EBTNodeResult::Type UCTSetUnitState_Task::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (OwnerComp.GetAIOwner())
	{
		ICTUnitInterface::Execute_SetUnitState(OwnerComp.GetAIOwner(), NewUnitState);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
