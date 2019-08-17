#include "BFFGui.h"
#include <iostream>

BFFGui::BFFGui() : 
	wxFrame(nullptr, wxID_ANY, "Bedrock Finder", wxPoint(50, 50), wxSize(1000, 600)),
	textlog_(this, 1, wxEmptyString, wxPoint(660, 20), wxSize(300, 520), wxTE_MULTILINE | wxTE_READONLY),
	context_label_(this, 2, wxString("CONTEXT"), wxPoint(20, 20), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	device_label_(this, 2, wxString("DEVICE"), wxPoint(20, 100), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	thread_count_label_(this, 2, wxString("THREAD COUNT"), wxPoint(20, 180), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	thread_count_(this, 2, wxEmptyString, wxPoint(20, 210), wxSize(180, 30))
{
	context_label_.SetFont(wxFont(wxFontInfo(12)));
	device_label_.SetFont(wxFont(wxFontInfo(12)));
	thread_count_label_.SetFont(wxFont(wxFontInfo(12)));

	wxString contexts[5] = { wxString("1.12 Overworld"), wxString("1.13 Overworld"), wxString("1.14 Overworld"), wxString("1.13 Nether"), wxString("1.14 Nether") };
	context_choice_ = std::make_unique<wxChoice>(this, 1, wxPoint(20, 50), wxSize(180, 30), 5, contexts);
	context_choice_->SetSelection(0);

	wxString devices[2] = { wxString("CPU"), wxString("GPU") };
	device_choice_ = std::make_unique<wxChoice>(this, 2, wxPoint(20, 130), wxSize(180, 30), 2, devices);
	device_choice_->SetSelection(0);

	textlog_ << "bunch of random ass text";
	textlog_ << "more random ass text\n";

	for (int i = 0; i < 100; i++)
		textlog_ << i << "\n";

}
BFFGui::~BFFGui()
{
}