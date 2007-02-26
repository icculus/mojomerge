/*
 * MojoMerge -- A cross-platform compare-and-merge utility.
 * Copyright (C) 2007 Ryan C. Gordon and Gregory S. Read.
 *
 * Please refer to LICENSE.txt in the root directory of the source
 *  distribution for licensing details.
 */

/*  SeparatorPanel.cpp - Implementation for the SeparatorPanel class
 *
 */

#include "SeparatorPanel.h"
#include "Identifiers.h"
#include "Application.h"

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
    // We have to create a DC even if we don't use it
    wxPaintDC dc(this);
    // Call the handler function in the compare window
    CompareWindow->OnSeparatorPainted(event, &dc);
    // Let the event continue to propogate
    event.Skip();
}

// Event mappings for the panel
BEGIN_EVENT_TABLE(SeparatorPanel, wxPanel)
    EVT_PAINT(SeparatorPanel::OnPaint)
END_EVENT_TABLE()