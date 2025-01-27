import java.util.Arrays;

class WiFi {

    /**
     * Implement your solution here
     */
    public static double computeDistance(int[] houses, int numOfAccessPoints) {
        Arrays.sort(houses);

        double low = 0;
        double high = (houses[houses.length-1] / numOfAccessPoints) / 2;

        while (high - low > 0.01) {
            double mid = low + (high - low) / 2.0;

            if (coverable(houses, numOfAccessPoints, mid)) high = mid;
            else low = mid;
        }

        return low;
    }

    /**
     * Implement your solution here
     */
    public static boolean coverable(int[] houses, int numOfAccessPoints, double distance) {
        Arrays.sort(houses);

        int currNum = 1;
        int curr = houses[0];

        for (int house : houses) {
            if (house > curr + distance * 2) {
                currNum++;
                curr = house;
            }
        }

        return currNum <= numOfAccessPoints;
    }
}
