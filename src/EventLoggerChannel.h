// =============================================================================
//
// Copyright (c) 2014-2015 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <chrono>
#include <string>
#include "ofLog.h"
#include "ofEvents.h"
#include "Poco/Timestamp.h"


class LoggerEventArgs: public ofEventArgs
{
public:
    ofLogLevel level;
    std::string module;
    std::string message;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
};


class EventLoggerChannel: public ofConsoleLoggerChannel
{
public:
    typedef std::shared_ptr<EventLoggerChannel> SharedPtr;

    EventLoggerChannel();

    virtual ~EventLoggerChannel();

    void log(ofLogLevel level,
             const std::string& module,
             const std::string& message);

    void log(ofLogLevel level,
             const std::string& module,
             const char* format, ...);

    void log(ofLogLevel level,
             const std::string& module,
             const char* format,
             va_list args);

    ofEvent<const LoggerEventArgs> event;

};
