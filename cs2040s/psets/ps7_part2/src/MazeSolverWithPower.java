import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;
public class MazeSolverWithPower implements IMazeSolverWithPower {
	private static final int NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3;
	private static int[][] DELTAS = new int[][] {
			{ -1, 0 }, // North
			{ 1, 0 }, // South
			{ 0, 1 }, // East
			{ 0, -1 } // West
	};
	private Maze maze;
	private boolean solved;
	private int endRow, endCol;
	private boolean[][][] visitedSp;
	// Tracks the earliest step a room was reached
	private boolean[][] visited;
	// List holding number of reachable nodes at each distance
	private List<Integer> reachable;

	public MazeSolverWithPower() {
		this.maze = null;
		this.solved = false;
	}

	@Override
	public void initialize(Maze maze) {
		this.maze = maze;
		this.reachable = new ArrayList<>();
		this.solved = false;
	}

	/**
	 * BFS Search for End, tracking number of rooms reachable at each step
	 * @param startRow Starting room row
	 * @param startCol Starting room column
	 * @param endRow Ending room row
	 * @param endCol Ending room column
	 * @return Steps to end
	 * */
	@Override
	public Integer pathSearch(int startRow, int startCol, int endRow, int endCol) throws Exception {
		return pathSearch(startRow, startCol, endRow, endCol, 0);
	}

	/**
	 * BFS Search for End, tracking number of rooms reachable at each step
	 * with superpowers to break walls
	 * @param startRow Starting room row
	 * @param startCol Starting room column
	 * @param endRow Ending room row
	 * @param endCol Ending room column
	 * @param superpowers Number of superpowers
	 * @return Steps to end
	 * */
	@Override
	public Integer pathSearch(int startRow, int startCol, int endRow, int endCol, int superpowers) throws Exception {
		if (maze == null) {
			throw new Exception("Maze not initialized");
		}
		if (!validCoord(startRow, startCol) || !validCoord(endRow, endCol)) {
			throw new IllegalArgumentException("Invalid coordinates");
		}

		// initialize visitedSp for required superpowers
		this.visited = new boolean[maze.getRows()][maze.getColumns()];
		this.visitedSp = new boolean[maze.getRows()][maze.getColumns()][superpowers + 1];
		for (int i = 0; i < maze.getRows(); i++) {
			for (int j = 0; j < maze.getColumns(); j++) {
				maze.getRoom(i, j).onPath = false;
			}
		}
		this.endRow = endRow;
		this.endCol = endCol;
		this.reachable.clear();
		this.solved = false;

		// create queue to track unvisited rooms
		Queue<Node> queue = new LinkedList<>();

		// add starting room to queue, and mark it as visited
		queue.add(new Node(startRow, startCol, null, superpowers));
		this.visitedSp[startRow][startCol][superpowers] = true;
		this.visited[startRow][startCol] = true;
		this.reachable.add(1);

		int distance = 0;
		Integer result = null;

		// while there are still rooms unexplored
		while (!queue.isEmpty()) {
			int numNodes = queue.size();
			int newlyExplored = 0;

			// go through nodes in current step count
			for (int i = 0; i < numNodes; i++) {
				Node curr = queue.poll();

				// if end found the for the first time, backtrack from it
				assert curr != null;
				if (curr.row == endRow && curr.col == endCol && !this.solved) {
					result = distance;
					backtrackPath(curr);
					this.solved = true;
				}
				newlyExplored += exploreNeighbours(queue, curr, distance);
			}

			reachable.add(newlyExplored);
			distance++;
		}
		return result;
	}

	/**
	 * Check if room coords in bounds
	 * @param row Row of room
	 * @param col Column of room
	 * @return If room in bounds
	 * */
	private boolean validCoord(int row, int col) {
		return row >= 0 && row < maze.getRows() && col >= 0 && col < maze.getColumns();
	}

	/**
	 * Backtrack all discovered Nodes, setting the onPath to true
	 * @param curr Starting Node to backtrack from
	 * */
	private void backtrackPath(Node curr) {
		while (curr != null) {
			maze.getRoom(curr.row, curr.col).onPath = true;
			curr = curr.parent;
		}
	}
	
	/**
	 * Explore adjacent unvisited rooms
	 * @param queue Queue holding current Nodes to visit
	 * @param curr Current room to explore around
	 * @return Number of new nodes explored
	 * */
	private int exploreNeighbours(Queue<Node> queue, Node curr, int distance) {
		int newlyExplored = 0;
		// check all adjacent rooms
		for (int dir = 0; dir < 4; dir++) {
			int newRow = curr.row + DELTAS[dir][0];
			int newCol = curr.col + DELTAS[dir][1];
			
			// if adjacent room out of bounds skip it
			if (!validCoord(newRow, newCol)) continue;
			
			// check if currently able to move in direction
			boolean canMove = canGo(curr.row, curr.col, dir);
			int cost = canMove ? 0 : 1;
			int remaining = curr.superpowers - cost;
			
			// check Room unvisited with (valid) new superpower count
			if (remaining >= 0 && !this.visitedSp[newRow][newCol][remaining]) {
				this.visitedSp[newRow][newCol][remaining] = true;
				if (!this.visited[newRow][newCol]) {
					this.visited[newRow][newCol] = true;
					newlyExplored++;
				}
				queue.add(new Node(newRow, newCol, curr, remaining));
			}
		}
		return newlyExplored;
	}
	
	/**
	 * Check if there is no wall between starting location and Node at direction
	 * @param row Row of starting location
	 * @param col Column of starting location
	 * @param dir Direction to check wall
	 * @return If there is no wall
	 * */
	private boolean canGo(int row, int col, int dir) {
		// not needed since our maze has a surrounding block of wall
		// but Joe the Average Coder is a defensive coder!
		if (row + DELTAS[dir][0] < 0 || row + DELTAS[dir][0] >= maze.getRows()) return false;
		if (col + DELTAS[dir][1] < 0 || col + DELTAS[dir][1] >= maze.getColumns()) return false;
		switch (dir) {
			case NORTH: return !maze.getRoom(row, col).hasNorthWall();
			case SOUTH: return !maze.getRoom(row, col).hasSouthWall();
			case EAST: return !maze.getRoom(row, col).hasEastWall();
			case WEST: return !maze.getRoom(row, col).hasWestWall();
			default: return false;
		}
	}
	
	/**
	 * Find number of rooms reachable with k steps
	 * @param k steps to take
	 * @return number of rooms reachable with k steps
	 * */
	@Override
	public Integer numReachable(int k) {
		if (k < 0 || k >= reachable.size()) return 0;
		return reachable.get(k);
	}
	
	// Helper Node class to store coord of each position explored in Queue
	private static class Node {
		int row, col, superpowers;
		Node parent;
		public Node(int row, int col, Node parent, int superpowers) {
			this.row = row;
			this.col = col;
			this.parent = parent;
			this.superpowers = superpowers;
		}
	}

	public static void main(String[] args) {
		try {
			Maze maze = Maze.readMaze("maze-sample.txt");
			IMazeSolverWithPower solver = new MazeSolverWithPower();
			solver.initialize(maze);
			System.out.println(solver.pathSearch(0, 0, 4, 4, 1));
			MazePrinter.printMaze(maze);
			for (int i = 0; i <= 9; ++i) {
				System.out.println("Steps " + i + " Rooms: " + solver.numReachable(i));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}