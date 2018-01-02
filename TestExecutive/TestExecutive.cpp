//////////////////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Test Executive for Code Publisher					//
// ver 1.0																	//
//--------------------------------------------------------------------------//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition	//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home					//
//	Application:	CSE 687 Project #3, Spring 2017							//
//	Author:			Amritbani Sondhi,										//
//					Graduate Student, Syracuse University					//
//					asondhi@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "../CodePublisher/Publisher.h"
#include "TestExecutive.h"

using namespace CodePublisher;
using namespace FileSystem;
using namespace Scanner;


void TestExecutive::requirement1()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #1 - ";
	std::cout << "\n\t Code was developed in Visual Studio 2015, it's C++ Windows console projects,\n";
	std::cout << "\t using it's standard libraries.";
	std::cout << "\n\t Also used a use Windows Presentation Foundation (WPF),";
	std::cout << "\n\t to provide a required client Graphical User Interface(GUI).";
	std::cout << "\n ===========================================================================================";
}

void TestExecutive::requirement2()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #2 - ";
	std::cout << "\n\t Used C++ standard library's streams for all console I/O and \n new and delete for all heap-based memory management. \n";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement3()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #3 - ";
	std::cout << "\n\t Provides a Repository program that provides functionality to publish,";
	std::cout << "\n\t\t as linked web pages, the contents of a set of C++ source code files.";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement4()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #4 - ";
	std::cout << "\n\t For the publishing process, satisfies the requirements of CodePublisher ";
	std::cout << "\n\t\t developed in Project #3.";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement5()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #5 - ";
	std::cout << "\n\t Provides a Client program that can upload files";
	std::cout << "\n\t\t (Not Implemented viewing the contents in the repository)";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement6()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #6 - ";
	std::cout << "\n\t Provides a message-passing communication system, based on Sockets,";
	std::cout << "\n\t\t used to access the Repository's functionality from another process or machine.";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement7()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #7 - ";
	std::cout << "\n\t The communication system provides support for passing HTTP style messages";
	std::cout << "\n\t\t using either synchronous request / response or asynchronous one - way messaging.";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement8()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #8 - ";
	std::cout << "\n\t The communication system also supports sending and receiving streams of bytes.";
	std::cout << "\n\t\t Streams are established with an initial exchange of messages.";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement9()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Demonstrating Req #9 - ";
	std::cout << "\n\t Included an Automated Unit Test Suite that demonstrates that this project meets";
	std::cout << "\n\t\t all the requirements";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement10()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Req #10 - ";
	std::cout << "\n\t Wasn't able to implement";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::requirement11()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Req #11 - ";
	std::cout << "\n\t Wasn't able to implement";
	std::cout << "\n ===========================================================================================";

}

void TestExecutive::otherNotes()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n\t Directory used for uploading the files from Client to Server";
	std::cout << "\n\t\t is set to ..\4.TestFiles";
	std::cout << "\n\t Directory used for showing that the files have been uploaded";
	std::cout << "\n\t\t to the Server is set to .\1.UploadedFilesOnServer";
	std::cout << "\n\t Directory used for showing that the files have been published";
	std::cout << "\n\t\t on the Server is set to .\2.PublishedFilesOnServer";
	std::cout << "\n\t Directory used for showing that the files have been downloaded";
	std::cout << "\n\t\t from the Server to the Client is set to ";
	std::cout << "\n\t\t .\3.DownloadedFilesToClient (not used)";
	std::cout << "\n ===========================================================================================";
}

void TestExecutive::testExecutive()
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Testing Code Publisher - Project4-S2017";
	std::cout << "\n Amritbani Sondhi";
	std::cout << "\n ===========================================================================================";
	std::cout << "\n\n";

	requirement1();
	requirement2();
	requirement3();
	requirement4();
	requirement5();
	requirement6();
	requirement7();
	requirement8();
	requirement9();
	requirement10();
	requirement11();
	otherNotes();
}

//int main(int argc, char* argv[])
//{
//	TestExecutive test;
//	test.testExecutive();
//
//	std::cout << "\n\n";
//	std::system("pause");
//	return 0;
//}
