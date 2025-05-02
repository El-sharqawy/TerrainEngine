#include "stdafx.h"
#include "window.h"
#include "screen.h"
#include "../../LibTerrain/source/terrain.h"
#include "../../LibTerrain/source/skybox.h"

static CWindow* appWnd = nullptr;
GLuint CWindow::m_uiRandSeed = 0;

CWindow::CWindow()
{
	m_pWindow = nullptr;
	m_pFrameBufObj = nullptr;
	m_uiWidth = DEFAULT_WINDOW_WIDTH;
	m_uiHeight = DEFAULT_WINDOW_HEIGHT;
	m_stWindowName = "NoWindow";
	m_bIsFullScreen = false;
	m_bIsWireFrame = false;

	m_bKeyBools = { false };

	if (appWnd)
	{
		printf("appWnd already initialized\n");
		exit(1);
	}
	appWnd = this;

	m_pCamera = CCameraManager::Instance().GetCurrentCamera();
	m_bMouseState.at(0) = GLFW_RELEASE;
	m_bMouseState.at(1) = GLFW_RELEASE;
	m_bIsMouseFocusedIn = true;
	m_eBrushType = BRUSH_TYPE_NONE;
#if defined(_WIN64)
	m_uiRandSeed = 0;
#else
	m_uiRandSeed = getpid();
#endif
}

CWindow::CWindow(const std::string& stTitle, const GLuint& width, const GLuint& height, const bool& bIsFullScreen)
{
	m_uiWidth = width;
	m_uiHeight = height;
	m_stWindowName = stTitle;
	m_bIsFullScreen = bIsFullScreen;
	m_bIsWireFrame = false;

	m_bKeyBools = { false };

	if (appWnd)
	{
		printf("appWnd already initialized\n");
		exit(1);
	}
	appWnd = this;

	m_pCamera = CCameraManager::Instance().GetCurrentCamera();

	InitializeWindow(stTitle, width, height, bIsFullScreen);
}

bool CWindow::InitializeWindow(const std::string& stTitle, const GLuint& width, const GLuint& height, const bool& bIsFullScreen)
{
	if (!glfwInit())
	{
		sys_err("Failed To Initialize GLFW.");
		glfwTerminate();
		return (false);
	}

	int Major, Minor, Rev;
	glfwGetVersion(&Major, &Minor, &Rev);
	sys_log("GLFW %d.%d.%d initialized", Major, Minor, Rev);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA

	if (bIsFullScreen)
	{
		m_pWindow = glfwCreateWindow(width, height, stTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}
	else
	{
		m_pWindow = glfwCreateWindow(width, height, stTitle.c_str(), nullptr, nullptr);
	}

	if (!m_pWindow)
	{
		sys_err("Failed to Initialize GL Window.");
		glfwTerminate();
		return (false);
	}

	glfwMakeContextCurrent(m_pWindow);

	if (!InitializeGLAD())
	{
		glfwTerminate();
		return (false);
	}

	glfwSetFramebufferSizeCallback(m_pWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(m_pWindow, mouse_callback);
	glfwSetScrollCallback(m_pWindow, scroll_callback);
	glfwSetKeyCallback(m_pWindow, keys_callback);
	glfwSetCursorPos(m_pWindow, static_cast<double>(width) / 2, static_cast<double>(height) / 2);
	glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);
	//glDebugMessageCallback(message_callback, nullptr);


	m_bIsMouseFocusedIn = true;
	m_bMouseState.at(0) = GLFW_RELEASE;
	m_bMouseState.at(1) = GLFW_RELEASE;

	if (m_bIsMouseFocusedIn)
	{
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	SetWindowIcon("resources/icon/terrain.png");

	return (true);
}

bool CWindow::InitializeGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		sys_err("Failed to Initialize GLAD.");
		return (false);
	}
	if (!GLAD_GL_ARB_bindless_texture) {
		sys_err("Bindless textures not supported!");
		return (false);
	}

	return (true);
}

bool CWindow::WindowLoop() const
{
	return (!glfwWindowShouldClose(m_pWindow));
}

/**
 * Forget to call this function in main loop and the app wont work.
 */
void CWindow::WindowSwapAndBufferEvents()
{
	glfwSwapBuffers(m_pWindow);
	glfwPollEvents();
}

