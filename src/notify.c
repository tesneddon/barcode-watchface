/*
**++
**  ABSTRACT:
**
**	Code for handling watchface notifications.
**
**  MODULE DESCRIPTION:
**
**	This module contains the notify*() routines.  These are used to
**  display full screen messages to the user from a watchface.  The
**  notification will display a full screen bitmap and an associated
**  message, enable the light and vibrate.
**
**	Although the notify() routine can be used to display any message/
**  image combo, the initialization can also subscribe to bluetooth and
**  battery warnings to display messages by default.
**
**  AUTHOR:     Tim Sneddon
**
**  Copyright (c) 2013, Endless Software Solutions.
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
**  CREATION DATE: 10-NOV-2013
**
**  MODIFICATION HISTORY:
**
**      10-NOV-2013 V1.0    Sneddon     Initial coding.
**	14-NOV-2013 V1.1    Sneddon	Add NOTIFY_[SUCCESS|FALIURE] constants.
**	14-NOV-2013 V1.2    Sneddon	Changed to reduce heap footprint.
**--
*/
#include <pebble.h>
#include "notify.h"
#define BATTERY_MINIMUM 15
#define WARNING_TIMEOUT 3000

/*
** Forward declarations
*/

    void notify(const char *message, GBitmap *image);
    void notify_init(bool subscribe, Window *window);
    void notify_deinit(void);
    static void handle_battery(BatteryChargeState charge);
    static void handle_bluetooth(bool connected);
    static void clear_warning(void *data);

/*
** Own storage
*/

    static bool subscribed = false;
    static BitmapLayer *warn_layer;
    static TextLayer *warn_text_layer;

void notify(const char *message,
            GBitmap *icon) {

    GBitmap *bmp;

    if (icon == NOTIFY_FAILURE) {
        bmp = gbitmap_create_with_resource(RESOURCE_ID_NOTIFY_FAILURE);
    } else if (icon == NOTIFY_SUCCESS) {
    	bmp = gbitmap_create_with_resource(RESOURCE_ID_NOTIFY_SUCCESS);
    } else {
	bmp = icon;
    }
    bitmap_layer_set_bitmap(warn_layer, icon);
    if (bmp != icon) gbitmap_destroy(bmp);

    text_layer_set_text(warn_text_layer, message);
    layer_set_hidden(bitmap_layer_get_layer(warn_layer), false);
    vibes_long_pulse();
    light_enable(true);
    app_timer_register(WARNING_TIMEOUT, clear_warning, 0);
}

void notify_init(bool subscribe,
		 Window *window) {

    Layer *window_layer = window_get_root_layer(window);

    /*
    ** Initialize the layers.
    */
    warn_layer = bitmap_layer_create(GRect(0, 0, 144, 168));
    layer_set_hidden(bitmap_layer_get_layer(warn_layer), true);
    layer_add_child(window_layer, bitmap_layer_get_layer(warn_layer));

    warn_text_layer = text_layer_create(GRect(0, 100, 144, 68));
    text_layer_set_background_color(warn_text_layer, GColorClear);
    text_layer_set_font(warn_text_layer,
                        fonts_get_system_font(FONT_KEY_GOTHIC_28));
    text_layer_set_text_alignment(warn_text_layer, GTextAlignmentCenter);
    text_layer_set_text_color(warn_text_layer, GColorWhite);
    layer_add_child(bitmap_layer_get_layer(warn_layer),
		    text_layer_get_layer(warn_text_layer));

    if (subscribe) {
        /*
    	** Subscribe to event services.
    	*/
	subscribed = true;

    	battery_state_service_subscribe(handle_battery);
    	bluetooth_connection_service_subscribe(handle_bluetooth);
    }
}

void notify_deinit(void) {

    if (subscribed) {
    	/*
    	** Unsubscribe from all events.
    	*/
	subscribed = false;

    	battery_state_service_unsubscribe();
    	bluetooth_connection_service_unsubscribe();
    }

    /*
    ** Destroy all layers.
    */
    bitmap_layer_destroy(warn_layer);
    text_layer_destroy(warn_text_layer);
}

static void handle_battery(BatteryChargeState charge) {

    GBitmap *icon;
    static char discmsg[25];

    if (!charge.is_charging && !charge.is_plugged
	&& (charge.charge_percent <= BATTERY_MINIMUM)) {
    	snprintf(discmsg, sizeof(discmsg), "Battery at %d%% Capacity",
		 charge.charge_percent);
        icon = gbitmap_create_with_resource(RESOURCE_ID_NOTIFY_LOW_BATTERY);
	notify(discmsg, icon);
	gbitmap_destroy(icon);
    }
}

static void handle_bluetooth(bool connected) {

    static const char connmsg[] = "Connection Restored";
    static const char discmsg[] = "Connection to Phone Lost";
    GBitmap *icon;

    if (!connected) {
        icon = gbitmap_create_with_resource(RESOURCE_ID_NOTIFY_NOT_CONNECTED);
	notify(discmsg, icon);
	gbitmap_destroy(icon);
    } else {
	notify(connmsg, NOTIFY_SUCCESS);
    }
}

static void clear_warning(void *data) {

    light_enable(false);
    layer_set_hidden(bitmap_layer_get_layer(warn_layer), true);
}
