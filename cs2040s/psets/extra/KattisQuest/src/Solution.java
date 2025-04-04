import java.util.TreeSet;

class Quest implements Comparable<Quest> {
    private Long cost;
    private Long reward;

    public Quest(Long energy, Long value) {
        this.cost = energy;
        this.reward = value;
    }

    public Long getReward() {
        return reward;
    }

    public Long getCost() {
        return cost;
    }

    @Override
    public int compareTo(Quest other) {
        int res = cost.compareTo(other.cost);
        if (res == 0) res = reward.compareTo(other.reward);
        return res;
    }

}

public class Solution {
    // TODO: Include your data structures here
    private final TreeSet<Quest> t;

    public Solution() {
        // TODO: Construct/Initialise your data structures here
        this.t = new TreeSet<Quest>();
    }

    void add(long energy, long value) {
        // TODO: Implement your insertion operation here
        t.add(new Quest(energy, value));
    }

    long query(long remainingEnergy) {
        // TODO: Implement your query operation here
        long reward = 0;

        while (remainingEnergy > 0) {
            Quest lowest = t.lower(new Quest(remainingEnergy, 0L));
            if (lowest == null) break;
            reward += lowest.getReward();
            remainingEnergy -= lowest.getCost();
            t.remove(lowest);
        }

        return reward;
    }

    public static void main(String[] args) {
        Solution s = new Solution();
        s.add(8, 10);
        s.add(3, 25);
        s.add(5, 6);
        System.out.println(s.query(7));
        System.out.println(s.query(7));
        s.add(1, 9);
        s.add(2, 13);
        System.out.println(s.query(20));
        System.out.println(s.query(1));
    }

}
