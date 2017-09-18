#ifndef KEYBOARD_H_INCLUDED
#define KEYBOARD_H_INCLUDED

#include <SFML/Window/Keyboard.hpp>

#include <map>

class Keyboard : public sf::Keyboard
{
	public :

		static std::map<sf::Keyboard::Key, std::string>& getKeysNames() {
			static std::map<sf::Keyboard::Key, std::string> names;

			typedef sf::Keyboard::Key Key;

			if (names.size() == 0) {
				names[Key::Unknown] = "Unknown";
				names[Key::A] = "A";
				names[Key::B] = "B";
				names[Key::C] = "C";
				names[Key::D] = "D";
				names[Key::E] = "E";
				names[Key::F] = "F";
				names[Key::G] = "G";
				names[Key::H] = "H";
				names[Key::I] = "I";
				names[Key::J] = "J";
				names[Key::K] = "K";
				names[Key::L] = "L";
				names[Key::M] = "M";
				names[Key::N] = "N";
				names[Key::O] = "O";
				names[Key::P] = "P";
				names[Key::Q] = "Q";
				names[Key::R] = "R";
				names[Key::S] = "S";
				names[Key::T] = "T";
				names[Key::U] = "U";
				names[Key::V] = "V";
				names[Key::W] = "W";
				names[Key::X] = "X";
				names[Key::Y] = "Y";
				names[Key::Z] = "Z";
				names[Key::Num0] = "Num0";
				names[Key::Num1] = "Num1";
				names[Key::Num2] = "Num2";
				names[Key::Num3] = "Num3";
				names[Key::Num4] = "Num4";
				names[Key::Num5] = "Num5";
				names[Key::Num6] = "Num6";
				names[Key::Num7] = "Num7";
				names[Key::Num8] = "Num8";
				names[Key::Num9] = "Num9";
				names[Key::Escape] = "Escape";
				names[Key::LControl] = "LControl";
				names[Key::LShift] = "LShift";
				names[Key::LAlt] = "LAlt";
				names[Key::LSystem] = "LSystem";
				names[Key::RControl] = "RControl";
				names[Key::RShift] = "RShift";
				names[Key::RAlt] = "RAlt";
				names[Key::RSystem] = "RSystem";
				names[Key::Menu] = "Menu";
				names[Key::LBracket] = "LBracket";
				names[Key::RBracket] = "RBracket";
				names[Key::SemiColon] = "SemiColon";
				names[Key::Comma] = "Comma";
				names[Key::Period] = "Period";
				names[Key::Quote] = "Quote";
				names[Key::Slash] = "Slash";
				names[Key::BackSlash] = "BackSlash";
				names[Key::Tilde] = "Tilde";
				names[Key::Equal] = "Equal";
				names[Key::Dash] = "Dash";
				names[Key::Space] = "Space";
				names[Key::Return] = "Return";
				names[Key::BackSpace] = "BackSpace";
				names[Key::Tab] = "Tab";
				names[Key::PageUp] = "PageUp";
				names[Key::PageDown] = "PageDown";
				names[Key::End] = "End";
				names[Key::Home] = "Home";
				names[Key::Insert] = "Insert";
				names[Key::Delete] = "Delete";
				names[Key::Add] = "Add";
				names[Key::Subtract] = "Subtract";
				names[Key::Multiply] = "Multiply";
				names[Key::Divide] = "Divide";
				names[Key::Left] = "Left";
				names[Key::Right] = "Right";
				names[Key::Up] = "Up";
				names[Key::Down] = "Down";
				names[Key::Numpad0] = "Numpad0";
				names[Key::Numpad1] = "Numpad1";
				names[Key::Numpad2] = "Numpad2";
				names[Key::Numpad3] = "Numpad3";
				names[Key::Numpad4] = "Numpad4";
				names[Key::Numpad5] = "Numpad5";
				names[Key::Numpad6] = "Numpad6";
				names[Key::Numpad7] = "Numpad7";
				names[Key::Numpad8] = "Numpad8";
				names[Key::Numpad9] = "Numpad9";
				names[Key::F1] = "F1";
				names[Key::F2] = "F2";
				names[Key::F3] = "F3";
				names[Key::F4] = "F4";
				names[Key::F5] = "F5";
				names[Key::F6] = "F6";
				names[Key::F7] = "F7";
				names[Key::F8] = "F8";
				names[Key::F9] = "F9";
				names[Key::F10] = "F10";
				names[Key::F11] = "F11";
				names[Key::F12] = "F12";
				names[Key::F13] = "F13";
				names[Key::F14] = "F14";
				names[Key::F15] = "F15";
				names[Key::Pause] = "Pause";
			}

			return names;
		}
};

#endif // KEYBOARD_H_INCLUDED