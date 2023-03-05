// Copyright 2023, Combat Training Shooter, All rights reserved

#include "CTSBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

ACTSBaseWeapon::ACTSBaseWeapon()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
    SetRootComponent(WeaponMesh);
}

void ACTSBaseWeapon::BeginPlay()
{
    Super::BeginPlay();
}