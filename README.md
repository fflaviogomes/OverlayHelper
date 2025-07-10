# OverlayHelper

A lightweight, semi-transparent image overlay tool for Windows.

## Description

OverlayHelper is a simple Windows utility designed for artists, designers, and anyone who needs to reference or trace an image on their screen. It creates a borderless, always-on-top window that can display a custom image with adjustable opacity. This allows you to see the image while working in another application underneath it.

The window is "click-through," meaning any clicks you make on the overlay will pass through to the window behind it, allowing you to draw or work without interruption.

## Features

*   **Image Overlay:** Display a PNG, BMP, or JPG image in a borderless window.
*   **Adjustable Opacity:** Make the image more or less transparent to suit your needs.
*   **Draggable:** Easily move the overlay anywhere on the screen by clicking and dragging.
*   **Always on Top:** The overlay window stays on top of all other applications.
*   **Click-Through:** The overlay does not interfere with mouse clicks, allowing you to interact with the application underneath.

## Hotkeys

*   **`O`**: Open a file dialog to select a new image.
*   **`Up Arrow`**: Increase the image opacity.
*   **`Down Arrow`**: Decrease the image opacity.
*   **`Mouse Drag`**: Click and drag to move the window.
*   **`ESC`**: Close the application.

## How to Use

1.  Run the `OverlayHelper.exe` application.
2.  Press the **`O`** key to open the file dialog and select your desired image.
3.  Use the **Up** and **Down Arrow keys** to adjust the transparency.
4.  Click and drag the image to position it on your screen.
5.  When you are finished, press the **`ESC`** key to exit.

## Building from Source

This project is a standard C++ application written for the Windows API.

### Dependencies
*   Windows API
*   GDI+

### Compilation
You can compile the project using a C++ compiler (like MinGW-w64) and a build system or IDE that can handle Windows projects, such as Code::Blocks. The project requires linking against the `Gdiplus.lib` and `Comdlg32.lib` libraries.
