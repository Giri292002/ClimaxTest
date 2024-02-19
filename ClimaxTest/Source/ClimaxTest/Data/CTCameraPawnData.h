// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CTCameraPawnData.generated.h"

/**
 * 
 */
UCLASS()
class CLIMAXTEST_API UCTCameraPawnData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MoveSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float StartingZoom = 1000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinZoom = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxZoom = 1500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Scroll Edge", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float ScrollEdgeDistance = 0.3f;

	//How fast we want the scroll edge to move the camera
	UPROPERTY(EditDefaultsOnly, Category = "Scroll Edge", meta = (ClampMin = "0.1"))
	float ScrollEdgeSpeed = 1.f;
	
};
