#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include "screen.h"

class CMidPointTerrain;

class CWindow : public CSingleton<CWindow>
{
public:
	/* OpenGL Functions */
	CWindow();
	CWindow(const std::string& stTitle, const GLuint& width = DEFAULT_WINDOW_WIDTH, const GLuint& height = DEFAULT_WINDOW_HEIGHT, const bool& bIsFullScreen = false);
	~CWindow();

	bool InitializeWindow(const std::string& stTitle = "My World", const GLuint& width = DEFAULT_WINDOW_WIDTH, const GLuint& height = DEFAULT_WINDOW_HEIGHT, const bool& bIsFullScreen = false);
	bool InitializeGLAD();
	bool WindowLoop() const;
	void WindowSwapAndBufferEvents();
	GLFWwindow* GetWindow();
	void ProcessInput(float deltaTime);
	void SetWindowIcon(const std::string& stIconFileName);

	GLuint GetWidth() const;
	GLuint GetHeight() const;
	void SetWidth(GLuint iWidth);
	void SetHeight(GLuint iHeight);

	void ResizeWindow(GLuint iWidth, GLuint iHeight);

	void SetCamera(CCamera* pCamera);
	CCamera* GetCamera();

	void SetScreen(CScreen* pScreen);
	CScreen* GetScreen();

	void SetBrushType(EBrushType eNewBrush);
	const EBrushType& GetBrushType() const;

	void SetMidPointTerrain(CMidPointTerrain* pMidPointTerrain);
	CMidPointTerrain* GetMidPointTerrain();

	void SetFrameBufferObject(CFrameBuffer* pFBO);
	CFrameBuffer* GetFrameBufferObject();

protected:
	void Destroy();

	// glfw: whenever the window size changed (by OS or user resize) this callback function executes
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void keys_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

	static GLuint m_uiRandSeed;

public: // Singleton Classes
	CCameraManager camera_manager;

private:
	GLFWwindow* m_pWindow;
	GLuint m_uiWidth;
	GLuint m_uiHeight;
	std::string m_stWindowName;
	bool m_bIsFullScreen;
	bool m_bIsMouseFocusedIn;
	bool m_bIsWireFrame;
	std::array<bool, 2> m_bMouseState;
	std::array<bool, 1024> m_bKeyBools;
	CCamera* m_pCamera;
	CScreen* m_pScreen;
	CMidPointTerrain* m_pTerrain;

	EBrushType m_eBrushType;

	CFrameBuffer* m_pFrameBufObj;
};