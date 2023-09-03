#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include "atsplugin.h"
#include "EBIni.h"
#include "Eb.hpp"
#include "Ats.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved )
{
	switch (ul_reason_for_call)
	{
		{
		case DLL_PROCESS_ATTACH:
			g_eb.Time = &g_time;
			g_eb.TrainSpeed = &g_speed;

			//�t�@�C���p�X�i�[
			char filePath[_MAX_PATH + 1] = _T("");
			//����������ւ̃|�C���^
			char* posIni;
			//Ats.dll�̃t�@�C���p�X���擾
			::GetModuleFileName((HMODULE)hModule, filePath, _MAX_PATH);
			//�p�X����.dll�̈ʒu������
			posIni = strstr(filePath, ".dll");
			//.dll��.ini�ɒu��
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
	g_eb.EmergencyNotch = g_emgBrake;
	g_pitch = g_ini.Emulate.Pitch == 0 ? 375 : abs(g_ini.Emulate.Pitch);
}

ATS_API void WINAPI Initialize(int brake)
{
	g_speed = 0;
	g_eb.initialize();
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int *panel, int *sound)
{
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;
	g_eb.execute(); // EB���u

	// �n���h���o��
	if(g_eb.Emergency == true)
	{
		g_output.Power = 0;
		g_output.Brake = g_emgBrake;
		g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;
	}
	else
	{
		g_output.Power = g_powerNotch;
		g_output.Brake = g_brakeNotch;
	}
	g_output.Reverser = g_reverser;

	if(g_eb.Emergency > 0 && g_ini.SN.UseSN > 0)
	{
		panel[0] = 0; // ATS���F��
	}
	if (g_ini.Index.Blinking >= 0) { // ATS�ԐF��
		if (g_ini.SN.UseSN > 0) { panel[IndexChange(g_ini.Index.Blinking)] = max(panel[IndexChange(g_ini.Index.Blinking)], g_eb.RedLamp); }
		else { panel[IndexChange(g_ini.Index.Blinking)] = g_eb.RedLamp; }
	}
	if (g_ini.Index.Lighting >= 0) { panel[IndexChange(g_ini.Index.Lighting)] = g_eb.Action; } // EB�쓮

	if (g_ini.Index.Buzzer >= 0) { panel[IndexChange(g_ini.Index.Buzzer)] = g_eb.Buzzer; } // EB�u�U�[
	if (g_ini.Index.SW >= 0) { panel[IndexChange(g_ini.Index.SW)] = g_eb.EbSwitch; } // EB�X�C�b�`

	if (g_ini.MetroPI.UseMetroPI > 0)
	{
		switch (panel[g_ini.MetroPI.MasConKey])
		{
		case 0:
			g_eb.DisableCompany = g_ini.MetroPI.Empty;
			break;
		case 1:
			g_eb.DisableCompany = g_ini.MetroPI.Metro;
			break;
		case 2:
			g_eb.DisableCompany = g_ini.MetroPI.Tobu;
			break;
		case 3:
			g_eb.DisableCompany = g_ini.MetroPI.Tokyu;
			break;
		case 4:
			g_eb.DisableCompany = g_ini.MetroPI.Seibu;
			break;
		case 5:
			g_eb.DisableCompany = g_ini.MetroPI.Sotetsu;
			break;
		case 6:
			g_eb.DisableCompany = g_ini.MetroPI.JR;
			break;
		case 7:
			g_eb.DisableCompany = g_ini.MetroPI.Odakyu;
			break;
		case 8:
			g_eb.DisableCompany = g_ini.MetroPI.Toyo;
			break;
		default:
			g_eb.DisableCompany = 1;
			break;
		}
	}
	else
	{
		g_eb.DisableCompany = 1;
	}

	return g_output;
}

ATS_API void WINAPI SetPower(int notch)
{
	g_powerNotch = notch;
	g_eb.ResetPhase1();
}

ATS_API void WINAPI SetBrake(int notch)
{
	g_brakeNotch = notch;
	g_eb.ResetPhase1();
	g_eb.ResetPhase2(notch);
}

ATS_API void WINAPI SetReverser(int pos)
{
	g_reverser = pos;
}

ATS_API void WINAPI KeyDown(int atsKeyCode)
{
	if(atsKeyCode == KeyIndex(g_ini.Key.Switch)) // EB���Z�b�g
		g_eb.ResetPhase1(1);
}

ATS_API void WINAPI KeyUp(int atsKeyCode)
{
}

ATS_API void WINAPI HornBlow(int hornType)
{
	g_eb.ResetPhase1();
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
