/*  Config.cpp - Implementation for Config class
 *
 */

#include "Config.h"

using namespace MojoMerge;

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
    bool i = MyConfigData->Read(wxT("ShowLineNumbers"), &Value, false);
    // Fails if write was not successful
    assert(i);

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
    bool i = MyConfigData->Read(wxT("ShowCRLF"), &Value, false);
    // Fails if write was not successful
    assert(i);

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
    bool i = MyConfigData->Read(wxT("ShowInlineChanges"), &Value, false);
    // Fails if write was not successful
    assert(i);

    return Value;
}