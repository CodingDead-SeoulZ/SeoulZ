// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SZTimeLimitWidget.h"
#include "Engine/Canvas.h"
#include "UObject/ConstructorHelpers.h"

ASZTimeLimitWidget::ASZTimeLimitWidget()
{
	RemainingTime = 240.0f;
	TimerColor = FColor::White;
	TimerPosition = FVector2D(0.0f, 50.0f);
	
	// 기본 폰트 로드.
    static ConstructorHelpers::FObjectFinder<UFont> FontObj(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	if (FontObj.Succeeded())
	{
		TimerFont = FontObj.Object;
	}
}

void ASZTimeLimitWidget::DrawHUD()
{
	Super::DrawHUD();
	
	if (Canvas && TimerFont)
	//if (Canvas)
	{
		// 분, 초 표시
		int Minutes = FMath::FloorToInt(RemainingTime / 60.0f);
		int Seconds = FMath::FloorToInt(RemainingTime) % 60;
		FString TimerText = FString::Printf(TEXT("%02d : %02d"), Minutes, Seconds);
		
		FVector2D TextSize;
		Canvas->StrLen(TimerFont, TimerText, TextSize.X, TextSize.Y);
		
		// 화면 중앙 상단 기준으로 위치 조정
		float X = (Canvas->SizeX - TextSize.X) / 2.f + TimerPosition.X;
		float Y = TimerPosition.Y;

		FCanvasTextItem TextItem(FVector2D(X, Y), FText::FromString(TimerText), TimerFont, TimerColor);
		Canvas->DrawItem(TextItem);
	}
}

void ASZTimeLimitWidget::UpdateTimer(float DeltaTime)
{
	if (RemainingTime > 0.0f)
	{
		RemainingTime -= DeltaTime;
		if (RemainingTime <= 0.0f)
		{
			RemainingTime = 0.0f;
		}
	}
}
