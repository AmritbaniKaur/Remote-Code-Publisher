//////////////////////////////////////////////////////////////////////////////////
// MsgClient.h -	Demonstrates simple one-way HTTP messaging					//
// Source:			Jim Fawcett, CSE687 - Object Oriented Design, Spring 2016	//
// ver 1.1																		//
//------------------------------------------------------------------------------//
//	Language:		Visual C++ 2015, Visual Studio 2015 Community Edition		//
//	Platform:		HP Envy x360, Core i7, Windows 10 Home						//
//	Application:	CSE 687- OOD Project #4, Spring 2017						//
//	Author:			Amritbani Sondhi,											//
//					Graduate Student, Syracuse University						//
//					asondhi@syr.edu												//
//////////////////////////////////////////////////////////////////////////////////

#include "../Sockets/Sockets.h"
#include "../FileSystem/FileSystem.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "MsgClient.h"

using namespace Logging;

using Show = StaticLogger<1>;
using namespace Utilities;
using Utils = StringHelper;

size_t ClientCounter::clientCount = 0;

//----< factory for creating messages >------------------------------
/*
 * This function only creates one type of message for this demo.
 * - To do that the first argument is 1, e.g., index for the type of message to create.
 * - The body may be an empty string.
 * - EndPoints are strings of the form ip:port, e.g., localhost:8081. This argument
 *   expects the receiver EndPoint for the toAddr attribute.
 */
HttpMessage MsgClient::makeMessage(size_t n, const std::string& body, const EndPoint& ep, std::string command, std::string filename)
{
  HttpMessage msg;
  HttpMessage::Attribute attrib;
  EndPoint myEndPoint = "localhost:8081";  
                                           
  switch (n)
  {
  case 1:
    msg.clear();

	// creates a message to pass to the client
	msg.addAttribute(HttpMessage::attribute(command, "Message"));
	msg.addAttribute(HttpMessage::Attribute("mode", "oneway"));
    msg.addAttribute(HttpMessage::parseAttribute("toAddr:" + ep));
    msg.addAttribute(HttpMessage::parseAttribute("fromAddr:" + myEndPoint));

	if (command == "DOWNLOAD")
	{
		if (filename != "")
		{
			// add a file name you want to download
			msg.addAttribute(HttpMessage::parseAttribute("downloadFile:" + filename));
		}
		else
		{
			std::cout << "\n Filename not specified with Download File Command";
		}
	}
	else if (command == "BROWSE")
	{
		if (filename != "")
		{
			// add a file name to browse
			msg.addAttribute(HttpMessage::parseAttribute("browseFile:" + filename));
		}
		else
		{
			std::cout << "\n Filename not specified with Browse File Command";
		}
	}
    msg.addBody(body);
    if (body.size() > 0)
    {
      attrib = HttpMessage::attribute("content-length", Converter<size_t>::toString(body.size()));
      msg.addAttribute(attrib);
    }
    break;

  default:
    msg.clear();
    msg.addAttribute(HttpMessage::attribute("Error", "unknown message type"));
  }
  return msg;
}

//----< send message using socket >----------------------------------
void MsgClient::sendMessage(HttpMessage& msg, Socket& socket)
{
  std::string msgString = msg.toString();
  socket.send(msgString.size(), (Socket::byte*)msgString.c_str());
}
//----< send file using socket >-------------------------------------
/*
 * - Sends a message to tell receiver a file is coming.
 * - Then sends a stream of bytes until the entire file
 *   has been sent.
 * - Sends in binary mode which works for either text or binary.
 */
bool MsgClient::sendFile(const std::string& filename, Socket& socket, std::string command)
{
	// assumes that socket is connected
	//std::string fqname = "../AbstractSyntaxTree/" + filename;
	std::string fqname = currentDirectory + filename;
  FileSystem::FileInfo fi(fqname);

  size_t fileSize = fi.size();
  std::string sizeString = Converter<size_t>::toString(fileSize);

  FileSystem::File file(fqname);
  file.open(FileSystem::File::in, FileSystem::File::binary);

  if (!file.isGood())
  {
	  std::cout << "file not found : " << fqname;
	  return false;
  }
  
  HttpMessage msg = makeMessage(1, "", "localhost::8080", command);
  msg.addAttribute(HttpMessage::Attribute("file", filename));
  msg.addAttribute(HttpMessage::Attribute("content-length", sizeString));
  sendMessage(msg, socket); // send the message to the server
  const size_t BlockSize = 2048;
  Socket::byte buffer[BlockSize];
  while (true)
  {
    FileSystem::Block blk = file.getBlock(BlockSize);
    if (blk.size() == 0)
      break;
    for (size_t i = 0; i < blk.size(); ++i)
      buffer[i] = blk[i];
    socket.send(blk.size(), buffer);
    if (!file.isGood())
      break;
  }
  file.close();
  return true;
}

