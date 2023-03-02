// Copyright 2023, Combat Training Shooter, All rights reserved

#include "CTSGameModeBase.h"
#include "CTSBaseCharacter.h"
#include "CTSPlayerController.h"

ACTSGameModeBase::ACTSGameModeBase()
{
    DefaultPawnClass = ACTSBaseCharacter::StaticClass();
    PlayerControllerClass = ACTSPlayerController::StaticClass();
}