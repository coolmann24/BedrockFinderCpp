#pragma once
#include <wx/wx.h>
#include <map>
#include <mutex>
#include <atomic>
#include <thread>

wxDECLARE_EVENT(LOG_EVENT, wxCommandEvent);
wxDECLARE_EVENT(SEARCH_END_EVENT, wxCommandEvent);

class BFFGui : public wxFrame
{
public:
	BFFGui();
	~BFFGui();

private:
	std::unique_ptr<wxChoice> context_choice_;
	std::unique_ptr<wxChoice> device_choice_;
	wxTextCtrl * textlog_;
	wxTextCtrl thread_count_;
	wxStaticText context_label_;
	wxStaticText device_label_;
	wxStaticText thread_count_label_;

	wxStaticText x_label_, y_label_, z_label_;
	wxTextCtrl x_min_, x_max_, y_min_, y_max_, z_min_, z_max_;

	wxStaticText numresults_label_;
	wxTextCtrl numresults_;

	wxStaticText relative_coords_label_;
	wxTextCtrl relative_x_, relative_y_, relative_z_;
	wxCheckBox is_bedrock_;

	wxButton add_, remove_, show_, reset_;

	wxStaticText search_label_;
	wxButton search_;

	wxStaticText progress_label_;
	wxStaticText progress_;

	void onAddClicked(wxCommandEvent& evt);
	void onRemoveClicked(wxCommandEvent& evt);
	void onShowClicked(wxCommandEvent& evt);
	void onResetClicked(wxCommandEvent& evt);

	void onSearch(wxCommandEvent& evt);
	void endSearch(wxCommandEvent& evt);

	void log(wxCommandEvent& evt);

	std::map<std::tuple<int, int, int>, bool> formation_;
	std::atomic_bool searching_, gpu_searching_;
	std::atomic_int num_results_left_;
	std::thread current_search_;

	wxDECLARE_EVENT_TABLE();
	
};