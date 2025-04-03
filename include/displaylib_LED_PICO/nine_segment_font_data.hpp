/*!
	@file   nine_segment_font_data.hpp
	@brief  nine-segment font data class for LED segment modules.
*/

#ifndef NINESEG_COMMON_H
#define NINESEG_COMMON_H

#include <cstdint>

/*!
	@class NineSegmentFont
	@brief Class that provides access to a nine-segment font data table.
 */
class NineSegmentFont {
protected:
	static const uint16_t* pFontNineSegptr();

private:
	static const uint16_t fontData[];
};

#endif