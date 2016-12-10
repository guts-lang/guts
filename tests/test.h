/*
 *  MIT License
 *
 * Copyright (c) 2016 .DRY (Don't Repeat Yourself)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
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

/* $Id$ */

#ifndef _SYNTAX_H
#define _SYNTAX_H

#define SYNTAX_TEST(prefix, postfix, ex_status, ex_cerr, code) \
  TEST(prefix, postfix) { \
    ddc::driver driver = ddc::driver(); \
    testing::internal::CaptureStderr(); \
    ASSERT_EQ(ex_status, driver.parse_string(code)); \
    std::string output = testing::internal::GetCapturedStderr(); \
    ASSERT_EQ(ex_cerr, output); \
  } \

#define RUN  \
  int main(int argc, char **argv) { \
    ::testing::InitGoogleTest(&argc, argv); \
    return RUN_ALL_TESTS(); \
  }

#endif /* _SYNTAX_H */

/*
 * Local variables:
 * tab-width: 2
 * c-basic-offset: 2
 * End:
 * vim600: noet sw=2 ts=2 fdm=marker
 * vim<600: noet sw=2 ts=2
 */