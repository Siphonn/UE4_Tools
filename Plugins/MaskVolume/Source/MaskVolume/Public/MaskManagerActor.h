// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/World.h"
#include "MaskManagerActor.generated.h"

class AMaskVolumeActor;

UCLASS()
class MASKVOLUME_API AMaskManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMaskManagerActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mask Manager")
		UMaterialParameterCollection* MaskParameters = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mask Manager")
		TArray<AMaskVolumeActor*> MaskVolumes;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Mask Manager") 
		void UpdateVolume();
	UFUNCTION()
		void RemoveVolume(AMaskVolumeActor* Volume);

protected:
	virtual void Destroyed() override;

private:
	UMaterialParameterCollectionInstance* MaskParametersInstance = nullptr;

public:
#if WITH_EDITOR
	UFUNCTION()
		void HideVolumeInEditor(AMaskVolumeActor* Volume, bool bShowVolume);

#endif
};
