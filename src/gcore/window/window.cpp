//
// => gcore/window/window.cpp
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

#include <gcore/window/window.h>

#ifdef _WIN32
#include <thread>
#include <chrono>

#define usleep(x) std::this_thread::sleep_for(std::chrono::microseconds((long long)(x)))
#else
#include <unistd.h>
#endif

#define TARGET_FPS 60
#define FPS_TIME 1.0f/TARGET_FPS

using namespace gcore;

static GLFWmonitor *getMonitor(Window::monitor_index monitorIndex) {
    if (monitorIndex == GCORE_WINDOW_NO_FULLSCREEN) {
        return nullptr;
    }
    
    if (monitorIndex == GCORE_WINDOW_MONITOR_DEFAULT) {
        return glfwGetPrimaryMonitor();
    }
    
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);
    if (monitorIndex < monitorCount) {
        return monitors[monitorIndex];
    }
    return nullptr;
}

Window::Window(const char *title, window_size width, window_size height, monitor_index monitorIndex) : title(title), width(width), height(height), monitorIndex(monitorIndex) {
    
    if ((_monitor = getMonitor(monitorIndex)) != nullptr) {
        int w, h;
        glfwGetMonitorPhysicalSize(_monitor, &w, &h);
        
        width = w;
        height = h;
    }

}

void Window::setSize(window_size w, window_size h) {
    width = w;
    height = h;
    glfwSetWindowSize(_window, w, h);
    getDrawer().doResize();
}

void Window::setTitle(const char *title) {
    this->title = title;
    glfwSetWindowTitle(_window, title);
}

bool Window::make() {

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    _window = glfwCreateWindow(width, height, title, _monitor, nullptr);
    
    if (!_window) {
        return false;
    }
    
    glfwSetInputMode(_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(_window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);

    return true;
}


bool Window::goFullScreen(monitor_index monitorIndex) {
    assert(monitorIndex != GCORE_WINDOW_NO_FULLSCREEN);
    
    if (GLFWmonitor *newMonitor = getMonitor(monitorIndex)) {
        int w, h;
        glfwGetMonitorPhysicalSize(newMonitor, &w, &h);
        
        width = w;
        height = h;
        
        _monitor = newMonitor;
        glfwSetWindowMonitor(_window, _monitor, 0, 0, width, height, GLFW_DONT_CARE);
        return true;
    }
    return false;
}

bool Window::goWindow(window_size w, window_size h) {
    width = w;
    height = h;
    
    _monitor = nullptr;
    glfwSetWindowMonitor(_window, nullptr, 0, 0, w, h, GLFW_DONT_CARE);
    return true;
}


bool Window::startLoop() {
    
    WindowDrawer &drawer = getDrawer();
    GLFWwindow *glfwWin = _window;
    
    drawer.doInit();
    drawer.doResize();
    
    double lastTime = 0;
    glfwSetTime(0);
    
    double startFrameTime, endFrameTime, frameTime;
    
    do {
        startFrameTime = glfwGetTime();
        
        {
            double dt = startFrameTime - lastTime;
            lastTime = startFrameTime;
            
            drawer.doUpdate(dt);
        }
        
        drawer.doRender();
        
        glfwSwapBuffers(glfwWin);
        glfwPollEvents();
        
        endFrameTime = glfwGetTime();
        
        frameTime = endFrameTime - startFrameTime;
        if (frameTime < FPS_TIME) {
            usleep((FPS_TIME - frameTime) * 10e5);
        }
        
    } while (!shouldClose());
    
    drawer.doDestroy();
    return true;
}
