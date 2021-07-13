// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaskVolumeActor.generated.h"

class AMaskManagerActor;

UCLASS()
class MASKVOLUME_API AMaskVolumeActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMaskVolumeActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mask Values")
		AMaskManagerActor* MaskManager = nullptr;
	UPROPERTY(EditAnywhere, Interp, Category = "Mask Values")
		bool bEnableMask = true;
	UPROPERTY(EditAnywhere, Interp, Category = "Mask Values")
		bool bInvertMask = false;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
		USceneComponent* SceneComponent;

protected:
	virtual void OnConstruction(const FTransform& Transform);
	virtual void Destroyed() override;
#if WITH_EDITOR
	virtual void SetIsTemporarilyHiddenInEditor(bool bIsHidden);
#endif

private:
	UFUNCTION()
		void CreateMaskManger();
};
