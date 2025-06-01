set(SRCS
    src/Core/FileManager.cpp
    src/Core/ProjectManager.cpp
    src/Core/ImGuiRenderer.cpp
    src/Core/ImGuiCommands.cpp
    src/Possum/PossumLayer.cpp
    src/Possum/PossumApp.cpp
    src/Core/Buffer.h
    src/Core/ImGuiRenderer.h
    src/Core/ProjectManager.h
    src/Core/ImGuiCommands.h
    src/Core/FileManager.h
    src/Possum/PossumLayer.h
    src/psmpch.h
)
add_executable(Possum ${SRCS})
set_property(TARGET Possum PROPERTY CXX_STANDARD 20)

target_precompile_headers(Possum PUBLIC src/psmpch.h)
target_include_directories(Possum PRIVATE
    src
    ${CMAKE_SOURCE_DIR}/Ferret/Ferret/src
    ${CMAKE_SOURCE_DIR}/Ferret/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/Ferret/vendor/glad/include
    ${CMAKE_SOURCE_DIR}/Ferret/vendor/spdlog/include
    ${CMAKE_SOURCE_DIR}/Ferret/vendor/glm
    ${CMAKE_SOURCE_DIR}/Ferret/vendor/imgui
    ${CMAKE_SOURCE_DIR}/vendor/ImGuiFileDialog
)

target_link_libraries(Possum
    Ferret
    ImGuiFileDialog
)

