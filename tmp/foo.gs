namespace foo {
  
  struct Bar<T> {
    callback: <()>;
    callback_2: <(): ?u8>;
    
    ~self();
    self {
      callback: <()> = method,
      callback_2: <(): ?u8> = method_2
    } {
      self.callback = callback;
      self.callback_2 = callback_2;
    }
    
    method() { }
    method2() : ?u8 => null;
  }
}

namespace ds {

  interface Hashable {
    hash(): u32;
  }

  struct Map<TKey: Hashable<TKey>, TValue, TSize = usize> {
    enum Factor { Grow(0.66), Shrink(0.21), }
    struct Entry { key: TKey; value: TValue; }
    struct Span {
      private flag: enum { Taken(0b01), Deleted(0b10), };

      hash: u32;

      available: bool => !self.flag || self.deleted;
      taken: bool {
        get => self.flag & self::Taken;
        set(v: bool) => v ? self::Taken : 0;
      }
      deleted: bool {
        get => self.flag & self::Deleted;
        set(v: bool) => value ? self::Deleted : 0;
      }
    }

    private entries: *Entry;
    private spans: *Span;
    private bit: TSize;

    length: TSize;
    
    self();
    ~self() {
      destroy self.entries;
      destroy self.spans;
    }
    
    [](key: TKey, value: TValue): <u32, *const Entry> {
      if !self.bit
        self.resize(4);
      else if self.len >= (1u << self.bit) * self::Factor::Grow
        self.resize(self.bit + 1);
        
      let hash: u32 = key.hash();
      let mask: u32 = (1U << self.bit) - 1;
      let i: u32 = hash & mask;

      loop {
        let span: *Span = self.spans + i;
        let entry: *Entry = self.entries + i;

        if span.available || (hash == span.hash && entry.key == key) {
          span.taken = true;
          span.hash = hash;
          entry = { key, value };
          ++self.len;
          return i, entry;
        }
        i = (i + 1) & mask;
      }
    }
  }
}
