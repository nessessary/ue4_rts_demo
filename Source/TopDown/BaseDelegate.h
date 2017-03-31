// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseDelegate.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UBaseDelegate : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class TOPDOWN_API IBaseDelegate
{
	GENERATED_IINTERFACE_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//virtual void MySelect() = 0;
	virtual void* GetEmbed() { return embed_; }
	virtual void SetEmeded(void* p) { embed_ = p; }
	void* embed_;

};

class Embed {

};
namespace RTS {
	// select , unselect, move, attack
	class IObject {
	public:
		virtual void SelectMe(bool is_select) = 0;
		virtual bool IsSelectMe() = 0;
	};
	class IPawn {
	public:
		virtual void MoveTo() = 0;
		virtual void Attack() = 0;
		virtual void Jump() = 0;
		virtual void Stop() = 0;
	};
	class IBuild {
	public:
		virtual void SetRailyPoint() = 0;
		virtual void Spawn() = 0;
	};
};