// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class THECUBIST_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		UButton* Button;

	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text;
	
	int num;
	void SetText(int value);

	UFUNCTION()
		void OnButtonClick();
};
