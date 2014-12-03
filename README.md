kibio
=====

### Use

1. Collect videos and mask and place them in the proejct folder.  You can start with the `HelloWorld` project in `/Users/YOUR_USERNAME/Documents/Kibio/`.
1. Add videos and any mask images to the project folder.
1. Open the app.
1. Drag a video from your project folder onto the app to create a layer.
1. Drag a mask onto the video to create a mask.
1. Position layer anchor points, edit the mask and then toggle into presentation mode.  
1. Go full-screen and enjoy! 

### Keyboard Shortcuts

Project
- ⌘O - Open Project
  - Open a project files with `.kibio` file extensions.
- ⌘S - Save
  - Save the current open project.
- ⌘⇧S - Save As ...
  - Save the current project with a new file name ... _(incomplete)_
- ⎋ - Quit App and Save Project

Application
- ⌘F - Fullscreen Toggle
- ⌘E - Edit / Presentation Mode Toggle

Editor
- With mouse over layer:
  - ⌫ - Delete Layer
  - ⌘⌫ - Delete Layer Mask
- Masking
  - ⌘Drag - Mask Pen Tool
  - ⌘⇧S - Mask Erase Tool

_Key_

- ⌘ the Command Key symbol
- ⌥ the Option Key symbol
- ⇧ the Shift Key symbol
- ⎋ the ESC Key symbol
- ⇪ the Capslock symbol
- ⏎ the Return symbol
- ⌫ the Delete / Backspace symbol

### Building

This app requires the following addons:
- ofxOpenCv (in the openFrameworks core)
- https://github.com/bakercp/ofxMediaType
- https://github.com/jefftimesten/ofxJSON
- https://github.com/julapy/ofxQuadWarp

Additionally, the app must be built from openFrameworks master branch.

- https://github.com/openframeworks/openFrameworks
