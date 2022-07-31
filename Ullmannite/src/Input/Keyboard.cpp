#include "Ullpch.h"
#include "Keyboard.h"
#include "Logger/Logger.h"

using namespace Ull;

bool Keyboard::m_created = false;


Keyboard::Keyboard()
{
    if(!m_created)
    {
        InitKeyMap();
        m_created = true;
    }
    else
        ULOGD("Keyboard already created!!!");
}

bool Keyboard::IsKeyPressed(Key key)
{
    return m_keyMap[key];
}

void Keyboard::UpdateKeyMap(std::map<Key, bool> updatedKeyMap)
{
    for(auto& key : m_keyMap)
        key.second = updatedKeyMap[key.first];
}

void Keyboard::InitKeyMap()
{
    m_keyMap[Key::SPACE] = false;
    m_keyMap[Key::ZERO] = false;
    m_keyMap[Key::ONE] = false;
    m_keyMap[Key::TWO] = false;
    m_keyMap[Key::THREE] = false;
    m_keyMap[Key::FOUR] = false;
    m_keyMap[Key::FIVE] = false;
    m_keyMap[Key::SIX] = false;
    m_keyMap[Key::SEVEN] = false;
    m_keyMap[Key::EIGHT] = false;
    m_keyMap[Key::NINE] = false;
    m_keyMap[Key::A] = false;
    m_keyMap[Key::B] = false;
    m_keyMap[Key::C] = false;
    m_keyMap[Key::D] = false;
    m_keyMap[Key::E] = false;
    m_keyMap[Key::F] = false;
    m_keyMap[Key::G] = false;
    m_keyMap[Key::H] = false;
    m_keyMap[Key::I] = false;
    m_keyMap[Key::J] = false;
    m_keyMap[Key::K] = false;
    m_keyMap[Key::L] = false;
    m_keyMap[Key::M] = false;
    m_keyMap[Key::N] = false;
    m_keyMap[Key::O] = false;
    m_keyMap[Key::P] = false;
    m_keyMap[Key::Q] = false;
    m_keyMap[Key::R] = false;
    m_keyMap[Key::S] = false;
    m_keyMap[Key::T] = false;
    m_keyMap[Key::U] = false;
    m_keyMap[Key::V] = false;
    m_keyMap[Key::W] = false;
    m_keyMap[Key::Y] = false;
    m_keyMap[Key::Z] = false;
    m_keyMap[Key::ESCAPE] = false;
    m_keyMap[Key::ENTER] = false;
    m_keyMap[Key::TAB] = false;
    m_keyMap[Key::BACKSPACE] = false;
    m_keyMap[Key::LEFT] = false;
    m_keyMap[Key::RIGHT] = false;
    m_keyMap[Key::UP] = false;
    m_keyMap[Key::DOWN] = false;
    m_keyMap[Key::F1] = false;
    m_keyMap[Key::F2] = false;
    m_keyMap[Key::F3] = false;
    m_keyMap[Key::F4] = false;
    m_keyMap[Key::F5] = false;
    m_keyMap[Key::F6] = false;
    m_keyMap[Key::F7] = false;
    m_keyMap[Key::F8] = false;
    m_keyMap[Key::F9] = false;
    m_keyMap[Key::F10] = false;
    m_keyMap[Key::F11] = false;
    m_keyMap[Key::F12] = false;
    m_keyMap[Key::L_SHFT] = false;
    m_keyMap[Key::R_SHIFT] = false;
    m_keyMap[Key::L_CONTROL] = false;
    m_keyMap[Key::R_CONTROL] = false;
    m_keyMap[Key::L_ALT] = false;
    m_keyMap[Key::R_ALT] = false;
}