// 以下の ifdef ブロックは DLL から簡単にエクスポートさせるマクロを作成する標準的な方法です。 
// この DLL 内のすべてのファイルはコマンドラインで定義された ATS_EXPORTS シンボル
// でコンパイルされます。このシンボルはこの DLL が使用するどのプロジェクト上でも未定義でなけ
// ればなりません。この方法ではソースファイルにこのファイルを含むすべてのプロジェクトが DLL 
// からインポートされたものとして ATS_API 関数を参照し、そのためこの DLL はこのマク 
// ロで定義されたシンボルをエクスポートされたものとして参照します。
//#pragma data_seg(".shared")
//#pragma data_seg()

int g_emgBrake; // 非常ノッチ
int g_svcBrake; // 常用最大ノッチ
int g_brakeNotch; // ブレーキノッチ
int g_powerNotch; // 力行ノッチ
int g_svcNotch; // 最大ノッチ
int g_reverser; // レバーサ
bool g_pilotlamp; // パイロットランプ
int g_time; // 現在時刻
float g_speed; // 速度計の速度[km/h]
int g_deltaT; // フレーム時間[ms/frame]
int beforepos; //1フレーム前のノッチ段数
int cscstate; //定速の状態
int cscdisp; //定速表示灯
int cscdisp_lag[200]; //定速表示灯
int timer_lag[200]; //タイマー
bool BSkey; //BackSpace押下判定

int ini_cscEnable; //定速有効速度
int ini_cscDisable; //定速無効速度
int ini_csc_TM; //ハンドル定速有効速度
int ini_cscMode; //定速モード

ATS_HANDLES g_output; // 出力
CscIni g_ini; //INIファイル