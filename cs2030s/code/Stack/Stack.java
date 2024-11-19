public class Stack<T> {
  private T[] items;
  private int top;

  public Stack(int capacity) {
    // The only way we can put an object into the array is through
    // the method push() and we can only put an object of type T inside. 
    // So it is type safe to cast `Object[]` to `T[]`
    @SuppressWarnings("unchecked")
    T[] temp = (T[]) new Object[capacity];
    this.items = temp;
    this.top = 0;
  }

  public int capacity() {
    return this.items.length;
  }

  public int size() {
    return this.top;
  }

  public boolean isFull() {
    return this.size() == this.capacity();
  }

  public boolean isEmpty() {
    return this.size() == 0;
  }

  public void push(T t) throws StackException {
    if (!(this.isFull())) {
      this.items[this.top++] = t;
    }
    throw new StackException("Stack is full. Cannot push " + t);
  }

  public T pop() throws StackException {
    if (!(this.isEmpty())) {
      return this.items[--this.top];
    }
    throw new StackException("Stack is empty. Cannot pop.");
  }

  public T peek() throws StackException {
    if (!(this.isEmpty())) {
      T item = this.items[--this.top];
      this.top += 1;
      return item;
    }
    throw new StackException("Stack is empty. Cannot peek.");
  }
}
