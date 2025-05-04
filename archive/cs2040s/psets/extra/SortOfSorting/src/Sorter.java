class Sorter {

    public static void sortStrings(String[] arr) {
        for (int i = 1; i < arr.length; i++) {
            String key = arr[i];
            int j = i-1;

            while (j >= 0 && isGreaterThan(arr[j], key)) {
                arr[j+1] = arr[j];
                j--;
            }
            arr[j+1] = key;
        }
    }

    public static boolean isGreaterThan(String str1, String str2) {
        int str1End = Math.min(2, str1.length());
        int str2End = Math.min(2, str2.length());
        return str1.substring(0, str1End).compareTo(str2.substring(0, str2End)) > 0;
    }
}
