/*++
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
**--
*/
#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"
#define BAR_WIDTH 3

/*
** Forward declarations
*/

    void pbl_main(void *params);
    void handle_init(AppContextRef ctx);
    void handle_tick(AppContextRef ctx, PebbleTickEvent *event);
    void update_time_layer(Layer *layer, GContext *gc);
    void handle_deinit(AppContextRef ctx);

/*
** Pebble Watchapp Identification
*/

#define MY_UUID { 0xF7, 0x8F, 0xBD, 0xDE, \
		  0x9A, 0x65, 0x40, 0x3A, \
		  0x84, 0xB8, 0x54, 0xDE, \
		  0x35, 0x6A, 0x10, 0x41 }

PBL_APP_INFO(MY_UUID,
             "Barcode Watchface", "Tim Sneddon",
             1, 0, /* App version */
             DEFAULT_MENU_ICON,
             APP_INFO_WATCH_FACE);

/*
** Global storage
*/

    HeapBitmap left, bar[10], right;
    BitmapLayer bar_layer[6], left_layer, right_layer;;
    TextLayer text_layer;
    Window window;

void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
    	.init_handler = handle_init,
	.tick_info = {
	    .tick_handler = handle_tick,
	    .tick_units = SECOND_UNIT | MINUTE_UNIT | HOUR_UNIT,
	},
	.deinit_handler = handle_deinit,
    };
    app_event_loop(params, &handlers);
}

void handle_init(AppContextRef ctx) {

    PebbleTickEvent event;
    PblTm now;
    unsigned i;

    window_init(&window, "Barcode Watchface");
    window_stack_push(&window, true /* Animated */);
    window_set_background_color(&window, GColorWhite);

    resource_init_current_app(&APP_RESOURCES);

    /*
    ** Configure the time barcode layer.
    */
    heap_bitmap_init(&bar[0], RESOURCE_ID_0);
    heap_bitmap_init(&bar[1], RESOURCE_ID_1);
    heap_bitmap_init(&bar[2], RESOURCE_ID_2);
    heap_bitmap_init(&bar[3], RESOURCE_ID_3);
    heap_bitmap_init(&bar[4], RESOURCE_ID_4);
    heap_bitmap_init(&bar[5], RESOURCE_ID_5);
    heap_bitmap_init(&bar[6], RESOURCE_ID_6);
    heap_bitmap_init(&bar[7], RESOURCE_ID_7);
    heap_bitmap_init(&bar[8], RESOURCE_ID_8);
    heap_bitmap_init(&bar[9], RESOURCE_ID_9);

    heap_bitmap_init(&left, RESOURCE_ID_LEFT);
    heap_bitmap_init(&right, RESOURCE_ID_RIGHT);

    /*
    ** Configure the layers...
    */
    bitmap_layer_init(&left_layer, GRect(21, 10, 6, 135));
    bitmap_layer_set_bitmap(&left_layer, &left.bmp);
    layer_add_child(&window.layer, &left_layer.layer);

    for (i = 0; i < sizeof(bar_layer) / sizeof(bar_layer[0]); i++) {
	bitmap_layer_init(&bar_layer[i],
			  GRect(27 + (i * 14), 10, 14, 120));
	layer_add_child(&window.layer, &bar_layer[i].layer);
    }

    bitmap_layer_init(&right_layer, GRect(111, 10, 12, 135));
    bitmap_layer_set_bitmap(&right_layer, &right.bmp);
    layer_add_child(&window.layer, &right_layer.layer);

    text_layer_init(&text_layer, GRect(29, 134, 82, 21));
    text_layer_set_font(&text_layer,
			fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21));
    text_layer_set_text_alignment(&text_layer, GTextAlignmentCenter);
    layer_add_child(&window.layer, &text_layer.layer);

    /*
    ** Set time initially...
    */
    event.units_changed = HOUR_UNIT | MINUTE_UNIT | SECOND_UNIT;
    event.tick_time = &now;
    get_time(&now);
    handle_tick(ctx, &event);
}

void handle_tick(AppContextRef ctx,
		 PebbleTickEvent *event) {

    static char text_string[] = "HHMMSS"; /* Filled out with maximum values
					     for sizeof(). */
    int ones, tens;

    if (event->units_changed & DAY_UNIT) {
    	/* do nothing, right now... */
	return;
    } else {
    	if (event->units_changed & HOUR_UNIT) {
	    tens = event->tick_time->tm_hour / 10;
    	    ones = event->tick_time->tm_hour % 10;

	    if (!clock_is_24h_style() && (event->tick_time->tm_hour > 12)) {
		tens--;
		ones -= 2;
	    }

    	    bitmap_layer_set_bitmap(&bar_layer[0], &bar[tens].bmp);
    	    bitmap_layer_set_bitmap(&bar_layer[1], &bar[ones].bmp);
    	}
        if (event->units_changed & MINUTE_UNIT) {
	    tens = event->tick_time->tm_min / 10;
    	    ones = event->tick_time->tm_min % 10;

    	    bitmap_layer_set_bitmap(&bar_layer[2], &bar[tens].bmp);
    	    bitmap_layer_set_bitmap(&bar_layer[3], &bar[ones].bmp);
    	}
   	if (event->units_changed & SECOND_UNIT) {
	    tens = event->tick_time->tm_sec / 10;
    	    ones = event->tick_time->tm_sec % 10;

    	    bitmap_layer_set_bitmap(&bar_layer[4], &bar[tens].bmp);
    	    bitmap_layer_set_bitmap(&bar_layer[5], &bar[ones].bmp);
    	}

	string_format_time(text_string, sizeof(text_string),
			   (clock_is_24h_style() ? "%k%M%S" : "%l%M%S"),
			   event->tick_time);
	text_layer_set_text(&text_layer, text_string);
    }
}

void handle_deinit(AppContextRef ctx) {

    unsigned i;

    /*
    ** Tidy up the markers.
    */
    heap_bitmap_deinit(&left);
    heap_bitmap_deinit(&right);

    /*
    ** Tidy up the bars.
    */
    for (i = 0; i < sizeof(bar)/sizeof(bar[0]); i++) {
        heap_bitmap_deinit(&bar[i]);
    }

    return;
}
