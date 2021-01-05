workspace("BasicBounceEngine")
configurations {"Debug", "Release"}
platforms {"x64"}

local project_action = "UNDEFINED"
if _ACTION ~= nill then
	project_action = _ACTION
end

location ("BasicBounceEngine")

filter "configurations:Debug"
symbols "On"

filter "configurations:Release"
optimize "On"

filter{}

project "BasicBounceEngine"
  kind "ConsoleApp"
  language "C++"
  
  targetdir "%{cfg.buildcfg}_%{cfg.platform}"
  targetname "BasicBounceEngine"

  files {"./Source/**.cpp", "./Source/**.h", "./Source/**.inl", "./Dependencies/**.cpp"}

  vpaths 
  {
    ["Header Files/*"] = { "./Source/**.h"},
    ["Source Files/*"] = {"./Source/**.cpp"},
    ["Registration Files/*"] = {"./Source/**.inl"},
    ["External CPP Files/*"] = {"./Dependencies/**.cpp"},
  }

  includedirs 
  {
    "./Dependencies"
  }

  libdirs
  {
    "./Dependencies/*"
  }

  links 
  {
    "glew32.lib", "glu32.lib", "opengl32.lib", "glfw3.lib"
  }
  
  configuration "Debug"
    links
    {
      
    }
    
  configuration "Release"
  links
  {
    
  }
	
	
	