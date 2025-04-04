/**
 * Game Tree class
 *
 * This class contains the basic code for implementing a Game Tree. It includes functionality to load a game tree.
 */

public class GameTree {

    // Standard enumerations for tic-tac-toe variations
    public enum Player {ONE, TWO}

    public enum Game {Regular, Misere, NoTie, Arbitrary}

    // Some constants for tic-tac-toe
    final static int bsize = 3;
    final static int btotal = bsize * bsize;
    final static char EMPTYCHAR = '_';

    // Specifies the values for the arbitrary variant
    final int[] valArray = {1, -2, 1, -3, -2, 2, -2, -1, 1};

    /**
     * Simple TreeNode class.
     *
     * This class holds the data for a node in a game tree.
     *
     * Note: we have made things public here to facilitate problem set grading/testing.
     * In general, making everything public like this is a bad idea!
     *
     */
    static class TreeNode {
        public String name = "";
        public TreeNode[] children = null;
        public int numChildren = 0;
        public int value = Integer.MIN_VALUE;
        public boolean leaf = false;

        // Empty constructor for building an empty node
        TreeNode() {}

        // Simple constructor for build a node with a name and a leaf specification
        TreeNode(String s, boolean l) {
            name = s;
            leaf = l;
            children = new TreeNode[btotal];
            for (int i = 0; i < btotal; i++) {
                children[i] = null;
            }
            numChildren = 0;
        }
    }

    // Root of the tree - public to facilitate grading
    public TreeNode root = null;

    //--------------
    // Utility Functions
    //--------------

    // Simple function for determining the other player
    private Player other(Player p) {
        if (p == Player.ONE) return Player.TWO;
        else return Player.ONE;
    }

    // Draws a board using the game state stored as the name of the node.
    public void drawBoard(String s) {
        System.out.println("-------");
        for (int j = 0; j < bsize; j++) {
            System.out.print("|");
            for (int i = 0; i < bsize; i++) {
                char c = s.charAt(i + 3 * j);
                if (c != EMPTYCHAR)
                    System.out.print(c);
                else System.out.print(" ");
                System.out.print("|");
            }
            System.out.println();
            System.out.println("-------");
        }
    }

    /**
     * readTree reads a game tree from the specified file. If the file does not exist, cannot be found, or there is
     * otherwise an error opening or reading the file, it prints out "Error reading file" along with additional error
     * information.
     *
     * @param fName name of file to read from
     */
    public void readTree(String fName) {
        try {
            java.io.BufferedReader reader = new java.io.BufferedReader(new java.io.FileReader(fName));
            root = readTree(reader);
        } catch (java.io.IOException e) {
            System.out.println("Error reading file: " + e);
        }
    }

    // Helper function: reads a game tree from the specified reader
    private TreeNode readTree(java.io.BufferedReader reader) throws java.io.IOException {
        String s = reader.readLine();
        if (s == null) {
            throw new java.io.IOException("File ended too soon.");
        }
        TreeNode node = new TreeNode();
        node.numChildren = Integer.parseInt(s.substring(0, 1));
        node.children = new TreeNode[node.numChildren];
        node.leaf = (s.charAt(1) == '1');
        node.value = Integer.MIN_VALUE;
        if (node.leaf) {
            char v = s.charAt(2);
            node.value = Character.getNumericValue(v);
            node.value--;
        }
        node.name = s.substring(3);

        for (int i = 0; i < node.numChildren; i++) {
            node.children[i] = readTree(reader);
        }
        return node;
    }

    /**
     * findValue determines the value for every node in the game tree and sets the value field of each node. If the root
     * is null (i.e., no tree exists), then it returns Integer.MIN_VALUE.
     *
     * @return value of the root node of the game tree
     */
    int findValue() {
        if (this.root == null) return Integer.MIN_VALUE;
        return findValueHelper(this.root, Player.ONE);
    }

    private static int findValueHelper(TreeNode node, Player p) {
        if (node.leaf) return node.value;

        if (p == Player.ONE) {
            node.value = Integer.MIN_VALUE;
            for (TreeNode child : node.children)
                node.value = Math.max(node.value, findValueHelper(child, Player.TWO));
        } else {
            node.value = Integer.MAX_VALUE;
            for (TreeNode child : node.children)
                node.value = Math.min(node.value, findValueHelper(child, Player.ONE));
        }

        return node.value;
    }


    // Simple main for testing purposes
    public static void main(String[] args) {
        GameTree tree = new GameTree();
        tree.readTree("games/tictac_3_empty_2.txt");
        System.out.println(tree.findValue());
    }

}
