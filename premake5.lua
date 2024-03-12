workspace "Avataar_Assignment"

architecture "x86_64"

startproject "Renderer3D"

configurations
{
	"Debug",
	"Release",
	"Dist"
}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Renderer3D/thirdparty/GLFW/include"
IncludeDir["Glad"] = "Renderer3D/thirdparty/Glad/include"
IncludeDir["glm"] = "Renderer3D/thirdparty/glm"

include "Renderer3D/thirdparty/GLFW"
include "Renderer3D/thirdparty/Glad"

project "Renderer3D"

	location "Renderer3D"
	kind "ConsoleApp"
	language "c++"
	staticruntime "off"
	cppdialect "c++17"

	targetdir ("bin/"..outputdir.."/%{prj.name}")
	objdir ("bin-int/"..outputdir.."/%{prj.name}")


	files
	{
		"%{prj.name}/Renderer/**.h",
		"%{prj.name}/Renderer/**.cpp",		
	}

	includedirs
	{
		"%{prj.name}/Renderer",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.glm}",		
	}

	links{
		"GLFW",
		"Glad",		
		"opengl32.lib",
	}

	filter "system:windows"
		
		systemversion "latest"

		defines
		{		
			"GLFW_INCLUDE_NONE",
		}


	filter "configurations:Debug"
		staticruntime "off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		staticruntime "off"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		staticruntime "off"
		runtime "Release"
		optimize "On"
