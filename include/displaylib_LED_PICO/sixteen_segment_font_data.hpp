/*!
	@file   sixteen_segment_font_data.hpp
	@brief  Sixteen-segment font data class for LED segment modules.
*/

#ifndef SIXTEENSEG_COMMON_H
#define SIXTEENSEG_COMMON_H

#include <cstdint>

/*!
	@class SixteenSegmentFont
	@brief Class that provides access to a sixteen-segment font data table.
 */
class SixteenSegmentFont{
protected:
	static const uint16_t* pFontSixteenSegptr();
private:
	static const uint16_t fontData[];
};

#endif