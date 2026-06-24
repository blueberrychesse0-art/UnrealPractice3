#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class UNREALPRACTICE3_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnRandomItem();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    USceneComponent* Scene;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
    UBoxComponent* SpawningBox;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
    UDataTable* ItemDataTable;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    FVector GetRandomPointInVolume() const;
    FItemSpawnRow* GetRandomItem() const;

    UFUNCTION(BlueprintCallable, Category = "Spawning")
    AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

};
