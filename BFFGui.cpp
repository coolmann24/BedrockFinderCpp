#include "BFFGui.h"
#include "GPUBedrockSearch.h"
#include "BedrockSearch.h"
#include "BedrockGen.h"
#include <iostream>
#include <string>
#include <tuple>
#include <memory>
#include <wx/wx.h>


wxDEFINE_EVENT(LOG_EVENT, wxCommandEvent);
wxDEFINE_EVENT(SEARCH_END_EVENT, wxCommandEvent);
wxDEFINE_EVENT(LOG_PROGRESS_EVENT, wxCommandEvent);

wxBEGIN_EVENT_TABLE(BFFGui, wxFrame)
	EVT_BUTTON(1001, onAddClicked)
	EVT_BUTTON(1002, onRemoveClicked)
	EVT_BUTTON(1003, onShowClicked)
	EVT_BUTTON(1004, onResetClicked)
	EVT_BUTTON(1005, onSearch)
	EVT_COMMAND(1006, LOG_EVENT, log)
	EVT_COMMAND(1007, SEARCH_END_EVENT, endSearch)
	EVT_COMMAND(1008, LOG_PROGRESS_EVENT, logProgress)
wxEND_EVENT_TABLE()



BFFGui::BFFGui() : 
	wxFrame(nullptr, wxID_ANY, "Bedrock Finder", wxPoint(50, 50), wxSize(1000, 600)),
	textlog_(new wxTextCtrl(this, 1, wxEmptyString, wxPoint(600, 20), wxSize(340, 520), wxTE_MULTILINE | wxTE_READONLY)),

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

	numresults_label_(this, 2, wxString("MAX RESULTS"), wxPoint(310, 20), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	numresults_(this, 2, wxString("100"), wxPoint(310, 50), wxSize(180, 30)),

	relative_coords_label_(this, 2, wxString("RELATIVE COORDS\n X ---- Y ---- Z          "), wxPoint(310, 100), wxSize(180, 40), wxALIGN_CENTRE_HORIZONTAL),
	relative_x_(this, 2, wxString("0"), wxPoint(310, 150), wxSize(45, 30)),
	relative_y_(this, 2, wxString("0"), wxPoint(355, 150), wxSize(45, 30)),
	relative_z_(this, 2, wxString("0"), wxPoint(400, 150), wxSize(45, 30)),
	is_bedrock_(this, 2, wxString("Bedrock?"), wxPoint(460, 150), wxSize(90, 30)),

	add_(this, 1001, wxString("ADD"), wxPoint(310, 200), wxSize(90, 30)),
	remove_(this, 1002, wxString("REMOVE"), wxPoint(400, 200), wxSize(90, 30)),
	show_(this, 1003, wxString("SHOW"), wxPoint(310, 230), wxSize(90, 30)),
	reset_(this, 1004, wxString("RESET"), wxPoint(400, 230), wxSize(90, 30)),

	search_label_(this, 2, wxString("SEARCH"), wxPoint(310, 300), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	search_(this, 1005, wxString("BEGIN"), wxPoint(310, 330), wxSize(180, 30)),
	searching_(false),

	progress_label_(this, 2, wxString("PROGRESS (CPU)"), wxPoint(310, 420), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL),
	progress_(this, 2, wxString("0%"), wxPoint(310, 460), wxSize(180, 20), wxALIGN_CENTRE_HORIZONTAL)
{	
	wxTheApp->Bind(LOG_EVENT, &BFFGui::log, this, 1006);
	wxTheApp->Bind(SEARCH_END_EVENT, &BFFGui::endSearch, this, 1007);
	wxTheApp->Bind(LOG_PROGRESS_EVENT, &BFFGui::logProgress, this, 1008);

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

	std::vector<std::string> gpus;
	getGPUDevices(gpus);

	wxArrayString device_strings;
	device_strings.Add("CPU");
	for (const auto& gpu : gpus)
		device_strings.Add(wxString(gpu));

	device_choice_ = std::make_unique<wxChoice>(this, 2, wxPoint(50, 50), wxSize(180, 30), device_strings);
	device_choice_->SetSelection(0);

	/*textlog_ << "bunch of random ass text";
	textlog_ << "more random ass text\n";

	for (int i = 0; i < 100; i++)
		textlog_ << i << "\n";*/

}
BFFGui::~BFFGui()
{
}


void BFFGui::onAddClicked(wxCommandEvent& evt)
{
	if (searching_) return;

	int x, y, z;

	try
	{
		x = std::stoi(std::string(relative_x_.GetValue()));
		y = std::stoi(std::string(relative_y_.GetValue()));
		z = std::stoi(std::string(relative_z_.GetValue()));
	}
	catch (...)
	{
		*textlog_ << "Invalid numbers\n";
		return;
	}

	bool bedrock = is_bedrock_.GetValue();

	formation_[std::make_tuple(x, y, z)] = bedrock;

	*textlog_ << "Added (" << x << ", " << y << ", " << z << ") as " << (bedrock ? "BEDROCK" : "NOT BEDROCK") << "\n";
}
void BFFGui::onRemoveClicked(wxCommandEvent& evt)
{
	if (searching_) return;

	int x, y, z;

	try
	{
		x = std::stoi(std::string(relative_x_.GetValue()));
		y = std::stoi(std::string(relative_y_.GetValue()));
		z = std::stoi(std::string(relative_z_.GetValue()));
	}
	catch (...)
	{
		*textlog_ << "Invalid numbers\n";
		return;
	}

	auto it = formation_.find(std::make_tuple(x, y, z));

	if (it == formation_.end())
	{
		*textlog_ << "That block isn't in the formation\n";
		return;
	}

	formation_.erase(it);
	*textlog_ << "Removed (" << x << ", " << y << ", " << z << ")\n";
}
void BFFGui::onShowClicked(wxCommandEvent& evt)
{
	if (searching_) return;

	*textlog_ << "Formation:\n";

	for (const auto& block : formation_)
	{
		*textlog_ << "(" << std::get<0>(block.first) << ", " << std::get<1>(block.first) << ", " << std::get<2>(block.first) << ") = " << (block.second ? "BEDROCK" : "NOT BEDROCK") << "\n";
	}
}
void BFFGui::onResetClicked(wxCommandEvent& evt)
{
	if (searching_) return;

	formation_.clear();
	*textlog_ << "Formation Cleared\n";
}

void BFFGui::endSearch(wxCommandEvent& evt)
{
	searching_ = false;
	if(current_search_.joinable())
		current_search_.join();
	*textlog_ << evt.GetString();
	search_.SetLabel("BEGIN");
}

void BFFGui::log(wxCommandEvent& evt)
{
	*textlog_ << evt.GetString();
}

void BFFGui::logProgress(wxCommandEvent& evt)
{
	progress_.SetLabel(evt.GetString());
	progress_.Refresh();
}

void BFFGui::onSearch(wxCommandEvent& evt)
{
	if (searching_)
	{
		wxCommandEvent* evt = new wxCommandEvent;
		evt->SetId(1007);
		evt->SetEventType(SEARCH_END_EVENT);
		evt->SetString("Search aborted\n");
		wxTheApp->QueueEvent(evt);
		return;
	}

	int bounds[6];
	int num_threads;
	int num_results;
	std::string device;

	try
	{
		bounds[0] = std::stoi(std::string(x_min_.GetValue()));
		bounds[1] = std::stoi(std::string(x_max_.GetValue()));

		bounds[2] = std::stoi(std::string(z_min_.GetValue()));
		bounds[3] = std::stoi(std::string(z_max_.GetValue()));

		bounds[4] = std::stoi(std::string(y_min_.GetValue()));
		bounds[5] = std::stoi(std::string(y_max_.GetValue()));

		num_threads = std::stoi(std::string(thread_count_.GetValue()));

		num_results = std::stoi(std::string(numresults_.GetValue()));

		device = device_choice_.get()->GetString(device_choice_.get()->GetCurrentSelection());
	}
	catch (...)
	{
		*textlog_ << "Search parameters invalid\n";
		return;
	}

	*textlog_ << "Beginning Search\n";
	searching_ = true;
	search_.SetLabel("END");
	num_results_left_ = num_results;

	if (device != "CPU")
	{
		current_search_ = std::thread(GPUSearch, std::string(context_choice_->GetStringSelection()), device, formation_, std::make_tuple(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]), num_results);
		return;
	}

	std::function<bool(int, int, int)> bedrock_gen_func;

	if (context_choice_->GetStringSelection() == "1.12 Overworld")
		bedrock_gen_func = bedrockOverworld112;
	else if (context_choice_->GetStringSelection() == "1.13 Overworld")
		bedrock_gen_func = bedrockOverworld113;
	else if (context_choice_->GetStringSelection() == "1.14 Overworld")
		bedrock_gen_func = bedrockOverworld114;
	else if (context_choice_->GetStringSelection() == "1.13 Nether")
		bedrock_gen_func = bedrockNether113;
	else if (context_choice_->GetStringSelection() == "1.14 Nether")
		bedrock_gen_func = bedrockNether114;

	current_search_ = std::thread(threadedSearch,
		std::make_tuple(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]),
		formation_,
		bedrock_gen_func,
		num_threads,
		&searching_,
		&num_results_left_);
}
