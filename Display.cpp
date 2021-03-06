#include "Display.h"
#include "U8glib.h"


U8GLIB_ST7920_128X64_4X u8g(SCK_DISPLAY, MOSI_DISPLAY, CS_DISPLAY);


Display::Display()
{
    u8g.setColorIndex(1);
    Serial.begin(9600);
}


void Display::draw_main_page(float temp, bool state, int set_temp, byte process) {
    /*
     * Method for drawings main page
     *
     * temp: current temperature in *C
     * state: 0 - Off, 1 - On
     * set_temp: setting temperature in box
     * process: 0 - none, 1 - heating, 2 - cooling
     *
     */

    // current t *C
    String text = current_t_text + temp + degrees_by_C;

    uint8_t len_text = text.length() + 1;

    char char_text[len_text];
    text.toCharArray(char_text, len_text);

    // set t *C. Current state
    text = set_t_text + set_temp + degrees_by_C;
    len_text = text.length() + 1;

    char char_set_t[len_text];
    text.toCharArray(char_set_t, len_text);

    // settings for display
    u8g.setFontRefHeightText();
    u8g.setFontPosTop();
    u8g.setFont(u8g_font_6x12);
    u8g.setDefaultForegroundColor();

    u8g.firstPage();
    do {
        u8g.drawStr(5, 2, char_text);
        u8g.drawLine(5, 13, 123, 13);

        if (state) {
            u8g.drawStr(5, 22, char_set_t);

            if (process == 1)
                u8g.drawStr(5, 36, heating_state);

            else if (process == 2)
                u8g.drawStr(5, 36, cooling_state);
        }
    } while( u8g.nextPage() );

}


void Display::update_current_position(bool update, byte max_len) {
    // if update == 0, sub 1; if update == 1, add 1
    if (update) {
        if (max_len - 1 - cur_pos_set_t)
            cur_pos_set_t++;
        else
            cur_pos_set_t = 0;
    }
    else {
        if (cur_pos_set_t > 0)
            cur_pos_set_t--;
        else
            cur_pos_set_t = max_len - 1;

    }
}


void Display::draw_set_t(bool state, byte temperature) {

    char buf[4];
    const char *menu_strings[] = {state ? on_state : off_state, itoa(temperature, buf, 10)};
//    const char *menu_strings[] = {"Thermostat: " + state ? on_state : off_state, "t: " + itoa(temperature, buf, 10)};

    uint8_t i, height_elem;
    u8g_uint_t center_elem;

    u8g.setFontRefHeightText();
    u8g.setFontPosTop();

    height_elem = u8g.getFontAscent()-u8g.getFontDescent()+3;

    u8g.firstPage();
    do {

        for( i = 0; i < max_len_t; i++ ) {
            center_elem = (full_width-u8g.getStrWidth(menu_strings[i]))/2;
            u8g.setDefaultForegroundColor();
            if ( i == cur_pos_set_t ) {
                u8g.drawBox(0, i*height_elem+2, full_width, height_elem+2);
                u8g.setDefaultBackgroundColor();
                u8g.drawStr( arrow_l_x, i*height_elem+5, l_arrow);
                u8g.drawStr( arrow_r_x, i*height_elem+5, r_arrow);
            }

            u8g.drawStr(center_elem, i*height_elem+5, menu_strings[i]);
        }
    } while( u8g.nextPage() );
}

void Display::draw_set_light(byte state, byte _UV, byte _R, byte _G, byte _B) {

    char buf_UV[4];
    char buf_R[4];
    char buf_G[4];
    char buf_B[4];

    const char *menu_strings[] = {state ?
                                    (state == UV_STATE ? uv_state : state == RGB_STATE ? rgb_state : rgb_uv_state)
                                  : off_state,
                                  itoa(_UV, buf_UV, 10),
                                  itoa(_R, buf_R, 10),
                                  itoa(_G, buf_G, 10),
                                  itoa(_B, buf_B, 10)};


    uint8_t i, height_elem;
    u8g_uint_t center_elem;

    u8g.setFontRefHeightText();
    u8g.setFontPosTop();

    height_elem = u8g.getFontAscent()-u8g.getFontDescent()+3;

    u8g.firstPage();
    do {

        for( i = 0; i < max_len_light; i++ ) {
            center_elem = (full_width-u8g.getStrWidth(menu_strings[i]))/2;
            u8g.setDefaultForegroundColor();
            if ( i == cur_pos_set_t ) {
                u8g.drawBox(0, i*height_elem+2, full_width, height_elem+2);
                u8g.setDefaultBackgroundColor();
                u8g.drawStr( arrow_l_x, i*height_elem+5, l_arrow);
                u8g.drawStr( arrow_r_x, i*height_elem+5, r_arrow);
            }

            u8g.drawStr(center_elem, i*height_elem+5, menu_strings[i]);

            // Black magic of the Mordor
            u8g.drawStr(text_x, i*height_elem+5, (
                i ? (i==1 ?
                    uv_text
                    : (i == 2 ?
                        r_text
                        : (i == 3 ?
                            g_text
                            : b_text
                          )
                      )
                    )
                : blank_text
              )
            );
        }

    } while( u8g.nextPage() );
}
