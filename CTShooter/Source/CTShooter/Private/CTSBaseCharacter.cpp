// Copyright 2023, Combat Training Shooter, All rights reserved

#include "CTSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CTSCharacterMovementComponent.h"

ACTSBaseCharacter::ACTSBaseCharacter(const FObjectInitializer& ObjInit)
    : Super(ObjInit.SetDefaultSubobjectClass<UCTSCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
    PrimaryActorTick.bCanEverTick = true;

    SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
    SpringArmComponent->SetupAttachment(GetRootComponent());

    /*установлен на true, компонент SpringArm будет поворачивать камеру вместе
    с актером (или пешеходом), когда тот поворачиваетс€. ≈сли вы не установите
    этот флаг, камера будет оставатьс€ в одном месте и не будет поворачиватьс€ вместе с актером.*/
    SpringArmComponent->bUsePawnControlRotation = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
    CameraComponent->SetupAttachment(SpringArmComponent);
}

void ACTSBaseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ACTSBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACTSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ACTSBaseCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ACTSBaseCharacter::MoveRight);

    PlayerInputComponent->BindAxis("LookUp", this, &ACTSBaseCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("TurnAround", this, &ACTSBaseCharacter::AddControllerYawInput);

    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACTSBaseCharacter::Jump);
    PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ACTSBaseCharacter::OnStartRunning);
    PlayerInputComponent->BindAction("Run", IE_Released, this, &ACTSBaseCharacter::OnStopRunning);
}

float ACTSBaseCharacter::GetMovementDirection() const
{
    if (GetVelocity().IsZero())
    {
        return 0.0f;
    }

    const auto VelocityNormal = GetVelocity().GetSafeNormal();
    const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
    const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
    const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

    return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ACTSBaseCharacter::MoveForward(float Amount)
{
    IsMovingForward = Amount > 0.0f;
    if (Amount == 0.0f)
    {
        return;
    }
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ACTSBaseCharacter::MoveRight(float Amount)
{
    if (Amount == 0.0f)
    {
        return;
    }
    AddMovementInput(GetActorRightVector(), Amount);
}

void ACTSBaseCharacter::OnStartRunning()
{
    WantsToRun = true;
}

void ACTSBaseCharacter::OnStopRunning()
{
    WantsToRun = false;
}

bool ACTSBaseCharacter::IsRunning() const
{
    return WantsToRun && IsMovingForward && !GetVelocity().IsZero();
}