#include "stdafx.h"
#include <windows.h>
#include "atsplugin.h"
#include "WIPERIni.h"
#include "Ats.h"

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
		{
	case DLL_PROCESS_ATTACH:
		//ファイルパス格納
		char filePath[_MAX_PATH + 1] = _T("");
		//検索文字列へのポインタ
		char* posIni;
		//Ats.dllのファイルパスを取得
		::GetModuleFileName((HMODULE)hModule, filePath, _MAX_PATH);
		//パスから.dllの位置を検索
		posIni = strstr(filePath, ".dll");
		//.dllを.iniに置換
		memmove(posIni, ".ini", 4);
		g_ini.load(filePath);
		if (!g_ini.load(filePath)) { g_ini.save(); }
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}

	return TRUE;
}

ATS_API void WINAPI Load()
{
}

ATS_API void WINAPI Dispose()
{
}

ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec)
{
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
}

ATS_API void WINAPI Initialize(int brake)
{
	g_nfb = 0; // ワイパースイッチ状態
	g_state = 0; // ワイパー稼働状態
	g_waiting = 0; // 次に状態が変わる時間
	g_wiper1 = ATS_SOUND_STOP; // 窓拭音1
	g_wiper2 = ATS_SOUND_STOP; // 窓拭音2
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int* panel, int* sound)
{
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;

	// ハンドル出力
	g_output.Brake = g_brakeNotch;
	g_output.Reverser = g_reverser;
	g_output.Power = g_powerNotch;
	g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

	if ((g_nfb == true || g_state == 1) && g_waiting <= g_time)
	{
		g_waiting = g_time + abs(g_ini.Emulate.Pitch);

		if (g_state == 0 && g_nfb == true)
		{
			g_state = 1;
			g_wiper1 = ATS_SOUND_PLAY;
		}
		else
		{
			g_state = 0;
			g_wiper2 = ATS_SOUND_PLAY;
		}
	}

	if (g_ini.Index.Right >= 0 && g_ini.Index.Right <= 255) { panel[g_ini.Index.Right] = g_state ? -1 : 0; } // 右定位
	if (g_ini.Index.Left >= 0 && g_ini.Index.Left <= 255) { panel[g_ini.Index.Left] = g_state ? 1 : 0; } // 左定位

	if (g_ini.Index.ON >= 0 && g_ini.Index.ON <= 255) { sound[g_ini.Index.ON] = g_wiper1; }
	g_wiper1 = ATS_SOUND_CONTINUE;
	if (g_ini.Index.OFF >= 0 && g_ini.Index.OFF <= 255) { sound[g_ini.Index.OFF] = g_wiper2; }
	g_wiper2 = ATS_SOUND_CONTINUE;

	return g_output;
}

ATS_API void WINAPI SetPower(int notch)
{
	g_powerNotch = notch;
}

ATS_API void WINAPI SetBrake(int notch)
{
	g_brakeNotch = notch;
}

ATS_API void WINAPI SetReverser(int pos)
{
	g_reverser = pos;
}

ATS_API void WINAPI KeyDown(int atsKeyCode)
{
	if(atsKeyCode == KeyIndex(g_ini.Key.Switch)) // 9key
		g_nfb = g_nfb > 0 ? 0 : 1;
}

ATS_API void WINAPI KeyUp(int atsKeyCode)
{
}

ATS_API void WINAPI HornBlow(int hornType)
{
}

ATS_API void WINAPI DoorOpen()
{
	g_pilotlamp = false;
}

ATS_API void WINAPI DoorClose()
{
	g_pilotlamp = true;
}

ATS_API void WINAPI SetSignal(int signal)
{
}

ATS_API void WINAPI SetBeaconData(ATS_BEACONDATA beaconData)
{
}
