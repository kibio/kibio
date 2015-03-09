// =============================================================================
//
// Copyright (c) 2015 Brannon Dorsey <http://brannondorsey.com>
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


#include "UserInterface.h"

namespace Kibio {
    
// ImageButton class
// =============================================================================

ImageButton::ImageButton(const std::string& imagePath,
                         UIButtonType _type,
                         ofColor color,
                         ofColor highlightColor,
                         ofColor shadowColor):
type(_type),
_color(color),
_highlightColor(highlightColor),
_shadowColor(shadowColor),
_bSelected(false),
_bHovered(false)
{
    ofLoadImage(_texture, imagePath);
}
    
ImageButton::~ImageButton()
{

}
    
void ImageButton::set(int x, int y, int width, int height)
{
    _rect.set(x, y, width, height);
}
    
void ImageButton::update(const ofPoint& mouse)
{
    if (_rect.inside(mouse))
    {
        cout << "hovered" << endl;
        _bHovered = true;
    }
    else
    {
        _bHovered = false;
    }
}

void ImageButton::draw(const ofPoint& shadowOffset)
{
    ofPushStyle();
    
    // draw shadow
    if (shadowOffset != ofPoint::zero())
    {
        ofSetColor(_shadowColor);
        _texture.draw(_rect.x + shadowOffset.x,
                      _rect.y + shadowOffset.y,
                      _rect.width,
                      _rect.height);
    }
    
    if (isHovered() || isSelected())
    {
        ofSetColor(_highlightColor);
    }
    else
    {
        ofSetColor(_color);
    }
    
    _texture.draw(_rect.x, _rect.y, _rect.width, _rect.height);
    
    ofPopStyle();
}
        
void ImageButton::select()
{
    _bSelected = true;
}
  
void ImageButton::unselect()
{
    _bSelected = false;
}
    
void ImageButton::setSelected(bool b)
{
    _bSelected = b;
}
    
bool ImageButton::isSelected()
{
    return _bSelected;
}

bool ImageButton::isHovered()
{
    return _bHovered;
}
    
// UserInterface class
// =============================================================================
    
UserInterface::UserInterface():
_color(ofColor(255, 255, 255)),
_highlightColor(ofColor(255, 255, 0)),
_shadowColor(ofColor(30, 120, 165)),
_bDrawIconShadows(true),
_iconPadding(10),
_iconSize(30),
_shadowOffset(ofVec2f(2, 2)),
_openProjectButton(ImageButton("images/archive.png", BUTTON_OPEN_PROJECT, _color, _highlightColor, _shadowColor)),
_newProjectButton(ImageButton("images/plus.png", BUTTON_NEW_PROJECT, _color, _highlightColor, _shadowColor)),
_saveProjectButton(ImageButton("images/save.png", BUTTON_SAVE_PROJECT, _color, _highlightColor, _shadowColor)),
_infoButton(ImageButton("images/info.png", BUTTON_INFO, _color, _highlightColor, _shadowColor)),
_toggleModeButton(ImageButton("images/light-down.png", BUTTON_TOGGLE_MODE, _color, _highlightColor, _shadowColor)),
_toolBrushButton(ImageButton("images/brush.png", BUTTON_TOOL_BRUSH, _color, _highlightColor, _shadowColor)),
_toolTranslateButton(ImageButton("images/hand.png", BUTTON_TOOL_TRANSLATE, _color, _highlightColor, _shadowColor)),
_toolRotateButton(ImageButton("images/cycle.png", BUTTON_TOOL_ROTATE, _color, _highlightColor, _shadowColor)),
_toolScaleButton(ImageButton("images/resize-full-screen.png", BUTTON_TOOL_SCALE, _color, _highlightColor, _shadowColor))
{
    placeIcons();
    show();
}
    
UserInterface::~UserInterface()
{
    
}
    
void UserInterface::update()
{
    ofPoint mouse(ofGetMouseX(), ofGetMouseY());
    
    _openProjectButton.update(mouse);
    _newProjectButton.update(mouse);
    _saveProjectButton.update(mouse);
    _infoButton.update(mouse);
    _toggleModeButton.update(mouse);
    _toolBrushButton.update(mouse);
    _toolTranslateButton.update(mouse);
    _toolRotateButton.update(mouse);
    _toolScaleButton.update(mouse);
}
    
void UserInterface::draw()
{

    ofPushStyle();
    ofSetColor(_color);
    
    _openProjectButton.draw(_shadowOffset);
    _newProjectButton.draw(_shadowOffset);
    _saveProjectButton.draw(_shadowOffset);
    _infoButton.draw(_shadowOffset);
    _toggleModeButton.draw(_shadowOffset);
    _toolBrushButton.draw(_shadowOffset);
    _toolTranslateButton.draw(_shadowOffset);
    _toolRotateButton.draw(_shadowOffset);
    _toolScaleButton.draw(_shadowOffset);
    
    ofPopStyle();
}

void UserInterface::placeIcons()
{
    
    int w = ofGetWidth();
    int h = ofGetHeight();
    
    // top right
    int x = w - _iconPadding - _iconSize;
    int y = _iconPadding;
    
    _toolScaleButton.set(x, y, _iconSize, _iconSize);
    x -= _iconSize + _iconPadding;
    
    _toolRotateButton.set(x, y, _iconSize, _iconSize);
    x -= _iconSize + _iconPadding;
    
    _toolTranslateButton.set(x, y, _iconSize, _iconSize);
    x -= _iconSize + _iconPadding * 2; // place brush further to left
    
    _toolBrushButton.set(x, y, _iconSize, _iconSize);
    
    _openProjectButton;
    _newProjectButton;
    _saveProjectButton;
    _infoButton;
    _toggleModeButton;
    
}

void UserInterface::toggleVisible()
{
    _bVisible = !_bVisible;
}

void UserInterface::hide()
{
    _bVisible = false;
}

void UserInterface::show()
{
    _bVisible = true;
}

void UserInterface::setProjectName(const std::string& name)
{
    _projectName = name;
}

bool UserInterface::isVisible() const
{
    return _bVisible;
}

} // namespace Kibio
