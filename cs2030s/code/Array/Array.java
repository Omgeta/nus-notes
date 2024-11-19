public class Array<T> {
  private T[] items;

  public Array(int capacity) {
    // The only way we can put an object into the array is through
    // the method set() and we can only put an object of type T inside. 
    // So it is type safe to cast `Object[]` to `T[]`
    @SuppressWarnings("unchecked")
    T[] temp = (T[]) new Object[capacity];
    this.items = temp;
  }

  public int length() {
    return this.items.length;
  }

  public void set(int index, T t) {
    this.items[index] = t;
  }

  public T get(int index) {
    return this.items[index];
  }

  public void copyFrom(Array<? extends T> src) {
    int len = Math.min(this.length(), src.length());
    for (int i = 0; i < len; i++) {
      this.set(i, src.get(i)); 
    }
  }

  public void copyTo(Array<? super T> dst) {
    int len = Math.min(this.length(), dst.length());
    for (int i = 0; i < len; i++) {
      dst.set(i, this.get(i)); 
    }
  }
}
