#include "GUI.h"
#include "GUIPanel.h"
#include "GUICursor.h"

namespace Toast 
{
	GUI::GUI() 
	{
	}

	GUI::~GUI() 
	{
	}

	void GUI::AddPanel(float startX, float startY, float width, float height, std::string* title, bool titleBar)
	{
		GUIPanel *guiPanel = new GUIPanel();
		Toast::System::tSys->mResources->mGUIPanels.push_back(guiPanel);

		guiPanel->CreatePanel(startX, startY, width, height, title, titleBar);

		mGUIPanels.push_back(guiPanel);
	}

	void GUI::AddCursor(POINT pos, float width, float height)
	{
		mCursor = new GUICursor();

		mCursor->CreateCursor(pos, width, height);
	}

	void GUI::Draw(D3D &d3d)
	{
		for (int i = 0; i < mGUIPanels.size(); i++) 
		{
			if (mGUIPanels[i]->mVisible) 
			{
				d3d.mDeviceContext->IASetInputLayout(this->mGUIPanels[i]->mMaterial->mInputLayout);
				mGUIPanels[i]->Draw(d3d);
			}
		}

		d3d.mDeviceContext->IASetInputLayout(this->mCursor->mMaterial->mInputLayout);
		mCursor->Draw(d3d);
	}
}