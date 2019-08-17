#include "BFFApp.h"

wxIMPLEMENT_APP(BFFApp);

BFFApp::BFFApp()
{
}

BFFApp::~BFFApp()
{
}

bool BFFApp::OnInit()
{
	gui = new BFFGui();
	gui->Show();
	return true;
}