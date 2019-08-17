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

	wxStaticText x_label_, y_label_, z_label_;
	wxTextCtrl x_min_, x_max_, y_min_, y_max_, z_min_, z_max_;

	wxStaticText numresults_label_;
	wxTextCtrl numresults_;

	wxStaticText relative_coords_label_;
	wxTextCtrl relative_x_, relative_y_, relative_z_;

	wxButton add_, remove_, show_, reset_;

	wxStaticText search_label_;
	wxButton search_;

	wxStaticText progress_label_;
	wxStaticText progress_;
};