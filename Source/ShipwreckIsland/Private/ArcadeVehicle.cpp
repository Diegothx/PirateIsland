#include "ArcadeVehicle.h"

#include "PhysicsEngine/BodyInstance.h"

#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/PlayerController.h"

AArcadeVehicle::AArcadeVehicle()
{
    PrimaryActorTick.bCanEverTick = true;

	// Root

    VehicleBody = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("VehicleBody")
    );

    RootComponent = VehicleBody;

    VehicleBody->SetSimulatePhysics(true);

    VehicleBody->SetEnableGravity(true);

    VehicleBody->SetLinearDamping(LinearDamping);
    VehicleBody->SetAngularDamping(AngularDamping);

    VehicleBody->SetCollisionEnabled(
        ECollisionEnabled::QueryAndPhysics
    );

    VehicleBody->SetCollisionObjectType(
        ECC_Pawn
    );

    VehicleBody->SetCollisionResponseToAllChannels(
        ECR_Block
    );
	// 2D Movement
    VehicleBody->BodyInstance.bLockZTranslation = true;

    VehicleBody->BodyInstance.bLockXRotation = true;
    VehicleBody->BodyInstance.bLockYRotation = true;

    VehicleBody->BodyInstance.bLockZRotation = false;

    // Camera

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(
        TEXT("SpringArm")
    );

    SpringArm->SetupAttachment(RootComponent);

    SpringArm->TargetArmLength = 1200.0f;

    SpringArm->SetRelativeRotation(
        FRotator(-60.0f, 0.0f, 0.0f)
    );

    SpringArm->bDoCollisionTest = true;

    Camera = CreateDefaultSubobject<UCameraComponent>(
        TEXT("Camera")
    );

    Camera->SetupAttachment(SpringArm);

    // Pawn

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}


void AArcadeVehicle::BeginPlay()
{
    Super::BeginPlay();

    // Enhanced Input

    if (APlayerController* PlayerController =
        Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer =
            PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<
                    UEnhancedInputLocalPlayerSubsystem>())
            {
                if (InputMappingContext)
                {
                    Subsystem->AddMappingContext(
                        InputMappingContext,
                        0
                    );
                }
            }
        }
    }
	// Physics settings

    VehicleBody->SetLinearDamping(LinearDamping);
    VehicleBody->SetAngularDamping(AngularDamping);

    VehicleBody->WakeAllRigidBodies();
}


void AArcadeVehicle::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!VehicleBody->IsSimulatingPhysics())
    {
        return;
    }

    ApplyMovementForce();
    ApplyTurningTorque();

    LimitPhysics();
}

void AArcadeVehicle::ApplyMovementForce()
{
    if (FMath::IsNearlyZero(MoveInput))
    {
        return;
    }

    const FVector Forward =
        GetActorForwardVector();

    float Force = 0.0f;

    if (MoveInput > 0.0f)
    {
        Force = EngineForce * MoveInput;
    }
    else
    {
        Force = ReverseForce * MoveInput;
    }

    VehicleBody->AddForce(
        Forward * Force,
        NAME_None,
        false
    );
}

void AArcadeVehicle::ApplyTurningTorque()
{
    if (FMath::IsNearlyZero(TurnInput))
    {
        return;
    }

    const FVector Torque(
        0.0f,
        0.0f,
        TurnInput * TurnTorque
    );

    VehicleBody->AddTorqueInRadians(
        Torque,
        NAME_None,
        false
    );
}

void AArcadeVehicle::LimitPhysics()
{
    // Linear velocity

    FVector Velocity =
        VehicleBody->GetPhysicsLinearVelocity();

    Velocity.Z = 0.0f;

    if (Velocity.SizeSquared() >
        FMath::Square(MaxSpeed))
    {
        Velocity =
            Velocity.GetSafeNormal() * MaxSpeed;
    }

    VehicleBody->SetPhysicsLinearVelocity(
        Velocity,
        false
    );
	// Angular velocity
    FVector AngularVelocity =
        VehicleBody->GetPhysicsAngularVelocityInRadians();

    AngularVelocity.X = 0.0f;
    AngularVelocity.Y = 0.0f;

    AngularVelocity.Z =
        FMath::Clamp(
            AngularVelocity.Z,
            -MaxAngularSpeed,
            MaxAngularSpeed
        );

    VehicleBody->SetPhysicsAngularVelocityInRadians(
        AngularVelocity,
        false
    );
}

void AArcadeVehicle::OnMove(
    const FInputActionValue& Value
)
{
    MoveInput = Value.Get<float>();
}


void AArcadeVehicle::OnTurn(
    const FInputActionValue& Value
)
{
    TurnInput = Value.Get<float>();
}

void AArcadeVehicle::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent
)
{
    Super::SetupPlayerInputComponent(
        PlayerInputComponent
    );

    UEnhancedInputComponent* EnhancedInput =
        Cast<UEnhancedInputComponent>(
            PlayerInputComponent
        );

    if (!EnhancedInput)
    {
        return;
    }

    if (MoveAction)
    {
        EnhancedInput->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &AArcadeVehicle::OnMove
        );

        EnhancedInput->BindAction(
            MoveAction,
            ETriggerEvent::Completed,
            this,
            &AArcadeVehicle::OnMove
        );
    }

    if (TurnAction)
    {
        EnhancedInput->BindAction(
            TurnAction,
            ETriggerEvent::Triggered,
            this,
            &AArcadeVehicle::OnTurn
        );

        EnhancedInput->BindAction(
            TurnAction,
            ETriggerEvent::Completed,
            this,
            &AArcadeVehicle::OnTurn
        );
    }
}