// similar to execute() of MsgClient
// it defines the behavior of the client, to create a message and send the file to the server
void MsgClient::client_sendMessage(std::string command, std::string currentDir)
{
	// send NumMessages messages
	ClientCounter counter;
	size_t myCount = counter.count();
	std::string myCountString = Utilities::Converter<size_t>::toString(myCount);

	// initializing the Logger
	Show::attach(&std::cout);
	Show::start();

	Show::title("\n Starting Publish Message from the client" + myCountString);

	try
	{
		// initialize the socket
		SocketSystem ss;
		SocketConnecter si;
		while (!si.connect("localhost", 8080))
		{
			Show::write("\n client waiting to connect");
			::Sleep(100);
		}

		if (currentDir == "" && command == "PUBLISH")
		{
			currentDir = "../../1.UploadedFilesOnServer/";
		}
		if (currentDir == "")
		{
			currentDir = "..";
		}

		currentDirectory = currentDir;
		exeCommand = command;

		// send a set of messages
		HttpMessage msg;

		// triggers a breakpoint
		// to get All files in the directory for sending as a message
		std::vector<std::string> files = FileSystem::Directory::getFiles(currentDir, "*.cpp");
		for (size_t i = 0; i < files.size(); ++i)
		{
			std::string filename = files[i];
			std::string msgBody = "<msg> This is a " + command + " Message </msg>";
			msg = makeMessage(1, msgBody, "localhost:8080", command, filename);

			/*
			* Sender class will need to accept messages from an input queue
			* and examine the toAddr attribute to see if a new connection
			* is needed.  If so, it would either close the existing connection
			* or save it in a map[url] = socket, then open a new connection.
			*/
			sendMessage(msg, si);
			Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());
		}

		// only for upload
		if (command == "POST")
		{
			client_uploadFiles(si, currentDirectory, exeCommand);
		}

		if(command == "CLOSE")
		{
			// shut down server's client handler
			msg = makeMessage(1, "quit", "toAddr:localhost:8080", command);
			sendMessage(msg, si);
		}
		Show::write("\n\n  client" + myCountString + " sent\n" + msg.toIndentedString());

		Show::write("\n");
		Show::write("\n  All done folks");

	}
	catch (std::exception& exc)
	{
		Show::write("\n  Exception caught: ");
		std::string exMsg = "\n  " + std::string(exc.what()) + "\n\n";
		Show::write(exMsg);
	}
}

// upload the files to the server
void MsgClient::client_uploadFiles(Socket& si, std::string command, std::string currentDir) // error cmd = null
{
	command = exeCommand;

	currentDirectory = FileSystem::Path::getPath(currentDirectory);
	
	if (!FileSystem::Directory::exists(currentDirectory))
	{
		FileSystem::Directory::create(currentDirectory);
	}

	// sends the cpp files
	std::vector<std::string> files = FileSystem::Directory::getFiles(currentDirectory, "*.cpp");
	std::cout << "files:" << files.size();
	for (size_t i = 0; i < files.size(); ++i)
	{
		Show::write("\n\n  sending file " + files[i]);
		sendFile(files[i], si, command);
	}

	// sends the h files
	files = FileSystem::Directory::getFiles(currentDirectory, "*.h");
	for (size_t i = 0; i < files.size(); ++i)
	{
		Show::write("\n\n  sending file " + files[i]);
		sendFile(files[i], si, command);
	}
}

// entry point to the message client
bool MsgClient::startup_Client(std::string message)
{
	bool status = false;
	std::string guiMessage = message;
	std::string uploadDirectory = "../../4.TestFiles/"; // test directory
	std::string uploadDirec = FileSystem::Path::getPath(uploadDirectory);

	// checking the Case of the message
	transform(guiMessage.begin(), guiMessage.end(), guiMessage.begin(), ::toupper);

	if (guiMessage == "POST" || guiMessage == "DOWNLOAD" || guiMessage == "PUBLISH" || guiMessage == "BROWSE")
	{
		// message is validated, call for action
		MsgClient c1;
		c1.client_sendMessage(guiMessage, uploadDirec);
		status = true;
	}
	else
	{
		// return message as invalid
		status = false;
	}

	return status;
}


//----< entry point - runs two clients each on its own thread >------

//int main()
//{
//  ::SetConsoleTitle(L"Clients Running on Threads");
//  
//  std::string upload = "POST";
//  std::string download = "DOWNLOAD";
//  std::string publish = "PUBLISH";
//  std::string browse = "BROWSE";
//
//	MsgClient c1;
//	c1.startup_Client(upload);
//
//	//MsgClient c2;
//	//c1.client_sendMessage(publish);
//
//}
//
