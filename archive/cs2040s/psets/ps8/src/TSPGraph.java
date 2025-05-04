import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;

public class TSPGraph implements IApproximateTSP {
    @Override
    public void MST(TSPMap map) {
        int n = map.getCount();
        HashSet<Integer> visited = new HashSet<>();
        visited.add(0);

        // for n-1 edges
        for (int e = 0; e < n - 1; e++) {
            // keep track of closest unvisited node
            double minDist = Double.MAX_VALUE;
            Integer from = null, to = null;

            // for all visited nodes
            for (Integer i : visited) {
                // find disconnected node with minimum distance
                for (int j = 0; j < n; j++) {
                    if (visited.contains(j)) continue;

                    double dist = map.pointDistance(i, j);
                    if (dist < minDist) {
                        minDist = dist;
                        from = j;
                        to = i;
                    }
                }
            }

            // there will always be atleast one disconnected node
            assert from != null;
            map.setLink(from, to, false);
            visited.add(from);
        }

        map.redraw();
    }


    @Override
    public void TSP(TSPMap map) {
        MST(map);

        int n = map.getCount();

        // create adjacency list for each point index
        HashMap<Integer, ArrayList<Integer>> adj = new HashMap<>();
        for (int i = 0; i < n; i++) adj.put(i, new ArrayList<>());
        for (int i = 1; i < n; i++) adj.get(map.getLink(i)).add(i);

        // dfs in O(v)
        ArrayList<Integer> tourOrder = new ArrayList<>();
        boolean[] visited = new boolean[n];
        dfs(0, adj, visited, tourOrder);

        // shortcut in O(v)
        for (int i = 0; i < tourOrder.size() - 1; i++)
            map.setLink(tourOrder.get(i), tourOrder.get(i + 1), false);
        map.setLink(tourOrder.getLast(), tourOrder.getFirst(), false);

        map.redraw();
    }

    private void dfs(int current, HashMap<Integer, ArrayList<Integer>> adj, boolean[] visited, ArrayList<Integer> order) {
        visited[current] = true;
        order.add(current);
        for (Integer child : adj.get(current)) {
            if (!visited[child]) {
                dfs(child, adj, visited, order);
            }
        }
    }

    @Override
    public boolean isValidTour(TSPMap map) {
        HashSet<Integer> visited = new HashSet<>();
        int curr = 0, n = 0;

        // follow all links starting from the origin,
        // ending when cannot go anymore, or when we re-reach the same visited node
        while (curr != -1 && !visited.contains(curr)) {
            visited.add(curr);
            curr = map.getLink(curr);
            n++;
        }
        return curr == 0 && // returned to origin node
                map.getCount() == n; // never visited more than twice
    }

    @Override
    public double tourDistance(TSPMap map) {
        if (!isValidTour(map)) return -1;

        double dist = map.pointDistance(0, map.getLink(0));
        int curr = map.getLink(0);

        // stop when reached origin again
        while (curr != 0) {
            dist += map.pointDistance(curr, map.getLink(curr));
            curr = map.getLink(curr);
        }
        return dist;
    }

    public static void main(String[] args) {
        TSPMap map = new TSPMap(args.length > 0 ? args[0] : "hundredpoints.txt");
        TSPGraph graph = new TSPGraph();

        graph.MST(map);
        graph.TSP(map);
        System.out.println(graph.isValidTour(map));
        // System.out.println(graph.tourDistance(map));
    }
}
