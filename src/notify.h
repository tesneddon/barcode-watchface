/*
**++
**  ABSTRACT:
**
**	Code for handling watchface notifications.
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
**--
*/
#ifndef notify_h__
#define notify_h__
#include <pebble.h>

#define BATTERY_MINIMUM 15
#define WARNING_TIMEOUT 3000

/*
**  NOTIFY
*/

    void notify(const char *message, GBitmap *image);
    void notify_init(bool subscribe, Window *window);
    void notify_deinit(void);

#endif /* notify_h__ */
