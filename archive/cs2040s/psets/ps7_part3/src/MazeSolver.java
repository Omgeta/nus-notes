import java.util.Arrays;
import java.util.List;
import java.util.Queue;
import java.util.function.Function;
import java.util.PriorityQueue;
import java.util.Queue;

public class MazeSolver implements IMazeSolver {
	private static final int TRUE_WALL = Integer.MAX_VALUE;
	private static final int EMPTY_SPACE = 0;
	private static final List<Function<Room, Integer>> WALL_FUNCTIONS = Arrays.asList(
			Room::getNorthWall,
			Room::getEastWall,
			Room::getWestWall,
			Room::getSouthWall
	);
	private static final int[][] DELTAS = new int[][] {
			{ -1, 0 }, // North
			{ 0, 1 }, // East
			{ 0, -1 }, // West
			{ 1, 0 } // South
	};

	private Maze maze;
	private int dist[][];
	// 3-d distance array for different determination states (same logic as part 2)
	private int distDetermination[][][];

	public MazeSolver() {
		this.maze = null;
	}

	@Override
	public void initialize(Maze maze) {
		this.maze = maze;
		this.dist = new int[maze.getRows()][maze.getColumns()];
		this.distDetermination = new int[maze.getRows()][maze.getColumns()][2];
	}

	/**
	 * Find path from start to end with minimum fear score
	 * @param startRow the row index of the starting coordinate
	 * @param startCol the column index of the starting coordinate
	 * @param endRow the row index of the target coordinate
	 * @param endCol the column index of the target coordinate
	 * @return Minimum fear score to reach the end
	 */
	@Override
	public Integer pathSearch(int startRow, int startCol, int endRow, int endCol) throws Exception {
		if (maze == null) {
			throw new Exception("Maze not initialized");
		}
		if (!validCoord(startRow, startCol) || !validCoord(endRow, endCol)) {
			throw new IllegalArgumentException("Invalid coordinates");
		}

		// reset structures
		for (int i = 0; i < maze.getRows(); i++) {
			for (int j = 0; j < maze.getColumns(); j++) {
				this.dist[i][j] = Integer.MAX_VALUE;
			}
		}
		// create pq to store unvisited nodes in order of fear
		Queue<Node> queue = new PriorityQueue<>();

		// add starting room to queue, and mark as visited at fear 0
		queue.add(new Node(startRow, startCol, 0));
		this.dist[startRow][startCol] = 0;

		// while there are rooms still unexplored
		while (!queue.isEmpty()) {
			Node curr = queue.poll();

			// if current node is end, return fear taken to reach that node
			if (curr.row == endRow && curr.col == endCol) {
				return curr.fear;
			}

			if (curr.fear > dist[curr.row][curr.col]) continue;

			// else, explore reachable neighbours and add them to the queue
			// with the fear required to reach them
			exploreNeighbours(queue, curr);
		}

		return null;
	}

