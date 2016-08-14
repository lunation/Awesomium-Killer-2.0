solution "Awesomium-Killer-2.0"
	language "C++"
	location "project"

	configurations { "1-Test", "2-Release" }

	includedirs { "cef" }

	libdirs { "cef/Release", "cef/libcef_dll_wrapper/Release" }
	links { "libcef", "libcef_dll_wrapper" }

	buildoptions { "/MT" }

	configuration "1-Test"
		defines { "TESTING" }
		flags { "Symbols" }
		targetdir "C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/bin/"

	configuration "2-Release"
		buildoptions { "/O2" }
		targetdir "release"


	project "Awesomium-Replacement"
		targetname "Awesomium"
		kind "SharedLib"
		files { "src/**.h", "src/**.cpp" }
		
		configuration "1-Test"
			postbuildcommands {
				"xcopy /ys ..\\cef\\Release \"C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/bin\"",
				"xcopy /ys ..\\cef\\Resources \"C:/Program Files (x86)/Steam/steamapps/common/GarrysMod/bin\""
			}

		configuration "2-Release"
			postbuildcommands {
				"copy ..\\LICENSE ..\\release\\license.txt",
				"copy ..\\cef\\LICENSE.txt ..\\release\\license-cef.txt",

				"xcopy /ys ..\\cef\\Release ..\\release",
				"xcopy /ys ..\\cef\\Resources ..\\release",

				"del ..\\release\\*.exp ..\\release\\*.ilk ..\\release\\*.lib"
			}
	
	project "SubProcess"
		targetname "gmod_cef"
		kind "WindowedApp"
		flags { "WinMain" }
		files { "subprocess.cpp" }
