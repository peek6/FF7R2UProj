#pragma once
#include "CoreMinimal.h"
#include "EFieldActionActorType.generated.h"

UENUM()
enum class EFieldActionActorType : int8 {
    None,
    Ladder,
    Door,
    CrouchedMove,
    CrackMove,
    AutoDoor,
    BalanceMove,
    Cocoon,
    Jump,
    Trap,
    MonkeyBars,
    OperationBoard,
    VendingMachine,
    DuctMove,
    BenchBreak,
    HookShot,
    Sliding,
    SlideDown,
    PassOver,
    ParkourJump,
    CrackFan,
    Climb,
    TarzanJump,
    CardKeyDoor,
    JukeBox,
    HangMove,
    Curtain,
    PassThrough,
    RubbleJump,
    HeavyDoor,
    WMElevator,
    BarCounter,
    Evade,
    PushBox,
    ShinraElevator,
    WallRun,
    Peek,
    TarzanRope,
    WireMeshMove,
    PoleSlider,
    LiftingRope,
    Common,
    Swimming,
    Max,
};

