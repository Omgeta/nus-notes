public class QueueException extends RuntimeException {
  public QueueException(String msg) {
    super("QUEUE: " + msg);
  }
}
