// =============================================================================
//
// Copyright (c) 2014 Christopher Baker <http://christopherbaker.net>
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


#include "SimpleApp.h"
#include "Poco/Environment.h"
#include "Poco/FileStream.h"
#include "ofLog.h"
#include "ofUtils.h"


namespace Kibio {


const std::string SimpleApp::DEFAULT_SETTINGS_PATH("defaults/");
const std::string SimpleApp::DEFAULT_EXAMPLES_PATH("examples/");
const std::string SimpleApp::DEFAULT_TEMPLATE_PROJECT_PATH("templates/TemplateProject");
const std::string SimpleApp::SETTINGS_FILENAME("settings.json");
const std::string SimpleApp::USER_SETTINGS_PATH(".kibio/");
const std::string SimpleApp::DEFAULT_USER_PROJECTS_PATH("Kibio/");
const std::string SimpleApp::DEFAULT_PROJECT("HelloWorld");


SimpleApp::SimpleApp():
    _version(0),
    _mode(EDIT),
    _logger(EventLoggerChannel::makeShared())
{
    ofAddListener(_logger->event, this, &SimpleApp::onLoggerEvent);
    ofSetLoggerChannel(_logger);
    ofSetLogLevel("ofFbo", OF_LOG_ERROR);
    ofSetLogLevel("ofAVFoundationPlayer::play", OF_LOG_ERROR);
    ofSetLogLevel("ofShader", OF_LOG_ERROR);
    ofSetLogLevel("linkProgram()", OF_LOG_ERROR);
}


SimpleApp::~SimpleApp()
{
    ofRemoveListener(_logger->event, this, &SimpleApp::onLoggerEvent);
    ofLogToConsole();
}


void SimpleApp::setup()
{
#ifdef TARGET_OSX
    ofSetDataPathRoot("../Resources/data/");
#endif

    ofSetFrameRate(60);
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetWindowTitle("Kibio");
    ofEnableAntiAliasing();
    ofEnableTextureEdgeHack();
    ofSetCircleResolution(50);

    ofLoadImage(_kibioLogo, "images/kibio.png");
    ofLoadImage(_kibioLogoMini, "images/kibio-k.png");

    loadSettings();
    _ui.placeIcons();
}

void SimpleApp::exit()
{
    if (_currentProject)
    {
        if (!saveProject())
        {
            ofLogError("SimpleApp::exit") << "Failed to save project: " << _currentProject->getName();
        }
    }

    saveSettings();
}


void SimpleApp::update()
{
    if (_currentProject)
    {
        _currentProject->update();
    }
    _ui.update();
}


void SimpleApp::draw()
{
    switch (_mode)
    {
        case EDIT:
            ofBackgroundHex(0x00AEEF);
            break;
        case PRESENT:
            ofBackground(0);
            break;
    }

    ofSetColor(255);

    if (_currentProject)
    {
        _currentProject->draw();
    }


    if (EDIT == _mode)
    {
//        ofSetColor(255);
//        _kibioLogoMini.draw(10, ofGetHeight() - _kibioLogoMini.getHeight() - 10);
//    
//        std:stringstream str;
//        
//        str << "cmd-e: Toggle Edit Mode\n";
//        str << "cmd-f: Toggle Fullscreen\n";
//        str << "cmd-n: New Project\n";
//        str << "cmd-o: Open Project\n";
//        str << "cmd-s: Save Project\n";
//        str << "cmd-shift-s: Save As Project\n";
//        str << "delete: Delete Video (when hovered)\n";
//        
//        ofDrawBitmapString(str.str(), 15, 15);
    }
    
    if (_ui.isVisible())
    {
        _ui.draw();
    }
}


void SimpleApp::keyPressed(ofKeyEventArgs& key)
{
    if (ofGetKeyPressed(OF_KEY_COMMAND))
    {
        if ('k' == key.key)
        {
            // is this platform independent?
            ofSystem("open " + getUserProjectsPath().toString());
        }
        else if ('e' == key.key)
        {
            switch (_mode)
            {
                case EDIT:
                    _mode = PRESENT;
                    break;
                case PRESENT:
                    _mode = EDIT;
                    break;
            }
        }
        else if ('f' == key.key)
        {
            ofToggleFullscreen();
        }
        else if ('o' == key.key)
        {
            ofFileDialogResult result = ofSystemLoadDialog("Open Project",
                                                           false,
                                                           getUserProjectsPath().toString());

            Poco::Path relativePath(result.getPath());

            if (result.bSuccess && makeRelativeToUserProjectsFolder(relativePath))
            {
                std::string projectName = relativePath.getBaseName();
                loadProject(projectName);
            }
            else
            {
                ofSystemAlertDialog("Invalid project file.");
            }
        }
        else if ('n' == key.key)
        {
            
            std::string result = ofSystemTextBoxDialog("Project Name");
            if (!result.empty())
            {
                createProject(result);
            }
        }
        else if ('s' == key.key)
        {
            if (_currentProject)
            {
                if (ofGetKeyPressed(OF_KEY_SHIFT))
                {
                    std::string result = ofSystemTextBoxDialog("Project Name");
                    if (!result.empty())
                    {
                        saveProjectAs(result);
                    }
                }
                else
                {
                    _currentProject->save();
                }
            }
        }
    }
}
    
void SimpleApp::windowResized(ofResizeEventArgs &resize)
{
    _ui.placeIcons();
}

AbstractApp::Mode SimpleApp::getMode() const
{
    return _mode;
}


Poco::Path SimpleApp::getUserProjectsPath() const
{
    Poco::File userProjectsFile(_userProjectsPath);

    if (!userProjectsFile.exists())
    {
        ofLogVerbose("Settings::setup") << "Creating project directory: " << userProjectsFile.path();

        try
        {
            // Only create one level deep.
            // If the user does not have a /User/*/Documents/ folder, fail.
            userProjectsFile.createDirectory();

            Poco::Path exampleProjectPath(getExamplesPath(),
                                          Poco::Path::forDirectory(DEFAULT_PROJECT));

            Poco::File exampleProjectFile(exampleProjectPath);

            exampleProjectFile.copyTo(userProjectsFile.path());
        }
        catch (const Poco::Exception& exc)
        {
            ofLogError("Settings::setup") << exc.displayText();
            ofSystemAlertDialog("Error: A Kibio projects folder does not exist and one could not be created");
            
        }
    }

    return _userProjectsPath;
}


void SimpleApp::setUserProjectsPath(const std::string& path)
{
    _userProjectsPath = Poco::Path::forDirectory(path);
}


Poco::Path SimpleApp::getExamplesPath() const
{
    return Poco::Path::forDirectory(ofToDataPath(DEFAULT_EXAMPLES_PATH, true));
}


std::shared_ptr<Project> SimpleApp::getCurrentProject()
{
    return _currentProject;
}
    
bool SimpleApp::createProject(const std::string& name)
{
    std::shared_ptr<Project> project = std::shared_ptr<Project>(new Project(*this));
    if (!project->create(name, SimpleApp::DEFAULT_TEMPLATE_PROJECT_PATH)) {
        ofSystemAlertDialog("Could not create project.\nMake sure that you do not already have a project by that name.");
        return false;
    } else {
        return loadProject(name, project);
    }
}

bool SimpleApp::loadProject(const std::string& name)
{
    std::shared_ptr<Project> project = std::shared_ptr<Project>(new Project(*this));
    return loadProject(name, project);
}

bool SimpleApp::loadProject(const std::string& name, std::shared_ptr<Project> project)
{
    // \todo Move this method

    if (project->load(name))
    {
        // if there is an existing project
        if (_currentProject)
        {
            if (!saveProject()) ofSystemAlertDialog("Error saving current project.");
        }

        _currentProject = project;
        return true;
    }
    else
    {
        ofLogError("SimpleApp::loadProject") << "Project \"" << name << "\" does not exist.";
        ofSystemAlertDialog("Error loading project. Are you sure that is a \".kibio\" file?");
        return false;
    }

}


bool SimpleApp::onLoggerEvent(const LoggerEventArgs& e)
{
    return false;
}


bool SimpleApp::saveProject()
{
    if (_currentProject)
    {
        return _currentProject->save();
    }
    else
    {
        return false;
    }
}


bool SimpleApp::saveProjectAs(const std::string& name)
{
    if (_currentProject)
    {
        if (!_currentProject->saveAs(name))
        {
            ofSystemAlertDialog("Could not Save As project.\nMake sure that you do not already have a project by that name.");
            return false;
        }
        
        return loadProject(name);
    }
    else
    {
        return false;
    }
}

void SimpleApp::loadSettings()
{
    Poco::Path settingsDirectoryPath(Poco::Path::home(), Poco::Path(USER_SETTINGS_PATH));
    Poco::File settingsDirectory(settingsDirectoryPath);

    try
    {
        if (!settingsDirectory.exists())
        {
            ofLogVerbose("SimpleApp::loadSettings") << "Settings directory " << settingsDirectory.path() << " does not exist.";

            if (settingsDirectory.createDirectory())
            {
                ofLogVerbose("SimpleApp::loadSettings") << "Creating settings directory " << settingsDirectory.path();
            }
            else
            {
                ofLogError("SimpleApp::loadSettings") << "Unable to create settings directory: " << settingsDirectory.path();
            }
        }

        Poco::Path userSettingsPath(settingsDirectoryPath, SETTINGS_FILENAME);
        Poco::File userSettingsFile(userSettingsPath);

        if (!userSettingsFile.exists())
        {
            Poco::Path defaultSettingsPath(ofToDataPath(DEFAULT_SETTINGS_PATH,
                                                        true),
                                           SETTINGS_FILENAME);

            Poco::File defaultSettingsFile(defaultSettingsPath);

            ofLogVerbose("SimpleApp::loadSettings") << "Default settings at: " << defaultSettingsFile.path();

            if (defaultSettingsFile.exists())
            {
                ofLogVerbose("SimpleApp::loadSettings") << "Default settings at: " << defaultSettingsFile.path() << " are being copied to " << settingsDirectory.path();
                defaultSettingsFile.copyTo(userSettingsFile.path());
            }
            else
            {
                ofLogFatalError("SimpleApp::loadSettings") << "Unable to locate default settings: " << defaultSettingsFile.path();
            }
        }

        try
        {
            Poco::FileInputStream fis(userSettingsFile.path());

            Json::Value json;
            Json::Reader reader;

            if (reader.parse(fis, json))
            {
                fromJSON(json, *this);
            }
            else
            {
                ofLogError("SimpleApp::loadSettings") << "Unable to parse " << userSettingsFile.path() << ": " << reader.getFormattedErrorMessages();
            }

            fis.close();
        }
        catch (const Poco::Exception& exc)
        {
            ofLogError("Settings::setup") << exc.displayText();
        }
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Settings::setup") << exc.displayText();
    }

}


void SimpleApp::saveSettings()
{
    Poco::Path settingsDirectoryPath(Poco::Path::home(),
                                     Poco::Path(USER_SETTINGS_PATH,
                                                SETTINGS_FILENAME));
    try
    {
        Poco::FileOutputStream fos(settingsDirectoryPath.toString());
        Json::StyledWriter writer;
        fos << writer.write(toJSON(*this));
        fos.close();
    }
    catch (const Poco::Exception& exc)
    {
        ofLogError("Settings::save") << exc.displayText();
    }
}


bool SimpleApp::fromJSON(const Json::Value& json, SimpleApp& object)
{
    std::string mode = json.get("mode", "edit").asString();

    if ("edit" == mode)
    {
        object._mode = EDIT;
    }
    else
    {
        object._mode = PRESENT;
    }

#ifdef OF_TARGET_LINUX
    Poco::Path defaultUserProjectsPath(Poco::Path::home(), Poco::Path(DEFAULT_USER_PROJECTS_PATH));
#else
    Poco::Path defaultUserProjectsPath(Poco::Path::home(), Poco::Path("Documents/", DEFAULT_USER_PROJECTS_PATH));
#endif

    if (json.isMember("paths") && json["paths"].isMember("project"))
    {
        object.setUserProjectsPath(json["paths"]["project"].asString());
    }
    else
    {
        object.setUserProjectsPath(defaultUserProjectsPath.toString());
    }

    object._version = json.get("version", 0).asInt();


    if (json.isMember("project"))
    {
        // TODO: load default project if last open project has been deleted
        object.loadProject(json["project"].asString());
    }


    if (json.isMember("screen"))
    {
        ofSetWindowPosition(json["screen"].get("x", 100).asInt(),
                            json["screen"].get("y", 100).asInt());

        ofSetWindowShape(json["screen"].get("width", 640).asInt(),
                         json["screen"].get("height", 480).asInt());
        
        ofSetFullscreen(json["screen"].get("fullscreen", false).asBool());
    }
    
    
    return true;
}


Json::Value SimpleApp::toJSON(const SimpleApp& object)
{
    Json::Value json;

    switch (object._mode)
    {
        case EDIT:
            json["mode"] = "edit";
            break;
        case PRESENT:
            json["mode"] = "present";
            break;
    }

    json["paths"]["projects"] = object.getUserProjectsPath().toString();

    json["version"] = object._version;

    if (object._currentProject && object._currentProject->isLoaded())
    {
        json["project"] = object._currentProject->getName();
    }

    json["screen"]["x"] = ofGetWindowPositionX();
    json["screen"]["y"] = ofGetWindowPositionY();
    json["screen"]["width"] = ofGetWindowWidth();
    json["screen"]["height"] = ofGetWindowHeight();
    json["screen"]["fullscreen"] = (ofGetWindowMode() == OF_FULLSCREEN);
    
    return json;
}



} // namespace Kibio
