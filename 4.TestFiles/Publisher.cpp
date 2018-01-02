//////////////////////////////////////////////////////////////////////////////
// Publisher.cpp	-	has definitions for the methods of Publisher and	//
//						HTMLPublisher class									//
// ver 1.0																	//
//--------------------------------------------------------------------------//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition	//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home					//
//	Application:	CSE 687 Project #3, Spring 2017							//
//	Author:			Amritbani Sondhi,										//
//					Graduate Student, Syracuse University					//
//					asondhi@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "Publisher.h"

using namespace FileSystem;
using namespace Scanner;

// finds #include dependencies, creates output html files (blank)
bool CodePublisher::Publisher::createFilesAndFindDependencies()
{
	// Open the input file
	inFileStream.open(inputFileName.c_str());

	if (inFileStream.fail()) {
		cout << "Unable to open file" << endl;
		return false;
	}
	else
	{
		// find #include dependencies and saving it in vector: depFiles
		checkFileDependencies();

		// Create the output file
		outputFileName = testDirectory + "/" + inputFileName + ".html";
		outFileStream.open(outputFileName.c_str());
		return true;
	}
}

// creates HTML files and stores the data into it
// accepts CSS and JScript file names as two optional fields
void CodePublisher::Publisher::createHTMLFile(string css = "null", string jScript = "null")
{
	// Start all the HTML tags
	outFileStream << "<!DOCTYPE html> \n";
	outFileStream << "<html> \n";
	createCompleteHeadTag(css, jScript);
	createCompleteBodyTag();
	outFileStream << "</html> \n";

}

// creates HTML Head Tag to be added to the main HTML File
void CodePublisher::Publisher::createCompleteHeadTag(string css, string jScript)
{
	outFileStream << "<head> \n";

	if (css!="null")
	{
		outFileStream << "<link href=";
		outFileStream << css;
		outFileStream <<" rel=\"stylesheet\" type=\"text/css\"/> \n"; //escape sequence for " is \"
	}

	if(jScript!="null")
	{
		outFileStream << "<script  type=\"text/javascript\" src=";
		outFileStream << jScript;
		outFileStream << "></script>"; // yet to configure
	}
	outFileStream << "</head> \n";
}

// Creates Body Tag to be added to the main HTML File (has links to the dependency files as well)
void CodePublisher::Publisher::createCompleteBodyTag()
{
	outFileStream << "<body> \n";

	// Title
	outFileStream << "<div id=\"titleHeader\"> \n";
	outFileStream << "<h1>Code Files with Dependencies</h1>";

	// Showing the dependency files
	outFileStream << "<div id=\"menuBar\"> \n";
	outFileStream << "<h3> Dependencies </h3>";
	for (int i = 0; i < depFiles.size(); i++) // have to add an onclick event
	{
		outFileStream << "<a href = \"";
		outFileStream << depFiles[i] << ".html";
		outFileStream << "\">";
		outFileStream << depFiles[i];
		outFileStream << "</a>";
		outFileStream << "<br/>";
	}
	outFileStream << "</div> \n";

	// Showing all the code
	outFileStream << "<pre id=\"matter\">" << endl;
	// Loop for getting all the characters in a fileStream
	int count = 0;
	stringstream val;

	while (!inFileStream.eof())
	{
		inFileStream.get(characters);    // Get character
							 // conditions for checking and replacing < , > , &
		if (characters == '<') {
			outFileStream << "&lt;"; 		// Replacing < with &lt; 
		}
		else if (characters == '>') {
			outFileStream << "&gt;"; 		// Replacing > with &gt; 
		}
		else if (characters == '&')   // check for this in the html doc; else remove
		{
			outFileStream << "&amp;"; 		// Replacing & with &amp; 
		}
		// also add the <div> tags if you find { and }
		else if(characters == '{')
		{
			++count;
			val.clear();
			val << count;
			outFileStream << "<button class=\"buttonClass\" id=\"btn";
			outFileStream << val.str();
			outFileStream << "\" onclick=\"expandAndCollapse(";
			outFileStream << val.str();
			outFileStream << ", id";
			outFileStream << ")\">-</button><div class=\"buttonClass\" id=\"";
			outFileStream << val.str();
			outFileStream << "\">{";
		}
		else if (characters == '};')
		{
			outFileStream << "};</div>";
		}
		else if (characters == '}')
		{
			outFileStream << "}</div>";
		}
		else
		{
			outFileStream << characters;
		}
	}
	// End all the HTML tags
	outFileStream << "</pre>" << endl;
	outFileStream << "</body> \n";
}

