int g_emgBrake; // ���m�b�`
int g_svcBrake; // ��p�ő�m�b�`
int g_brakeNotch; // �u���[�L�m�b�`
int g_powerNotch; // �͍s�m�b�`
int g_reverser; // ���o�[�T
bool g_pilotlamp; // �p�C���b�g�����v

#ifdef RPB_STEP8
#define RPB_STEP 8
#else
#define RPB_STEP 7
#endif

RpbIni g_ini;
ATS_HANDLES g_output; // �o��
