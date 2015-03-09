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
#include "ofTypes.h"
#include "ofVideoPlayer.h"
#include "ofFbo.h"
#include "Layer.h"
#include "AbstractTypes.h"
#include "ofxMediaType.h"
//#include "ofxLibav.h"


namespace Kibio {


/// \brief A collection or Layers to be edited and presented.
class Project
{
public:
    
    enum TransformType {
        TRANSLATE,
        ROTATE,
        SCALE
    };
    
    /// \brief Create a project.
    /// \param parent A reference to the Project's parent.
    Project(AbstractApp& parent);

    /// \brief Destroy the project.
    ~Project();

    void update();
    void draw();

    /// \brief Drag event callback.
    /// \param dragInfo The drag event info object
    void dragEvent(ofDragInfo& dragInfo);

    bool makeRelativeToProjectFolder(Poco::Path& path) const;
    bool isFileInProjectFolder(const Poco::Path& path) const;
    
    /// \brief Create a new layer.
    /// \param videoPath The path to the video to use for the layer
    /// \param point The point at which to place the video layer
    void newLayerWithVideoAtPoint(const Poco::Path& videoPath, const ofPoint& point);
    
    /// \brief Set a mask for the layer.
    /// \param maskPath The path to the mask to use for the layer
    /// \param point The point at which to place the mask on the layer
    void setMaskForLayerAtPoint(const Poco::Path& maskPath, const ofPoint& point);

    /// \brief Delete a layer.
    /// \param point The point used to select the layer to delete
    void deleteLayerAtPoint(const ofPoint& point);
    
    /// \brief Delete the mask used by the layer.
    /// \param point The point used to select the mask to delete
    void clearMaskAtPoint(const ofPoint& point);

    /// \brief Get the layer at point.
    /// \param point The point used to get the layer by
    /// \returns The layer at point
    Layer::SharedPtr getLayerAtPoint(const ofPoint& point) const;

    /// \brief Load a project by name.
    /// \param name The name of the project in the user's project folder.
    /// \returns true if project is loaded.
    bool load(const std::string& name);
    
    /// \brief Create a new project
    /// \param name The name of the new project
    /// \param templateDir The name of the template project directory
    /// \returns true if project was successfully created
    bool create(const std::string& name, const std::string& templateDir);

    /// \brief Save a project.
    /// \returns true if project was saved successfully
    bool save();
    
    /// \brief Save As a project.
    /// \returns true if project was saved successfully
    bool saveAs(const std::string& name);

    /// \returns true if project is loaded.
    bool isLoaded() const;
    
    bool isCornerHovered(const ofPoint& point) const;

    /// \brief Get the project name.
    /// \returns the project name.
    std::string getName() const;

    /// \brief Get project path.
    /// \returns the project path.
    Poco::Path getPath() const;

    // static std::string toDebugString(const ofx::Media::AVMediaInfo& info);

    /// \brief Save the object to JSON.
    /// \param The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const Project& object);

    /// \brief Load the object from JSON.
    /// \param json the object as JSON.
    /// \param object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, Project& object);

    /// \brief The kibio settings file extension.
    static const std::string FILE_EXTENSION;

    void keyPressed(ofKeyEventArgs& key);
    void keyReleased(ofKeyEventArgs& key);

    void mouseMoved(ofMouseEventArgs& mouse);
    void mouseDragged(ofMouseEventArgs& mouse);
    void mousePressed(ofMouseEventArgs& mouse);
    void mouseReleased(ofMouseEventArgs& mouse);
    void mouseScrolled(ofMouseEventArgs& mouse);

private:
    /// \brief A reference to the project's parent.
    AbstractApp& _parent;
    
    TransformType _transform;

    /// \brief true iff the project is loaded.
    bool _isLoaded;

    /// \brief The project path.
    Poco::Path _path;

    /// \brief The layers.
    std::vector<Layer::SharedPtr> _layers;

    Layer::SharedPtr _dragging;
    ofPoint _dragStart;

    friend class Layer;
};

    
} // namespace Kibio
