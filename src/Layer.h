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
#include "Poco/UUIDGenerator.h"
#include "Poco/UUID.h"
#include "ofTypes.h"
#include "ofVideoPlayer.h"
#include "ofFbo.h"
#include "ofxQuadWarp.h"


namespace Kibio {


class Project;


/// \brief A single video and mask layer.
class Layer
{
public:
    /// \brief A typedef for a shared layer.
    typedef std::shared_ptr<Layer> SharedPtr;

    /// \brief Layer constructor.
    /// \param A reference to the layer's project.
    Layer(Project& parent);

    /// \brief Destroy a layer.
    ~Layer();

    void update();
    void draw();

    /// \brief Check if the point is inside the layer.
    /// \param point The point to test.
    /// \returns true if point is inside the layer.
    bool hitTest(const ofPoint& point) const;
    
    /// \brief Get the corner at mouse position
    /// \param mouse The point to test the hit with
    /// \returns pointer to corner if mouse is inside, nullptr if not
    const ofPoint* getHoveredCorner(const ofPoint& mouse) const;
    
    /// \brief Get the centroid of the layer
    /// \returns ofPoint shared pointer representing the centroid of the layer
    const ofPoint getCentroid() const;

    /// \brief Get the screen point in layer space coordinates.
    /// \param point The point in screen space.
    /// \returns point The point in layer space.
    ofPoint screenToLayer(const ofPoint& point); // const;
    
    /// \brief Get the layer point in screen space coordinates
    /// \param point point in layer space
    /// \returns point in screen space
    ofPoint layerToScreen(const ofPoint& point); // const;

    /// \brief Load a video into the layer.
    /// \param path The path to video file.
    /// \returns true if loaded successfully.
    bool loadVideo(const std::string& path);
    
    /// \brief Load a mask for the layer.
    /// \param path The path to mask file.
    /// \returns true if loaded successfully.
    bool loadMask(const std::string& path);
    
    /// \brief Save the mask for the layer.
    /// \returns true if saved successfully.
    bool saveMask();

    /// \brief Clear the layer mask.
    void clearMask();
    
    /// \brief Translate the layer.
    void translate(const ofPoint& delta);
    
    /// \brief Rotate the layer.
    void rotate(int degrees);
    
    /// \brief Scale the layer.
    void scale(int mult);
    
    const Poco::UUID getId() const;

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const Layer& object);

    /// \brief Load the object from JSON.
    /// \brief json The object as JSON.
    /// \brief object The object to load from JSON.
    /// \returns true iff deserialized successfully.
    static bool fromJSON(const Json::Value& json, Layer& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const std::vector<ofPoint>& object);

    /// \brief Load the object from JSON.
    /// \brief json The object as JSON.
    /// \brief object The object to load from JSON.
    /// \returns true iff deserialized successfully.
    static bool fromJSON(const Json::Value& json, std::vector<ofPoint>& object);

private:
    Project& _parent;
    Poco::UUID _id;

    ofFbo _surface;
    ofFbo _maskSurface;
    
    ofColor _color;
    ofColor _highlightColor;

    std::string _videoPath;
    std::string _maskPath;

    std::shared_ptr<ofVideoPlayer> _video;
    std::shared_ptr<ofTexture> _mask;

    bool _maskDirty;

    /// \brief The quad warper.
    ofxQuadWarp _warper;

    ofTexture _brushTex;

    ofShader _maskShader;
    ofShader _frameCombineShader;

    friend class Project;
};

    
} // namespace Kibio
