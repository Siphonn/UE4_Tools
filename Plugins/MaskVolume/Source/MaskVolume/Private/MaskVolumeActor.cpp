// Fill out your copyright notice in the Description page of Project Settings.

#include "MaskVolumeActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MaskManagerActor.h"


// Sets default values
AMaskVolumeActor::AMaskVolumeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComponent;

#pragma region First working test. Spawns a MaskManager if there is not one present.
#if WITH_EDITOR
	if (!UObject::IsTemplate(EObjectFlags::RF_Transient))
	{
		UWorld* World = GetWorld();
		if (World)
		{
			if (!MaskManager)
			{
				TArray<AActor*> FoundManagers;
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMaskManagerActor::StaticClass(), FoundManagers);

				if (FoundManagers.Num() <= 0)
				{
					CreateMaskManger();
				}
				else
				{
					MaskManager = Cast<AMaskManagerActor>(FoundManagers[0]);

					if (MaskManager->MaskVolumes.Num() < 2)
					{
						MaskManager->MaskVolumes.Add(this);
					}
					else
					{
						MaskManager = nullptr;
						UE_LOG(LogTemp, Warning, TEXT("Only 2 Mask Volumes can work simultaneously!"));
						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, TEXT("Only 2 Mask Volumes can work simultaneously!"));
					}
				}
			}
		}
	}
#endif
#pragma endregion
}

/// <summary>
///  Changing the Box colour depending on the "InvertMask" value in blueprint
/// </summary>
/// <param name="Transform"></param>
void AMaskVolumeActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	TArray<UBoxComponent*> BoxComponents;
	GetComponents(BoxComponents);

	if (BoxComponents.Num() > 0)
	{
		UBoxComponent* Box = BoxComponents[0];
		Box->ShapeColor = (bInvertMask ? FColor::Purple : FColor(0, 255, 255, 255));
	}
}

/// <summary>
/// Inherited method. UNDO
/// Used to HIDE or SHOW mask volume effect. It did not work in OnConstruction()
/// **NOTE: It might be better to move this code to a different function
/// </summary>
void AMaskVolumeActor::SetIsTemporarilyHiddenInEditor(bool bIsHidden)
{
	Super::SetIsTemporarilyHiddenInEditor(bIsHidden);

	if (!MaskManager) { return; }
	MaskManager->HideVolumeInEditor(this, !bIsHidden);
}

/// <summary>
/// Create a Mask Manager. 
/// The manager will handle the updates to the material parameter collection which inturn is connected to a material function.
/// </summary>
void AMaskVolumeActor::CreateMaskManger()
{
	ConstructorHelpers::FObjectFinder<UBlueprint> ManagerBlueprint(TEXT("Blueprint'/MaskVolume/MaskManager.MaskManager'"));
	
	if (!UObject::IsTemplate(RF_Transient))
	{
		if (ManagerBlueprint.Object)
		{
			FActorSpawnParameters SpawnParams;
			UObject* ManagerObject = GetWorld()->SpawnActor<AActor>(ManagerBlueprint.Object->GeneratedClass, SpawnParams);
			MaskManager = Cast<AMaskManagerActor>(ManagerObject);
			MaskManager->MaskVolumes.Add(this);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("C++: No MaskManager found"));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No MaskManager found! Check Path."));
		}
	}
}

/// <summary>
/// Calls the "RemoveVolume" function in the manager when destroyed.
/// </summary>
void AMaskVolumeActor::Destroyed()
{
	Super::Destroyed();

	if (!UObject::IsTemplate(EObjectFlags::RF_Transient))
	{
		if (MaskManager)
		{
			MaskManager->RemoveVolume(this);
		}
	}
}