#define WIPER_MOVE_TIMEMS 1087
//2304�ǋL
#define WIPER_SWITCH ATS_KEY_K
#define WIPER_L_INDEX 97
#define WIPER_R_INDEX 96

int g_emgBrake; // ���m�b�`
int g_svcBrake; // ��p�ő�m�b�`
int g_brakeNotch; // �u���[�L�m�b�`
int g_powerNotch; // �͍s�m�b�`
int g_reverser; // ���o�[�T
bool g_pilotlamp; // �p�C���b�g�����v
int g_time; // ���ݎ���
float g_speed; // ���x�v�̑��x[km/h]

bool g_nfb; // ���C�p�[�X�C�b�`���
int g_state; // ���C�p�[�ғ����
int g_waiting; // ���ɏ�Ԃ��ς�鎞��

int g_wiper1; // ���@��1
int g_wiper2; // ���@��2

ATS_HANDLES g_output; // �o��
WIPERIni g_ini; //INI�t�@�C��

int KeyIndex(string InputKey)
{
	int n_InputKey;
	if (InputKey == _T("S"))
		n_InputKey = ATS_KEY_S;
	else if (InputKey == _T("A1"))
		n_InputKey = ATS_KEY_A1;
	else if (InputKey == _T("A2"))
		n_InputKey = ATS_KEY_A2;
	else if (InputKey == _T("B1"))
		n_InputKey = ATS_KEY_B1;
	else if (InputKey == _T("B2"))
		n_InputKey = ATS_KEY_B2;
	else if (InputKey == _T("C1"))
		n_InputKey = ATS_KEY_C1;
	else if (InputKey == _T("C2"))
		n_InputKey = ATS_KEY_C2;
	else if (InputKey == _T("D"))
		n_InputKey = ATS_KEY_D;
	else if (InputKey == _T("E"))
		n_InputKey = ATS_KEY_E;
	else if (InputKey == _T("F"))
		n_InputKey = ATS_KEY_F;
	else if (InputKey == _T("G"))
		n_InputKey = ATS_KEY_G;
	else if (InputKey == _T("H"))
		n_InputKey = ATS_KEY_H;
	else if (InputKey == _T("I"))
		n_InputKey = ATS_KEY_I;
	else if (InputKey == _T("J"))
		n_InputKey = ATS_KEY_J;
	else if (InputKey == _T("K"))
		n_InputKey = ATS_KEY_K;
	else if (InputKey == _T("L"))
		n_InputKey = ATS_KEY_L;
	else if (InputKey == _T("BS"))
		n_InputKey = 16;
	else
		n_InputKey = -1;

	return n_InputKey;
}