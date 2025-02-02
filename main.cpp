#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
    MyFrame() : wxFrame(nullptr, wxID_ANY, "How are you feeling? (1-5)", wxDefaultPosition, wxSize(500, 250)) {
        wxPanel* panel = new wxPanel(this);
        panel->SetBackgroundColour(wxColour(30, 30, 30)); // Dark mode background

        wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

        // Question Label
        wxStaticText* question = new wxStaticText(panel, wxID_ANY, "How are you doing on a scale from 1 to 5?");
        question->SetForegroundColour(wxColour(255, 255, 255)); // White text
        vbox->Add(question, 0, wxALIGN_CENTER | wxTOP, 20);

        // Button Row
        wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);
        for (int i = 1; i <= 5; i++) {
            wxButton* button = new wxButton(panel, 1000 + i, wxString::Format("%d", i), wxDefaultPosition, wxSize(50, 40));
            button->SetBackgroundColour(wxColour(50, 50, 50)); // Dark button
            button->SetForegroundColour(wxColour(255, 255, 255)); // White text
            hbox->Add(button, 0, wxALIGN_CENTER | wxALL, 10);
            button->Bind(wxEVT_BUTTON, &MyFrame::OnButtonClick, this);
        }

        vbox->Add(hbox, 1, wxALIGN_CENTER | wxALL, 10);
        panel->SetSizer(vbox);
    }

private:
    void OnButtonClick(wxCommandEvent& event) {
        int selection = event.GetId() - 1000;
        wxMessageBox(wxString::Format("You selected: %d", selection), "Response", wxOK | wxICON_INFORMATION);
    }
};

class MyApp : public wxApp {
public:
    virtual bool OnInit() {
        MyFrame* frame = new MyFrame();
        frame->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
