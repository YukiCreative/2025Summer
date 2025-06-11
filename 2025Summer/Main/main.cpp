#include "DxLib.h"
#include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Application& app = Application::GetInstance();

	// エラーが出てたら強制終了
	if (!app.Init()) return -1;
	app.Run();
	app.Tarminate();

	return 0;
}