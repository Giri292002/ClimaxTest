// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CTUnitStates.generated.h"

UENUM(BlueprintType)
enum class ECTUnitState : uint8
{
	Idle,
	Moving
};