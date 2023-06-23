// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "Turrets.generated.h"


UCLASS()
class THECUBIST_API ATurrets : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnyWhere)
		USceneComponent* Root;

	UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* TurretMesh;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Beam;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Target1;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* Target2;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* BeamTarget;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
		UParticleSystemComponent* P_MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, Category = Shooting)
		USoundBase* ShootSound;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* FollowTarget;

	UPROPERTY()
		FTimerHandle ChangeTargetTimerHandle;

	UPROPERTY()
		FTimerHandle TraceTimerHandle;

	UPROPERTY()
		FTimerHandle ShootTimerHandle;

	//Rotation related variables
	int TimerCount = 0;
	FRotator LookAtRotation;
	FRotator TargetRotation;
	FRotator RotationDelta;


	UPROPERTY(EditAnywhere)
		float ChangeTargetDelay = 5.0f;

	UPROPERTY(EditAnywhere)
		float RotationRateMultiplier = 1.0f;

	UPROPERTY()
		AActor* Enemy = nullptr;
  
	UFUNCTION()
		void UpdateLookAtTarget(float DeltaTime);

	UFUNCTION()
		void ChangeBeamTarget();

	UFUNCTION(BlueprintCallable)
		void SetBeamLength(float Length);

	UFUNCTION()
		void TraceBeam();

	UFUNCTION()
		void CheckEnemy(AActor* HitActor);

	UFUNCTION()
		void FollowEnemy(float DeltaTime);

	UFUNCTION()
		void Shoot();

public:	
	// Sets default values for this actor's properties
	ATurrets();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//--------
	//Beam length
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BeamLength = 1000.0f;
    //---------

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