// Close all the open file streams
void CodePublisher::Publisher::closeAllOpenFiles()
{
	inFileStream.close();
	outFileStream.close();
}

// Check and get all the file dependencies in a vector : depFiles
void CodePublisher::Publisher::checkFileDependencies()
{
	//Performing requirement 7
	Toker tok;

	if (!inFileStream.good())
	{
		std::cout << "\n  can't open file " << inputFileName << "\n\n";
	}
	else
	{
		std::cout << "\n\t Processing File: \"" << inputFileName << "\"\n";
	}
	tok.attach(&inFileStream);
	SemiExp semiEx(&tok);

	while (semiEx.get())
	{
		string exp = semiEx.show();
		string include = "# include"; //searches for #include as well as # include

		// int to size_t to ignore warnings
		size_t pos = exp.find(include, 0);
		if (pos != string::npos)
		{
			// skip 10 characters
			string depFileName = exp.erase(0, 9);
			string fileName = "";
			int i = 0, startIndex = -1, endIndex = -1;
			
			while (depFileName[i] != '\0') 
			{
				if (depFileName[i] == '\"' && startIndex == -1) {
					startIndex = i;
				}
				else if (depFileName[i] == '\"' && endIndex == -1) {
					endIndex = i;
				}
				i++;
			}

			if (startIndex > -1 && endIndex < depFileName.length()) {
				depFileName = depFileName.substr(startIndex + 1, endIndex - startIndex - 1);
			}
			if (depFileName.find("/"))
			{
				std::size_t found = depFileName.find_last_of('/');
				if (found != string::npos)
					depFileName = depFileName.substr(found + 1, depFileName.length() - found);
			}
			if (depFileName.find(".") == string::npos) {
				depFileName = "";
			}
			if (depFileName.length() > 0)
			{
				depFiles.push_back(depFileName);
			}
		}
	}

	//printing the dependent files
	cout << "\n\t\t" << "Dependent Files: " << endl;
	for (int i = 0; i < depFiles.size(); i++)
	{
		cout << "\t\t\t" << depFiles[i] << endl;
	}

	inFileStream.clear();
	inFileStream.close();
	inFileStream.open(inputFileName.c_str());
}

// Gets Directory details, CSS and JS files and then calls to create an HTML file
void CodePublisher::HTMLPublisher::startHTMLPublish(std::string inputFileName)
{
	Publisher codePublish;
	codePublish.inputFileName = inputFileName;

	// provide full path to the test Directory
	codePublish.testDirectory = "../2.PublishedFilesOnServer"; // by default

	//codePublish.testDirectory = FileSystem::Path::getPath(codePublish.testDirectory);

	string cssFile = "";
	string jQueryFile = "";

	if (codePublish.createFilesAndFindDependencies())
	{
		codePublish.createHTMLFile("1.StyleSheet.css","2.JavaScript.js");
		codePublish.closeAllOpenFiles();
	}

	cout << "\n \t File: " << codePublish.inputFileName << " is successfully converted." << endl;
	cout << "\t HTML version of the file is: " << codePublish.outputFileName << endl;
}

// takes Directory Path as an argument and converts all it's files into HTML
void CodePublisher::HTMLPublisher::processDirectoryToHTML(string currentDir)
{
	Directory::setCurrentDirectory(currentDir);
	
	std::vector<std::string> currentFiles = Directory::getFiles(currentDir, "*.h"); // gets files in current directories
	std::vector<std::string> currentCppFiles = Directory::getFiles(currentDir, "*.cpp");

	for (size_t i = 0; i < currentCppFiles.size(); ++i)
	{
		currentFiles.push_back(currentCppFiles[i]);	// currentFiles will contain all .cpp as well as .h files now
	}

	for (size_t i = 0; i<currentFiles.size(); ++i)
	{
		startHTMLPublish(currentFiles[i]);
	}
	std::cout << "\n -------------------------------------------------------------------------------------";
	std::cout << "\n -------------------------------------------------------------------------------------";
	std::cout << "\n Current Directory: \n";
	std::vector<std::string> currentDirs = Directory::getDirectories(); // gets current directories

	auto iter = std::find(currentDirs.begin(), currentDirs.end(), ".");
	if (iter != currentDirs.end())
		currentDirs.erase(iter);

	auto iter2 = std::find(currentDirs.begin(), currentDirs.end(), "..");
	if (iter2 != currentDirs.end())
		currentDirs.erase(iter2);

	for (size_t i = 0; i<currentDirs.size(); ++i)
	{
		std::cout << "\n    " << currentDirs[i].c_str();
		string fullCurrentDir, curDir = currentDir;
		fullCurrentDir = curDir.append("\\");
		fullCurrentDir = fullCurrentDir.append(currentDirs[i].c_str());

		processDirectoryToHTML(fullCurrentDir);
	}

}

