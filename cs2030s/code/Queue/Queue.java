public class Queue<T> {
  private T[] items;
  private int head;
  private int tail;

  public Queue(int capacity) {
    // The only way we can put an object into the array is through
    // the method push() and we can only put an object of type T inside. 
    // So it is type safe to cast `Object[]` to `T[]`
    @SuppressWarnings("unchecked")
    T[] temp = (T[]) new Object[capacity];
    this.items = temp;
    this.head = 0;
    this.tail = 0;
  }

  public int capacity() {
    return this.items.length;
  }

  public int size() {
    return Math.abs(this.tail - this.head);
  }

  public boolean isFull() {
    return this.head == (this.tail + 1) % this.capacity();
  }

  public boolean isEmpty() {
    return this.head == this.tail;
  }

  public void push(T t) throws QueueException {
    if (!(this.isFull())) {
      this.items[this.tail] = t;
      this.tail = (this.tail + 1) % this.capacity();
    }
    throw new QueueException("Queue is full. Cannot push " + t);
  }

  public T pop() throws QueueException {
    if (!(this.isEmpty())) {
      this.tail = (this.tail - 1) % this.capacity();
      return this.items[this.tail];
    }
    throw new QueueException("Queue is empty. Cannot pop.");
  }

  public T peek() throws QueueException {
    if (!(this.isEmpty())) {
      this.tail = (this.tail - 1) % this.capacity();
      T t = this.items[this.tail];
      this.tail = (this.tail + 1) % this.capacity();
      return t;
    }
    throw new QueueException("Queue is empty. Cannot peek.");
  }
}
