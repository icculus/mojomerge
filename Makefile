CXX = $(shell wx-config --cxx)
PROGRAM = mojomerge

OBJECTS = Application.o CompareFilePanel.o CompareFilesUI.o \
	CompareFolderPanel.o CompareFolders.o CompareFoldersUI.o Config.o \
	ConfigUI.o DiffTextEdit.o FileMergeTransaction.o FolderHunk.o \
	FolderMerge.o FolderMergeTransaction.o GNUDiff.o Hunk.o LineBuffer.o \
	MainWindow.o Menu.o Merge.o SeparatorPanel.o TabBrowser.o \
	Toolbar.o TransactionBuffer.o #Test.o

.SUFFIXES:	.o .cpp

.cpp.o :
	$(CXX) -c `wx-config --cxxflags` -o $@ $<

all:    $(PROGRAM)

$(PROGRAM):	$(OBJECTS)
	$(CXX) -o $(PROGRAM) $(OBJECTS) /usr/local/lib/lib_stc-2.4.so `wx-config --libs`

clean:
	rm -f *.o $(PROGRAM)

