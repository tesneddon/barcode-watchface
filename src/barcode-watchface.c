/*++
**  MODULE DESCRIPTION:
**
**      This is a Pebble Watchface App...
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

/*
** Forward declarations
*/

    void pbl_main(void *params);
    void handle_init(AppContextRef ctx);
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

    Window window;


void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
    	.init_handler = &handle_init
    };
    app_event_loop(params, &handlers);
}

void handle_init(AppContextRef ctx) {

    window_init(&window, "Barcode Watchface");
    window_stack_push(&window, true /* Animated */);
}


