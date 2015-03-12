Kibio
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

#### Project
- ⌘N - Open Project Folder in Finder
- ⌘K - Open Project Folder in Finder
- ⌘O - Open Project
  - Open a project files with `.kibio` file extensions.
- ⌘S - Save
  - Save the current open project.
- ⌘⇧S - Save As ...
  - Save the current project with a new file name


#### Application
- ⌘F - Fullscreen Toggle
- ⌘E - Edit / Presentation Mode Toggle
- ⎋ - Quit App and Save Project

#### Editor

- Tools

  - T - Transform tool
  - R - Rotate tool
  - S - Scale tool
  - B - Mask brush tool
  
- With mouse over layer:
  - ⌫ - Delete Layer
  - ⌘⌫ - Delete Layer Mask
- Masking
  - ⌘Drag - Mask Pen Tool
  - ⌘⇧S - Mask Erase Tool

####_Key_

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

Additionally, this app must be built from the openFrameworks master branch. Or, to fix a current mouse disappearing bug with the openFrameworks master GLFW library we use this [`develop-apothecary`](https://github.com/bakercp/openframeworks/tree/develop-apothecary) branch in development.

- https://github.com/openframeworks/openFrameworks

### License and Attribution

Kibio is developed by members of the SAIC [openLab](http://olab.io) and is released under an MIT License.

The glyphs used in the Kibio user interface were created by Daniel Bruce and released under a CC BY-SA 4.0 License.

