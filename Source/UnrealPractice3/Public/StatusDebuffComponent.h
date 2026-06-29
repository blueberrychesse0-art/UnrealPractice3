#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusDebuffComponent.generated.h"

UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	None,
	Slow,
	ReverseControl,
	Blind
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREALPRACTICE3_API UStatusDebuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UStatusDebuffComponent();

	void ApplyDebuff(EDebuffType Type, float Duration);

	bool IsDebuffActive(EDebuffType Type) const { return ActiveDebuffs.Contains(Type); }

	int32 GetDebuffStack(EDebuffType Type) const;

	//UPROPERTY(BlueprintAssignable, Category = "Debuff|UI")
	//FOnDebuffStateChanged OnDebuffStateChanged;
protected:
	void ClearDebuff(EDebuffType Type);

private:
	TMap<EDebuffType, FTimerHandle> ActiveDebuffs;
	TMap<EDebuffType, int32> DebuffStacks;

		
};
