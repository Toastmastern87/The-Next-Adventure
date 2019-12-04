#pragma once

#include <vector>
#include "System.h"

namespace Toast 
{
	class GUIPanel;
	class GUICursor;

	class GUI 
	{
	public:
		GUI();
		~GUI();

		void Draw(D3D &d3d);
		void AddPanel(float startX, float startY, float width, float height, std::string* title = nullptr, bool titleBar = false);
		void AddCursor(POINT pos, float width, float height);

		Toast::GUICursor* mCursor = nullptr;
		std::vector<Toast::GUIPanel*> mGUIPanels;
	};
}