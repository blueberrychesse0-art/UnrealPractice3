#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveRow.generated.h"

USTRUCT(BlueprintType)
struct FWaveRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Wave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SpawnCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName SpawnSet;
};
