/*
**++
**  ABSTRACT:
**
**      This is a Pebble Watchface App that displays the current time as
**  a UPC-E barcode.
**
**  MODULE DESCRIPTION:
**
**	....describe how it works (including that each bar is 2px.
**
**  AUTHOR:     Tim Sneddon
**
**  Copyright (c) 2013, Tim Sneddon <tim@sneddon.id.au>.
**
**  All rights reserved.
**
**  Redistribution and use in source and binary forms, with or without
**  modification, are permitted provided that the following conditions
**  are met:
**
**      * Redistributions of source code must retain the above
**        copyright notice, this list of conditions and the following
**        disclaimer.
**      * Redistributions in binary form must reproduce the above
**        copyright notice, this list of conditions and the following
**        disclaimer in the documentation and/or other materials provided
**        with the distribution.
**      * Neither the name of the copyright owner nor the names of any
**        other contributors may be used to endorse or promote products
**        derived from this software without specific prior written
**        permission.
**
**  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
**  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
**  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
**  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
**  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
**  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
**  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
**  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
**  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
**  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**  CREATION DATE: 22-OCT-2013
**
**  MODIFICATION HISTORY:
**
**      22-OCT-2013 V1.0    Sneddon     Initial coding.
**      28-OCT-2013 V1.1    Sneddon     Fixed 24-hour time string to include
**					leading zeroes.
**	07-NOV-2013 V2.0    Sneddon	Make SDK 2.0 compliant.  Add bluetooth
**					connection check.
**--
*/
#include <pebble.h>

/*
** Forward declarations
*/

    static void init(void);
    static void deinit(void);
    static void tick(struct tm *tick_time, TimeUnits units_changed);
    static void accel_tap(AccelAxisType axis, int32_t direction);
    static void bluetooth_connection(bool connected);
    int main(void);

/*
** Own storage
*/

    static const int BAR_RESOURCE_ID[] = {
    	RESOURCE_ID_0, RESOURCE_ID_1, RESOURCE_ID_2, RESOURCE_ID_3,
    	RESOURCE_ID_4, RESOURCE_ID_5, RESOURCE_ID_6, RESOURCE_ID_7,
	RESOURCE_ID_8, RESOURCE_ID_9,
    };
    static GBitmap *bar[10], *left, *right;
    static BitmapLayer *bar_layer[6], *left_layer, *right_layer;
    static TextLayer *text_layer;
    static Window *window;

/*
** Global storage
*/

