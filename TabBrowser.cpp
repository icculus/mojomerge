/*  TabBrowser.cpp - Implementation for the TabBrowser class
 *
 */

#include "TabBrowser.h"
#include "Identifiers.h"

using namespace MojoMerge;

TabBrowser::TabBrowser(wxWindow *Container)
{
    // Initialize member variables
    Notebook = NULL;
    BoxSizer = NULL;
    NotebookSizer = NULL;

    // Container can't be NULL
    assert(Container);
    // Create our panel window where the tab control will be placed
    Create(Container, ID_TAB_BROWSER_PANEL);
    // Create new notebook control
    Notebook = new wxNotebook(this, ID_TAB_BROWSER_NOTEBOOK, wxDefaultPosition,
        wxDefaultSize, wxCLIP_CHILDREN);
    // Create the notebook sizer
    NotebookSizer = new wxNotebookSizer(Notebook);
    // Create our box sizer
    BoxSizer = new wxBoxSizer(wxVERTICAL);
    // Our new objects can't be NULL
    assert(Notebook);
    assert(BoxSizer);
    assert(NotebookSizer);
    // Add the notebook sizer to the sizer object
    BoxSizer->Add(NotebookSizer, 1, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);
    // Initialize sizer stuff so that the browser will take up
    //  the entire panel.
    this->SetAutoLayout(true);
    this->SetSizer(BoxSizer);
    BoxSizer->Fit(this);
    BoxSizer->SetSizeHints(this);
    // Show the notebook control
    Notebook->Show();
    // Show this panel control
    Show();
}

void TabBrowser::AddWindow(TabWindow *Window)
{
    Notebook->AddPage(Window, "New Comparison Window", true);
}

void TabBrowser::RemoveActiveWindow()
{
}

TabWindow *TabBrowser::GetActiveWindow()
{
    return NULL;
}