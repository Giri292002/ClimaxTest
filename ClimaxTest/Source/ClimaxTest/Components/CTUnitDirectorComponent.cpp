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

	ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
	//We need to update viewport scale when viewport is resized. Much better than putting it in tick.
	FViewport::ViewportResizedEvent.AddUObject(this, &UCTUnitDirectorComponent::OnViewportSizeChanged);
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
	//Start marquee selecting
	bIsMarqueeSelecting = true;
	CTHUD->RunRectSelect(bIsMarqueeSelecting);

	//Store Mouse Starting Location
	StartMousePosition = CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector2D ScaledPos = StartMousePosition * ViewportScale;
	CTHUD->SetStartMousePosition(ScaledPos);
	CTHUD->SetCurrentMousePosition(ScaledPos);

	//Update Marquee select visual
	MarqueeWidget->AddToViewport();
	MarqueeWidget->UpdateMarqueeTransform(StartMousePosition, CurrentMousePosition);
}

void UCTUnitDirectorComponent::StopMarqueeSelectUnits()
{
	bIsMarqueeSelecting = false;
	CTHUD->RunRectSelect(bIsMarqueeSelecting);
	MarqueeWidget->RemoveFromParent();
}

void UCTUnitDirectorComponent::DeselectUnits()
{
	if (SelectedUnits.IsEmpty())
	{
		return;
	}

	for (auto Unit : SelectedUnits)
	{
		ICTUnitInterface::Execute_Deselect(Unit);
	}	

	SelectedUnits.Empty();
}

void UCTUnitDirectorComponent::MoveUnits()
{
	if (SelectedUnits.IsEmpty())
	{
		return;
	}

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
	TSet<ACTCharacter*> UnitsToDeselect = SelectedUnits.Difference(TSet<ACTCharacter*>(FoundActors));

	//Deslect units that are not in marquee select anymore
	for (ACTCharacter* Unit : UnitsToDeselect)
	{
		ICTUnitInterface::Execute_Deselect(Unit);
		SelectedUnits.Remove(Unit);
	}

	for (ACTCharacter* Unit : FoundActors)
	{
		ICTUnitInterface::Execute_Select(Unit);
		SelectedUnits.Add(Unit);
	}
}

void UCTUnitDirectorComponent::OnViewportSizeChanged(FViewport* ViewPort, uint32 val)
{
	ViewportScale = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
}


// Called every frame
void UCTUnitDirectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsMarqueeSelecting)
	{
		return;
	}

	//Only run trace and update widget if mouse moved
	const FVector2D PreviousMousePosition = CurrentMousePosition;
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	bool bCanRunRectSelect = !(CurrentMousePosition.Equals(PreviousMousePosition));

	CTHUD->RunRectSelect(bCanRunRectSelect);

	if (!bCanRunRectSelect)
	{
		return;
	}

	MarqueeWidget->UpdateMarqueeTransform(StartMousePosition, CurrentMousePosition);
	FVector2D ScaledPos = CurrentMousePosition * ViewportScale;
	CTHUD->SetCurrentMousePosition(ScaledPos);
}