static void init(void) {

    time_t now;
    Layer *window_layer;
    unsigned i;

    /*
    ** Initialize the main window.
    */
    window = window_create();
    if (window == 0) {
    	APP_LOG(APP_LOG_LEVEL_DEBUG, "Could not allocate window");
	return;
    }
    window_stack_push(window, true);
    window_layer = window_get_root_layer(window);

    /*
    ** Load graphics for each bar.
    */
    for (i = 0; i < sizeof(bar) / sizeof(bar[0]); i++)
    	bar[i] = gbitmap_create_with_resource(BAR_RESOURCE_ID[i]);
    left = gbitmap_create_with_resource(RESOURCE_ID_LEFT);
    right = gbitmap_create_with_resource(RESOURCE_ID_RIGHT);

    /*
    ** Initialize the layers.
    */
    left_layer = bitmap_layer_create(GRect(21, 10, 6, 135));
    bitmap_layer_set_bitmap(left_layer, left);
    layer_add_child(window_layer, bitmap_layer_get_layer(left_layer));
    for (i = 0; i < sizeof(bar_layer) / sizeof(bar_layer[0]); i++) {
    	bar_layer[i] = bitmap_layer_create(GRect(27 + (i * 14), 10, 14, 120));
	layer_add_child(window_layer, bitmap_layer_get_layer(bar_layer[i]));
    }
    right_layer = bitmap_layer_create(GRect(111, 10, 12, 135));
    bitmap_layer_set_bitmap(right_layer, right);
    layer_add_child(window_layer, bitmap_layer_get_layer(right_layer));

    text_layer = text_layer_create(GRect(29, 134, 82, 21));
    text_layer_set_font(text_layer,
                        fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
    layer_add_child(window_layer, text_layer_get_layer(text_layer));

    /*
    ** Subscribe to event services.
    */
    bluetooth_connection_service_subscribe(bluetooth_connection);
    accel_tap_service_subscribe(accel_tap);
    tick_timer_service_subscribe(HOUR_UNIT | MINUTE_UNIT | SECOND_UNIT, tick);

    /*
    ** Set initial time...
    */
    now = time(0);
    tick(localtime(&now), HOUR_UNIT | MINUTE_UNIT | SECOND_UNIT);
}

static void deinit(void) {

    unsigned i;

    /*
    ** Unsubscribe from all events.
    */
    bluetooth_connection_service_unsubscribe();
    accel_tap_service_unsubscribe();
    tick_timer_service_unsubscribe();

    /*
    ** Destroy all bitmaps.
    */
    for (i = 0; i < sizeof(bar) / sizeof(bar[0]); i++)
    	gbitmap_destroy(bar[i]);
    gbitmap_destroy(left);
    gbitmap_destroy(right);

    /*
    ** Destroy all layers.
    */
    for (i = 0; i < sizeof(bar_layer) / sizeof(bar_layer[0]); i++)
    	bitmap_layer_destroy(bar_layer[i]);
    bitmap_layer_destroy(left_layer);
    bitmap_layer_destroy(right_layer);

    text_layer_destroy(text_layer);

    window_destroy(window);
}

static void tick(struct tm *tick_time,
		 TimeUnits units_changed) {

    static char buffer[6+1];  /* HHMMSS + '\0' */
    static unsigned show_date = 0;
    int ones, tens;

    if (show_date != 0) {
	show_date--;
	return;
    }

    if (units_changed & DAY_UNIT) {
	tens = tick_time->tm_mday / 10;
	ones = tick_time->tm_mday % 10;

	bitmap_layer_set_bitmap(bar_layer[0], bar[tens]);
	bitmap_layer_set_bitmap(bar_layer[1], bar[ones]);

	tens = tick_time->tm_mon / 10;
	ones = tick_time->tm_mon % 10;

	bitmap_layer_set_bitmap(bar_layer[2], bar[tens]);
	bitmap_layer_set_bitmap(bar_layer[3], bar[ones]);

	tens = (tick_time->tm_year % 1000);
    	ones = tens & 10;
	tens /= 10;

	bitmap_layer_set_bitmap(bar_layer[4], bar[tens]);
	bitmap_layer_set_bitmap(bar_layer[5], bar[ones]);

    	strftime(buffer, sizeof(buffer), "%d%m%y", tick_time);

	show_date = 3;
    } else {
    	if (units_changed & HOUR_UNIT) {
    	    tens = tick_time->tm_hour / 10;
	    ones = tick_time->tm_hour % 10;
	    if (!clock_is_24h_style() && (tick_time->tm_hour > 12)) {
	    	tens--;
	        ones -= 2;
	    }

	    bitmap_layer_set_bitmap(bar_layer[0], bar[tens]);
	    bitmap_layer_set_bitmap(bar_layer[1], bar[ones]);
    	}
        if (units_changed & MINUTE_UNIT) {
    	    tens = tick_time->tm_min / 10;
	    ones = tick_time->tm_min % 10;

	    bitmap_layer_set_bitmap(bar_layer[2], bar[tens]);
	    bitmap_layer_set_bitmap(bar_layer[3], bar[ones]);
    	}
        tens = tick_time->tm_sec / 10;
        ones = tick_time->tm_sec % 10;

    	bitmap_layer_set_bitmap(bar_layer[4], bar[tens]);
    	bitmap_layer_set_bitmap(bar_layer[5], bar[ones]);

    	strftime(buffer, sizeof(buffer),
	     	 (clock_is_24h_style() ? "%H%M%S" : "%I%M%S"), tick_time);
    }

    text_layer_set_text(text_layer, buffer);
}

static void accel_tap(AccelAxisType axis,
		      int32_t direction) {
    time_t now;

    now = time(0);
    tick(localtime(&now), DAY_UNIT);
}

static void bluetooth_connection(bool connected) {

    // buzz, light up
    // show an image for three seconds showing the phone is not connected.

}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
