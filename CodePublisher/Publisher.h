#pragma once
//////////////////////////////////////////////////////////////////////////////
// Publisher.h		-	has Publisher and HTMLPublisher class which helps	//
//						in publishing C++ code in an HTML file				//
// ver 1.0																	//
//--------------------------------------------------------------------------//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition	//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home					//
//	Application:	CSE 687 Project #3, Spring 2017							//
//	Author:			Amritbani Sondhi,										//
//					Graduate Student, Syracuse University					//
//					asondhi@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ==================
* The Code Publisher defines methods to create new HTML files by creating important
* HTML tags. It also scans the c++ files for finding the dependent files with
* the help of tokenizer and semi exp. These files are given as a link on the top
* of the HTML page. The Code Publisher takes the input and output directory
* from the command line and processes and converts all the code files (*.h and 
* *.cpp) into HTML files. It is mandatory to provide the input directory. If you
* do not provide an output directory then by default it saves all the HTML code in
* the same input directory.
* 
* Public Methods:
* ==============
*	Publisher Class	:	
*	- createFilesAndFindDependencies()	:	finds #include dependencies, 
*											creates output html files (blank)
*	- createHTMLFile()			:	creates HTML files and stores the data into it
*	- createCompleteHeadTag()	:	creates HTML Head Tag to be added to the main HTML File
*	- createCompleteBodyTag()	:	Creates Body Tag to be added to the main HTML File 
*									(adds links to the dependency files)
*	- closeAllOpenFiles()		:	Close all the open file streams
*	- checkFileDependencies()	:	Check and get all the file dependencies 
*									in a vector : depFiles
*
*	HTMLPublisher Class	:
*	- struct HtmlConfig			:	saves patterns, input and output directory
*	- startHTMLPublish()		:	Gets Directory details, CSS and JS files and 
*									then calls to create an HTML file
*	- processDirectoryToHTML()	:	takes Directory Path as an argument and 
*									converts all it's files into HTML
*	- directoryEnd()			:	checks if the end of directory contains 
*									'//' and if it does then returns true
*	- ProcessCommandLineArgs()	:	processes command line arguments and saves 
*									the input, output directory and file patterns
*	- openPage()				:	runs Shell to open a page in the browser
*
* Build Process:
* ==============
*	- Required Files:
*		SemiExp.h, Tokenizer.h, FileSystem.h
*		SemiExp.cpp, Tokenizer.cpp, FileSystem.cpp
*	- Build commands:
*		devenv TestExecutive.sln
*
* Maintenance History:
* ===================
* ver 1.0 : 27 Mar 2017
* - first release
*/

#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../FileSystem/FileSystem.h"
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;
using Key = std::string;
using FileValue = std::string;

namespace CodePublisher
{
	class Publisher
	{
	public:
		// Input File to Convert
		string inputFileName;
		
		//Directory for keeping all the test files
		string testDirectory; 
		
		// Output HTML File
		string outputFileName;
		std::vector<std::string> depFiles;

		char characters;
		ifstream inFileStream;
		ofstream outFileStream;

		bool createFilesAndFindDependencies();
		void createHTMLFile(string css, string jScript);
		void createCompleteHeadTag(string css, string JScript);
		void createCompleteBodyTag();
		void closeAllOpenFiles();

		void checkFileDependencies();
	};

	

	class HTMLPublisher
	{
		struct HtmlConfig
		{
			string inputDirectory;
			string outputDirectory;
			string openWebPage;
			vector<string> patterns;
		};

	public:
		void ProcessCommandLineArgs(int argc, char* argv[]);
		void startHTMLPublish(std::string inputFileName);
		void processDirectoryToHTML(string currentDir);
		bool directoryEnd(std::string const & dir, std::string end);
		void openPage(string openPage);

		HtmlConfig config;
	};


}