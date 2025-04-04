import com.sun.source.tree.Tree;

/**
 * Scapegoat Tree class
 *
 * This class contains an implementation of a Scapegoat tree.
 */

public class SGTree {
    /**
     * TreeNode class.
     *
     * This class holds the data for a node in a binary tree.
     *
     * Note: we have made things public here to facilitate problem set grading/testing.
     * In general, making everything public like this is a bad idea!
     *
     */
    public static class TreeNode {
        int key;
        public TreeNode left = null;
        public TreeNode right = null;
        public TreeNode parent = null;
        public int weight = 1;

        TreeNode(int k) {
            key = k;
        }
    }

    // Root of the binary tree
    public TreeNode root = null;

    /**
     * Counts the number of nodes in the subtree rooted at node
     *
     * @param node the root of the subtree
     * @return number of nodes
     */
    public int countNodes(TreeNode node) {
        if (node == null) return 0;
        else return countNodes(node.left) + countNodes(node.right) + 1;
    }

    /**
     * Builds an array of nodes in the subtree rooted at node
     *
     * @param node the root of the subtree
     * @return array of nodes
     */
    public TreeNode[] enumerateNodes(TreeNode node) {
        if (node == null) return new TreeNode[0];
        TreeNode[] nodes = new TreeNode[countNodes(node)];
        enumerateNodesHelper(node, nodes, 0);
        return nodes;
    }

    private int enumerateNodesHelper(TreeNode node, TreeNode[] nodes, int i) {
        if (node.left != null) i = enumerateNodesHelper(node.left, nodes, i);
        nodes[i++] = node;
        if (node.right != null) i = enumerateNodesHelper(node.right, nodes, i);
        return i;
    }

    /**
     * Builds a tree from the list of nodes
     * Returns the node that is the new root of the subtree
     *
     * @param nodeList ordered array of nodes
     * @return the new root node
     */
    public TreeNode buildTree(TreeNode[] nodeList) {
        return buildTreeHelper(nodeList, 0, nodeList.length);
    }

    private TreeNode buildTreeHelper(TreeNode[] nodeList, int start, int end) {
        if (start >= end) return null;

        int mid = start + (end - start) / 2;
        TreeNode root = nodeList[mid];
        root.weight = end - start;

        root.left = buildTreeHelper(nodeList, start, mid);
        if (root.left != null) root.left.parent = root;
        root.right = buildTreeHelper(nodeList, mid+1, end);
        if (root.right != null) root.right.parent = root;

        return root;
    }

    /**
     * Determines if a node is balanced. If the node is balanced, this should return true. Otherwise, it should return
     * false. A node is unbalanced if either of its children has weight greater than 2/3 of its weight.
     *
     * @param node a node to check balance on
     * @return true if the node is balanced, false otherwise
     */
    public boolean checkBalance(TreeNode node) {
        double nWeight = (2.0/3) * node.weight;
        if (node.left != null && node.left.weight > nWeight)
            return false;
        else if (node.right != null && node.right.weight > nWeight)
            return false;
        else
            return true;
    }

    /**
    * Rebuilds the subtree rooted at node
    * 
    * @param node the root of the subtree to rebuild
    */
    public void rebuild(TreeNode node) {
        // Error checking: cannot rebuild null tree
        if (node == null) {
            return;
        }

        TreeNode p = node.parent;
        TreeNode[] nodeList = enumerateNodes(node);
        TreeNode newRoot = buildTree(nodeList);

        if (p == null) {
            root = newRoot;
        } else if (node == p.left) {
            p.left = newRoot;
        } else {
            p.right = newRoot;
        }

        newRoot.parent = p;
    }

    /**
    * Inserts a key into the tree
    *
    * @param key the key to insert
    */
    public void insert(int key) {
        if (this.root == null) {
            this.root = new TreeNode(key);
            return;
        }

        TreeNode inserted = insert(key, this.root);
        TreeNode unbalanced = findUnbalancedParent(inserted);
        rebuild(unbalanced);
    }

    // Helper method to insert a key into the tree
    private TreeNode insert(int key, TreeNode node) {
        node.weight++;
        if (key <= node.key) {
            if (node.left == null) {
                node.left = new TreeNode(key);
                node.left.parent = node;
                return node.left;
            } else {
                return insert(key, node.left);
            }
        } else {
            if (node.right == null) {
                node.right = new TreeNode(key);
                node.right.parent = node;
                return node.right;
            } else {
                return insert(key, node.right);
            }
        }
    }

    // Helper method to find the unbalanced parent node
    private TreeNode findUnbalancedParent(TreeNode node) {
        TreeNode unbalanced = null;
        for (; node != null; node = node.parent)
            if (!checkBalance(node)) unbalanced = node;
        return unbalanced;
    }

    // Simple main function for debugging purposes
    public static void main(String[] args) {
        SGTree tree = new SGTree();
        for (int i = 0; i < 100; i++) {
            tree.insert(i);
        }
        tree.rebuild(tree.root);
    }
}
