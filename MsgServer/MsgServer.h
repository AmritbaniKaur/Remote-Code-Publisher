#pragma once
//////////////////////////////////////////////////////////////////////////////////
// MsgServer.h -	Demonstrates simple one-way HTTP style messaging			//
//					and file transfer											//
// Source:			Jim Fawcett, CSE687 - Object Oriented Design, Spring 2017	//
// ver 1.1																		//
//------------------------------------------------------------------------------//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition		//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home						//
//	Application:	CSE 687- OOD Project #4, Spring 2017						//
//	Author:			Amritbani Sondhi,											//
//					Graduate Student, Syracuse University						//
//					asondhi@syr.edu												//
//////////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* ==================
* This package implements a server that receives HTTP style messages and
* files from multiple concurrent clients and simply displays the messages
* and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
* 
* Public Methods:
* ==============
*	ClientHandler Class	:
*		void operator()(Socket socket)
*
* Build Process:
* ==============
*	- Required Files:
*		MsgClient.cpp, MsgServer.cpp
*		HttpMessage.h, HttpMessage.cpp
*		Cpp11-BlockingQueue.h
*		Sockets.h, Sockets.cpp
*		FileSystem.h, FileSystem.cpp
*		Logger.h, Logger.cpp
*		Utilities.h, Utilities.cpp
*	- Build commands:
*		devenv CodeAnalyzerEx.sln
*
* Maintenance History:
* ===================
* ver 1.0 : 
* - first release
* ver 1.1 : May 03, 2017
* - added uploadFile(), publishFiles(), downloadFile(), browseFile()
*/

#include "../Logger/Cpp11-BlockingQueue.h"
#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"


/////////////////////////////////////////////////////////////////////
// ClientHandler class
/////////////////////////////////////////////////////////////////////
// - instances of this class are passed by reference to a SocketListener
// - when the listener returns from Accept with a socket it creates an
//   instance of this class to manage communication with the client.
// - You need to be careful using data members of this class
//   because each client handler thread gets a reference to this 
//   instance so you may get unwanted sharing.
// - I may change the SocketListener semantics (this summer) to pass
//   instances of this class by value.
// - that would mean that all ClientHandlers would need either copy or
//   move semantics.
//

class ClientHandler
{
public:
	ClientHandler(BlockingQueue<HttpMessage>& msgQ) : msgQ_(msgQ) {}
	void operator()(Socket socket);

	std::string currentDirectory;

private:
	bool connectionClosed_;
	HttpMessage readMessage(Socket& socket);
	void uploadFile(HttpMessage & msg, Socket & socket);
	bool publishFiles(std::string currentDir = "..");
	void downloadFile(HttpMessage msg);
	void browseFile(HttpMessage msg);
	bool readFile(const std::string& filename, size_t fileSize, Socket& socket);
	BlockingQueue<HttpMessage>& msgQ_;
};
