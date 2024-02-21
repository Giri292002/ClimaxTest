// Fill out your copyright notice in the Description page of Project Settings.


#include "CTAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "CTUnitStates.h"
#include "Navigation/CrowdFollowingComponent.h"

ACTAIController::ACTAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{

}

void ACTAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (RunBehaviorTree(BehaviorTree))
	{
		SetUnitState_Implementation(ECTUnitState::Idle);
	}
}

void ACTAIController::Move_Command_Implementation(FVector& InMoveToLocation)
{
	MoveToCommandLocation = InMoveToLocation;
	GetBlackboardComponent()->SetValueAsVector(FName(TEXT("MoveToLocation")), MoveToCommandLocation);
	SetUnitState_Implementation(ECTUnitState::Moving);
}

void ACTAIController::Wait_Command_Implementation()
{
	SetUnitState_Implementation(ECTUnitState::Idle);
}

FVector ACTAIController::GetMoveToLocation_Implementation()
{
	return MoveToCommandLocation;
}

void ACTAIController::SetUnitState_Implementation(ECTUnitState NewUnitState)
{
	GetBlackboardComponent()->SetValueAsEnum(FName(TEXT("UnitState")), (uint8)NewUnitState);
}

ECTUnitState ACTAIController::GetUnitState_Implementation()
{
	return (ECTUnitState)(GetBlackboardComponent()->GetValueAsEnum(FName(TEXT("UnitState"))));
}
