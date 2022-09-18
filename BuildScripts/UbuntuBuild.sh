cd ..
mv -v Ullmannite/ThirdPartyLibs/imgui/backends/imgui_impl_opengl3_loader.h Ullmannite/ThirdPartyLibs/imgui
mv -v Ullmannite/ThirdPartyLibs/imgui/backends/imgui_impl_opengl3.h Ullmannite/ThirdPartyLibs/imgui
mv -v Ullmannite/ThirdPartyLibs/imgui/backends/imgui_impl_opengl3.cpp Ullmannite/ThirdPartyLibs/imgui
mv -v Ullmannite/ThirdPartyLibs/imgui/backends/imgui_impl_glfw.h Ullmannite/ThirdPartyLibs/imgui
mv -v Ullmannite/ThirdPartyLibs/imgui/backends/imgui_impl_glfw.cpp Ullmannite/ThirdPartyLibs/imgui
chmod +x ThirdParty/bin/premake/LinuxPremake5
./ThirdParty/bin/premake/LinuxPremake5 gmake2
make