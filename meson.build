
project('graphics', 'c', 'cpp', default_options: ['cpp_std=c++11'])

target = 'graphics'
glfw = dependency('glfw3')
hdrs = include_directories('extern/glad/include', 'extern/linalg', 'extern/imgui', 'extern/imgui/backends')
srcs = ['src/main.cpp']
srcs += ['./extern/glad/src/glad.c']
srcs += ['./extern/linalg/linalg.cpp']
srcs += ['./extern/imgui/imgui_widgets.cpp', './extern/imgui/backends/imgui_impl_opengl3.cpp', './extern/imgui/backends/imgui_impl_glfw.cpp', './extern/imgui/imgui.cpp', './extern/imgui/imgui_tables.cpp', './extern/imgui/imgui_demo.cpp', './extern/imgui/imgui_draw.cpp']

executable(target, srcs, include_directories: hdrs, dependencies: [glfw])

