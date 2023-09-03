// JR�e�� EB���u
// Copyright (C) 2009 Unicorn

int g_pitch;

#ifndef ATS_EB_HPP_INCLUDED
#define ATS_EB_HPP_INCLUDED

class CEb
{
private:
	int m_tmrPhase1; // 60�b�^�C�}�[
	int m_tmrPhase2; // 5�b�^�C�}�[
	int m_ebSwitch; // EB�X�C�b�`

public:
	int EmergencyNotch; // ���m�b�`
	int *Time; // ���ݎ���
	float *TrainSpeed; // ���x�v�̑��x[km/h]
	int Emergency; // EB���
	int Action; // EB�쓮
	int RedLamp; // ATS�ԐF��
	int Bell; // ATS�x��
	int Buzzer; // EB�u�U�[
	int EbSwitch; // EB�X�C�b�`
	int DisableCompany; //EB�����̉��

	// Initalize�Ŏ��s���܂�
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

	// Elapse�Ŏ��s���܂�
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

	// �e�֐��C�x���g�Ŏ��s���܂�
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

	// SetBrake�Ŏ��s���܂�
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
