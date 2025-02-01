#include "MainFrame.h"
#include <wx/msgdlg.h>
#include <fstream>

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(wxID_ANY, MainFrame::OnButtonClick)
wxEND_EVENT_TABLE()

MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(400, 250))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    // Dark Mode Theme
    panel->SetBackgroundColour(wxColour(25, 25, 25));

    // Create Question Label
    wxStaticText* question = new wxStaticText(panel, wxID_ANY,
        "How are you feeling from 1 to 5?", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    question->SetForegroundColour(*wxWHITE);
    vbox->Add(question, 0, wxALL | wxEXPAND, 10);

    // Create Buttons
    wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
    for (int i = 1; i <= 5; i++) {
        wxButton* btn = new wxButton(panel, 1000 + i, std::to_string(i), wxDefaultPosition, wxSize(60, 35));
        btn->SetBackgroundColour(wxColour(50, 50, 50));
        btn->SetForegroundColour(*wxWHITE);
        hbox->Add(btn, 1, wxALL, 5);
    }

    vbox->Add(hbox, 1, wxALIGN_CENTER);
    panel->SetSizer(vbox);
}

void MainFrame::OnButtonClick(wxCommandEvent& event) {
    int choice = event.GetId() - 1000;
    wxMessageBox("You selected: " + std::to_string(choice), "Response", wxOK | wxICON_INFORMATION);

    SaveResultToFile(choice);
}

void MainFrame::SaveResultToFile(int choice) {
    std::ofstream file("results.txt", std::ios::app);
    if (file.is_open()) {
        file << choice << "\n";
        file.close();
    }
    else {
        wxMessageBox("Error saving result!", "File Error", wxOK | wxICON_ERROR);
    }
}
