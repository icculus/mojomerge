/*  SeparatorPanel.cpp - Implementation for the SeparatorPanel class
 *
 */

#include "SeparatorPanel.h"
#include "Identifiers.h"

using namespace MojoMerge;

SeparatorPanel::SeparatorPanel(CompareUI *CompareWindow) :
    wxPanel(CompareWindow, ID_SEPARATOR_PANEL, wxDefaultPosition,
    wxSize(SEPARATOR_WIDTH, SEPARATOR_WIDTH), 0)
{
    // Save the window
    this->CompareWindow = CompareWindow;
}

void SeparatorPanel::OnPaint(wxPaintEvent &event)
{
    // Call the handler function in the compare window
    CompareWindow->OnSeparatorPainted(event);
}

// Event mappings for the panel
BEGIN_EVENT_TABLE(SeparatorPanel, wxPanel)
    EVT_PAINT(SeparatorPanel::OnPaint)
END_EVENT_TABLE()

