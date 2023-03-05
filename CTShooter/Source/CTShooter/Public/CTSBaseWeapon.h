// Copyright 2023, Combat Training Shooter, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTSBaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class CTSHOOTER_API ACTSBaseWeapon : public AActor
{
    GENERATED_BODY()

public:
    ACTSBaseWeapon();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent* WeaponMesh;

    virtual void BeginPlay() override;
};
