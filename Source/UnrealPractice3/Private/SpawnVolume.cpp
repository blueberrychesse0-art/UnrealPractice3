#include "SpawnVolume.h"
#include "MyGameState.h"
#include "WaveRow.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ASpawnVolume::ASpawnVolume()
{
    PrimaryActorTick.bCanEverTick = false;

    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    return BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
    );
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass) return nullptr;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
        ItemClass,
        GetRandomPointInVolume(),
        FRotator::ZeroRotator
    );

    return SpawnedActor;
}

AActor* ASpawnVolume::SpawnRandomItem(int32 lv, int32 wave)
{
    if (FItemSpawnRow* SelectedRow = GetRandomItem(lv, wave))
    {
        if (UClass* ActualClass = SelectedRow->ItemClass.Get())
        {
            return SpawnItem(ActualClass);
        }
    }

    return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem(int32 lv, int32 wave) const
{
    if (!ItemDataTable) return nullptr;

    if (!WaveTable) return nullptr;

    FName TargetSpawnSet = NAME_None;
    TArray<FWaveRow*> AllWaveRows;
    static const FString WaveContext(TEXT("WaveContext"));
    WaveTable->GetAllRows(WaveContext, AllWaveRows);

    for (const FWaveRow* WaveRow : AllWaveRows)
    {
        if (WaveRow && WaveRow->Level == lv && WaveRow->Wave == wave)
        {
            TargetSpawnSet = WaveRow->SpawnSet;
            break;
        }
    }

    if (TargetSpawnSet.IsNone()) return nullptr;

    TArray<FItemSpawnRow*> AllItemRows;
    static const FString ContextString(TEXT("ItemSpawnContext"));
    ItemDataTable->GetAllRows(ContextString, AllItemRows);
    
    if (AllItemRows.IsEmpty()) return nullptr;
    TArray<FItemSpawnRow*> FilteredItemRows;
    float TotalChance = 0.0f;

    for (FItemSpawnRow* ItemRow : AllItemRows)
    {
        if (ItemRow && ItemRow->SpawnSet == TargetSpawnSet)
        {
            FilteredItemRows.Add(ItemRow);
            TotalChance += ItemRow->SpawnChance;
        }
    }

    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    for (FItemSpawnRow* ItemRow : FilteredItemRows)
    {
        AccumulateChance += ItemRow->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return ItemRow;
        }
    }

    return nullptr;
}

int32 ASpawnVolume::GetSpawnCount(int32 lv, int32 wave) const
{
    if (!WaveTable) return 0;

    TArray<FWaveRow*> AllRows;
    static const FString ContextString(TEXT("WaveContext"));
    WaveTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return 0;

    for (const FWaveRow* Row : AllRows)
    {
        if (Row->Level == lv)
        {
            if (Row->Wave == wave)
            {
                return Row->SpawnCount;
            }
        }
    }

    return 0;
}

float ASpawnVolume::GetWaveDuration(int32 lv, int32 wave) const
{
    if (!WaveTable) return 0.0f;

    TArray<FWaveRow*> AllWaveRows;
    static const FString WaveContext(TEXT("WaveContext"));
    WaveTable->GetAllRows(WaveContext, AllWaveRows);

    if (AllWaveRows.Num() > 0)
    {
        for (const FWaveRow* WaveRow : AllWaveRows)
        {
            if (WaveRow && WaveRow->Level == lv && WaveRow->Wave == wave)
            {
                return WaveRow->Duration;
            }
        }
    }

    return 0.0f;
}
