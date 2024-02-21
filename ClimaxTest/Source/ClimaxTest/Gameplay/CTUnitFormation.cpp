// Fill out your copyright notice in the Description page of Project Settings.


#include "CTUnitFormation.h"
#include "../AI/CTUnitInterface.h"
#include "../Core/CTCharacter.h"
#include "../Data/CTFormationData.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"

// Sets default values
ACTUnitFormation::ACTUnitFormation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACTUnitFormation::BeginPlay()
{
	Super::BeginPlay();	
}

void ACTUnitFormation::MoveUnitsInFormation(TSet<ACTCharacter*> InSelectedUnits, EUnitFormationShape FormationShape)
{
	SelectedUnits = InSelectedUnits.Array();
	if (FormationQueries.Contains(FormationShape))
	{
		UCTFormationData* FormationData = *FormationQueries.Find(FormationShape);
		FEnvQueryRequest EnvQuery = FEnvQueryRequest(FormationData->EnvironmentQuery, this);
		for (auto& Element : FormationData->Parameters)
		{
			EnvQuery.SetIntParam(Element.Key, Element.Value * SelectedUnits.Num());
		}
		EnvQuery.Execute(EEnvQueryRunMode::AllMatching, this, &ACTUnitFormation::OnQueryCompleted);
	}
}

void ACTUnitFormation::OnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccsessful())
	{
		TArray<FVector> Locations;
		Result->GetAllAsLocations(Locations);
		if (Locations.Num() > 0)
		{
			for (int i = 0; i < SelectedUnits.Num(); i++)
			{
				//There is a possibility the generator returns locations lesser than our needed number of AI. In that case, try to find a closeby point.
				if (Locations.IsValidIndex(i))
				{
					ICTUnitInterface::Execute_Move_Command(SelectedUnits[i]->GetController(), Locations[i]);
				}
			}
		}
	}
	Destroy();
}

// Called every frame
void ACTUnitFormation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

