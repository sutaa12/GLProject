#include "Renderer.h"
#include "Object.h"
HDC		CRenderer::OpenGLDevice	=NULL;
HGLRC	CRenderer::OpenGLContext=NULL;
CObject* CRenderer::pScenes[SCENE_MAX+1] = {NULL};
int CRenderer::SceneNum = 0;

HRESULT CRenderer::Init(HWND hWnd)
{
	// OpenGL初期化
	// ピクセルフォーマット初期化
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
		PFD_TYPE_RGBA, //ピクセルフォーマットの種類
		32, //フレームバッファの深度(?)
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, //深度バッファに使用するビット数
		8, //ステンシルバッファに使用するビット数
		0, //補助バッファの数
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	OpenGLDevice = GetDC(hWnd);

	int format = ChoosePixelFormat(OpenGLDevice, &pfd);

	if (format == 0)
	{
		return E_FAIL; // 該当するピクセルフォーマットが無い
	}

	// OpenGLが使うデバイスコンテキストに指定のピクセルフォーマットをセット
	if (!SetPixelFormat(OpenGLDevice, format, &pfd))
	{
		return E_FAIL; // DCへフォーマットを設定するのに失敗
	}

	// OpenGLのコンテキストを作成
	OpenGLContext = wglCreateContext(OpenGLDevice);
	// 作成されたコンテキストが現在使用中のコンテキストか？
	if (!wglMakeCurrent(OpenGLDevice, OpenGLContext))
	{
		return E_FAIL; // 何か正しくない
	}
	//OpenGL描画設定
	glEnable(GL_CULL_FACE);//カリングする
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	SceneNum = 0;
	for(int cnt=0;cnt<SCENE_MAX+1;cnt++)
	{
		pScenes[cnt]=NULL;
	}
	return S_OK;
}

void CRenderer::Uninit(HWND hWnd)
{
	CObject::ReleaseAll();
	// カレントコンテキストを無効にする
	wglMakeCurrent(NULL, NULL);
	// カレントコンテキストを削除
	wglDeleteContext(OpenGLContext);
	// デバイスコンテキスト解放
	ReleaseDC(hWnd, OpenGLDevice);
}

void CRenderer::Update(void)
{
	CObject::UpdateAll();
}
void CRenderer::Draw(void)
{
	//画面のクリア
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//RGBA
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	CObject::DrawAll();

	SwapBuffers(OpenGLDevice);//バッファの入れ替え

}

void CRenderer::AddScene(CObject* Scene)
{
	if(SceneNum<SCENE_MAX)
	{
		pScenes[SceneNum] = Scene;
		SceneNum++;
	}
}