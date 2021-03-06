/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <fs.h>
#include "fs/ostream.h"

i32_t
main(void)
{
  ex_t *e;
  ostream_t s1;

  init(&s1, ostream_t);
  if (ostream_open(&s1, "some1.log")) {
    usize_t off;

    loop:
    ostream_puts(&s1, "This is an apple");
    off = ostream_tell(&s1);
    ostream_seek(&s1, off - 7);
    ostream_puts(&s1, " sam");
    ostream_resume(&s1);
    ostream_rewind(&s1, 6);
    ostream_putc(&s1, 'S');
    ostream_forward(&s1, 5);
    ostream_putc(&s1, '.');
    TRY {
      ostream_rewind(&s1, 18);
    } CATCH(e) {
      ex_dump(e, stdout);
    }
    TRY {
      ostream_forward(&s1, 1);
    } CATCH(e) {
      ex_dump(e, stdout);
    }
    ostream_writef(&s1, " This is a digit: '%d'.", 5);
    if (s1.kind == OSTREAM_FILE) {
      assert(0 == memcmp(
        "This is a Sample. This is a digit: '5'.",
        s1.u.file.buf, s1.u.file.cur
      ));
      ostream_close(&s1);
      ostream_memopen(&s1);
      goto loop;
    } else {
      assert(0 == memcmp(
        "This is a Sample. This is a digit: '5'.",
        s1.u.mem.buf, s1.u.mem.cur
      ));
      ostream_close(&s1);
    }
  }
  cout_puts("Hello world");
  cout_putc('!');
  cout_write(" uael", 5);
  cout_writef(" with %s.", "love");
  cout_flush();
  return 0;
}
