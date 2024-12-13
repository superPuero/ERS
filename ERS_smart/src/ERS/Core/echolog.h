#pragma once

#include <iostream>
#include <string>

#ifndef BIT
	#define BIT(x) (1 << x)
#endif // !BIT

enum EchoLogColor {
	ECHO_COLOR_DEFAULT = 0,
	ECHO_COLOR_WHITE = BIT(0),
	ECHO_COLOR_BLACK = BIT(1),
	ECHO_COLOR_RED = BIT(2),
	ECHO_COLOR_YELLOW = BIT(3),
	ECHO_COLOR_GREEN = BIT(4),
	ECHO_COLOR_BLUE = BIT(5),
	ECHO_COLOR_MAGENTA = BIT(6),
	ECHO_COLOR_CYAN = BIT(7),
};

enum EchoLogStyle {
	ECHO_STYLE_DEFAULT = 0,
	ECHO_STYLE_BOLD = BIT(0),
	ECHO_STYLE_ITALIC = BIT(1),
	ECHO_STYLE_UNDELINE = BIT(2),
	ECHO_STYLE_BLINKING = BIT(3),

};

EchoLogStyle operator|(EchoLogStyle left_val, EchoLogStyle right_val) {
	return (EchoLogStyle)((uint32_t)left_val | (uint32_t)right_val);
}

namespace echolog {

	std::string get_string_code_color_text(EchoLogColor color) {

		switch (color)
		{
		case ECHO_COLOR_DEFAULT:
			return ";39";
			break;
		case ECHO_COLOR_WHITE:
			return ";37";
			break;
		case ECHO_COLOR_BLACK:
			return ";30";
			break;
		case ECHO_COLOR_RED:
			return ";31";
			break;
		case ECHO_COLOR_GREEN:
			return ";32";
			break;
		case ECHO_COLOR_YELLOW:
			return ";33";
			break;
		case ECHO_COLOR_BLUE:
			return ";34";
			break;
		case ECHO_COLOR_MAGENTA:
			return ";35";
			break;
		case ECHO_COLOR_CYAN:
			return ";36";
			break;
		default:
			return ";39";
			break;
		}
	}

	std::string get_string_code_color_background(EchoLogColor color) {
		switch (color)
		{
		case ECHO_COLOR_DEFAULT:
			return ";49";
			break;
		case ECHO_COLOR_WHITE:
			return ";47";
			break;
		case ECHO_COLOR_BLACK:
			return ";40";
			break;
		case ECHO_COLOR_RED:
			return ";41";
			break;
		case ECHO_COLOR_GREEN:
			return ";42";
			break;
		case ECHO_COLOR_YELLOW:
			return ";43";
			break;
		case ECHO_COLOR_BLUE:
			return ";44";
			break;
		case ECHO_COLOR_MAGENTA:
			return ";45";
			break;
		case ECHO_COLOR_CYAN:
			return ";46";
			break;
		default:
			return ";49";
			break;
		}
	}

	std::string get_string_code_style_text(EchoLogStyle style) {
		std::string string_code_output;

		if (style == ECHO_STYLE_DEFAULT) {
			return string_code_output;
		}
		if (style & ECHO_STYLE_BOLD) {
			string_code_output += ";1";
		}
		if (style & ECHO_STYLE_ITALIC) {
			string_code_output += ";3";
		}
		if (style & ECHO_STYLE_UNDELINE) {
			string_code_output += ";4";
		}
		if (style & ECHO_STYLE_BLINKING) {
			string_code_output += ";5";
		}

		return string_code_output;

	}
	template<typename...content>
	void log_decorated(uint32_t flag_color_text, uint32_t flag_color_background, uint32_t flags_style_text, content&&...contetnt) {

		std::string prefix = +"\033["
			+ get_string_code_color_text((EchoLogColor)flag_color_text)
			+ get_string_code_color_background((EchoLogColor)flag_color_background)
			+ get_string_code_style_text((EchoLogStyle)flags_style_text)
			+ "m";

		((std::cout << prefix << std::forward<content>(contetnt) << ' '), ...);

		std::cout << "\033[0m" << '\n';

	}

	template<typename...content>
	void log_raw(content&&...contetnt) {

		((std::cout << std::forward<content>(contetnt) << ' '), ...);

		std::cout << '\n';

	}
}
