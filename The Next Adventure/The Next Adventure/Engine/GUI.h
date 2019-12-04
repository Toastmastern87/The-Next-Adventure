#pragma once

#include <vector>
#include "System.h"

namespace Toast 
{
	class GUIPanel;

	class GUI 
	{
	public:
		GUI();
		~GUI();

		void Draw(D3D &d3d);
		void AddPanel(float startX, float startY, float width, float height, std::string* title = nullptr, bool titleBar = false);

		std::vector<Toast::GUIPanel*> mGUIPanels;
	};
}