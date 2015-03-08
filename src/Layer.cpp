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


#include "Layer.h"
#include "Project.h"


namespace Kibio {


Layer::Layer(Project& parent):
    _parent(parent),
    _maskDirty(true),
    _id(Poco::UUIDGenerator().createRandom()),
    _color(ofColor(255, 255, 255)),
    _highlightColor(255, 255, 0)
{
    _maskShader.load("shaders/GL3/mask");
    _frameCombineShader.load("shaders/GL3/frame_combine");

    _surface.allocate(1, 1, GL_RGBA, 8);
    _maskSurface.allocate(1, 1, GL_RGBA, 8);

    ofLoadImage(_brushTex, "brushes/brush.png");
    
}


Layer::~Layer()
{
}


void Layer::update()
{
    if (_video)
    {
        _video->update();

        if (_video->isLoaded() &&
            (_surface.getWidth() != _video->getWidth() ||
            _surface.getHeight() != _video->getHeight()))
        {
            _video->play();
            _video->setLoopState(OF_LOOP_NORMAL);
            _video->getTexture();

            float sW = _video->getWidth();
            float sH = _video->getHeight();

            ofLogNotice("Layer::update") << "Allocating surface: " << sW << " / " << sH;
            _surface.allocate(sW, sH, GL_RGBA, 8);
            _maskSurface.allocate(sW, sH, GL_RGBA, 8);
            _maskDirty = true;
            ofLogNotice("Layer::update") << "Initializing warper.";

            bool isWarperSourceSet = false;

            for (std::size_t i = 0; i < 4; ++i)
            {
                if (_warper.srcPoints[i] != ofPoint::zero())
                {
                    isWarperSourceSet = true;
                    break;
                }
            }

            if (!isWarperSourceSet)
            {
                _warper.setSourceRect(ofRectangle(0,
                                                  0,
                                                  _video->getWidth(),
                                                  _video->getHeight()));
            }

            bool isWarperDestinationSet = false;

            for (std::size_t i = 0; i < 4; ++i)
            {
                if (_warper.dstPoints[i] != ofPoint::zero())
                {
                    isWarperDestinationSet = true;
                    break;
                }
            }

            if (!isWarperDestinationSet)
            {
                ofRectangle bounds(0, 0, sW, sH);
                bounds.scaleTo(ofRectangle(0, 0, 320, 240));
                int x = (ofGetWidth() - bounds.getWidth()) * 0.5;
                int y = (ofGetHeight() - bounds.getHeight()) * 0.5;
                int w = bounds.getWidth();
                int h = bounds.getHeight();

                _warper.setTopLeftCornerPosition(ofPoint(x, y));
                _warper.setTopRightCornerPosition(ofPoint(x + w, y));
                _warper.setBottomLeftCornerPosition(ofPoint(x, y + h));
                _warper.setBottomRightCornerPosition(ofPoint(x + w, y + h));
            }

            _warper.enableMouseControls();
            //_warper.enableKeyboardShortcuts();
        }
    }

    if (_maskDirty)
    {
        _maskSurface.begin();
        ofClear(0, 0, 0, 0);
        ofPushStyle();
        ofSetColor(255);
        if (_mask && _mask->isAllocated())
        {
            _mask->draw(0, 0, _maskSurface.getWidth(), _maskSurface.getHeight());
        }
        else
        {
            ofRect(0, 0, _maskSurface.getWidth(), _maskSurface.getHeight());
        }
        ofPopStyle();
        _maskSurface.end();

        _maskDirty = false;
    }

    if (_parent._parent.getMode() == AbstractApp::EDIT)
    {
        _warper.show();
    }
    else
    {
        _warper.hide();
    }
}


void Layer::draw()
{
    ofPoint mouse(ofGetMouseX(), ofGetMouseY());

    if (ofGetMousePressed())
    {
        ofPoint layerMouse = screenToLayer(mouse);

        if (ofGetKeyPressed(OF_KEY_COMMAND))
        {
            _maskPath.clear();
            _maskSurface.begin();
            ofPushStyle();

            if (ofGetKeyPressed(OF_KEY_SHIFT))
            {
                ofSetColor(255);
            }
            else
            {
                ofSetColor(0);
            }

            _brushTex.draw(layerMouse.x-25,layerMouse.y-25,50,50);
            ofPopStyle();
            _maskSurface.end();
        }
    }


    _surface.begin();
    ofClear(0, 0, 0, 0);

    ofPushStyle();

    _maskShader.begin();
    _maskShader.setUniformTexture("maskTex", _maskSurface.getTextureReference(), 1);


    if (_video && _video->isLoaded())
    {
        _video->draw(0, 0);
    }

    _maskShader.end();

    ofPopStyle();

    _surface.end();

    // Warp.
    ofPushMatrix();
    ofMultMatrix(_warper.getMatrix());
    _surface.draw(0, 0);
    ofPopMatrix();

    if (_warper.isShowing())
    {
        ofPushStyle();

        std::shared_ptr<Layer> layer = _parent.getLayerAtPoint(mouse);
        
        // TODO: Only when layer is on top
        if (layer && layer->getId() == getId())
        {
            ofSetColor(_highlightColor);
        }
        else
        {
            ofSetColor(_color);
        }
        
        _warper.drawQuadOutline();
        
        ofPoint* corners = _warper.getTargetPoints();
        for (std::size_t i = 0; i < 4; ++i) {
            ofCircle(corners[i], 6);
        }
        
        const ofPoint* hoveredCorner = getHoveredCorner(mouse);
        if (hoveredCorner) {
            ofSetColor(_highlightColor);
            ofCircle(*hoveredCorner, 6);
        }

        ofPopStyle();
        
    }
}


bool Layer::hitTest(const ofPoint& point) const
{
    const ofPoint* p = getHoveredCorner(point);
    return (!p && ofPolyline(std::vector<ofPoint>(_warper.dstPoints,
                                           _warper.dstPoints + 4)).inside(point));
}

    
const ofPoint* Layer::getHoveredCorner(const ofPoint& mouse) const
{
    
    const ofPoint* p = 0;
    
    const ofPoint* points = _warper.dstPoints;
    
    if (points)
    {
        for (std::size_t i = 0; i < 4; ++i)
        {
            if (mouse.distance(points[i]) <= 5)
            {
                p = &points[i];
                break;
            }
        }
    }
    
    return p;
}
    
const ofPoint Layer::getCentroid() const
{
    
    const ofPoint* dstPoints = _warper.dstPoints;
    float maxX = max(max(dstPoints[0].x, dstPoints[1].x), max(dstPoints[2].x, dstPoints[3].x));
    float minX = min(min(dstPoints[0].x, dstPoints[1].x), min(dstPoints[2].x, dstPoints[3].x));
    float maxY = max(max(dstPoints[0].y, dstPoints[1].y), max(dstPoints[2].y, dstPoints[3].y));
    float minY = min(min(dstPoints[0].y, dstPoints[1].y), min(dstPoints[2].y, dstPoints[3].y));
    
    return ofPoint(minX + ((maxX - minX) * 0.5), minY + ((maxY - minY) * 0.5));
}


ofPoint Layer::screenToLayer(const ofPoint& point)
{
    return _warper.getMatrixInverse().preMult(point);
//
//    return _warper.getMatrix().postMult(point);
}


ofPoint Layer::layerToScreen(const ofPoint& point)
{
    return _warper.getMatrix().preMult(point);
}


bool Layer::loadVideo(const std::string& path)
{
    Poco::Path fullyQualifiedPath(_parent.getPath(), path);

    _video = std::shared_ptr<ofVideoPlayer>(new ofVideoPlayer());

    if (_video->loadMovie(fullyQualifiedPath.toString()))
    {
        _videoPath = path;
        _maskDirty = true;
        return true;
    }
    else
    {
        _videoPath = "";
        _video.reset();
        _maskDirty = true;
        return false;
    }
}


bool Layer::loadMask(const std::string& path)
{
    Poco::Path fullyQualifiedPath(_parent.getPath(), path);

    if (_video)
    {
        _maskPath = path;
        _mask = std::shared_ptr<ofTexture>(new ofTexture());
        _maskDirty = true;
        ofLoadImage(*_mask, fullyQualifiedPath.toString());
        return true;
    }
    else
    {
        _maskDirty = true;
        _maskPath = "";
        ofLogError("Layer::loadMask") << "Mask cannot be loaded before the video.";
        return false;
    }
}


bool Layer::saveMask()
{
    if (!_mask)
    {
        ofLogWarning("Layer::saveMask") << "There is no mask to save.";
        return false;
    }
    if (!_maskPath.empty())
    {
        ofLogWarning("Layer::saveMask") << "The mask has not been modified.";
        return false;
    }
    else
    {
        _maskPath = "assets/masks/" + ofGetTimestampString() + "-lastmask.png";
        ofPixels pixels;
        _maskSurface.getTextureReference().readToPixels(pixels);
        Poco::Path fullyQualifiedPath(_parent.getPath(), _maskPath);
        ofSaveImage(pixels, fullyQualifiedPath.toString());
        return true;
    }
}


void Layer::clearMask()
{
    _mask.reset();
    _maskPath.clear();
    _maskDirty = true;
}

void Layer::translate(const ofPoint& delta)
{
    for (size_t i = 0; i < 4; ++i)
    {
        _warper.getTargetPoints()[i] += delta;
    }
}

void Layer::rotate(int degrees)
{
    ofVec2f centroid = getCentroid();
    
    for (size_t i = 0; i < 4; ++i)
    {
        ofVec2f point = _warper.getTargetPoints()[i];
        point.rotate(degrees, centroid);
        _warper.getTargetPoints()[i] = point;
    }
}
    

void Layer::scale(int mult)
{
    
}


Json::Value Layer::toJSON(const Layer& object)
{
    std::vector<ofPoint> sourcePoints(object._warper.srcPoints,
                                      object._warper.srcPoints + 4);

    std::vector<ofPoint> destinationPoints(object._warper.dstPoints,
                                           object._warper.dstPoints + 4);

    Json::Value json;

    json["video"]["path"] = object._videoPath;
    json["mask"]["path"] = object._maskPath;
    json["quad"]["source"] = toJSON(sourcePoints);
    json["quad"]["destination"] = toJSON(destinationPoints);

    return json;
}


bool Layer::fromJSON(const Json::Value& json, Layer& object)
{
    if (json.isMember("video"))
    {
        const Json::Value& video = json["video"];

        if (video.isMember("path"))
        {
            std::string path = video["path"].asString();

            if (!object.loadVideo(path))
            {
                ofLogError("Layer::fromJSON") << "could not load video at " << path;
                return false;
            }
        }
    }
    else
    {
        ofLogWarning("Layer::fromJSON") << "No video specified.";
    }

    if (json.isMember("mask"))
    {
        const Json::Value& mask = json["mask"];

        if (mask.isMember("path"))
        {
            std::string path = mask["path"].asString();

            if (!object.loadMask(path))
            {
                ofLogError("Layer::fromJSON") << "could not load mask at " << path;
                return false;
            }
        }
    }
    else
    {
        ofLogWarning("Layer::fromJSON") << "No mask specified.";
    }

    if (json.isMember("quad"))
    {
        const Json::Value& quad = json["quad"];

        if (quad.isMember("source"))
        {
            const Json::Value& points = quad["source"];

            std::vector<ofPoint> _points;

            if (fromJSON(points, _points))
            {
                object._warper.setSourcePoints(_points);
            }
            else
            {
                object._warper.setSourceRect(ofRectangle(0, 0, 0, 0));
                ofLogWarning("Layer::fromJSON") << "Invalid quad source size: " << points.size();
            }
        }
        else
        {
            object._warper.setSourceRect(ofRectangle(0, 0, 0, 0));
            ofLogWarning("Layer::fromJSON") << "Now source quad info found.";
        }

        if (quad.isMember("destination"))
        {
            const Json::Value& points = quad["destination"];

            std::vector<ofPoint> _points;

            if (fromJSON(points, _points))
            {
                object._warper.setTargetPoints(_points);
            }
            else
            {
                object._warper.setTargetRect(ofRectangle(0, 0, 0, 0));
                ofLogWarning("Layer::fromJSON") << "Invalid quad destination size: " << points.size();
            }
        }
        else
        {
            object._warper.setTargetRect(ofRectangle(0, 0, 0, 0));
            ofLogWarning("Layer::fromJSON") << "No destination quad info found.";
        }

    }
    else
    {
        object._warper.setSourceRect(ofRectangle(0, 0, 0, 0));
        object._warper.setTargetRect(ofRectangle(0, 0, 0, 0));
        ofLogWarning("Layer::fromJSON") << "No quad specified.";
    }

    return true;
}
    
const Poco::UUID Layer::getId() const
{
    return _id;
}

Json::Value Layer::toJSON(const std::vector<ofPoint>& object)
{
    Json::Value json;

    std::vector<ofPoint>::const_iterator iter = object.begin();

    while (iter != object.end())
    {
        Json::Value point;
        point["x"] = (*iter).x;
        point["y"] = (*iter).y;
        json.append(point);
        ++iter;
    }

    return json;
}

bool Layer::fromJSON(const Json::Value& json, std::vector<ofPoint>& object)
{
    if (4 == json.size())
    {
        for (Json::ArrayIndex i = 0; i < json.size(); ++i)
        {
            float x = json[i].get("x", -1).asFloat();
            float y = json[i].get("y", -1).asFloat();
            object.push_back(ofPoint(x, y));
        }

        return true;
    }
    else
    {
        return false;
    }
}


} // namespace Kibio
