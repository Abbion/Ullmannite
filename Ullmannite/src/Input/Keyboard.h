#pragma once
#include "Ullpch.h"
#include <GLFW/glfw3.h>
#include <map>

namespace Ull
{
    class Keyboard 
    {
    public:
        enum class Key : uint16_t
        {
            SPACE = GLFW_KEY_SPACE,
            ZERO = GLFW_KEY_0,
            ONE = GLFW_KEY_1,
            TWO = GLFW_KEY_2,
            THREE = GLFW_KEY_3,
            FOUR = GLFW_KEY_4,
            FIVE = GLFW_KEY_5,
            SIX = GLFW_KEY_6,
            SEVEN = GLFW_KEY_7,
            EIGHT = GLFW_KEY_8,
            NINE = GLFW_KEY_9,
            A = GLFW_KEY_A,
            B = GLFW_KEY_B,
            C = GLFW_KEY_C,
            D = GLFW_KEY_D,
            E = GLFW_KEY_E,
            F = GLFW_KEY_F,
            G = GLFW_KEY_G,
            H = GLFW_KEY_H,
            I = GLFW_KEY_I,
            J = GLFW_KEY_J,
            K = GLFW_KEY_K,
            L = GLFW_KEY_L,
            M = GLFW_KEY_M,
            N = GLFW_KEY_N,
            O = GLFW_KEY_O,
            P = GLFW_KEY_P,
            Q = GLFW_KEY_Q,
            R = GLFW_KEY_R,
            S = GLFW_KEY_S,
            T = GLFW_KEY_T,
            U = GLFW_KEY_U,
            V = GLFW_KEY_V,
            W = GLFW_KEY_W,
            X = GLFW_KEY_X,
            Y = GLFW_KEY_Y,
            Z = GLFW_KEY_Z,
            ESCAPE = GLFW_KEY_ESCAPE,
            ENTER = GLFW_KEY_ENTER,
            TAB = GLFW_KEY_TAB,
            BACKSPACE = GLFW_KEY_BACKSPACE,
            LEFT = GLFW_KEY_LEFT,
            RIGHT = GLFW_KEY_RIGHT,
            UP = GLFW_KEY_UP,
            DOWN = GLFW_KEY_DOWN,
            F1 = GLFW_KEY_F1,
            F2 = GLFW_KEY_F2,
            F3 = GLFW_KEY_F3,
            F4 = GLFW_KEY_F4,
            F5 = GLFW_KEY_F5,
            F6 = GLFW_KEY_F6,
            F7 = GLFW_KEY_F7,
            F8 = GLFW_KEY_F8,
            F9 = GLFW_KEY_F9,
            F10 = GLFW_KEY_F10,
            F11 = GLFW_KEY_F11,
            F12 = GLFW_KEY_F12,
            L_SHFT = GLFW_KEY_LEFT_SHIFT,
            R_SHIFT = GLFW_KEY_RIGHT_SHIFT,
            L_CONTROL = GLFW_KEY_LEFT_CONTROL,
            R_CONTROL = GLFW_KEY_RIGHT_CONTROL,
            L_ALT = GLFW_KEY_LEFT_ALT,
            R_ALT = GLFW_KEY_RIGHT_ALT
        };

    public:
        NON_COPYABLE(Keyboard);

        ~Keyboard();

        static Keyboard& GetInstance();
        bool IsKeyPressed(Key key) const;

    protected:
        Keyboard();
        
        static Keyboard m_keyboardInstance;

    private:
        std::map<Key, bool> m_keyMap;

        void UpdateKeyMap(const std::map<Key, bool>& updatedKeyMap);
        void InitKeyMap();

        friend class Application;
    };
}