GLFWwindow* CWindow::GetWindow()
{
	return (m_pWindow);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void CWindow::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	appWnd->ResizeWindow(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

void CWindow::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (appWnd->m_bIsMouseFocusedIn)
		{
			glfwSetInputMode(appWnd->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			appWnd->m_bIsMouseFocusedIn = false;
			appWnd->GetCamera()->SetLock(true);
		}
		else
		{
			glfwSetInputMode(appWnd->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			appWnd->m_bIsMouseFocusedIn = true;
			appWnd->GetCamera()->SetLock(false);
		}
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		CScreen::Instance().ApplyTerrainBrush(appWnd->GetBrushType());
	}
}

void CWindow::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (appWnd->GetCamera())
	{
		appWnd->GetCamera()->ProcessMouseMovement(static_cast<float>(xpos), static_cast<float>(ypos));
	}
}

void CWindow::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (appWnd->GetCamera())
	{
		appWnd->GetCamera()->ProcessMouseScroll(static_cast<float>(yoffset));
	}
}

void CWindow::ProcessInput(float deltaTime)
{
	if (glfwGetKey(m_pWindow, GLFW_KEY_W))
	{
		GetCamera()->ProcessKeyboardInput(DIRECTION_FORWARD, deltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_S))
	{
		GetCamera()->ProcessKeyboardInput(DIRECTION_BACKWARD, deltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_D))
	{
		GetCamera()->ProcessKeyboardInput(DIRECTION_RIGHT, deltaTime);
	}
	if (glfwGetKey(m_pWindow, GLFW_KEY_A))
	{
		GetCamera()->ProcessKeyboardInput(DIRECTION_LEFT, deltaTime);
	}
}

void CWindow::keys_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	bool bHandled = true;

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			appWnd->m_bKeyBools[key] = true;
		else if (action == GLFW_RELEASE)
			appWnd->m_bKeyBools[key] = false;
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, true);
			break;
			
		case GLFW_KEY_Q:
			glfwSetWindowShouldClose(window, true);
			break;

		case GLFW_KEY_X:
			appWnd->GetCamera()->SetLock(!appWnd->GetCamera()->IsLocked());
			break;

		case GLFW_KEY_LEFT_CONTROL:
			appWnd->m_bIsWireFrame = !appWnd->m_bIsWireFrame;
			if (appWnd->m_bIsWireFrame)
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
			break;
		}
	}
}

GLuint CWindow::GetWidth() const
{
	return (m_uiWidth);
}

GLuint CWindow::GetHeight() const
{
	return (m_uiHeight);
}

void CWindow::SetWidth(GLuint iWidth)
{
	m_uiWidth = iWidth;
}

void CWindow::SetHeight(GLuint iHeight)
{
	m_uiHeight = iHeight;
}

void CWindow::ResizeWindow(GLuint iWidth, GLuint iHeight)
{
	m_uiWidth = iWidth;
	m_uiHeight = iHeight;

	GetFrameBuffer()->BindForWriting();
	glViewport(0, 0, iWidth, iHeight);
	GetFrameBuffer()->UnBindWriting();
}

void CWindow::Destroy()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

CWindow::~CWindow()
{
	Destroy();
}

void CWindow::SetWindowIcon(const std::string& stIconFileName)
{
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	unsigned char* data = stbi_load(stIconFileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format = GL_RGBA;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		GLFWimage images[1]{};
		images[0].width = width;
		images[0].height = height;
		images[0].pixels = data;

		glfwSetWindowIcon(m_pWindow, 1, images);
		sys_log("CWindow::SetWindowIcon Loaded Icon: %s", stIconFileName.c_str());
	}
	else
	{
		sys_err("Failed to Load Icon: %s", stIconFileName.c_str());
	}
	stbi_image_free(data);
}


void CWindow::SetCamera(CCamera* pCamera)
{
	m_pCamera = pCamera;
}

CCamera* CWindow::GetCamera()
{
	assert(m_pCamera);
	return m_pCamera;
}

void CWindow::SetBrushType(EBrushType eNewBrush)
{
	m_eBrushType = eNewBrush;
}

const EBrushType& CWindow::GetBrushType() const
{
	return (m_eBrushType);
}

void CWindow::SetFrameBuffer(CFrameBuffer* pFBO)
{
	m_pFrameBufObj = pFBO;
}

CFrameBuffer* CWindow::GetFrameBuffer()
{
	assert(m_pFrameBufObj);
	return (m_pFrameBufObj);
}