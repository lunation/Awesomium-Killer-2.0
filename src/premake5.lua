solution "FuckAwesomium"
	language "C++"
	location "project"

	--configuration "vs*" -- speed shit
	--gr- = no typeinfo
	--buildoptions({"/Qpar", "/Qfast_transcendentals", "/GL", "/Ot", "/Gm", "/MP", "/Gy", "/Gw"})
	--linkoptions { "/OPT:REF", "/OPT:ICF", "/LTCG"}

	--flags { "Optimize", "NoMinimalRebuild", "NoFramePointer", "EnableSSE2", "FloatFast", "NoBufferSecurityCheck"}

	configurations { "Debug", "Release" }

	
	--[[ dumb sdk shit
	local sdk_dir = "C:/sdk13/mp/src/" -- Change this to suit your setup

	local function sdk(s) return sdk_dir..s end

	includedirs { sdk"public",       sdk"public/tier0", sdk"public/tier1",
				  sdk"public/tier2", sdk"public/tier3", sdk"public/tier0",
				  sdk"game",         sdk"game/client",  sdk"game/shared",
				  sdk"tier1",        sdk"tier0",        sdk"common",
				  sdk "", }

	libdirs     { sdk"lib", sdk"lib/public" }
]]
	includedirs { "cef" }

	--libdirs { "cef/Debug" }

	-- NOTE: The location of the wrapper lib changes a lot between CEF versions!
	links { "libcef", "libcef_dll_wrapper" }

	targetdir "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/bin/"
	

	project "AwesomiumReplacement"
		targetname "Awesomium"
		kind "SharedLib"
		files { "src/**.h", "src/**.cpp" }
		
		configuration "Release"
			buildoptions { "/MT" }
			--defines { "NDEBUG", "_GENERIC" }
			--targetdir "../dieawesomium/bin"
			libdirs { "cef/Release", "cef/libcef_dll_wrapper/Release" }

		configuration "Debug"
			buildoptions { "/MT" }
			--defines { "DEBUG", "_GENERIC" }
			--flags { "Symbols", "EnableSSE2" }
			libdirs { "cef/Release", "cef/libcef_dll_wrapper/Release" }
	
	project "SubProcess"
		targetname "gmod_cef"
		kind "WindowedApp"
		flags { "WinMain" }
		files { "subprocess.cpp" }

		configuration "Release"
			buildoptions { "/MT" }
			--defines { "NDEBUG", "_GENERIC" }
			--targetdir "../dieawesomium/bin"
			libdirs { "cef/Release", "cef/libcef_dll_wrapper/Release" }

		configuration "Debug"
			buildoptions { "/MT" }
			--defines { "DEBUG", "_GENERIC" }
			--flags { "Symbols", "EnableSSE2" }
			libdirs { "cef/Release", "cef/libcef_dll_wrapper/Release" }
