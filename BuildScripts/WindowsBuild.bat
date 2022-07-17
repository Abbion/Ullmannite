call cd ..
move Ullmannite\ThirdPartyLibs\imgui\backends\imgui_impl_opengl3_loader.h Ullmannite\ThirdPartyLibs\imgui\imgui_impl_opengl3_loader.h
move Ullmannite\ThirdPartyLibs\imgui\backends\imgui_impl_opengl3.cpp Ullmannite\ThirdPartyLibs\imgui\imgui_impl_opengl3.cpp
move Ullmannite\ThirdPartyLibs\imgui\backends\imgui_impl_opengl3.h Ullmannite\ThirdPartyLibs\imgui\imgui_impl_opengl3.h
move Ullmannite\ThirdPartyLibs\imgui\backends\imgui_impl_glfw.cpp Ullmannite\ThirdPartyLibs\imgui\imgui_impl_glfw.cpp
move Ullmannite\ThirdPartyLibs\imgui\backends\imgui_impl_glfw.h Ullmannite\ThirdPartyLibs\imgui\imgui_impl_glfw.h
call cls
call ThirdParty\bin\premake\premake5.exe vs2022
PAUSE