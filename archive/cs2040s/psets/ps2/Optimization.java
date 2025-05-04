/**
 * The Optimization class contains a static routine to find the maximum in an array that changes direction at most once.
 */
public class Optimization {

    /**
     * A set of test cases.
     */
    static int[][] testCases = {
            {1, 3, 5, 7, 9, 11, 10, 8, 6, 4},
            {67, 65, 43, 42, 23, 17, 9, 100},
            {4, -100, -80, 15, 20, 25, 30},
            {2, 3, 4, 5, 6, 7, 8, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83}
    };

    /**
     * Returns the maximum item in the specified array of integers which changes direction at most once.
     *
     * @param dataArray an array of integers which changes direction at most once.
     * @return the maximum item in data Array
     */
    // Discussed with Lee Shu Xian but code written independently
    // Time complexity is O(log n)
    public static int searchMax(int[] dataArray) {
        if (dataArray.length == 0) {
            return 0;
        } else if (dataArray.length == 1) {
            return dataArray[0];
        } else if (dataArray.length == 2 || dataArray[0] > dataArray[1]) {
            // if decreasing initially
            return Math.max(dataArray[0], dataArray[dataArray.length - 1]);
        } else {
            // if increasing initially
            int left = 0;
            int right = dataArray.length - 1;

            while (left < right) {
                int mid = left + (right - left) / 2;

                if (dataArray[mid] > dataArray[mid + 1]) {
                    right = mid;
                } else {
                    left = mid + 1;
                }
            }

            return dataArray[left];
        }
    }

    /**
     * A routine to test the searchMax routine.
     */
    public static void main(String[] args) {
        for (int[] testCase : testCases) {
            System.out.println(searchMax(testCase));
        }
    }
}
