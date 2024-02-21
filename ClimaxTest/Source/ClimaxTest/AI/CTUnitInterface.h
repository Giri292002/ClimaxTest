// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CTUnitStates.h"
#include "CTUnitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCTUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CLIMAXTEST_API ICTUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Select();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Deselect();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetUnitState(ECTUnitState NewUnitState);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	ECTUnitState GetUnitState();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetMoveToLocation();

	// Commands
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Move_Command(FVector& MoveToLocation);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Wait_Command();

	//Could add attack, chop wood command, build commands here
};
