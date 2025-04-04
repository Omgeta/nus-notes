import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.stream.Stream;

public class MazeGenerator {
	private static final int NORTH = 0, SOUTH = 1, EAST = 2, WEST = 3;
	private static int[][] DELTAS = new int[][] {
			{ -1, 0 }, // North
			{ 1, 0 }, // South
			{ 0, 1 }, // East
			{ 0, -1 } // West
	};
	private MazeGenerator() { }

	public static Maze generateMaze(int rows, int columns) {
		Room[][] rooms = initRooms(rows, columns);
		boolean[][] visited = new boolean[rows][columns];
		Random rng = new Random();

		// Choosing random start point
		int[] curr = { rng.nextInt(rows), rng.nextInt(columns) };
		visited[curr[0]][curr[1]] = true;

		while (true) {
			curr = walk(rooms, curr, visited, rng);
			if (curr == null) curr = hunt(rooms, visited, rng);
			if (curr == null) break;
		}

		return new Maze(rooms);
	}

	private static Room[][] initRooms(int rows, int columns) {
		Room[][] rooms = new Room[rows][columns];
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
				rooms[i][j] = new Room(true, true, true, true);
		return rooms;
	}

	private static int[] walk(Room[][] rooms, int[] start, boolean[][] visited, Random rng) {
		int[] directions = shuffle(new int[]{0, 1, 2, 3}, rng);
		for (int dir : directions) {
			int newRow = start[0] + DELTAS[dir][0];
			int newCol = start[1] + DELTAS[dir][1];
			if (isValid(newRow, newCol, rooms) && !visited[newRow][newCol]) {
				int[] neighbour = { newRow, newCol };
				connect(rooms, start, dir);
				visited[newRow][newCol] = true;
				return neighbour;
			}
		}
		return null;
	}

	private static int[] hunt(Room[][] rooms, boolean[][] visited, Random rng) {
		for (int row = 0; row < rooms.length; row++) {
			for (int col = 0; col < rooms[row].length; col++) {
				if (visited[row][col]) continue;

				List<Integer> neighbourDirs = new ArrayList<>();
				for (int dir = 0; dir < 4; dir++) {
					int nr = row + DELTAS[dir][0];
					int nc = col + DELTAS[dir][1];
					if (isValid(nr, nc, rooms) && visited[nr][nc]) {
						neighbourDirs.add(dir);
					}
				}

				if (!neighbourDirs.isEmpty()) {
					int direction = neighbourDirs.get(rng.nextInt(neighbourDirs.size()));
					int[] room = new int[] {row, col};
					connect(rooms, room, direction);
					visited[row][col] = true;
					return room;
				}
			}
		}
		return null;
	}

	private static int[] shuffle(int[] arr, Random rng) {
		for (int i = arr.length - 1; i > 0; i--)
		{
			int index = rng.nextInt(i + 1);
			int old = arr[index];
			arr[index] = arr[i];
			arr[i] = old;
		}
		return arr;
	}

	private static boolean isValid(int row, int col, Room[][] rooms) {
		return row >= 0 && col >= 0 && row < rooms.length && col < rooms[row].length;
	}

	private static void connect(Room[][] rooms, int[] cell, int direction) {
		int row = cell[0], col = cell[1];
		switch (direction) {
			case NORTH -> {
				rooms[row][col] = openedWall(rooms[row][col], NORTH);
				if (row > 0) rooms[row-1][col] = openedWall(rooms[row-1][col], SOUTH);
			}
			case SOUTH -> {
				rooms[row][col] = openedWall(rooms[row][col], SOUTH);
				if (row < rooms.length-1) rooms[row+1][col] = openedWall(rooms[row+1][col], NORTH);
			}
			case EAST -> {
				rooms[row][col] = openedWall(rooms[row][col], EAST);
				if (col < rooms[row].length-1) rooms[row][col+1] = openedWall(rooms[row][col+1], WEST);
			}
			case WEST -> {
				rooms[row][col] = openedWall(rooms[row][col], WEST);
				if (col > 0) rooms[row][col-1] = openedWall(rooms[row][col-1], EAST);
			}
		}
	}

	private static Room openedWall(Room room, int direction) {
		switch (direction) {
			case NORTH -> {
				return new Room(false, room.hasSouthWall(), room.hasEastWall(), room.hasWestWall());
			}
			case SOUTH -> {
				return new Room(room.hasNorthWall(), false, room.hasEastWall(), room.hasWestWall());
			}
			case EAST -> {
				return new Room(room.hasNorthWall(), room.hasSouthWall(), false, room.hasWestWall());
			}
			case WEST -> {
				return new Room(room.hasNorthWall(), room.hasSouthWall(), room.hasEastWall(), false);
			}
			default -> {
				return room;
			}
		}
	}

	public static void main(String[] args) {
		// Do remember to remove any references to ImprovedMazePrinter before submitting
		// your code!
		try {
			Maze maze = MazeGenerator.generateMaze(20, 40);
			IMazeSolver solver = new MazeSolver();
			solver.initialize(maze);

			System.out.println(solver.pathSearch(0, 0, 19, 39));
			MazePrinter.printMaze(maze);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
