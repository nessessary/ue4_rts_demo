// Fill out your copyright notice in the Description page of Project Settings.

#include "TopDown.h"
#include "MyGameStateBase.h"


AMyGameStateBase::AMyGameStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	MiniMapCamera = nullptr;
}