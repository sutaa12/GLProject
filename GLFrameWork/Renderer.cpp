#include "Renderer.h"
#include "Object.h"
HDC		CRenderer::OpenGLDevice	=NULL;
HGLRC	CRenderer::OpenGLContext=NULL;
CObject* CRenderer::pScenes[SCENE_MAX+1] = {NULL};
int CRenderer::SceneNum = 0;

HRESULT CRenderer::Init(HWND hWnd)
{
	// OpenGL������
	// �s�N�Z���t�H�[�}�b�g������
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //Flags
		PFD_TYPE_RGBA, //�s�N�Z���t�H�[�}�b�g�̎��
		32, //�t���[���o�b�t�@�̐[�x(?)
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		24, //�[�x�o�b�t�@�Ɏg�p����r�b�g��
		8, //�X�e���V���o�b�t�@�Ɏg�p����r�b�g��
		0, //�⏕�o�b�t�@�̐�
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	OpenGLDevice = GetDC(hWnd);

	int format = ChoosePixelFormat(OpenGLDevice, &pfd);

	if (format == 0)
	{
		return E_FAIL; // �Y������s�N�Z���t�H�[�}�b�g������
	}

	// OpenGL���g���f�o�C�X�R���e�L�X�g�Ɏw��̃s�N�Z���t�H�[�}�b�g���Z�b�g
	if (!SetPixelFormat(OpenGLDevice, format, &pfd))
	{
		return E_FAIL; // DC�փt�H�[�}�b�g��ݒ肷��̂Ɏ��s
	}

	// OpenGL�̃R���e�L�X�g���쐬
	OpenGLContext = wglCreateContext(OpenGLDevice);
	// �쐬���ꂽ�R���e�L�X�g�����ݎg�p���̃R���e�L�X�g���H
	if (!wglMakeCurrent(OpenGLDevice, OpenGLContext))
	{
		return E_FAIL; // �����������Ȃ�
	}
	//OpenGL�`��ݒ�
	glEnable(GL_CULL_FACE);//�J�����O����
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
	// �J�����g�R���e�L�X�g�𖳌��ɂ���
	wglMakeCurrent(NULL, NULL);
	// �J�����g�R���e�L�X�g���폜
	wglDeleteContext(OpenGLContext);
	// �f�o�C�X�R���e�L�X�g���
	ReleaseDC(hWnd, OpenGLDevice);
}

void CRenderer::Update(void)
{
	CObject::UpdateAll();
}
void CRenderer::Draw(void)
{
	//��ʂ̃N���A
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);//RGBA
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	CObject::DrawAll();

	SwapBuffers(OpenGLDevice);//�o�b�t�@�̓���ւ�

}

void CRenderer::AddScene(CObject* Scene)
{
	if(SceneNum<SCENE_MAX)
	{
		pScenes[SceneNum] = Scene;
		SceneNum++;
	}
}