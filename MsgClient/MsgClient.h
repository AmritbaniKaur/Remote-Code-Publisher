#pragma once
//////////////////////////////////////////////////////////////////////////////////
// MsgClient.h -	Demonstrates simple one-way HTTP messaging					//
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
* This package implements a client that sends HTTP style messages and
* files to a server that simply displays messages and stores files.
*
* It's purpose is to provide a very simple illustration of how to use
* the Socket Package provided for Project #4.
*
* Public Methods:
* ==============
*	ClientCounter Class	:
*		count() - returns clientCount
*
*	MsgClient Class:
*		client_sendMessage() - sends relevant message to the Server
*		client_uploadFiles() - sends files to the server
*		startup_Client()	 - starts up client
*
* Build Process:
* ==============
* - Required Files:
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
* - added client_sendMessage(), client_uploadFiles(), startup_Client()
*/


#include "../Sockets/Sockets.h"
#include "../HttpMessage/HttpMessage.h"
#include <string>
#include <iostream>
#include <thread>

/////////////////////////////////////////////////////////////////////
// ClientCounter creates a sequential number for each client
//
class ClientCounter
{
public:
	ClientCounter() { ++clientCount; }
	size_t count() { return clientCount; }

private:
	static size_t clientCount;
};


/////////////////////////////////////////////////////////////////////
// MsgClient class
// - was created as a class so more than one instance could be 
//   run on child thread
//
class MsgClient
{
public:
	using EndPoint = std::string;

	std::string currentDirectory;
	std::string exeCommand;
	void client_sendMessage(std::string command, std::string currentDir = "");
	void client_uploadFiles(Socket& si, std::string command, std::string currentDir);
	bool startup_Client(std::string message);

private:
	HttpMessage makeMessage(size_t n, const std::string& msgBody, const EndPoint& ep, std::string command, std::string filename = "");
	void sendMessage(HttpMessage& msg, Socket& socket);
	bool sendFile(const std::string& fqname, Socket& socket, std::string command);

};
