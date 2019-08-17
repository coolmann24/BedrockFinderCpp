#pragma once
#include <wx/wx.h>
#include "BFFGui.h"

class BFFApp : public wxApp
{
public:
	BFFApp();
	~BFFApp();

public:
	virtual bool OnInit();

private:
	BFFGui* gui;
};