/**
 * Contains static routines for solving the problem of balancing m jobs on p processors
 * with the constraint that each processor can only perform consecutive jobs.
 */
public class LoadBalancing {

    /**
     * Checks if it is possible to assign the specified jobs to the specified number of processors such that no
     * processor's load is higher than the specified query load.
     *
     * @param jobSizes the sizes of the jobs to be performed
     * @param queryLoad the maximum load allowed for any processor
     * @param p the number of processors
     * @return true iff it is possible to assign the jobs to p processors so that no processor has more than queryLoad load.
     */
    // Time complexity is O(n)
    public static boolean isFeasibleLoad(int[] jobSizes, int queryLoad, int p) {
        if (jobSizes.length == 0) return false;

        int currLoad = 0;
        int i;
        for (i = 0; i < jobSizes.length && p > 0; i++) {
            if (currLoad + jobSizes[i] <= queryLoad) {
                currLoad += jobSizes[i];
            } else {
                currLoad = 0;
                i--;
                p--;
            }
        }
        return i == jobSizes.length;
    }

    /**
     * Returns the minimum achievable load given the specified jobs and number of processors.
     *
     * @param jobSizes the sizes of the jobs to be performed
     * @param p the number of processors
     * @return the maximum load for a job assignment that minimizes the maximum load
     */
    // Time complexity of the binary search is O(log(nM))
    // Time complexity of each call to isFeasibleLoad is O(n)
    // Therefore, total time complexity is O(n log(nM))
    public static int findLoad(int[] jobSizes, int p) {
        if (jobSizes.length == 0 || p <= 0) return -1;

        int left = 0;
        int right = 0;
        for (int jobSize : jobSizes) {
            left = Math.max(left, jobSize);
            right += jobSize;
        }

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (LoadBalancing.isFeasibleLoad(jobSizes, mid, p)) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }

        return left;
    }

    // These are some arbitrary testcases.
    public static int[][] testCases = {
            {1, 3, 5, 7, 9, 11, 10, 8, 6, 4},
            {67, 65, 43, 42, 23, 17, 9, 100},
            {4, 100, 80, 15, 20, 25, 30},
            {2, 3, 4, 5, 6, 7, 8, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83},
            {7}
    };

    /**
     * Some simple tests for the findLoad routine.
     */
    public static void main(String[] args) {
        for (int p = 1; p < 30; p++) {
            System.out.println("Processors: " + p);
            for (int[] testCase : testCases) {
                System.out.println(findLoad(testCase, p));
            }
        }
    }
}
