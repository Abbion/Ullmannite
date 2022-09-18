#include "Ullpch.h"
#include "Keyboard.h"

using namespace Ull;

Keyboard Keyboard::m_keyboardInstance;

Keyboard& Keyboard::GetInstance()
{
    return m_keyboardInstance;
}

Keyboard::Keyboard()
{
    InitKeyMap();
}

Keyboard::~Keyboard()
{
    m_keyMap.clear();
}

bool Keyboard::IsKeyPressed(Key key) const
{
    return m_keyMap.at(key);
}

void Keyboard::UpdateKeyMap(const std::map<Key, bool>& updatedKeyMap)
{
    for (const auto& key : updatedKeyMap)
        m_keyMap[key.first] = key.second;
}

void Keyboard::InitKeyMap()
{
    m_keyMap.insert({ 
        {Key::SPACE, false},
        {Key::ZERO, false},
        {Key::ONE, false},
        {Key::TWO, false},
        {Key::THREE, false},
        {Key::FOUR, false},
        {Key::FIVE, false},
        {Key::SIX, false},
        {Key::SEVEN, false},
        {Key::EIGHT, false},
        {Key::NINE, false},
        {Key::A, false},
        {Key::B, false},
        {Key::C, false},
        {Key::D, false},
        {Key::E, false},
        {Key::F, false},
        {Key::G, false},
        {Key::H, false},
        {Key::I, false},
        {Key::J, false},
        {Key::K, false},
        {Key::L, false},
        {Key::M, false},
        {Key::N, false},
        {Key::O, false},
        {Key::P, false},
        {Key::Q, false},
        {Key::R, false},
        {Key::S, false},
        {Key::T, false},
        {Key::U, false},
        {Key::V, false},
        {Key::W, false},
        {Key::Y, false},
        {Key::X, false},
        {Key::Z, false},
        {Key::ESCAPE, false},
        {Key::ENTER, false},
        {Key::TAB, false},
        {Key::BACKSPACE, false},
        {Key::LEFT, false},
        {Key::RIGHT, false},
        {Key::UP, false},
        {Key::DOWN, false},
        {Key::F1, false},
        {Key::F2, false},
        {Key::F3, false},
        {Key::F4, false},
        {Key::F5, false},
        {Key::F6, false},
        {Key::F7, false},
        {Key::F8, false},
        {Key::F9, false},
        {Key::F10, false},
        {Key::F11, false},
        {Key::F12, false},
        {Key::L_SHFT, false},
        {Key::R_SHIFT, false},
        {Key::L_CONTROL, false},
        {Key::R_CONTROL, false},
        {Key::L_ALT, false},
        {Key::R_ALT, false},
     });
}