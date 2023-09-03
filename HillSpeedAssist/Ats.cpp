#include "stdafx.h"
#include <windows.h>
#include "atsplugin.h"
#include "HsaIni.h"
#include "Ats.h"

BOOL APIENTRY DllMain(HANDLE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved)
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
			if (!g_ini.load(filePath)){g_ini.save();}
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
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int* panel, int* sound)
{
	g_speed = vehicleState.Speed;

	// ハンドル出力
	if (g_pilotlamp == true)
	{
		if (g_keyState == true && g_brakeNotch <= g_ini.Config.BrakeNotch && g_speed < 1.0f)
		{
			g_output.Brake = g_ini.Config.BrakeNotch;
		}
		else
		{
			g_output.Brake = g_brakeNotch;
		}
	}
	else
	{
		g_output.Brake = g_brakeNotch;
	}
	g_output.Reverser = g_reverser;
	g_output.Power = g_powerNotch;
	g_output.ConstantSpeed = ATS_CONSTANTSPEED_CONTINUE;

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
	if (atsKeyCode == KeyIndex(g_ini.Config.Key))
		g_keyState = true;
}

ATS_API void WINAPI KeyUp(int atsKeyCode)
{
	if (atsKeyCode == KeyIndex(g_ini.Config.Key))
		g_keyState = false;
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
