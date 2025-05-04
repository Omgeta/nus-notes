import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class MazeSolver implements IMazeSolver {
	private static final int NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3;
	private static int[][] DELTAS = new int[][] {
		{ -1, 0 }, // North
		{ 1, 0 }, // South
		{ 0, 1 }, // East
		{ 0, -1 } // West
	};
	private Maze maze;
	private boolean solved;
	private boolean[][] visited;
	private int endRow, endCol;
	private List<Integer> reachable;


	public MazeSolver() {
		this.maze = null;
		this.solved = false;
	}

	@Override
	public void initialize(Maze maze) {
		this.maze = maze;
		this.visited = new boolean[maze.getRows()][maze.getColumns()];
		this.reachable = new ArrayList<>();
		this.solved = false;
	}

	@Override
	public Integer pathSearch(int startRow, int startCol, int endRow, int endCol) throws Exception {
		if (maze == null) {
			throw new Exception("Oh no! You cannot call me without initializing the maze!");
		}

		if (startRow < 0 || startCol < 0 || startRow >= maze.getRows() || startCol >= maze.getColumns() ||
				endRow < 0 || endCol < 0 || endRow >= maze.getRows() || endCol >= maze.getColumns()) {
			throw new IllegalArgumentException("Invalid start/end coordinate");
		}

		// set all visited flag to false
		// before we begin our search
		for (int i = 0; i < maze.getRows(); ++i) {
			for (int j = 0; j < maze.getColumns(); ++j) {
				this.visited[i][j] = false;
				maze.getRoom(i, j).onPath = false;
			}
		}

		this.endRow = endRow;
		this.endCol = endCol;
		Queue<Node> queue = new LinkedList<>();
		queue.add(new Node(startRow, startCol, null));
		reachable.clear();
		int distance = 0;
		Integer res = null;

		while (!queue.isEmpty()) {
			int numNodes = queue.size();
			this.reachable.add(numNodes);

			for (int i = 0; i < numNodes; i++) {
				Node next = queue.poll();

                assert next != null;
                if (next.row == this.endRow && next.col == this.endCol) {
					res = distance;
					backtrackPath(next);
					this.solved = true;
				}

				exploreNeighbours(queue, next);
			}

			distance++;
		}

		return res;
	}

	private void exploreNeighbours(Queue<Node> queue, Node curr) {
		this.visited[curr.row][curr.col] = true;

		for (int direction = 0; direction < 4; direction++) {
			if (canGo(curr.row, curr.col, direction)) {
				int newRow = curr.row + DELTAS[direction][0];
				int newCol = curr.col + DELTAS[direction][1];

				if (!this.visited[newRow][newCol]) {
					this.visited[newRow][newCol] = true;
					queue.add(new Node(newRow, newCol, curr));
				}
			}
		}
	}

	private int backtrackPath(Node curr) {
		int i = -1;
		while (curr != null) {
			maze.getRoom(curr.row, curr.col).onPath = true;
			curr = curr.parent;
			i++;
		}
		return i;
	}

	private boolean canGo(int row, int col, int dir) {
		// not needed since our maze has a surrounding block of wall
		// but Joe the Average Coder is a defensive coder!
		if (row + DELTAS[dir][0] < 0 || row + DELTAS[dir][0] >= maze.getRows()) return false;
		if (col + DELTAS[dir][1] < 0 || col + DELTAS[dir][1] >= maze.getColumns()) return false;

		switch (dir) {
			case NORTH:
				return !maze.getRoom(row, col).hasNorthWall();
			case SOUTH:
				return !maze.getRoom(row, col).hasSouthWall();
			case EAST:
				return !maze.getRoom(row, col).hasEastWall();
			case WEST:
				return !maze.getRoom(row, col).hasWestWall();
		}

		return false;
	}


	@Override
	public Integer numReachable(int k) throws Exception {
		return (k >= 0 && k < reachable.size()) ? reachable.get(k) : 0;
	}

	public static void main(String[] args) {
		// Do remember to remove any references to ImprovedMazePrinter before submitting
		// your code!
		try {
			Maze maze = Maze.readMaze("maze-sample.txt");
			IMazeSolver solver = new MazeSolver();
			solver.initialize(maze);

			System.out.println(solver.pathSearch(0, 0, 4, 3));
			ImprovedMazePrinter.printMaze(maze, 0, 0);

			for (int i = 0; i <= 9; ++i) {
				System.out.println("Steps " + i + " Rooms: " + solver.numReachable(i));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static class Node {
		private int row, col;
		private Node parent;
		public Node(int row, int col, Node parent) {
			this.row = row;
			this.col = col;
			this.parent = parent;
		}
	}
}
