#include <wx/wx.h>
#include "MainFrame.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);  // Ensures wxWidgets starts properly

bool MyApp::OnInit() {
    MainFrame* frame = new MainFrame("Feelings Survey (Dark Mode)");
    frame->Show(true);
    return true;
}
