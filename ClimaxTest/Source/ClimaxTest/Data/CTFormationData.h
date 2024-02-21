// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "../Gameplay/CTUnitFormation.h"
#include "CTFormationData.generated.h"

/**
 * 
 */
UCLASS()
class CLIMAXTEST_API UCTFormationData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UEnvQuery* EnvironmentQuery;

	//The named params to input into generator.
	//If this was production we would have our own generators with standardized values and we wouldn't need to do this weird conversions. For example
	//if we had a grid generator we could just have 2 params called width/height and we can just have a map of those values here. Right now its just scaled
	//by the number of units we have, so the int part of this pair refers to the base value we need to multiply our number of units by. 
	//For eg: If its a circle formation the number of points on that circle is determined by int (which is 1) * Number of units, so n points will be generated  
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, int> Parameters;	
};
