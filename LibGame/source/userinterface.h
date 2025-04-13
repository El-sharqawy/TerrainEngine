#pragma once

#include "../../LibGL/source/window.h"
#include "../../LibTerrain/source/object.h"

#include <list>

class CUserInterface : public CObject
{
public:
	CUserInterface(CWindow* pWindow);
	~CUserInterface();

	virtual void Render();
	virtual void Update();

	void AddObject(CObject* pObject);

private:
	CWindow* m_pWindow;
	std::list<CObject*> m_lObjectsUI;

	bool m_bShowDemonWindow;
	bool m_bShowAboutInfo;
};