============================================================================================================================================

Remote Repository:
------------------

Req 1 : uses Visual Studio 2015 and its C++ Windows console projects. Also, used Windows Presentation Foundation (WPF) to provide a required 
		client Graphical User Interface (GUI).
	
Req 2 : used the C++ standard library's streams for all console I/O and new and delete for all heap-based memory management.

Req 3 : provides a Repository program that provides functionality to publish, as linked web pages, the contents of a set of C++ source code 
		files.
	
Req 4 : for the publishing process, satisfies the requirements of CodePublisher developed in Project #3.

Req 5 : provides a Client program that can upload files, and views Repository contents.

Req 6 : provides a message-passing communication system, based on Sockets, used to access the Repository's functionality from another process 
		or machine.
	
Req 7 : The communication system provides support for passing HTTP style messages using either synchronous request/response or asynchronous 
		one-way messaging.
	
Req 8 : The communication system also supports sending and receiving streams of bytes. Streams will be established with an initial exchange 
		of messages.
	
Req 9 : includes an automated unit test suite that demonstrates all the requirements of this project including the transmission of files.

Req 10: (optional) use a lazy download strategy, that, when presented with a name of a source code web page, will download that file and 
		all the files it links to. This allows you to demonstrate the project using local webpages instead of downloading the entire contents 
		of the Code Publisher for demonstration.
	
Req 11: (optional) have the publisher accept a path, on the commandline, to a virtual directory on the server. Then support browsing directly 
		from the server by supplying a url to that path when you start a browser. This works only if IIS is setup on the machine and make the 
		path a virtual directory.

============================================================================================================================================