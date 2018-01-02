#ifndef LOGGER_H
#define LOGGER_H
/////////////////////////////////////////////////////////////////////
// Logger.h - log text messages to std::ostream                    //
// ver 1.3                                                         //
//-----------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                  //
// All rights granted provided this copyright notice is retained   //
//-----------------------------------------------------------------//
// Language:    C++, Visual Studio 2015                            //
// Application: Several Projects, CSE687 - Object Oriented Design  //
// Author:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package supports logging for multiple concurrent clients to a
* single std::ostream.  It does this be enqueuing messages in a
* blocking queue and dequeuing with a single thread that writes to
* the std::ostream.
*
* Build Process:
* --------------
* Required Files: Logger.h, Logger.cpp, Utilities.h, Utilities.cpp
*
* Build Command: devenv logger.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 22 Feb 2016
* - first release
* ver 1.2 : 
* - used in CommPrototype
* ver 1.3 : 28 Apr 2017
* - merged namespace and functions from ver 1.0 and ver 1.2
* Planned Additions and Changes:
* ------------------------------
* - none yet
*/

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include "Cpp11-BlockingQueue.h"

namespace Logging
{
	class Logger
	{
	public:
	  Logger() {}
	  void attach(std::ostream* pOut);
	  void start();
	  bool running();
	  void pause(bool doPause);
	  bool paused();
	  void flush();
	  void stop(const std::string& msg = "");
	  void wait();
	  void write(const std::string& msg, std::string abc = "");
	  void title(const std::string& msg, char underline = '-');
	  ~Logger();
	  Logger(const Logger&) = delete;
	  Logger& operator=(const Logger&) = delete;
	private:
	  std::thread* _pThr;
	  std::vector<std::ostream*> streams_;
	  std::ostream* _pOut;
	  BlockingQueue<std::string> _queue;
	  bool _ThreadRunning = false;
	  bool _Paused = false;

	};

	template<int i>
	class StaticLogger
	{
	public:
	  static void attach(std::ostream* pOut) { _logger.attach(pOut); }
	  static void start() { _logger.start(); }
	  static bool running() { return _logger.running(); }
	  static void pause(bool doPause) { _logger.pause(doPause); }
	  static void paused() { return _logger.paused(); }
	  static void flush() { _logger.flush(); }
	  static void stop(const std::string& msg="") { _logger.stop(msg); }
	  static void wait() { _logger.wait(); }
	  static void write(const std::string& msg) { _logger.write(msg); }
	  static void title(const std::string& msg, char underline = '-') { _logger.title(msg, underline); }
	  static Logger& instance() { return _logger; }
	  StaticLogger(const StaticLogger&) = delete;
	  StaticLogger& operator=(const StaticLogger&) = delete;
	private:
	  static Logger _logger;
	};

	template<int i>
	Logger StaticLogger<i>::_logger;
}
#endif
