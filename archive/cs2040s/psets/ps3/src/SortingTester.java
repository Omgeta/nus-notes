import java.security.Key;
import java.util.Random;

public class SortingTester {
    private static final Random rng = new Random();

    public static boolean checkSort(ISort sorter, int size) {
        KeyValuePair[] arr = randomArray(size);

        sorter.sort(arr);

        KeyValuePair prev = new KeyValuePair(-1, 1);
        for (KeyValuePair kv : arr) {
            if (kv.compareTo(prev) < 0)
                return false;
            prev = kv;
        }

        return true;
    }

    public static boolean isStable(ISort sorter, int size) {
        if (size <= 1) return true;

        KeyValuePair[] arr = new KeyValuePair[size];
        int i;
        for (i = 0; i < size - 2; i++)
            arr[i] = new KeyValuePair(1, i+2);
        arr[i++] = new KeyValuePair(0, 0);
        arr[i] = new KeyValuePair(0, 1);

        sorter.sort(arr);

        int last = -1;
        for (KeyValuePair kv : arr) {
            if (kv.getValue() <= last) return false;
            last = kv.getValue();
        }
        return true;
    }

    public static KeyValuePair[] randomArray(int size) {
        KeyValuePair[] arr = new KeyValuePair[size];
        for (int i = 0; i < size; i++)
            arr[i] = new KeyValuePair(SortingTester.rng.nextInt(0xFFF), i);
        return arr;
    }

    public static KeyValuePair[] oneOffArray(int size) {
        KeyValuePair[] arr = sortedArray(size);
        arr[arr.length/2] = new KeyValuePair(-1, -1);
        return arr;
    }

    public static KeyValuePair[] sameArray(int size) {
        KeyValuePair[] arr = new KeyValuePair[size];
        for (int i = 0; i < size; i++)
            arr[i] = new KeyValuePair(0, i);
        return arr;
    }

    public static KeyValuePair[] sortedArray(int size) {
        KeyValuePair[] arr = new KeyValuePair[size];
        for (int i = 0; i < size; i++)
            arr[i] = new KeyValuePair(i, i);
        return arr;
    }

    public static void main(String[] args) {
        ISort[] sorters = {
                new SorterA(),
                new SorterB(),
                new SorterC(),
                new SorterD(),
                new SorterE(),
                new SorterF(),
        };
        int[] sizes = {0xF, 0xFF, 0xFFF};

        // Checks
        for (int i = 0, s = 'A'; i < sorters.length; i++, s++) {
            ISort sorter = sorters[i];
            System.out.println(STR."Sorter\{(char) s}");

            for (int size : sizes) {
                System.out.println(STR."Size \{size}: \{checkSort(sorter, size) ? "sorted" : "unsorted"} \{isStable(sorter, size) ? "stable" : "unstable"} \{sorter.sort(randomArray(size))}" +
                        STR." (sorted: \{sorter.sort(sortedArray(size))}, one-off: \{sorter.sort(oneOffArray(size))}, same: \{sorter.sort(sameArray(size))})");
            }
        }

        // Analysis:
        // SorterA: MergeSort (stable; similar cost across different array types)
        // SorterB: EvilSort (fails to sort some arrays)
        // SorterC: BubbleSort (stable; grows very fast; better on same and sorted arrays)
        // SorterD: SelectionSort (unstable; grows very fast; similar cost across different array types)
        // SorterE: QuickSort (unstable; faster on same arrays)
        // SorterF: InsertionSort (stable; one-off arrays and same arrays are much faster to sort)
    }
}
