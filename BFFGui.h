#pragma once
#include <wx/wx.h>

class BFFGui : public wxFrame
{
public:
	BFFGui();
	~BFFGui();

private:
	std::unique_ptr<wxChoice> context_choice_;
	std::unique_ptr<wxChoice> device_choice_;
	wxTextCtrl textlog_;
	wxTextCtrl thread_count_;
	wxStaticText context_label_;
	wxStaticText device_label_;
	wxStaticText thread_count_label_;
};