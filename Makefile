CXX = $(shell wx-config --cxx)
PROGRAM = MojoMerge

OBJECTS = Application.o CompareFilePanel.o CompareFilesUI.o \
	CompareFolderPanel.o CompareFolders.o CompareFoldersUI.o Config.o \
	ConfigUI.o DiffTextEdit.o FileMergeTransaction.o FolderHunk.o \
	FolderMerge.o FolderMergeTransaction.o GNUDiff.o Hunk.o LineBuffer.o \
	MainWindow.o Menu.o Merge.o SeparatorPanel.o TabBrowser.o \
	Toolbar.o TransactionBuffer.o Test.o

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c `wx-config --cxxflags` -D_DEBUG -D__DEBUG__ -D__WXDEBUG__ -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) -lwx_gtkd_stc-2.4 `wx-config --libs`

clean:
	rm -f *.o $(PROGRAM)

