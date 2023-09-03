int g_emgBrake; // 非常ノッチ
int g_svcBrake; // 常用最大ノッチ
int g_brakeNotch; // ブレーキノッチ
int g_powerNotch; // 力行ノッチ
int g_reverser; // レバーサ
bool g_pilotlamp; // パイロットランプ

#ifdef RPB_STEP8
#define RPB_STEP 8
#else
#define RPB_STEP 7
#endif

RpbIni g_ini;
ATS_HANDLES g_output; // 出力