// checks if the end of directory contains '//' and if it does then returns true
bool CodePublisher::HTMLPublisher::directoryEnd(std::string const &dir, std::string end="\\")
{
	if (dir.length() >= end.length()) 
	{
		return (0 == dir.compare(dir.length() - end.length(), end.length(), end));
	}
	else 
	{
		return false;
	}
}

// TestExecutive.exe -o ..\2.PublishedFilesOnServer -i .. -p *.h *.cpp
// processes command line arguments and saves the input, output directory and file patterns in HTMLPublisher's HTMLconfig struct
void CodePublisher::HTMLPublisher::ProcessCommandLineArgs(int argc, char* argv[]) {

	string input, output, page;
	vector<string> patterns;
	
	for (int i = 1; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1])
			{
			case 'o':
				output = argv[i + 1];
				break;
			case 'p':
				patterns.push_back(argv[i + 1]);
				break;
			case 'i':
				input = argv[i + 1];
				break;
			case 'w':
				page = argv[i + 1];
				break;
			default:
				std::cout << "Invalid / Directories Not Specified";
				break;
			}
		}
	}

	// input Directory should always exist
	// if output directory doesn't exist, create
	if (!Directory::exists(output) && Directory::exists(input))
	{
		if (!Directory::create(output))
		{
			// if it is still not able to create, ouptut directory will be specified
			// same as input directory
			output = input;
		}
		else
		{
			// ouput directory created
			cout << "ouput directory created";
		}
	}

	// input directory and webpage must be specified
	// you can do it by appending -i <directory name> in the command line arguments
	if (!Directory::exists(input))
	{
		std::cout << "Invalid / Directories Not Specified";
	}

	// when both the directories are configured, save it to HTMLPublisher class
	if (Directory::exists(input) && Directory::exists(output))
	{
		config.inputDirectory = input;
		config.outputDirectory = output;
		config.patterns = patterns;
		config.openWebPage = page;
	}
}

// runs Shell to open a page in the browser
void CodePublisher::HTMLPublisher::openPage(string openPage) 
{
	string outputDirectory = this->config.outputDirectory;
	outputDirectory = FileSystem::Path::getFullFileSpec(outputDirectory);
	string pageToOpen = outputDirectory + "\\" + openPage;
	//string p_ = FileSystem::Path::getFullFileSpec(pageToOpen);
	if (FileSystem::File::exists(pageToOpen))
	{
		const char * file = pageToOpen.c_str();
		std::wstring sPath(pageToOpen.begin(), pageToOpen.end());
		ShellExecute(NULL, "open", (LPCSTR)sPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
		//ShellExecute(NULL, L"open", (LPCWSTR)file, NULL, NULL, SW_SHOWNORMAL);
	}
}

#ifdef TEST_PUBLISHER

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing CodePublisher class";
	std::cout << "\n ============================= \n\n";

	/*
	HTMLPublisher htmlPublish;
	
	//Directory::setCurrentDirectory("..");
	Directory::setCurrentDirectory(this->config.inputDirectory);

	// gets contents of current directory
	string currentDir = Directory::getCurrentDirectory(); // gets current directory
	
	htmlPublish.processDirectoryToHTML(currentDir);
	*/

	CodePublisher::HTMLPublisher htmlPublish;

	htmlPublish.ProcessCommandLineArgs(argc, argv);
	string currentDir = htmlPublish.config.inputDirectory;

	// Hardcoding
	Directory::setCurrentDirectory("..");
	//Directory::setCurrentDirectory(currentDir);

	// gets contents of current directory
	currentDir = Directory::getCurrentDirectory(); // gets current directory
	htmlPublish.processDirectoryToHTML(currentDir);

	string openPage = htmlPublish.config.openWebPage;
	htmlPublish.openPage(openPage);


	std::cout << "\n\n";
	std::system("pause");
	return 0;
}
#endif // TEST_PUBLISHER
