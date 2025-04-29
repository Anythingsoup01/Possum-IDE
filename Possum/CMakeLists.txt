set(SRCS
    src/Core/FileManager.cpp
    src/PossumLayer.cpp
    src/PossumApp.cpp
    src/Core/Buffer.h
    src/Core/FileManager.h
    src/PossumLayer.h
)
add_executable(Possum ${SRCS})
set_property(TARGET Possum PROPERTY CXX_STANDARD 20)

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

