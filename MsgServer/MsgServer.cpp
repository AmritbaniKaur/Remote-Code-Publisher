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

#include "MsgServer.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../CodePublisher/Publisher.h"

#include "../HttpMessage/HttpMessage.h"
#include "../Sockets/Sockets.h"
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../TestExecutive/TestExecutive.h"

#include <string>
#include <iostream>

using namespace Logging;
using Show = StaticLogger<1>;
using namespace Utilities;
using namespace FileSystem;	

//----< this defines processing to frame messages >------------------
HttpMessage ClientHandler::readMessage(Socket& socket)
{
  connectionClosed_ = false;
  HttpMessage msg;

  // read message attributes
  while (true)
  {
    std::string attribString = socket.recvString('\n');

    if (attribString.size() > 1)
    {
      HttpMessage::Attribute attrib = HttpMessage::parseAttribute(attribString);
      msg.addAttribute(attrib);
    }
    else
    {
      break;
    }
  }

  // If client is done, connection breaks and recvString returns empty string
  if (msg.attributes().size() == 0)
  {
    connectionClosed_ = true;
    return msg;
  }

  // read body if POST - all messages in this demo are POSTs
  
  std::string command = "";
  if (msg.attributes()[0].first == "POST")
  {
	  uploadFile(msg, socket);
  }
  else if (msg.attributes()[0].first == "PUBLISH")
  {
	  bool publishStatus = publishFiles("../../1.UploadedFilesOnServer/");
  }
  else if (msg.attributes()[0].first == "DOWNLOAD")
  {
	  // ToDo: INCOMPLETE
  }
  else if (msg.attributes()[0].first == "BROWSE")
  {
	  // ToDo: INCOMPLETE
  }
  return msg;
}

// reads the message received from the client and uploads the file if message is POST
void ClientHandler::uploadFile(HttpMessage & msg, Socket& socket)
{
	std::cout << "\n ===========================================================================================";
	std::cout << "\n Starting Upload (to Server)";

	// is this a file message?
	std::string filename = msg.findValue("file");

	if (filename != "")
	{
		size_t contentSize;
		std::string sizeString = msg.findValue("content-length");
		if (sizeString != "")
			contentSize = Converter<size_t>::toValue(sizeString);

		readFile(filename, contentSize, socket);
	}

	if (filename != "")
	{
		// construct message body
		msg.removeAttribute("content-length");
		std::string bodyString = "<file>" + filename + "</file>";
		std::string sizeString = Converter<size_t>::toString(bodyString.size());
		msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
		msg.addBody(bodyString);
	}
	else
	{
		// read message body
		size_t numBytes = 0;
		size_t pos = msg.findAttribute("content-length");
		if (pos < msg.attributes().size())
		{
			numBytes = Converter<size_t>::toValue(msg.attributes()[pos].second);
			Socket::byte* buffer = new Socket::byte[numBytes + 1];
			socket.recv(numBytes, buffer);
			buffer[numBytes] = '\0';
			std::string msgBody(buffer);
			msg.addBody(msgBody);
			delete[] buffer;
		}
	}
	std::cout << "\n ===========================================================================================";
}

// reads the message received from the client and publishes the files if message is PUBLISH
bool ClientHandler::publishFiles(std::string currentDir)
{
	currentDirectory = currentDir;

	bool status = false;

	std::cout << "\n ==============================================================";
	std::cout << "\n Publishing Files to the Server";

	try
	{
		CodePublisher::HTMLPublisher htmlPublish;

		if (currentDir == "")
		{
			// Directory where files were Uploaded to the Server
			Directory::setCurrentDirectory("../../1.UploadedFilesOnServer/");
		}
		else
		{
			Directory::setCurrentDirectory(currentDirectory);
		}

		// gets contents of current directory
		string currentDir = Directory::getCurrentDirectory(); // gets current directory
		htmlPublish.processDirectoryToHTML(currentDir);

		std::cout << "\n Files successfully uploaded to the Server!";
		std::cout << "\n Peak the Files at: ../2.PublishedFilesOnServer/";
		std::cout << "\n ==============================================================";

		status = true;
	}
	catch (exception ex)
	{
		status = false;
		std::cout << "Exception occurred in RemotePublisher::publishFiles()";
	}
	return status;
}

//size_t ClientCounter::clientCount = 0;

