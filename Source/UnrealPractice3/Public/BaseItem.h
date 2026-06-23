#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInterface.h"
#include "BaseItem.generated.h"

UCLASS()
class UNREALPRACTICE3_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	ABaseItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemType;

	// IItemInterface에서 요구하는 함수들을 반드시 구현
	virtual void OnItemOverlap(AActor* OverlapActor) override;
	virtual void OnItemEndOverlap(AActor* OverlapActor) override;
	virtual void ActivateItem(AActor* Activator) override;
	virtual FName GetItemType() const override;

	// 아이템을 제거하는 공통 함수 (추가 이펙트나 로직을 넣을 수 있음)
	virtual void DestroyItem();

};
