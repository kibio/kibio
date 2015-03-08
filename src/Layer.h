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


#include <json/json.h>
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
    typedef std::shared_ptr<Layer> SharedPtr;

    Layer(Project& parent);
    ~Layer();

    void update();
    void draw();

    bool hitTest(const ofPoint& point) const;

    ofPoint screenToLayer(const ofPoint& point); // const;
    ofPoint layerToScreen(const ofPoint& point); // const;

    bool loadVideo(const std::string& path);
    bool loadMask(const std::string& path);
    bool saveMask();

    void clearMask();

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const Layer& object);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, Layer& object);

    /// \brief Save the object to JSON.
    /// \brief The object to save.
    /// \returns the object as JSON.
    static Json::Value toJSON(const std::vector<ofPoint>& object);

    /// \brief Load the object from JSON.
    /// \brief json the object as JSON.
    /// \brief object the object to load from JSON.
    /// \returns true iff successful.
    static bool fromJSON(const Json::Value& json, std::vector<ofPoint>& object);

private:
    Project& _parent;

    ofFbo _surface;
    ofFbo _maskSurface;

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
