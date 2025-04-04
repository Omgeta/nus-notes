class InversionCounter {

    public static long countSwaps(int[] arr) {
        if (arr == null || arr.length <= 1) return 0;
        else return mergeSortAndCount(arr, 0, arr.length-1);
    }

    public static long mergeSortAndCount(int[] arr, int left, int right) {
        if (left >= right) return 0;
        int mid = left + (right - left) / 2;
        return mergeSortAndCount(arr, left, mid) + mergeSortAndCount(arr, mid+1, right) + mergeAndCount(arr, left, mid, mid+1, right);
    }

    /**
     * Given an input array so that arr[left1] to arr[right1] is sorted and arr[left2] to arr[right2] is sorted
     * (also left2 = right1 + 1), merges the two so that arr[left1] to arr[right2] is sorted, and returns the
     * minimum amount of adjacent swaps needed to do so.
     */
    public static long mergeAndCount(int[] arr, int left1, int right1, int left2, int right2) {
        int[] tmp = new int[right2 - left1 + 1];
        long count = 0;
        int i = 0;

        while (left1 <= right1 && left2 <= right2) {
            if (arr[left1] <= arr[left2])
                tmp[i++] = arr[left1++];
            else {
                tmp[i++] = arr[left2++];
                count += right1 - left1 + 1;
            }
        }

        while (left1 <= right1) tmp[i++] = arr[left1++];
        while (left2 <= right2) tmp[i++] = arr[left2++];

        for (i = 0; i < tmp.length; i++) {
            arr[right2 - tmp.length + 1 + i] = tmp[i];
        }
        return count;
    }
}
