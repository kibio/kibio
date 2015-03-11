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


#pragma once
#include "ofMain.h"

namespace Kibio {
    
/// \brief Enum for all UserInterface button types
enum UIButtonType {
    BUTTON_OPEN_PROJECT,
    BUTTON_NEW_PROJECT,
    BUTTON_SAVE_PROJECT,
    BUTTON_INFO,
    BUTTON_TOGGLE_MODE,
    BUTTON_TOOL_BRUSH,
    BUTTON_TOOL_TRANSLATE,
    BUTTON_TOOL_ROTATE,
    BUTTON_TOOL_SCALE
};
    
/// \brief A user interface event class
class UserInterfaceEvent : public ofEventArgs
{
public:
    UserInterfaceEvent(UIButtonType _type):
    type(_type){};
    
    UIButtonType type;
};
    
/// \brief A an image button
class ImageButton
{
public:
    ImageButton(const std::string& imagePath,
                UIButtonType _type,
                bool sticky,
                ofEvent<const UserInterfaceEvent>& buttonSelectEvent,
                ofEvent<const UserInterfaceEvent>& buttonDeselectEvent,
                ofColor color,
                ofColor highlightColor,
                ofColor shadowColor);
    
    ~ImageButton();
    
    void set(int x, int y, int width, int height);
    void update(const ofPoint& mouse);
    void draw(const ofPoint& shadowOffset=ofPoint::zero());
    void mouseReleased(ofMouseEventArgs& args);
    void enable();
    void disable();
    void select(bool simulated=false);
    void deselect();
    void setSelected(bool b);
    
    bool isEnabled() const;
    bool isSelected() const;
    bool isHovered() const;
    
    UIButtonType type;
    
protected:
    
    bool _bSelected;
    bool _bHovered;
    bool _bSticky;
    bool _bClickSimulated;
    bool _bEnabled;
    
    ofColor _color;
    ofColor _highlightColor;
    ofColor _shadowColor;
    ofTexture _texture;
    ofRectangle _rect;
    
    ofEvent<const UserInterfaceEvent>& _buttonSelectEvent;
    ofEvent<const UserInterfaceEvent>& _buttonDeselectEvent;
};

/// \brief A class for managing the user interface
class UserInterface
{
public:
    
    UserInterface();
    ~UserInterface();
    
    void update();
    void draw();
    
    void placeIcons();
    void toggleVisible();
    void hide();
    void show();
    void enable();
    void disable();
    
    void setProjectName(const std::string& name);
    void setDrawIconShadows(bool drawIconShadows);
    void setUIButtonSelectState(const UIButtonType& type, bool state);
    void toggleUIButtonState(const UIButtonType& type);
    void simulateClick(const UIButtonType& type);

    void onButtonSelect(const UserInterfaceEvent& args);
    void onButtonDeselect(const UserInterfaceEvent& args);
    
    bool isVisible() const;
    bool isEnabled() const;
    bool getUIButtonSelectState(const UIButtonType& type);
    
    std::vector<ImageButton*> getSelectedButtons();
    
    ofEvent<const UserInterfaceEvent> buttonSelectEvent;
    ofEvent<const UserInterfaceEvent> buttonDeselectEvent;
    
protected:

    bool _bDrawIconShadows;
    bool _bVisible;
    
    int _iconPadding;
    int _iconSize;
    int _fontSize;
    
    std::string _projectName;
    
    ofVec2f _shadowOffset;
    
    ofColor _color;
    ofColor _highlightColor;
    ofColor _shadowColor;
    
    ofTrueTypeFont _font;

    ImageButton _openProjectButton;
    ImageButton _newProjectButton;
    ImageButton _saveProjectButton;
    ImageButton _infoButton;
    ImageButton _toggleModeButton;
    ImageButton _toolBrushButton;
    ImageButton _toolTranslateButton;
    ImageButton _toolRotateButton;
    ImageButton _toolScaleButton;
    
    ImageButton& _getButton(UIButtonType type);
    
};
    
} // namespace Kibio
