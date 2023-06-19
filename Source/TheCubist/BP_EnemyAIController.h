// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TheCubistCharacter.h"
#include "AIController.h"
#include "BP_EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class THECUBIST_API ABP_EnemyAIController : public AAIController
{
	GENERATED_BODY()
	
 public:
	void BeginPlay() override;

	ABP_EnemyAIController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
		class UPawnSensingComponent* PawnSensing;

	UFUNCTION()
		void OnSeePawn(APawn* PlayerPawn);

	UFUNCTION()
		void SetCanSeePlayer(bool SeePlayer, class UObject* Player);

	FTimerHandle RetriggerableTimerHandle;
	FTimerDelegate FunctionDelegate;
	void RunRetriggerableTimer();
};
