// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_EnemyAIController.h"

ABP_EnemyAIController::ABP_EnemyAIController()
{
    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
}

void ABP_EnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    PawnSensing->OnSeePawn.AddDynamic(this, &ABP_EnemyAIController::OnSeePawn);

    RunBehaviorTree(BehaviorTree);
}

void ABP_EnemyAIController::OnSeePawn(APawn* PlayerPawn)
{
    ATheCubistCharacter* Player = Cast<ATheCubistCharacter>(PlayerPawn);

    if (Player)
    {
        SetCanSeePlayer(true, Player);
        RunRetriggerableTimer();
    }
}

void ABP_EnemyAIController::SetCanSeePlayer(bool SeePlayer, UObject* Player)
{
    if (SeePlayer)
    {
        GetBlackboardComponent()->SetValueAsBool(FName("Can See Player"), SeePlayer);

        GetBlackboardComponent()->SetValueAsObject(FName("Player Target"), Player);
    }
    else
    {
        GetBlackboardComponent()->SetValueAsBool(FName("Can See Player"), SeePlayer);

        ACharacter* EnemyChar = Cast<ACharacter>(GetPawn());
    }
}

void ABP_EnemyAIController::RunRetriggerableTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(RetriggerableTimerHandle);

    FunctionDelegate.BindUFunction(this, FName("SetCanSeePlayer"), false, GetPawn());

    GetWorld()->GetTimerManager().SetTimer(RetriggerableTimerHandle, FunctionDelegate, PawnSensing->SensingInterval * 2.0f, false);
}
