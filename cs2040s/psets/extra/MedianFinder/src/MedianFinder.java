import java.util.TreeMap;
import java.util.TreeSet;

public class MedianFinder {
    private final TreeSet<Integer> lower;  // Stores the lower half of elements
    private final TreeSet<Integer> upper;  // Stores the upper half of elements

    public MedianFinder() {
        lower = new TreeSet<>();
        upper = new TreeSet<>();
    }

    // Inserts an element into the data structure
    public void insert(int e) {
        lower.add(e);
       if (upper.size() < lower.size() - 2) {
           upper.add(lower.removeLast());
       }
    }

    // Retrieves and removes the median element
    public int getMedian() {
        int res = lower.removeLast();
        if (lower.size() == upper.size() && !upper.isEmpty()) {
            lower.add(upper.removeFirst());
        }
        return res;
    }

    public static void main(String[] args) {
        MedianFinder sol = new MedianFinder();
        int[] test = {4, 2, 3, 1};
        for (int i : test) {
            sol.insert(i);
        }
        for (int _: test) {
            System.out.println(sol.getMedian());
        }
    }
}
