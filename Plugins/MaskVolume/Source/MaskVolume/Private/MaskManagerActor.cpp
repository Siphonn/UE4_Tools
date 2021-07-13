// Fill out your copyright notice in the Description page of Project Settings.


#include "MaskManagerActor.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"

// Sets default values
AMaskManagerActor::AMaskManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

/// <summary>
/// Called by SetIsTemporarilyHiddenInEditor (inherited) in MaskVolume.
/// </summary>
/// <param name="Volume"></param>
/// <param name="bShowVolume"></param>
void AMaskManagerActor::HideVolumeInEditor(AMaskVolumeActor* Volume, bool bShowVolume)
{
	int32 index;
	FName SetMaskParam;
	for (int i = 0; i < MaskVolumes.Num(); i++)
	{
		if (Volume == MaskVolumes[i])
		{
			index = i;
			break;
		}
	}

	SetMaskParam = ((index == 0) ? FName(TEXT("SetMask_1")) : FName(TEXT("SetMask_2")));

	if (!MaskParametersInstance)
	{
		MaskParametersInstance = GetWorld()->GetParameterCollectionInstance(MaskParameters);
	}
	MaskParametersInstance->SetScalarParameterValue(SetMaskParam, bShowVolume);
}

/// <summary>
/// Call when a Mask Volume is destroyed. 
/// Primarily to updates the material parameter collection when the volume is deleted.
/// </summary>
/// <param name="Volume"></param>
void AMaskManagerActor::RemoveVolume(AMaskVolumeActor* Volume)
{
	if (!MaskVolumes.Contains(Volume)) { return; } // Check is the Volume is in the array of MaskVolumes

	int32 index;
	FName SetMaskParam;
	for (int i = 0; i < MaskVolumes.Num(); ++i)
	{
		if (MaskVolumes[i] == Volume)
		{
			index = i;
			break;
		}
	}

	SetMaskParam = ((index == 0) ? FName(TEXT("SetMask_1")) : FName(TEXT("SetMask_2")));

	if (!MaskParametersInstance)
	{
		MaskParametersInstance = GetWorld()->GetParameterCollectionInstance(MaskParameters);
	}

	MaskParametersInstance->SetScalarParameterValue(SetMaskParam, 0.0f);
	MaskVolumes.Remove(Volume);

	// if there are no Mask Volume in the scene, destroy this manager
	if (MaskVolumes.Num() <= 0)
	{
		MaskParametersInstance->SetScalarParameterValue("SetMask_1", 0.0f);
		MaskParametersInstance->SetScalarParameterValue("SetMask_2", 0.0f);
		GetWorld()->DestroyActor(this);
	}
}

/// <summary>
/// Method called in blueprint.
/// </summary>
void AMaskManagerActor::UpdateVolume_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("C++: Update Volume"));
}

/// <summary>
/// Inherited Destroy method. Calls when actor is destroyed/deleted.
/// Sets "SetMask" variables in material parameter collection to 0.0
/// </summary>
void AMaskManagerActor::Destroyed()
{
	Super::Destroyed();

	if (!MaskParametersInstance)
	{
		MaskParametersInstance = GetWorld()->GetParameterCollectionInstance(MaskParameters);
	}
	MaskParametersInstance->SetScalarParameterValue("SetMask_1", 0.0f);
	MaskParametersInstance->SetScalarParameterValue("SetMask_2", 0.0f);
}