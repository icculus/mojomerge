/*  Toolbar.cpp - Implementation of Toolbar object
 *
 */

#include "Toolbar.h"
#include "Identifiers.h"
#include "Application.h"

using namespace MojoMerge;

Toolbar::Toolbar(wxWindow *Parent) : wxToolBar(Parent, ID_TOOLBAR)
{
    SetMargins( 2, 2 );
    
    // Set the size of the buttons to accomodate our bitmaps
    SetToolBitmapSize(wxSize(32, 32));

    AddTool(ID_TWOWAYFILECOMPARE_TOOL, wxT(""), GetBitmap(0), wxT("New two-way file comparison"));
    AddTool(ID_THREEWAYFILECOMPARE_TOOL, wxT(""), GetBitmap(1), wxT("New three-way file comparison"));
    AddTool(ID_TWOWAYFOLDERCOMPARE_TOOL, wxT(""), GetBitmap(2), wxT("New two-way folder comparison"));
    AddTool(ID_THREEWAYFOLDERCOMPARE_TOOL, wxT(""), GetBitmap(3), wxT("New three-way folder comparison"));
    AddSeparator();
    AddTool(ID_RECOMPARE_TOOL, wxT(""), GetBitmap(6), wxT("Perform a recompare for the active window"));
    AddTool(ID_CLOSEWINDOW_TOOL, GetBitmap(4), wxT("Close the current active window"));
    Realize();
}

void Toolbar::SetCommandStatus(uint32 Status)
{
    EnableTool(ID_RECOMPARE_TOOL, Cmd_Recompare & Status);
}

