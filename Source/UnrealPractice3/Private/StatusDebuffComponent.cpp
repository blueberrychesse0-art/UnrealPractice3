#include "StatusDebuffComponent.h"
#include "MyCharacter.h"

UStatusDebuffComponent::UStatusDebuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UStatusDebuffComponent::ApplyDebuff(EDebuffType Type, float Duration)
{
	if (Type == EDebuffType::None) return;

	if (DebuffStacks.Contains(Type)) { DebuffStacks[Type]++; }
	else { DebuffStacks.Add(Type, 1); }

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (ActiveDebuffs.Contains(Type))
	{
		TimerManager.ClearTimer(ActiveDebuffs[Type]);
	}
	else
	{
		ActiveDebuffs.Add(Type, FTimerHandle());
	}

	FTimerDelegate TimerDel;
	TimerDel.BindUObject(this, &UStatusDebuffComponent::ClearDebuff, Type);
	TimerManager.SetTimer(ActiveDebuffs[Type], TimerDel, Duration, false);

	if (Type == EDebuffType::Slow)
	{
		if (AMyCharacter* OwnerChar = Cast<AMyCharacter>(GetOwner()))
		{
			OwnerChar->UpdateMovementSpeed();
		}
	}

	//OnDebuffStateChanged.Broadcast(Type, true, DebuffStacks[Type]);
}

void UStatusDebuffComponent::ClearDebuff(EDebuffType Type)
{
	if (ActiveDebuffs.Contains(Type))
	{
		ActiveDebuffs.Remove(Type);
		DebuffStacks.Remove(Type);

		if (Type == EDebuffType::Slow)
		{
			if (AMyCharacter* OwnerChar = Cast<AMyCharacter>(GetOwner()))
			{
				OwnerChar->UpdateMovementSpeed();
			}
		}

		//OnDebuffStateChanged.Broadcast(Type, false, 0);
	}
}

int32 UStatusDebuffComponent::GetDebuffStack(EDebuffType Type) const
{
	if (DebuffStacks.Contains(Type))
	{
		return DebuffStacks[Type];
	}
	return 0;
}

