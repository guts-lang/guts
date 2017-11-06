frame u8: ty<1> {
  +(o: this) => this + o;
}

trait ctor<T: ty> () => mem::set(this, 0, T::size);
trait realloc<T: ty, TSize: ty, buf: var, cap: var> (n: TSize) => {
  if (cap != n) {
    b: *T;
    if ((b = mem::realloc(buf, n * T::size)) == nil) {
      throw ex::NoMemory();
    }
    buf = b;
    cap = n;
  }
}

trait BufTrait<> {

}

namespace ds {

  interface Seq<T: ty, TSize: ty = usize> {
    cap, len: TSize;
    buf: *T;

    this := ctor<self>;
    realloc := realloc<T, TSize, buf, cap>;
  }
}
