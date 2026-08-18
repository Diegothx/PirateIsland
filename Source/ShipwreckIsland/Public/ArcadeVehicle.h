#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputAction.h"

#include "ArcadeVehicle.generated.h"


UCLASS()
class SHIPWRECKISLAND_API AArcadeVehicle : public APawn
{
    GENERATED_BODY()

public:

    AArcadeVehicle();

protected:

    virtual void BeginPlay() override;

public:

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(
        class UInputComponent* PlayerInputComponent
    ) override;


    // ============================================================
    // COMPONENTS
    // ============================================================

    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Vehicle"
    )
    TObjectPtr<UStaticMeshComponent> VehicleBody;


    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Camera"
    )
    TObjectPtr<USpringArmComponent> SpringArm;


    UPROPERTY(
        VisibleAnywhere,
        BlueprintReadOnly,
        Category = "Camera"
    )
    TObjectPtr<UCameraComponent> Camera;


    // ============================================================
    // INPUT
    // ============================================================

    UPROPERTY(
        EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Input"
    )
    TObjectPtr<UInputMappingContext> InputMappingContext;


    UPROPERTY(
        EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Input"
    )
    TObjectPtr<UInputAction> MoveAction;


    UPROPERTY(
        EditDefaultsOnly,
        BlueprintReadOnly,
        Category = "Input"
    )
    TObjectPtr<UInputAction> TurnAction;


    // ============================================================
    // TANK MOVEMENT
    // ============================================================

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float EngineForce = 500000.0f;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float ReverseForce = 350000.0f;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float TurnTorque = 1500000.0f;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float MaxSpeed = 2500.0f;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float MaxAngularSpeed = 2.5f;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float LinearDamping = 0.5f;


    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Tank Movement"
    )
    float AngularDamping = 4.0f;


    // ============================================================
    // INPUT VALUES
    // ============================================================

    float MoveInput = 0.0f;

    float TurnInput = 0.0f;


    // ============================================================
    // INPUT FUNCTIONS
    // ============================================================

    void OnMove(
        const FInputActionValue& Value
    );


    void OnTurn(
        const FInputActionValue& Value
    );


    // ============================================================
    // PHYSICS FUNCTIONS
    // ============================================================

    void ApplyMovementForce();

    void ApplyTurningTorque();

    void LimitPhysics();
};