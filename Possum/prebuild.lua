project "Possum"
    language "C++"
    dialect "20"
    kind "ConsoleApp"

    pch "src/psmpch.h"

    files
    {
        "src/*.cpp",
        "src/*.h",
    }

    includedirs 
    {
        "src",
        "$(WORKSPACEDIR)/Ferret/Ferret/src",
        "$(WORKSPACEDIR)/Ferret/vendor/glfw/include",
        "$(WORKSPACEDIR)/Ferret/vendor/glad/include",
        "$(WORKSPACEDIR)/Ferret/vendor/spdlog/include",
        "$(WORKSPACEDIR)/Ferret/vendor/glm",
        "$(WORKSPACEDIR)/Ferret/vendor/imgui",
        "$(WORKSPACEDIR)/vendor/ImGuiFileDialog",
    }

    links
    {
        "Ferret",
        "ImGuiFileDialog",
    }
