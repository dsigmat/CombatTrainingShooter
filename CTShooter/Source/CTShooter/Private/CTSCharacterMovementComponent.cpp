// Copyright 2023, Combat Training Shooter, All rights reserved

#include "CTSCharacterMovementComponent.h"
#include "CTSBaseCharacter.h"


float UCTSCharacterMovementComponent::GetMaxSpeed() const
{
    const float MaxSpeed = Super::GetMaxSpeed();
    const ACTSBaseCharacter* Player = Cast<ACTSBaseCharacter>(GetPawnOwner());
    return Player && Player->IsRunning() ? MaxSpeed * RunModifier : MaxSpeed;
}