#include "BFFGui.h"
#include <iostream>

BFFGui::BFFGui() : 
	wxFrame(nullptr, wxID_ANY, "Bedrock Finder", wxPoint(50, 50), wxSize(1000, 600)),
	textlog_(this, 1, wxEmptyString, wxPoint(600, 20), wxSize(340, 520), wxTE_MULTILINE | wxTE_READONLY),

	context_label_(this, 2, wxString("CONTEXT"), wxPoint(50, 100), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	device_label_(this, 2, wxString("DEVICE"), wxPoint(50, 20), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	thread_count_label_(this, 2, wxString("THREAD COUNT"), wxPoint(50, 180), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	thread_count_(this, 2, wxString("1"), wxPoint(50, 210), wxSize(180, 30)),

	x_label_(this, 2, wxString("X MIN --- X MAX"), wxPoint(50, 260), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	x_min_(this, 2, wxString("-1000"), wxPoint(50, 290), wxSize(90, 30)),
	x_max_(this, 2, wxString("1000"), wxPoint(140, 290), wxSize(90, 30)),

	y_label_(this, 2, wxString("Y MIN --- Y MAX"), wxPoint(50, 340), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	y_min_(this, 2, wxString("4"), wxPoint(50, 370), wxSize(90, 30)),
	y_max_(this, 2, wxString("4"), wxPoint(140, 370), wxSize(90, 30)),

	z_label_(this, 2, wxString("Z MIN --- Z MAX"), wxPoint(50, 420), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	z_min_(this, 2, wxString("-1000"), wxPoint(50, 450), wxSize(90, 30)),
	z_max_(this, 2, wxString("1000"), wxPoint(140, 450), wxSize(90, 30)),

	numresults_label_(this, 2, wxString("MAX RESULTS (GPU)"), wxPoint(310, 20), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	numresults_(this, 2, wxString("100"), wxPoint(310, 50), wxSize(180, 30)),

	relative_coords_label_(this, 2, wxString("RELATIVE COORDS\n X ---- Y ---- Z"), wxPoint(310, 100), wxSize(180, 40), wxALIGN_CENTRE_HORIZONTAL),
	relative_x_(this, 2, wxString("0"), wxPoint(310, 150), wxSize(60, 30)),
	relative_y_(this, 2, wxString("0"), wxPoint(370, 150), wxSize(60, 30)),
	relative_z_(this, 2, wxString("0"), wxPoint(430, 150), wxSize(60, 30)),

	add_(this, 2, wxString("ADD"), wxPoint(310, 200), wxSize(90, 30)),
	remove_(this, 2, wxString("REMOVE"), wxPoint(400, 200), wxSize(90, 30)),
	show_(this, 2, wxString("SHOW"), wxPoint(310, 230), wxSize(90, 30)),
	reset_(this, 2, wxString("RESET"), wxPoint(400, 230), wxSize(90, 30)),

	search_label_(this, 2, wxString("SEARCH"), wxPoint(310, 300), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	search_(this, 2, wxString("BEGIN"), wxPoint(310, 330), wxSize(180, 30)),

	progress_label_(this, 2, wxString("PROGRESS (CPU)"), wxPoint(310, 420), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	progress_(this, 2, wxString("0%"), wxPoint(310, 460), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL)
{
	context_label_.SetFont(wxFont(wxFontInfo(12)));
	device_label_.SetFont(wxFont(wxFontInfo(12)));
	thread_count_label_.SetFont(wxFont(wxFontInfo(12)));
	x_label_.SetFont(wxFont(wxFontInfo(12)));
	y_label_.SetFont(wxFont(wxFontInfo(12)));
	z_label_.SetFont(wxFont(wxFontInfo(12)));
	numresults_label_.SetFont(wxFont(wxFontInfo(12)));
	relative_coords_label_.SetFont(wxFont(wxFontInfo(12)));
	search_label_.SetFont(wxFont(wxFontInfo(12)));
	progress_label_.SetFont(wxFont(wxFontInfo(12)));
	progress_.SetFont(wxFont(wxFontInfo(12)));

	wxString contexts[5] = { wxString("1.12 Overworld"), wxString("1.13 Overworld"), wxString("1.14 Overworld"), wxString("1.13 Nether"), wxString("1.14 Nether") };
	context_choice_ = std::make_unique<wxChoice>(this, 1, wxPoint(50, 130), wxSize(180, 30), 5, contexts);
	context_choice_->SetSelection(0);

	wxString devices[2] = { wxString("CPU"), wxString("GPU") };
	device_choice_ = std::make_unique<wxChoice>(this, 2, wxPoint(50, 50), wxSize(180, 30), 2, devices);
	device_choice_->SetSelection(0);

	textlog_ << "bunch of random ass text";
	textlog_ << "more random ass text\n";

	for (int i = 0; i < 100; i++)
		textlog_ << i << "\n";

}
BFFGui::~BFFGui()
{
}