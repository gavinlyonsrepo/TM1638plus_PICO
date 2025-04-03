/*!
	@file   fourteen_segment_font_data.hpp
	@brief  Fourteen-segment font data class for LED segment modules.
*/

#ifndef FOURTEENSEG_COMMON_H
#define FOURTEENSEG_COMMON_H

#include <cstdint>

/*!
	@class FourteenSegmentFont
	@brief Class that provides access to a fourteen-segment font data table.
 */
class FourteenSegmentFont{
protected:
	static const uint16_t* pFontFourteenSegptr();
private:
	static const uint16_t fontData[];
};

#endif