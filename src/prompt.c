/*
 * Copyright (c) 2015 Paul Fariello <paul@fariello.eu>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <stddef.h>
#include <stdlib.h>

#include <err.h>
#include <readpassphrase.h>
#include <sodium.h>
#include <string.h>

#include "kickpass.h"

#define PASSWORD_MAX_SIZE       1024
#define PASSWORD_PROMPT         "[kickpass] password: "
#define PASSWORD_CONFIRM_PROMPT "[kickpass] confirm: "

kp_error_t
kp_load_passwd(struct kp_ctx *ctx, bool confirm)
{
	ctx->password = sodium_malloc(PASSWORD_MAX_SIZE);
	if (!ctx->password) {
		warnx("memory error");
		return KP_ENOMEM;
	}

	if (readpassphrase(PASSWORD_PROMPT, ctx->password,
				PASSWORD_MAX_SIZE,
				RPP_ECHO_OFF | RPP_REQUIRE_TTY) == NULL) {
		warnx("cannot read password");
		return KP_EINPUT;
	}

	if (confirm) {
		char *confirm = sodium_malloc(PASSWORD_MAX_SIZE);
		if (readpassphrase(PASSWORD_CONFIRM_PROMPT, confirm,
					PASSWORD_MAX_SIZE,
					RPP_ECHO_OFF | RPP_REQUIRE_TTY)
				== NULL) {
			warnx("cannot read password");
			sodium_free(confirm);
			return KP_EINPUT;
		}

		if (strncmp(ctx->password, confirm, PASSWORD_MAX_SIZE) != 0) {
			warnx("mismatching password");
			sodium_free(confirm);
			return KP_EINPUT;
		}

		sodium_free(confirm);
	}

	return KP_SUCCESS;
}
