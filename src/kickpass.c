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

#include <err.h>
#include <readpassphrase.h>
#include <sodium.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "kickpass.h"

#define PASSWORD_MAX_SIZE 1024
#define PASSWORD_PROMPT   "[kickpass] password: "

kp_error_t
kp_init(struct kp_ctx *ctx)
{
	const char *home;

	home = getenv("HOME");
	if (!home)
		errx(KP_EINPUT, "cannot find $HOME environment variable");

	if (strlcpy(ctx->ws_path, home, PATH_MAX) >= PATH_MAX)
		errx(KP_ENOMEM, "memory error");

	if (strlcat(ctx->ws_path, "/" KP_PATH, PATH_MAX) >= PATH_MAX)
		errx(KP_ENOMEM, "memory error");

	if (sodium_init() != 0)
		err(KP_EINTERNAL, "cannot initialize sodium");

	ctx->memlimit = crypto_pwhash_scryptsalsa208sha256_MEMLIMIT_SENSITIVE/5;
	ctx->opslimit = crypto_pwhash_scryptsalsa208sha256_OPSLIMIT_SENSITIVE/5;

	return KP_SUCCESS;
}

kp_error_t
kp_load_passwd(struct kp_ctx *ctx)
{
	ctx->password = sodium_malloc(PASSWORD_MAX_SIZE);
	if (!ctx->password) {
		warnx("memory error");
		return KP_ENOMEM;
	}

	if (readpassphrase(PASSWORD_PROMPT, ctx->password, PASSWORD_MAX_SIZE, RPP_ECHO_OFF) == NULL) {
		warnx("cannot read password");
		return KP_EINPUT;
	}

	return KP_SUCCESS;
}

kp_error_t
kp_fini(struct kp_ctx *ctx)
{
	return KP_SUCCESS;
}