wxBitmap Toolbar::GetBitmap(size_t index)
{
    if (index == 0)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 2 1",
        "  c None",
        "a c Black",
        /* pixels */
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "            aaaaaaaa            ",
        "          aaaaaaaaaaaa          ",
        "        aaaa        aaaa        ",
        "       aaa            aaa       ",
        "      aa      aaaa      aa      ",
        "      aa      aaaa      aa      ",
        "     aa     aa    aa     aa     ",
        "     aa     aa    aa     aa     ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa          aa        aa    ",
        "    aa          aa        aa    ",
        "    aa        aa          aa    ",
        "    aa        aa          aa    ",
        "    aa      aa            aa    ",
        "     aa     aa           aa     ",
        "     aa     aa           aa     ",
        "      aa    aaaaaaaa    aa      ",
        "      aa    aaaaaaaa    aa      ",
        "       aaa            aaa       ",
        "        aaaa        aaaa        ",
        "          aaaaaaaaaaaa          ",
        "            aaaaaaaa            ",
        "                                ",
        "                                ",
        "                                ",
        "                                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 1)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 2 1",
        "  c None",
        "a c Black",
        /* pixels */
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "            aaaaaaaa            ",
        "          aaaaaaaaaaaa          ",
        "        aaaa        aaaa        ",
        "       aaa            aaa       ",
        "      aa      aaaa      aa      ",
        "      aa      aaaa      aa      ",
        "     aa     aa    aa     aa     ",
        "     aa     aa    aa     aa     ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa        aaaa        aa    ",
        "    aa        aaaa        aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "     aa     aa    aa     aa     ",
        "     aa     aa    aa     aa     ",
        "      aa      aaaa      aa      ",
        "      aa      aaaa      aa      ",
        "       aaa            aaa       ",
        "        aaaa        aaaa        ",
        "          aaaaaaaaaaaa          ",
        "            aaaaaaaa            ",
        "                                ",
        "                                ",
        "                                ",
        "                                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 2)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 5 1",
        "  c None",
        "a c Black",
        "b c #C0C0C0",
        "c c #FFFF00",
        "e c #FFFFFF",
        /* pixels */
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "            aaaaaaaa            ",
        "          aaaaaaaaaaaa          ",
        "        aaaa        aaaa        ",
        "       aaa            aaa       ",
        "      aa      aaaa      aa      ",
        "      aa      aaaa      aa      ",
        "     aa     aa    aa     aa     ",
        "     aa     aa    aa     aa     ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa          aa        aa    ",
        "    aa          aa        aa    ",
        "    aa        aa          aa    ",
        "    aa        aa          aa    ",
        "    aa      aa            aa    ",
        "     aa     aa           aa     ",
        "     aa     aa          aaa     ",
        "      aa    aaaaaaaaaaaaaa      ",
        "      aa    aaaaaaaaaacecea     ",
        "       aaa          aaaaaaaaaaa ",
        "        aaaa        acececececea",
        "          aaaaaaaaaaaecececececa",
        "            aaaaaaaaacececececea",
        "                    aecececececa",
        "                    acececececea",
        "                    abbbbbbbbbba",
        "                     aaaaaaaaaaa"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 3)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 5 1",
        "  c None",
        "a c Black",
        "b c #C0C0C0",
        "c c #FFFF00",
        "e c #FFFFFF",
        /* pixels */
        "                                ",
        "                                ",
        "                                ",
        "                                ",
        "            aaaaaaaa            ",
        "          aaaaaaaaaaaa          ",
        "        aaaa        aaaa        ",
        "       aaa            aaa       ",
        "      aa      aaaa      aa      ",
        "      aa      aaaa      aa      ",
        "     aa     aa    aa     aa     ",
        "     aa     aa    aa     aa     ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa        aaaa        aa    ",
        "    aa        aaaa        aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "    aa            aa      aa    ",
        "     aa     aa    aa     aa     ",
        "     aa     aa    aa    aaa     ",
        "      aa      aaaa    aaaa      ",
        "      aa      aaaa   acecea     ",
        "       aaa          aaaaaaaaaaa ",
        "        aaaa        acececececea",
        "          aaaaaaaaaaaecececececa",
        "            aaaaaaaaacececececea",
        "                    aecececececa",
        "                    acececececea",
        "                    abbbbbbbbbba",
        "                    eaaaaaaaaaaa"
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 4)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 5 1",
        "  c None",
        "a c #FF0000",
        "b c #800000",
        "d c #FFFFFF",
        "e c #808080",
        /* pixels */
        "           bbbbbbbb             ",
        "        bbbaaaaaaaabbb          ",
        "       baaaaaaaaaaaaaab         ",
        "     bbaaaaaaaaaaaaaaaabb       ",
        "    baaaaaaaaaaaaaaaaaaaab      ",
        "   baaaaaaaaaaaaaaaaaaaaaab     ",
        "   baaaaaaaaaaaaaaaaaaaaaabe    ",
        "  baaaaaadaaaaaaaaaadaaaaaabe   ",
        " baaaaaadddaaaaaaaadddaaaaaab   ",
        " baaaaadddddaaaaaadddddaaaaabe  ",
        " baaaaaadddddaaaadddddaaaaaabee ",
        "baaaaaaaadddddaadddddaaaaaaaabe ",
        "baaaaaaaaaddddddddddaaaaaaaaabe ",
        "baaaaaaaaaaddddddddaaaaaaaaaabee",
        "baaaaaaaaaaaddddddaaaaaaaaaaabee",
        "baaaaaaaaaaaddddddaaaaaaaaaaabee",
        "baaaaaaaaaaddddddddaaaaaaaaaabee",
        "baaaaaaaaaddddddddddaaaaaaaaabee",
        "baaaaaaaadddddaadddddaaaaaaaabee",
        " baaaaaadddddaaaadddddaaaaaabeee",
        " baaaaadddddaaaaaadddddaaaaabeee",
        " baaaaaadddaaaaaaaadddaaaaaabee ",
        "  baaaaaadaaaaaaaaaadaaaaaabeee ",
        "   baaaaaaaaaaaaaaaaaaaaaabeeee ",
        "   baaaaaaaaaaaaaaaaaaaaaabeee  ",
        "    baaaaaaaaaaaaaaaaaaaabeee   ",
        "     bbaaaaaaaaaaaaaaaabbeeee   ",
        "      ebaaaaaaaaaaaaaabeeeee    ",
        "       ebbbaaaaaaaabbbeeeee     ",
        "         eebbbbbbbbeeeeee       ",
        "          eeeeeeeeeeeeee        ",
        "             eeeeeeee           "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 5)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 7 1",
        "  c None",
        "a c Black",
        "b c #FF0000",
        "c c #808000",
        "d c #C0C0C0",
        "f c #FFFFFF",
        "g c #808080",
        /* pixels */
        "       aaaaa                    ",
        "     aaddaaaaaaaa               ",
        "    agfddabbbbbba               ",
        "    afdfdabbbbbbaa              ",
        "   afffffabbbbbbagaa            ",
        "   afffffaaaaaaaagggaa          ",
        "  afdfffffgggggaagddggaa        ",
        "  adfdfdffffffgaagddddggaa      ",
        "  afdfdfdfdfffgaagfdfdaadga     ",
        "  adddddfdfdffgaagffdagdgdga    ",
        "  addddddddfdfgaagffagdgdgda    ",
        "  adddddddddfdgaagfdadgdaagda   ",
        "  adddddddddddgaagdadgdaggaga   ",
        "  adddddddddddgaagfagdgaddgaa   ",
        "  adddddddddddgaagdadgdgafgaa   ",
        "  adddddddddddgaagdagdgdafgaa   ",
        "  aaddddddddddgaagdadgdgdaaga   ",
        "    aaddddddddgaagdagdgdgdgda   ",
        "      aadddddddaagdadgdgdgdga   ",
        "        aaddddddggdagdgdgdgda   ",
        "         aaadddddddadgdgdgdaa   ",
        "         accaadddddagdgdgaa     ",
        "         acccaaadddadgdaa       ",
        "         acccaagaadagaa         ",
        "         acccagagaaaa           ",
        "         acccaagaa              ",
        "         acccagaga              ",
        "         acccaagaa              ",
        "         acccagaga              ",
        "         acccaagaa              ",
        "           ccagaga              ",
        "             aag                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    if (index == 6)
    {
        /* XPM */
        static const char *xpm_data[] = {
        /* columns rows colors chars-per-pixel */
        "32 32 2 1",
        "  c None",
        "a c Black",
        /* pixels */
        "                                ",
        "                                ",
        "                                ",
        "     a                          ",
        "     a                          ",
        "     a       aa                 ",
        "     a     aaaa                 ",
        "     a   aaaaaa                 ",
        "     a aaaaaaa                  ",
        "     aaaaaaaaaaaaaaaaaaaaaa     ",
        "     aaaaaaaaaaaaaaaaaaaaaa     ",
        "     a aaaaaaa                  ",
        "     a   aaaaaa                 ",
        "     a     aaaa                 ",
        "     a       aa           a     ",
        "     a                    a     ",
        "     a           aa       a     ",
        "                 aaaa     a     ",
        "                 aaaaaa   a     ",
        "                  aaaaaaa a     ",
        "     aaaaaaaaaaaaaaaaaaaaaa     ",
        "     aaaaaaaaaaaaaaaaaaaaaa     ",
        "                  aaaaaaa a     ",
        "                 aaaaaa   a     ",
        "                 aaaa     a     ",
        "                 aa       a     ",
        "                          a     ",
        "                          a     ",
        "                                ",
        "                                ",
        "                                ",
        "                                "
        };
        wxBitmap bitmap( xpm_data );
        return bitmap;
    }
    return wxNullBitmap;
}