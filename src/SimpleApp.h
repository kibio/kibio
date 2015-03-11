// =============================================================================
//
// Copyright (c) 2014-2015 Christopher Baker <http://christopherbaker.net>
//               2015 Brannon Dorsey <http://brannondorsey.com>
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


#include <json/json.h>
#include "ofMain.h"
#include "UserInterface.h"
#include "Project.h"
#include "AbstractTypes.h"
#include "EventLoggerChannel.h"


namespace Kibio {


/// \brief A simple version of Kibio for direct screen interactions.
class SimpleApp: public AbstractApp, public ofBaseApp
{
public:
    /// \brief Create a SimpleApp.
    SimpleApp();

    /// \brief Destroy the SimpleApp.
    ~SimpleApp();
    
    void setup();
    void update();
    void draw();

    /// \brief Exit callback.
    void exit();

    void keyPressed(ofKeyEventArgs& key);
    void windowResized(ofResizeEventArgs &resize);
    
    /// \brief Get the current mode.
    Mode getMode() const;

    /// \brief Get the user's projects path.
    Poco::Path getUserProjectsPath() const;

    /// \brief Get the user's projects path.
    std::shared_ptr<Project> getCurrentProject();

    /// \brief Set the user's projects path.
    /// \param the user's projects path.
    void setUserProjectsPath(const std::string& path);

    /// \brief Get the examples path.
    /// \returns the examples path.
    Poco::Path getExamplesPath() const;

    /// \brief Load the app settings.
    void loadSettings();

    /// \brief Save the app settings.
    void saveSettings();

    /// \brief Create a new project.
    /// \param name The name of the new project.
    bool createProject(const std::string& name);
    
    /// \brief Load a project by name.
    /// \param name The name of the project.
    bool loadProject(const std::string& name);
    
    /// \brief Load a project by name.
    /// \param name The name of the project.
    /// \param project The project to load if the pointer already exists.
    bool loadProject(const std::string& name, std::shared_ptr<Project> project);

    /// \brief Save the current project.
    bool saveProject();

    /// \brief Save As the current project.
    bool saveProjectAs(const std::string& name);

    bool onLoggerEvent(const LoggerEventArgs& e);
    
    void onUIButtonSelect(const UserInterfaceEvent& args);
    void onUIButtonDeselect(const UserInterfaceEvent& args);
    
    enum
    {
        /// \brief Settings version.
        SETTINGS_VERSION = 0
    };

    /// \brief Load the object from JSON.
    /// \param json the object as JSON.
    /// \param object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, SimpleApp& object);

    /// \brief Save the object to JSON.
    /// \param The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const SimpleApp& object);

    /// \brief The path for the default settings, relative to bin/data.
    static const std::string DEFAULT_SETTINGS_PATH;

    /// \brief The path for the default examples, relative to bin/data.
    static const std::string DEFAULT_EXAMPLES_PATH;

    /// \brief The settings filename.
    static const std::string SETTINGS_FILENAME;

    /// \brief The absolute path for the user-specific Kibio settings.
    static const std::string USER_SETTINGS_PATH;

    /// \brief The default path for the user's projects.
    static const std::string DEFAULT_USER_PROJECTS_PATH;

    /// \brief The default name for the default project.
    static const std::string DEFAULT_PROJECT;
    
    /// \brief The default name for the template project directory.
    static const std::string DEFAULT_TEMPLATE_PROJECT_PATH;

    bool makeRelativeToUserProjectsFolder(Poco::Path& path) const
    {
        if (isFileInUserProjectsFolder(path))
        {
            Poco::Path projectPath = _userProjectsPath;
            Poco::Path otherPath = path;

            projectPath.makeAbsolute();
            otherPath.makeAbsolute();

            while (projectPath[0] == otherPath[0])
            {
                projectPath.popFrontDirectory();
                otherPath.popFrontDirectory();
            }

            path = Poco::Path(); // Make relative path.

            // Transfer directories.
            for (std::size_t i = 0; i < otherPath.depth(); ++i)
            {
                path.pushDirectory(otherPath.directory(i));
            }

            // Set the file name.
            path.setFileName(otherPath.getFileName());

            return true;
        }
        else
        {
            return false;
        }
    }


    bool isFileInUserProjectsFolder(const Poco::Path& path) const
    {
        Poco::Path projectPath = _userProjectsPath;
        Poco::Path otherPath = path;

        projectPath.makeAbsolute();
        otherPath.makeAbsolute();

        if (projectPath.depth() > otherPath.depth()) return false;

        for (std::size_t i = 0; i < projectPath.depth(); ++i)
        {
            if (i < otherPath.depth())
            {
                std::string parent = projectPath.directory(i);
                std::string other = otherPath.directory(i);
                
                if (parent != other)
                {
                    return false;
                }
            }
        }
        
        return true;
    }
    

protected:
    /// \brief The current app mode.
    Mode _mode;

    /// \brief The user projects path.
    Poco::Path _userProjectsPath;

    /// \brief The examples path.
    Poco::Path _examplesPath;

    /// \brief The settings version.
    int _version;
    
    UserInterface _ui;

    /// \brief The current project.
    std::shared_ptr<Project> _currentProject;

    /// \brief The event logger channel.
    std::shared_ptr<EventLoggerChannel> _logger;

    ofTexture _kibioLogo;
    ofTexture _kibioLogoMini;

};

    
} // namespace Kibio