	/**
	 * Explore adjacent unvisited rooms and add them to queue
	 * @param queue Queue holding current Nodes to visit
	 * @param curr Current room to explore around
	 **/
	private void exploreNeighbours(Queue<Node> queue, Node curr) {
		// check all adjacent rooms
		for (int dir = 0; dir < 4; dir++) {
			int newRow = curr.row + DELTAS[dir][0];
			int newCol = curr.col + DELTAS[dir][1];

			// if adjacent room out of bounds skip it
			if (!validCoord(newRow, newCol)) continue;

			// calculate wall spirit scariness
			Function<Room, Integer> wallFunc = MazeSolver.WALL_FUNCTIONS.get(dir);
			int scariness = wallFunc.apply(this.maze.getRoom(curr.row, curr.col));

			// if solid wall skip it
			if (scariness == MazeSolver.TRUE_WALL) continue;

			// calculate new fear
			int fearDelta = scariness == MazeSolver.EMPTY_SPACE ? 1 : scariness;
			int newFear = curr.fear + fearDelta;

			// check Room if unvisited with new fear score
			if (newFear < this.dist[newRow][newCol]) {
				this.dist[newRow][newCol] = newFear;
				queue.add(new Node(newRow, newCol, newFear));
			}
		}
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
	 * Find path from start to end with minimum fear score
	 * @param startRow the row index of the starting coordinate
	 * @param startCol the column index of the starting coordinate
	 * @param endRow the row index of the target coordinate
	 * @param endCol the column index of the target coordinate
	 * @return Minimum fear score to reach the end
	 */
	@Override
	public Integer bonusSearch(int startRow, int startCol, int endRow, int endCol) throws Exception {
		if (maze == null) {
			throw new Exception("Maze not initialized");
		}
		if (!validCoord(startRow, startCol) || !validCoord(endRow, endCol)) {
			throw new IllegalArgumentException("Invalid coordinates");
		}

		// reset structures
		for (int i = 0; i < maze.getRows(); i++) {
			for (int j = 0; j < maze.getColumns(); j++) {
				this.dist[i][j] = Integer.MAX_VALUE;
			}
		}
		// create pq to store unvisited nodes in order of fear
		Queue<Node> queue = new PriorityQueue<>();

		// add starting room to queue, and mark as visited at fear 0
		queue.add(new Node(startRow, startCol, 0));
		this.dist[startRow][startCol] = 0;

		// while there are rooms still unexplored
		while (!queue.isEmpty()) {
			Node curr = queue.poll();

			// if current node is end, return fear taken to reach that node
			if (curr.row == endRow && curr.col == endCol) {
				return curr.fear;
			}

			if (curr.fear > dist[curr.row][curr.col]) continue;

			// else, explore reachable neighbours and add them to the queue
			// with the fear required to reach them
			exploreNeighboursBonusA(queue, curr);
		}

		return null;
	}

	/**
	 * Explore adjacent unvisited rooms and add them to queue
	 * @param queue Queue holding current Nodes to visit
	 * @param curr Current room to explore around
	 **/
	private void exploreNeighboursBonusA(Queue<Node> queue, Node curr) {
		// check all adjacent rooms
		for (int dir = 0; dir < 4; dir++) {
			int newRow = curr.row + DELTAS[dir][0];
			int newCol = curr.col + DELTAS[dir][1];

			// if adjacent room out of bounds skip it
			if (!validCoord(newRow, newCol)) continue;

			// calculate wall spirit scariness
			Function<Room, Integer> wallFunc = MazeSolver.WALL_FUNCTIONS.get(dir);
			int scariness = wallFunc.apply(this.maze.getRoom(curr.row, curr.col));

			// if solid wall skip it
			if (scariness == MazeSolver.TRUE_WALL) continue;

			// calculate new fear, increment fear for empty spaces
			// else if going through wall spirit, take maximum fear between current and wall scariness
			int newFear = scariness == MazeSolver.EMPTY_SPACE ?
					curr.fear + 1 :
					Math.max(scariness, curr.fear);

			// check Room if unvisited with new fear score
			if (newFear < this.dist[newRow][newCol]) {
				this.dist[newRow][newCol] = newFear;
				queue.add(new Node(newRow, newCol, newFear));
			}
		}
	}

	/**
	 * Find path from start to end with minimum fear score, with a special coordinate which sets fear to -1
	 * @param startRow the row index of the starting coordinate
	 * @param startCol the column index of the starting coordinate
	 * @param endRow the row index of the target coordinate
	 * @param endCol the column index of the target coordinate
	 * @param sRow the row index of the special coordinate
	 * @param sCol the column index of the special coordinate
	 * @return Minimum fear score to reach the end
	 */
	@Override
	public Integer bonusSearch(int startRow, int startCol, int endRow, int endCol, int sRow, int sCol) throws Exception {
		if (maze == null) {
			throw new Exception("Maze not initialized");
		}
		if (!validCoord(startRow, startCol) || !validCoord(endRow, endCol)) {
			throw new IllegalArgumentException("Invalid coordinates");
		}

		// reset structures
		for (int i = 0; i < maze.getRows(); i++) {
			for (int j = 0; j < maze.getColumns(); j++) {
				for (int k = 0; k < 2; k++) {
					this.distDetermination[i][j][k] = Integer.MAX_VALUE;
				}
			}
		}
		// create pq to store unvisited nodes in order of fear
		Queue<Node> queue = new PriorityQueue<>();

		// add starting room to queue, and mark as visited at fear 0
		queue.add(new Node(startRow, startCol, 0, 0));
		this.distDetermination[startRow][startCol][0] = 0;

		// while there are rooms still unexplored
		while (!queue.isEmpty()) {
			Node curr = queue.poll();

			if (curr.fear > distDetermination[curr.row][curr.col][curr.determination]) continue;

			// if reached special room, we revisit current Room but at a different determination level
			// so that we can now rexplore the maze with the new starting fear at -1
			if (curr.row == sRow && curr.col == sCol && curr.determination == 0) {
				if (-1 < distDetermination[curr.row][curr.col][1]) {
					distDetermination[curr.row][curr.col][1] = -1;
					queue.add(new Node(curr.row, curr.col, -1, 1));
				}
			}

			// else, explore reachable neighbours and add them to the queue
			// with the fear required to reach them
			exploreNeighboursBonusB(queue, curr);
		}

		// only return result after doing a complete search
		// find the minimum between fear used to reach rooms at different determination levels
		int fear = Math.min(distDetermination[endRow][endCol][0], distDetermination[endRow][endCol][1]);
		return fear == Integer.MAX_VALUE ? null : fear;
	}

	/**
	 * Explore adjacent unvisited rooms and add them to queue
	 * @param queue Queue holding current Nodes to visit
	 * @param curr Current room to explore around
	 **/
	private void exploreNeighboursBonusB(Queue<Node> queue, Node curr) {
		// check all adjacent rooms
		for (int dir = 0; dir < 4; dir++) {
			int newRow = curr.row + DELTAS[dir][0];
			int newCol = curr.col + DELTAS[dir][1];

			// if adjacent room out of bounds skip it
			if (!validCoord(newRow, newCol)) continue;

			// calculate wall spirit scariness
			Function<Room, Integer> wallFunc = MazeSolver.WALL_FUNCTIONS.get(dir);
			int scariness = wallFunc.apply(this.maze.getRoom(curr.row, curr.col));

			// if solid wall skip it
			if (scariness == MazeSolver.TRUE_WALL) continue;

			// calculate new fear, increment fear for empty spaces
			// else if going through wall spirit, take maximum fear between current and wall scariness
			int newFear = scariness == MazeSolver.EMPTY_SPACE ?
					curr.fear + 1 :
					Math.max(scariness, curr.fear);

			// check Room if unvisited with new fear score
			if (newFear < this.distDetermination[newRow][newCol][curr.determination]) {
				this.distDetermination[newRow][newCol][curr.determination] = newFear;
				queue.add(new Node(newRow, newCol, newFear, curr.determination));
			}
		}
	}

	public static void main(String[] args) {
		try {
			Maze maze = Maze.readMaze("maze-empty.txt");
			IMazeSolver solver = new MazeSolver();
			solver.initialize(maze);

			System.out.println(solver.pathSearch(0, 0, 0, 3));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static class Node implements Comparable<Node> {
		private final int row, col, fear, determination;

		public Node(int row, int col, int fear) {
			this(row, col, fear, 0);
		}

		public Node(int row, int col, int fear, int determination) {
			this.row = row;
			this.col = col;
			this.fear = fear;
			this.determination = determination;
		}

		@Override
		public int compareTo(Node o) {
			return Integer.compare(this.fear, o.fear);
		}

	}
}
