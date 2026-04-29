// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HW7_API AMyGameMode : public AGameMode
{
	GENERATED_BODY()


public:
	AMyGameMode();

private:
	virtual void BeginPlay() override; // 게임이 시작될 때 호출되는 함수, 게임 모드가 초기화된 후에 호출
};
