#ifndef MAINFRAME_H
#define MAINFRAME_H

#include <wx/wx.h>
#include <fstream>

class MainFrame : public wxFrame {
public:
    MainFrame(const wxString& title);

private:
    void OnButtonClick(wxCommandEvent& event);
    void SaveResultToFile(int choice);

    wxDECLARE_EVENT_TABLE();
};

#endif // MAINFRAME_H
