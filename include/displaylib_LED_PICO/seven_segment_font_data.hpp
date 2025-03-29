/*!
	@file   seven_segment_font_data.hpp
	@brief  Seven-segment font data class for LED segment modules.
*/

#pragma once

#include <cstdint>

/*!
	@class SevenSegmentFont
	@brief Class that provides access to a seven-segment font data table.
 */
class SevenSegmentFont {
public:
	static const uint8_t* pFontSevenSegptr();

	const uint8_t _ASCII_FONT_OFFSET     = 0x20; /**< Offset in the ASCII table for font Start position */
	const uint8_t _ASCII_FONT_END        = 0x7B; /**< End of ASCII Table + 1*/
	const uint8_t _ASCII_FONT_HEX_OFFSET = 0x10; /**< ASCII table offset to reach the number position */
private:
	static const uint8_t fontData[];
};

