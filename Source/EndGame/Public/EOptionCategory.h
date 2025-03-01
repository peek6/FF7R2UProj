#pragma once
#include "CoreMinimal.h"
#include "EOptionCategory.generated.h"

UENUM(BlueprintType)
enum class EOptionCategory : uint8 {
    Difficulty,
    MenuCursor,
    BattleCursor,
    NaviMap,
    Subtitle,
    AutoTalk,
    BattleGuide,
    BrightnessSDR,
    BrightnessHDR,
    CameraDistanceField,
    CameraDistanceBattle,
    CameraMoveFollow,
    CameraAttackFollow,
    CameraSlopeFollow,
    CameraSpaceOffset,
    CameraSensitivity,
    CameraX,
    CameraY,
    FriendCommandCameraType,
    TargetLockOnKey,
    TargetLockOnType,
    SequenceNormalAttackTargetChange,
    Vibration,
    VibrationScreen,
    BGMVolume,
    SEVolume,
    VoiceVolume,
    VoiceLanguage,
    AreaNaviType,
    LocationNaviType,
    BattleChangeButton,
    MasterVolume,
    BackgroundGamepad,
    BattleMode,
    WindowMode,
    ScreenResolution,
    ZackNaviType,
    SkipMonsterGuide,
    PhsPadSpeaker,
    MotionSensor,
    StrengthHDR,
    IntensityHDR,
    MinLuminanceHDR,
    GraphicsMode,
    LODDistanceScale,
    TextureQuality,
    ShadowQuality,
    FPS,
    HDR,
    MemberShortcut,
    AutoCameraRotation,
    OldBattleMemberOrder,
    VibrationIntensityEvent,
    VibrationIntensityBattle,
    VibrationIntensityField,
    VibrationIntensityMenu,
    VibrationIntensityMiniGame,
    TextLanguage,
    AreaNaviMapScale,
    LocationNaviMapScale,
    ZackNaviMapScale,
    ParkourGuide,
    TriggerEffectIntensity,
    SongVolume,
    FieldCommandMember,
    BackgroundSound,
    FastForwardPlayRate,
    CastShadowDistScale,
    Display,
    MinScreenPercentage,
    GraphicsPreset,
    BgQuality,
    CharaQuality,
    AntiAliasing,
    DlssQuality,
    EffectQuality,
    PadSpeakerVolume,
    LowResolutionFont,
    MaxScreenPercentage,
    VSync,
    MouseSensitivity,
    CommandMenuWASD,
    OceanQuality,
    FogQuality,
    GamepadLeftStick,
    GamepadRightStick,
    GamepadDPad,
    GamepadFaceButtonLeft,
    GamepadFaceButtonTop,
    GamepadDecide,
    GamepadCancel,
    GamepadLeftShoulder,
    GamepadRightShoulder,
    GamepadLeftTrigger,
    GamepadRightTrigger,
    GamepadLeftThumb,
    GamepadRightThumb,
    GamepadSpecialLeft,
    GamepadSpecialRight,
    KeyboardLeftStickUp,
    KeyboardLeftStickDown,
    KeyboardLeftStickLeft,
    KeyboardLeftStickRight,
    KeyboardRightStickUp,
    KeyboardRightStickDown,
    KeyboardRightStickLeft,
    KeyboardRightStickRight,
    KeyboardPauseMenu,
    KeyboardCommandMenu,
    KeyboardAttack,
    KeyboardPrevRank,
    KeyboardNextRank,
    KeyboardMagnify,
    KeyboardFieldAction,
    KeyboardAccel,
    KeyboardDash,
    KeyboardVehicle,
    KeyboardCatch,
    KeyboardMapMenu,
    KeyboardShowStatus,
    KeyboardResetCamera,
    KeyboardPartySet1,
    KeyboardPartySet2,
    KeyboardPartySet3,
    KeyboardDodge,
    KeyboardUniqueAbility,
    KeyboardGuard,
    KeyboardTargetLock,
    KeyboardEnemyIntel,
    KeyboardShortcut5,
    KeyboardPrevTarget,
    KeyboardNextTarget,
    KeyboardPrevMember,
    KeyboardNextMember,
    KeyboardChangeLeaderPrev,
    KeyboardChangeLeaderNext,
    KeyboardShortcutMenu,
    KeyboardMember1ShortcutMenu,
    KeyboardMember2ShortcutMenu,
    KeyboardShortcut1,
    KeyboardShortcut2,
    KeyboardShortcut3,
    KeyboardShortcut4,
    KeyboardCoop1,
    KeyboardCoop2,
    KeyboardCoop3,
    KeyboardCoop4,
    KeyboardMember1Shortcut1,
    KeyboardMember1Shortcut2,
    KeyboardMember1Shortcut3,
    KeyboardMember1Shortcut4,
    KeyboardMember2Shortcut1,
    KeyboardMember2Shortcut2,
    KeyboardMember2Shortcut3,
    KeyboardMember2Shortcut4,
    KeyboardDPadUp,
    KeyboardDPadDown,
    KeyboardDPadLeft,
    KeyboardDPadRight,
    KeyboardDecide,
    KeyboardCancel,
    KeyboardLeftTrigger,
    KeyboardRightTrigger,
    KeyboardScrollUp,
    KeyboardScrollDown,
    KeyboardFaceButtonTop,
    KeyboardFaceButtonLeft,
    KeyboardLeftShoulder,
    KeyboardRightShoulder,
    KeyboardLeftThumb,
    KeyboardRightThumb,
    KeyboardSpecialRight,
    KeyboardSpecialLeft,
};

