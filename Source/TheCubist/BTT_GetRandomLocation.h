// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTT_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class THECUBIST_API UBTT_GetRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	class UnavigationSystemV1* NavArea;

	FVector RandomLocation;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