// reads the message received from the client and downloads the file if message is DOWNLOAD
void ClientHandler::downloadFile(HttpMessage msg)
{
	// is this a file message?
	std::string filename = msg.findValue("file");
	//std::string filename = msg.attributes()[msg.findAttribute("downloadFile")].second;

	if (filename != "")
	{
		// functionality to download file
	}
	else
	{
		std::cout << "File not found for downloading";
	}

}

// reads the message received from the client and browses all the files if message is BROWSE
void ClientHandler::browseFile(HttpMessage msg)
{
	// is this a file message?
	std::string filename = msg.attributes()[msg.findAttribute("downloadFile")].second;

	if (filename != "")
	{
		// functionality to download file
	}
	else
	{
		std::cout << "File not found for browsing";
	}

}

//----< read a binary file from socket and save >--------------------
/*
 * This function expects the sender to have already send a file message, 
 * and when this function is running, continuosly send bytes until
 * fileSize bytes have been sent.
 */
bool ClientHandler::readFile(const std::string& filename, size_t fileSize, Socket& socket)
{
	if (!FileSystem::Directory::exists("../../1.UploadedFilesOnServer/"))
	{
		FileSystem::Directory::create("../../1.UploadedFilesOnServer/");
	}

	//std::string fqname = "../TestFiles/" + filename; //+ ".snt";
	std::string fqname = "../../1.UploadedFilesOnServer/" + filename;

	FileSystem::File file(fqname);
	file.open(FileSystem::File::out, FileSystem::File::binary);
	if (!file.isGood())
	{
    /*
     * This error handling is incomplete.  The client will continue
     * to send bytes, but if the file can't be opened, then the server
     * doesn't gracefully collect and dump them as it should.  That's
     * an exercise left for students.
     */
    Show::write("\n\n  can't open file " + fqname);
    return false;
	}

	const size_t BlockSize = 2048;
	Socket::byte buffer[BlockSize];

	size_t bytesToRead;
	while (true)
	{
		if (fileSize > BlockSize)
		bytesToRead = BlockSize;
		else
		bytesToRead = fileSize;

    socket.recv(bytesToRead, buffer);

    FileSystem::Block blk;
    for (size_t i = 0; i < bytesToRead; ++i)
      blk.push_back(buffer[i]);

    file.putBlock(blk);
    if (fileSize < BlockSize)
      break;
    fileSize -= BlockSize;
  }
  file.close();
  return true;
}

//----< receiver functionality is defined by this function >---------
void ClientHandler::operator()(Socket socket)
{
  /*
   * There is a potential race condition due to the use of connectionClosed_.
   * If two clients are sending files at the same time they may make changes
   * to this member in ways that are incompatible with one another.  This
   * race is relatively benign in that it simply causes the readMessage to 
   * be called one extra time.
   *
   * The race is easy to fix by changing the socket listener to pass in a 
   * copy of the clienthandler to the clienthandling thread it created.  
   * I've briefly tested this and it seems to work.  However, I did not want
   * to change the socket classes this late in your project cycle so I didn't
   * attempt to fix this.
   */
  while (true)
  {
    HttpMessage msg = readMessage(socket);

    if (connectionClosed_ || msg.bodyString() == "quit")
    {
      Show::write("\n\n  clienthandler thread is terminating");
      break;
    }
    msgQ_.enQ(msg);
  }
}

//----< test stub >--------------------------------------------------
int main()
{
	TestExecutive test;
	test.testExecutive();

	::SetConsoleTitle(L"HttpMessage Server");

	Show::attach(&std::cout);
	Show::start();
	Show::title("\n HttpMessage Server started");

  BlockingQueue<HttpMessage> msgQ;

  try
  {
    SocketSystem ss;
    SocketListener sl(8080, Socket::IP6);
    ClientHandler cp(msgQ);
    sl.start(cp);

    /*
     * Since this is a server the loop below never terminates.
     * We could easily change that by sending a distinguished 
     * message for shutdown.
     */
    while (true)
    {
      HttpMessage msg = msgQ.deQ();
      Show::write("\n\n  server recvd message contents:\n" + msg.bodyString());
    }
  }
  catch (std::exception& exc)
  {
    Show::write("\n  Exception caught: ");
    std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
    Show::write(exMsg);
  }
}
