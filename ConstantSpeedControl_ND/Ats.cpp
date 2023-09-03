// Ats.cpp : DLL �A�v���P�[�V�����p�̃G���g�� �|�C���g���`���܂��B
// �ėp�葬�v���O�C���ł��B�}�X�R���L�[6�Ԃ̎��Ƀ{�^���葬�A���̑��̎��Ƀn���h���葬���g�p����悤�ɂȂ��Ă��܂��B
// (C) Line-16 2023

#include "stdafx.h"
#include "atsplugin.h"
#include "CscIni.h"
#include "Ats.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
		{
	case DLL_PROCESS_ATTACH:
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

ATS_API int WINAPI GetPluginVersion()
{
	return ATS_VERSION;
}

ATS_API void WINAPI SetVehicleSpec(ATS_VEHICLESPEC vehicleSpec)
{
	g_svcBrake = vehicleSpec.BrakeNotches;
	g_emgBrake = g_svcBrake + 1;
	g_svcNotch = min(vehicleSpec.PowerNotches, g_ini.Config.CSpdNotch);
	ini_cscDisable = g_ini.Config.CSpdDisableSpd;
	ini_cscEnable = g_ini.Config.CSpdEnableSpd;
	ini_csc_TM = g_ini.Config.CSpdND;
	ini_cscMode = g_ini.MetroPI.MasconKey;
}

ATS_API void WINAPI Initialize(int brake)
{
	g_speed = 0;
	cscstate = 0;
	int h;
	for (h = 0; h < 200; h = h + 1)
	{
		cscdisp_lag[h] = 0;
		timer_lag[h] = 0;
	}
}

ATS_API ATS_HANDLES WINAPI Elapse(ATS_VEHICLESTATE vehicleState, int *panel, int *sound)
{
	g_deltaT = vehicleState.Time - g_time;
	g_time = vehicleState.Time;
	g_speed = vehicleState.Speed;

	//����
	int p92 = ini_cscMode < 0 ? 6 : ini_cscMode > 255 ? 1 : panel[ini_cscMode];//���l�̓{�^���葬�A256�ȏ�̓n���h���葬�A0�`255�͂��̔ԍ���ǂݍ���

	// �n���h���o��
	g_output.Brake = g_brakeNotch;
	g_output.Reverser = g_reverser;
	g_output.Power = g_powerNotch;



	if (p92 == 6)//�{�^���葬
	{
		//BackSpace�L�[���m
		if (GetKeyState(0x08) < 0 && BSkey == true)//�O�t���[���ŉ�����Ă��Ȃ�
		{
			BSkey = false;
			if (g_speed > ini_cscEnable && g_powerNotch >= g_svcNotch)//55�L���ȏ�AP5�ŃX�C�b�`�����ꂽ�Ƃ��I��
			{
				cscstate = 1;//�葬�I��
			}
			else if (g_speed > ini_cscDisable && g_powerNotch >= g_svcNotch && cscstate == 1)//�ێ�
			{

			}
			else//55�L���ȉ��܂���P5�ȊO�ɂȂ����Ƃ��I�t
			{
				cscstate = 0;//�葬�I�t
			}
		}
		else if(GetKeyState(0x08) < 0)
		{
			if (cscstate == 1 && g_powerNotch >= g_svcNotch && g_speed > ini_cscDisable)//55�L���ȉ��܂���P5�ȊO�ɂȂ����Ƃ��I�t
			{
			}
			else//�ێ�
			{
				cscstate = 0;
			}
		}
		else
		{
			BSkey = true;
			if (cscstate == 1 && g_powerNotch >= g_svcNotch && g_speed > ini_cscDisable)//55�L���ȉ��܂���P5�ȊO�ɂȂ����Ƃ��I�t
			{
			}
			else//�ێ�
			{
				cscstate = 0;
			}
		}

		g_output.ConstantSpeed = cscstate == 1 ? ATS_CONSTANTSPEED_ENABLE : ATS_CONSTANTSPEED_DISABLE;		
	}
	else//�n���h���葬
	{
		if ((g_ini.Config.ReturnOnly == 0 || beforepos > 2) && g_powerNotch == 2 && g_speed > ini_csc_TM && cscstate == 0)//25�L���ȏ��P2�ɂȂ����Ƃ��I��
		{
			g_output.ConstantSpeed = ATS_CONSTANTSPEED_ENABLE;
			cscstate = 1;//�葬���䒆
		}
		else if (cscstate == 1 && g_powerNotch == 2 && g_speed > ini_csc_TM)
		{
			g_output.ConstantSpeed = ATS_CONSTANTSPEED_ENABLE;
		}
		else//25�L���ȉ��܂���P2�ȊO�ɂȂ����Ƃ��I�t
		{
			g_output.ConstantSpeed = ATS_CONSTANTSPEED_DISABLE;
			cscstate = 0;//�葬����
		}

	}
	beforepos = g_powerNotch;//1�t���[���O�̃m�b�`����

	// �p�l���o��
	int i;
	for (i = 198; i > -1; i = i - 1)
	{
		cscdisp_lag[i + 1] = cscdisp_lag[i];
		timer_lag[i + 1] = timer_lag[i];

		if (timer_lag[i + 1] > g_time)
			cscdisp = cscdisp_lag[i + 1];
	}

	cscdisp_lag[0] = cscstate;
	timer_lag[0] = g_time + 5000;
	if (abs(g_ini.Index.ConstantSpeedControl) <= 255) { panel[abs(g_ini.Index.ConstantSpeedControl)] = cscdisp * p92; } //�葬�\����

	// �T�E���h�o��
	// �Ȃ�

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
}

ATS_API void WINAPI KeyUp(int hornType)
{
}

ATS_API void WINAPI HornBlow(int atsHornBlowIndex)
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

ATS_API void WINAPI Load() {}
ATS_API void WINAPI Dispose() {}