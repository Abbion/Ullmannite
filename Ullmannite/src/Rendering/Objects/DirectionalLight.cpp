#include "Ullpch.h"
#include "DirectionalLight.h"

using namespace Ull;

DirectionalLight::DirectionalLight(const std::string& name, NotOwner<Scene> scene) :
    Node(name, scene)
{

}