// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include "Poco/Path.h"


namespace Kibio {


class Project;


/// \brief An abstract class representing application
class AbstractApp
{
public:
    /// \brief App modes.
    enum Mode
    {
        /// \brief In EDIT mode, users can modify the scene.
        EDIT,
        /// \brief In PRESENT mode, users cannot modify the scene.
        PRESENT
    };

    virtual ~AbstractApp()
    {
    }

    virtual Mode getMode() const = 0;

    /// \brief Get the user's projects path.
    /// \returns the user's projects path.
    virtual Poco::Path getUserProjectsPath() const = 0;

    /// \brief Get the current project.
    /// \returns the current project.
    virtual std::shared_ptr<Project> getCurrentProject() = 0;

};

    
} // namespace Kibio
