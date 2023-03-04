// Copyright 2023, Combat Training Shooter, All rights reserved

#include "CTSBaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CTSCharacterMovementComponent.h"
#include "CTSHealthComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(BaseCharacterLog, All, All)

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

    HealthComponent = CreateDefaultSubobject<UCTSHealthComponent>("HealthComponent");

    HealthTextComponent = CreateDefaultSubobject<UTextRenderComponent>("HealthTextComponent");
    HealthTextComponent->SetupAttachment(GetRootComponent());
}

void ACTSBaseCharacter::BeginPlay()
{
    Super::BeginPlay();

    check(HealthComponent);
    check(HealthTextComponent);
    check(GetCharacterMovement());

    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ACTSBaseCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ACTSBaseCharacter::OnHealthChanged);

    LandedDelegate.AddDynamic(this, &ACTSBaseCharacter::OnGroundLanded);
}

void ACTSBaseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ACTSBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    check(PlayerInputComponent);

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

void ACTSBaseCharacter::OnDeath()
{
    UE_LOG(BaseCharacterLog, Error, TEXT("Character %s is Dead"), *GetName());

    PlayAnimMontage(DeathAnimMontage);

    GetCharacterMovement()->DisableMovement();

    SetLifeSpan(LifeSpanOnDeath);

    if (Controller)
    {
        Controller->ChangeState(NAME_Spectating);
    }
}

void ACTSBaseCharacter::OnHealthChanged(float Health)
{
    HealthTextComponent->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Health)));
}

void ACTSBaseCharacter::OnGroundLanded(const FHitResult& Hit)
{
    const auto FallVelocityZ = -GetVelocity().Z;
    UE_LOG(BaseCharacterLog, Display, TEXT("On Landed: %f"), FallVelocityZ);

    if (FallVelocityZ < LandedDamageVelocity.X)
    {
        return;
    }

    const auto FinalDamage = FMath::GetMappedRangeValueClamped(LandedDamageVelocity, LandedDamage, FallVelocityZ);

    UE_LOG(BaseCharacterLog, Display, TEXT("Final Damage: %f"), FinalDamage);
    UGameplayStatics::ApplyDamage(this, FinalDamage, GetController(), nullptr, nullptr);
}
