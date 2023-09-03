// JR各社 EB装置
// Copyright (C) 2009 Unicorn

int g_pitch;

#ifndef ATS_EB_HPP_INCLUDED
#define ATS_EB_HPP_INCLUDED

class CEb
{
private:
	int m_tmrPhase1; // 60秒タイマー
	int m_tmrPhase2; // 5秒タイマー
	int m_ebSwitch; // EBスイッチ

public:
	int EmergencyNotch; // 非常ノッチ
	int *Time; // 現在時刻
	float *TrainSpeed; // 速度計の速度[km/h]
	int Emergency; // EB非常
	int Action; // EB作動
	int RedLamp; // ATS赤色灯
	int Bell; // ATSベル
	int Buzzer; // EBブザー
	int EbSwitch; // EBスイッチ
	int DisableCompany; //EB無効の会社

	// Initalizeで実行します
	void initialize(void)
	{
		Emergency = 0;
		Action = 0;
		RedLamp = 0;
		Bell = ATS_SOUND_STOP;
		Buzzer = ATS_SOUND_STOP;
		EbSwitch = ATS_SOUND_STOP;

		m_tmrPhase1 = *Time + 60000;
		m_tmrPhase2 = 0;
	}

	// Elapseで実行します
	void execute(void)
	{
		RedLamp = 0;
		Buzzer = ATS_SOUND_STOP;
		Bell = ATS_SOUND_STOP;

		if(*TrainSpeed == 0 || DisableCompany == 0)
		{
			ResetPhase1();
		}

		if(fabsf(*TrainSpeed) > 15 && *Time > m_tmrPhase1)
		{
			Action = 1;
			m_tmrPhase1 = *Time + 60000;
			if(m_tmrPhase2 == 0){m_tmrPhase2 = *Time + 5000;}
		}

		if(Action == 1 && *Time > m_tmrPhase2)
		{
			Emergency = 1;
			m_tmrPhase1 = *Time + 60000;
			m_tmrPhase2 = 0;
		}

		if(Action == 1)
		{
			Buzzer = ATS_SOUND_PLAYLOOPING;
		}

		if(Emergency == 1)
		{
			RedLamp = (*Time % (g_pitch * 2)) / g_pitch;
			Bell = ATS_SOUND_PLAYLOOPING;
		}

		EbSwitch = m_ebSwitch;
		m_ebSwitch = ATS_SOUND_CONTINUE;
	}

	// 各関数イベントで実行します
	void ResetPhase1(int is_sw=0)
	{
		if(Emergency != 1)
		{
			Action = 0;
			m_tmrPhase1 = *Time + 60000;
			m_tmrPhase2 = 0;
		}

		if(is_sw == 1)
		{
			m_ebSwitch = ATS_SOUND_PLAY;
		}
	}

	// SetBrakeで実行します
	void ResetPhase2(int notch)
	{
		if(notch == EmergencyNotch && *TrainSpeed == 0)
		{
			m_tmrPhase1 = *Time + 60000;
			m_tmrPhase2 = 0;
			Action = 0;
			Emergency = 0;
		}
	}

};	// CEb

#endif	// ATS_EB_HPP_INCLUDED
