// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUI.h"

void UGameUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonWidgetRef) 
	{
		for (int i = 0; i < 4; i++)
		{
			UUserWidget* widget = CreateWidget(this, ButtonWidgetRef);
			VerticalBox->AddChildToVerticalBox(widget);

			UButtonWidget* button = Cast<UButtonWidget>(widget);
			button->SetText(i);
		}
	}

	SetHealth(0.5);
}

void UGameUI::SetHealth(float percentage)
{
	if (HealthBar) 
	{
		HealthBar->SetPercent(percentage);
	}
}

void UGameUI::SetScore(int scoreAmt)
{
	UIScore += scoreAmt;
	
	if (TextBlock_0) 
	{
		TextBlock_0->SetText(FText::FromString("Hit: ' + FString::FromInt(UIScore"));
	}
}
