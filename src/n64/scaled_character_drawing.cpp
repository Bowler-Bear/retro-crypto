#include <display.h>
#include <graphics.h>
#include <font.h>

#include "scaled_character_drawing.h"

//graphics.c defines
#define __get_buffer( disp ) ((disp)->buffer)
#define __set_pixel( buffer, x, y, color ) \
    (buffer)[(x) + ((y) * pix_stride)] = color

void graphics_draw_scaled_character( surface_t* disp, int x, int y, char ch, int scale = 1)
{
    if( disp == 0 ) { return; }
    if( scale <= 0 ) { scale = 1; }

    int pix_stride = TEX_FORMAT_BYTES2PIX(surface_get_format(disp), disp->stride);
    int depth = display_get_bitdepth();

    /* Figure out if they want the background to be transparent */
    uint32_t b_color = color_to_packed32(RGBA32(BG_COLOR_RED, BG_COLOR_GREEN, BG_COLOR_BLUE, 255));
    uint32_t f_color = color_to_packed32(RGBA32(FG_COLOR_RED, FG_COLOR_GREEN, FG_COLOR_BLUE, 255));
    int trans = 0;
    if( depth == 2 )
    {
        /* Is alpha bit set? */
        if( (b_color & 0x1) == 0x0 ) { trans = 1; }
    }
    else
    {
        /* Is alpha byte set? */
        if( (b_color & 0xFF) == 0x00 ) { trans = 1; }
    }

        if( depth == 2 )
        {
            uint16_t *buffer = (uint16_t *)__get_buffer( disp );

            for( int row = 0; row < 8; row++ )
            {
                unsigned char c = __font_data[(ch * 8) + row];

                for( int col = 0; col < 8; col++ )
                {
                    int active = (c & 0x80) != 0;
                    if( trans )
                    {
                        if( active )
                        {
                            /* Only draw it if it is active */
                            for(int horizontalScale = 0; horizontalScale < scale; horizontalScale++)
                            for(int verticalScale = 0; verticalScale < scale; verticalScale++)
                            __set_pixel( buffer, x + scale*col + horizontalScale, y + scale*row + verticalScale, f_color );
                        }
                    }
                    else
                    {
                        /* Display foreground or background depending on font data */
                        for(int horizontalScale = 0; horizontalScale < scale; horizontalScale++)
                        for(int verticalScale = 0; verticalScale < scale; verticalScale++)
                        __set_pixel( buffer, x + scale*col + horizontalScale, y + scale*row + verticalScale, active ? f_color : b_color );
                    }

                    c <<= 1;
                }
            }
        }
        else
        {
            uint32_t *buffer = (uint32_t *)__get_buffer( disp );

            for( int row = 0; row < 8; row++ )
            {
                unsigned char c = __font_data[(ch * 8) + row];

                for( int col = 0; col < 8; col++ )
                {
                    int active = (c & 0x80) != 0;
                    if( trans )
                    {
                        if( active )
                        {
                            /* Only draw it if it is active */
                            for(int horizontalScale = 0; horizontalScale < scale; horizontalScale++)
                                for(int verticalScale = 0; verticalScale < scale; verticalScale++)
                                    __set_pixel( buffer, x + scale*col + horizontalScale, y + scale*row + verticalScale, f_color );
                        }
                    }
                    else
                    {
                        /* Display foreground or background depending on font data */
                        for(int horizontalScale = 0; horizontalScale < scale; horizontalScale++)
                            for(int verticalScale = 0; verticalScale < scale; verticalScale++)
                                __set_pixel( buffer, x + scale*col + horizontalScale, y + scale*row + verticalScale, active ? f_color : b_color );
                    }

                    c <<= 1;
                }
            }
        }
}
