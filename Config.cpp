/*  Config.cpp - Implementation for Config class
 *
 */

#include "Config.h"

using namespace MojoMerge;

#define UNDO_SIZE_DEFAULT       16
#define DIFF_FOLDER_DEFAULT     "diff"
#define DIFF3_FOLDER_DEFAULT    "diff3"

// Initialize static members
wxConfig *Config::MyConfigData = NULL;

void Config::Init()
{
    MyConfigData = new wxConfig("MojoMerge");
}

void Config::Deinit()
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    delete MyConfigData;
}

void Config::SetShowLineNumbers(bool Value)
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    bool i = MyConfigData->Write(wxT("ShowLineNumbers"), Value);
    // Fails if write was not successful
    assert(i);
}

bool Config::GetShowLineNumbers()
{
    // Return value
    bool Value;

    // MyConfigData can't be NULL
    assert(MyConfigData);
    MyConfigData->Read(wxT("ShowLineNumbers"), &Value, false);

    return Value;
}

void Config::SetShowCRLF(bool Value)
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    bool i = MyConfigData->Write(wxT("ShowCRLF"), Value);
    // Fails if write was not successful
    assert(i);
}

bool Config::GetShowCRLF()
{
    // Return value
    bool Value;

    // MyConfigData can't be NULL
    assert(MyConfigData);
    MyConfigData->Read(wxT("ShowCRLF"), &Value, false);

    return Value;
}

void Config::SetShowInlineChanges(bool Value)
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    bool i = MyConfigData->Write(wxT("ShowInlineChanges"), Value);
    // Fails if write was not successful
    assert(i);
}

bool Config::GetShowInlineChanges()
{
    // Return value
    bool Value;

    // MyConfigData can't be NULL
    assert(MyConfigData);
    MyConfigData->Read(wxT("ShowInlineChanges"), &Value, false);

    return Value;
}

uint32 Config::GetUndoSize()
{
    // Return value
    long Value;

    // MyConfigData can't be NULL
    assert(MyConfigData);
    MyConfigData->Read(wxT("UndoSize"), &Value, UNDO_SIZE_DEFAULT);
    // Make sure value is not negative since we're casting a long to an
    //  unsigned value
    assert(Value > 0);

    return (uint32)Value;
}

void Config::SetUndoSize(uint32 Size)
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    // TODO - Should we check for Size being to big?
    bool i = MyConfigData->Write(wxT("UndoSize"), (long)Size);
    // Fails if write was not successful
    assert(i);
}

wxString Config::GetDiffPath()
{
    wxString Value;

    // MyConfigData can't be NULL
    assert(MyConfigData);
    MyConfigData->Read(wxT("DiffPath"), &Value, DIFF_FOLDER_DEFAULT);

    return Value;
}

void Config::SetDiffPath(wxString &Value)
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    bool i = MyConfigData->Write(wxT("DiffPath"), Value);
    // Fails if write was not successful
    assert(i);
}

wxString Config::GetDiff3Path()
{
    wxString Value;

    // MyConfigData can't be NULL
    assert(MyConfigData);
    MyConfigData->Read(wxT("Diff3Path"), &Value, DIFF3_FOLDER_DEFAULT);

    return Value;
}

void Config::SetDiff3Path(wxString &Value)
{
    // MyConfigData can't be NULL
    assert(MyConfigData);
    bool i = MyConfigData->Write(wxT("Diff3Path"), Value);
    // Fails if write was not successful
    assert(i);
}
