// Fill out your copyright notice in the Description page of Project Settings.


#include "CTUnitDirectorComponent.h"

#include "../AI/CTAIController.h"
#include "../Core/CTCharacter.h"
#include "../Core/CTPlayerController.h"
#include "../Core/CTHUD.h"
#include "../Widget/CTMarqueeWidget.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Runtime/AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values for this component's properties
UCTUnitDirectorComponent::UCTUnitDirectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCTUnitDirectorComponent::BeginPlay()
{
	Super::BeginPlay();
	
	CTPlayerController = Cast<ACTPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	MarqueeWidget = CreateWidget<UCTMarqueeWidget>(GetWorld(), MarqueeWidgetClass);

	//Unreal has a built in get actors in rectange but the caveat is we can only use it in the draw hud event, so we need to pass on all the info to HUD and get back the selected actors.
	//I am not a huge fan of how this going out of this class and then coming back in with the actor info. If I had more time and this was a proper production setting I would look into decoupling that HUD function from draw HUD
	//and research how we can use it normally. Another method to go about this would be to calculate the 4 points of the box in the world space with the start mouse position and current mouse position and then do a box trace.
	CTHUD = Cast<ACTHUD>(CTPlayerController->GetHUD());
	CTHUD->OnMarqueeSelectDelegate.AddDynamic(this, &UCTUnitDirectorComponent::OnMarqueeSelectCallback);
}

void UCTUnitDirectorComponent::SelectUnit()
{
	//Deselect previously selected units
	DeselectUnits();

	FHitResult HitResult;
	CTPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, HitResult);

	if (HitResult.bBlockingHit)
	{
		if (ACTCharacter* SelectedUnit = Cast<ACTCharacter>(HitResult.GetActor()))
		{
			ICTUnitInterface::Execute_Select(SelectedUnit);
			SelectedUnits.Add(SelectedUnit);
		}
	}
}

void UCTUnitDirectorComponent::MarqueeSelectUnits()
{
	GEngine->AddOnScreenDebugMessage(0, 0.2f, FColor::Green, TEXT("Started Selecting"));

	//Store Mouse Starting Location
	bIsMarqueeSelecting = true;
	CTHUD->SetIsMarqueeSelecting(bIsMarqueeSelecting);

	StartMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D PCMousePos;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(PCMousePos.X, PCMousePos.Y);
	CTHUD->SetStartMousePosition(PCMousePos);

	MarqueeWidget->AddToViewport();
}

void UCTUnitDirectorComponent::StopMarqueeSelectUnits()
{
	GEngine->AddOnScreenDebugMessage(1, 0.2f, FColor::Green, TEXT("Stopped Selecting"));
	bIsMarqueeSelecting = false;
	CTHUD->SetIsMarqueeSelecting(bIsMarqueeSelecting);
	MarqueeWidget->RemoveFromParent();
}

void UCTUnitDirectorComponent::DeselectUnits()
{
	/*if (!SelectedUnits.IsEmpty())
	{
		for (auto Unit : SelectedUnits)
		{
			ICTUnitInterface::Execute_Deselect(Unit);
		}
	}
	SelectedUnits.Empty();*/
}

void UCTUnitDirectorComponent::MoveUnits()
{
	FHitResult HitResult;
	CTPlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Camera), false, HitResult);

	//Try to check if the projected point is on navmesh. If its not on navmesh, the units cant move to it.
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	FNavLocation ProjectedLocation;
	if (!NavSystem->ProjectPointToNavigation(HitResult.ImpactPoint, ProjectedLocation))
	{
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ArrowEffect, ProjectedLocation.Location);

	for (ACTCharacter* Unit : SelectedUnits)
	{
		Cast<ACTAIController>(Unit->GetController())->MoveToLocation(ProjectedLocation);
	}
}

void UCTUnitDirectorComponent::OnMarqueeSelectCallback(TArray<ACTCharacter*> FoundActors)
{
	DeselectUnits();

	SelectedUnits = FoundActors;

	for (ACTCharacter* Unit : SelectedUnits)
	{
		ICTUnitInterface::Execute_Select(Unit);
	}
}


// Called every frame
void UCTUnitDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO: Check if player moved the mouse, only then update the marquee
	if (bIsMarqueeSelecting)
	{
		CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		FVector2D PCMousePos;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(PCMousePos.X, PCMousePos.Y);
		CTHUD->SetCurrentMousePosition(PCMousePos);
		MarqueeWidget->UpdateMarqueeTransform(StartMousePosition, CurrentMousePosition);
		GEngine->AddOnScreenDebugMessage(2, 0.2f, FColor::Green, FString::Printf(TEXT("Selecting start %s, end %s"), *StartMousePosition.ToString(), *CurrentMousePosition.ToString()));
	}
}