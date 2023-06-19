// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "ButtonWidget.h"
#include "GameUI.generated.h"

/**
 * 
 */
UCLASS()
class THECUBIST_API UGameUI : public UUserWidget
{
	GENERATED_BODY()
	

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* TextBlock_0;

	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
		UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UButtonWidget> ButtonWidgetRef;

	UFUNCTION()
		void SetHealth(float percentage);

	UFUNCTION()
		void SetScore(int scoreAmt);

private:
	int UIScore;
};
