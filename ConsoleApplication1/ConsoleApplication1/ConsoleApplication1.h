// ���� ifdef ����� DLL���� ���������ϴ� �۾��� ���� �� �ִ� ��ũ�θ� ����� 
// ǥ�� ����Դϴ�. �� DLL�� ��� �ִ� ������ ��� ����ٿ� ���ǵ� _EXPORTS ��ȣ��
// �����ϵǸ�, �ٸ� ������Ʈ������ �� ��ȣ�� ������ �� �����ϴ�.
// �̷��� �ϸ� �ҽ� ���Ͽ� �� ������ ��� �ִ� �ٸ� ��� ������Ʈ������ 
// CONSOLEAPPLICATION1_API �Լ��� DLL���� �������� ������ ����, �� DLL��
// �� DLL�� �ش� ��ũ�η� ���ǵ� ��ȣ�� ���������� ������ ���ϴ�.
#ifdef CONSOLEAPPLICATION1_EXPORTS
#define CONSOLEAPPLICATION1_API __declspec(dllexport)
#else
#define CONSOLEAPPLICATION1_API __declspec(dllimport)
#endif

// �� Ŭ������ ConsoleApplication1.dll���� ������ ���Դϴ�.
class CONSOLEAPPLICATION1_API CConsoleApplication1 {
public:
	CConsoleApplication1(void);
	// TODO: ���⿡ �޼��带 �߰��մϴ�.
};

extern CONSOLEAPPLICATION1_API int nConsoleApplication1;

CONSOLEAPPLICATION1_API int fnConsoleApplication1(void);
