#include <wx/wx.h>
#include <iostream>
#include <unistd.h>
#include <ctime>

// Define the Python script location
#define PYTHON_SCRIPT "sniffles/database.py"

// Function to execute the Python script using execlp
void logData(int response) {
    pid_t pid = fork(); // Create a child process

    if (pid == -1) {
        std::cerr << "❌ Error: Failed to fork process." << std::endl;
    } else if (pid == 0) {
        // Get the current timestamp
        time_t now = time(0);
        std::string timestamp = std::to_string(now);
        
        std::cout << "Timestamp C++:" + timestamp.c_str() << std::endl;

        // Ecute Python script with arguments
        execlp("python3", "python3", PYTHON_SCRIPT, 
               "--user-response", "--set",
               std::to_string(response).c_str(), "--time", 
               timestamp.c_str(), (char*)NULL);

        // If execlp fails, print an error and exit child process
        std::cerr << "❌ Error: Failed to execute Python script." << std::endl;
        exit(1);
    } else {
        // Parent process continues
        std::cout << "✅ Sent response " << response << " to Python script." << std::endl;
    }
}

// GUI Frame
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
        int selection = event.GetId() - 1000; // Extract number (1-5)
        wxMessageBox(wxString::Format("You selected: %d", selection), "Response", wxOK | wxICON_INFORMATION);
        
        // Call Python script with response
        logData(selection);
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
