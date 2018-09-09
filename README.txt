======================================================================
This project built a remote code repository based on a NoSQL database.
-For course CSE 687: Object Oriented Design final project.
======================================================================
The functions of this repo include:
1. Query: Query source files by file name or regularexpression.
2. Checkin,Checkout: checkin or checkout files.
3. Version: add version number of every file.


======================================================================
Build Instruction:
======================================================================
- You will need Visual Studio 2017, Community Edition to build
- Be sure to use Windows SDK 10.0.15063.0.
1. Right-click on solution and select rebuild
   - CodeAnalyzer is built as a Windows console Application
   - CommLibWrapper is built as a static library
   - FileSystem is built as a static library
   - GUI is a WPF application, but we build as a .Net console
     application so console IO shows how things work
   - Message is built as a static library
   - MsgPassingComm is built as a static library
   - Process is built as a static library
   - ServerPrototype is build as a Windows console Application
   - Sockets is built as a static library
   - Translater is build as a .Net dynamic link library, using /clr
     /clr option. This is required so that the C# GUI can make calls 
     into it.
   - Utilities is built as a static library
2. To build static libraries:
   - Right-click on the project and select properties > static library
   - open Properties > C/C++ > Preprocessor > Preprocessor Definitions
     and change TEST_[packageName] to noTest_[packageName]
   - Right-click on project and select Rebuild
2. To re-build static libraries as executables:
   - Right-click on the project and select properties > Windows Application
   - open Properties > C/C++ > Preprocessor > Preprocessor Definitions
     and change noTEST_[packageName] to Test_[packageName]
   - Right-click on project and select Rebuild
   

======================================================================
Operation Instructions:
======================================================================
1. Double click "Complie.bat" file.
2. Double click "run.bat" file.
3. After step 1&2, you will get 4 windows: 
	Autotest console: display all test of all functions automatically.
	GUI console:display information about GUI.
	server console:display information about server.
	GUI:user interface provided to uesrs.
4. Click "Local" button on GUI: display file list on local storage
5. Click "Remote" button: display file list on remote storage.
6. Click "Connect" button: connect remote and local.
7. Click "Query": check files in the repo.
8. Double Click file name: display file's content in a new window.
9. Click "Analyze" :display source code analysis result.
