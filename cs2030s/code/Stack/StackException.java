public class StackException extends RuntimeException {
  public StackException(String msg) {
    super("STACK: " + msg);
  }
}
