//
// => gcore/window/window.h
//
//                                 GraphCore
//
// Copyright (c) 2018 Lorenzo Laneve
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef __ogl_graphcore_window
#define __ogl_graphcore_window

#include <GL/glew.h> // glew must be included before any OpenGL header
#include <GLFW/glfw3.h>

#include <assert.h>

#define GCORE_WINDOW_NO_FULLSCREEN -2
#define GCORE_WINDOW_MONITOR_DEFAULT -1

namespace gcore {
    
    class Window;
    
    class WindowDrawer {
        
        Window &targetWindow;
        
    public:
        WindowDrawer(Window &targetWindow) : targetWindow(targetWindow) {  }
        
        virtual ~WindowDrawer() {  }
        
        inline Window &getTargetWindow() const { return targetWindow; }
        
        virtual void doInit() = 0;
        
        virtual void doResize() = 0;
        
        virtual void doUpdate(double dt) = 0;
        
        virtual void doRender() = 0;
        
        virtual void doDestroy() = 0;
        
    };
    
    /*!
     \brief Abstraction class for the GLFW window.
     */
    class Window {
    public:
        /*!
         \brief Type used for the width and height of the window in the current OS window system coordinates.
         */
        typedef unsigned int window_size;
        /*!
         \brief Type used for the GLFW monitor index.
         */
        typedef int monitor_index;
        /*!
         \brief Type used to store information about keyboard keys.
         */
        typedef int key_id;
        /*!
         \brief Type used to store information about mouse buttons.
         */
        typedef int mouse_button_id;
        
    private:
        
        /*!
         \brief The GLFW window created and handled by the class.
         */
        GLFWwindow *_window = nullptr;
        
        /*!
         \brief A C string containing the window title.
         */
        const char *title;
        
        /*!
         \brief The window current width, in the current OS window system coordinates.
         */
        window_size width;
        /*!
         \brief The window current height, in the current OS window system coordinates.
         */
        window_size height;
        
        /*!
         \brief The GLFW monitor describing the monitor currently in use, or \c nullptr if the window is not fullscreen.
         */
        GLFWmonitor *_monitor = nullptr;
        /*!
         \brief The index of the monitor.
         */
        monitor_index monitorIndex;
        
        /*!
         \brief The window drawer object taking the graphics loop callbacks.
         */
        WindowDrawer *drawer = nullptr;
        
    public:
        /*!
         \brief Initializes the window object with a title, a size, and a monitor index that defaults to \c GCORE_WINDOW_NO_FULLSCREEN.
         \param title The title the newly created window will have in the current window system
         \param width The window's width, in the current window system coordinates.
         \param height The window's height, in the current window system coordinates.
         \param monitorIndex The index of the monitor this window will take over. \c GCORE_WINDOW_MONITOR_DEFAULT will take the system's main monitor, \c GCORE_WINDOW_MONITOR_NO_FULLSCREEN will pass the window to the window system.
         \note If the window is set to be full screen, width and height will be ignored.
         */
        Window(const char *title, window_size width, window_size height, monitor_index monitorIndex = GCORE_WINDOW_NO_FULLSCREEN);
        
        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        
        
        ~Window() {
            if (_window) {
                glfwDestroyWindow(_window);
            }
        }
        
        /*!
         \brief Returns a reference to the current drawer for this window.
         \note The method will assert if no drawer is set.
         */
        inline WindowDrawer &getDrawer() const {
            assert(drawer && "No drawer set.");
            return *drawer;
        }
        /*!
         \brief Sets the given drawer as the current drawer for this window.
         \note The method will assert if the given drawer is not built with this window as the target window.
         */
        inline void setDrawer(WindowDrawer *newDrawer) {
            assert(&newDrawer->getTargetWindow() == this && "The drawer's target window must be the window taking it.");
            drawer = newDrawer;
        }
        
        
        /*!
         \brief Returns whether the window is full screen.
         */
        inline bool isFullScreen() const {
            return monitorIndex != GCORE_WINDOW_NO_FULLSCREEN;
        }
        
        /*!
         \brief Returns the window width.
         */
        inline window_size getWidth() const { return width; }
        /*!
         \brief Returns the window height.
         */
        inline window_size getHeight() const { return height; }
        /*!
         \brief Sets the window size with the given width and height.
         */
        void setSize(window_size w, window_size h);
        /*!
         \brief Sets the window title with the given C string.
         */
        void setTitle(const char *title);
        
        /*!
         \brief Returns the window aspect ratio.
         */
        inline float getAspectRatio() const { return (float)width/height; }
        
        /*!
         \brief Returns whether this window should be closed at the end of this drawing cycle.
         */
        inline bool shouldClose() {
            return glfwWindowShouldClose(_window);
        }
        /*!
         \brief Gives the window render context to the calling thread.
         */
        inline void takeWindowContext() {
            glfwMakeContextCurrent(_window);
        }
        /*!
         \brief Returns whether the key of the given ID is pressed.
         */
        inline bool isKeyPressed(key_id key) {
            return glfwGetKey(_window, key) == GLFW_PRESS;
        }
        /*!
         \brief Returns whether the mouse button of the given ID is pressed.
         */
        inline bool isMouseButtonPressed(mouse_button_id button) {
            return glfwGetMouseButton(_window, button) == GLFW_PRESS;
        }

        /*!
         \brief Creates the window with the given information.
         \return \c true if the window has been created successfully, \c false otherwise.
         */
        bool make();
        
        /*!
         \brief Makes the window pass to fullscreen mode, or to another monitor provided by the system.
         \return \c true if the window has successfully gone full screen on the given monitor, \c false otherwise.
         */
        bool goFullScreen(monitor_index monitorIndex = GCORE_WINDOW_MONITOR_DEFAULT);
        /*!
         \brief Makes the window pass to window mode, with the given width and height.
         \return \c true if the window has successfully gone in window mode, \c false otherwise.
         */
        bool goWindow(window_size w, window_size h);
        
        /*!
         \brief Starts the drawing loop. This method locks the calling thread until the window needs to be closed.
         \return \c true if the loop finished successfully, \c false if the drawing loop needed to be finished for an error.
         */
        bool startLoop();

    };
    
    
    
}

#endif
