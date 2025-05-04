import java.util.ArrayList;

public class Trie {

    // Wildcards
    static final char START = '^';
    static final char WILDCARD = '.';

    // Fields
    final TrieNode root;

    private class TrieNode {
        // TODO: Create your TrieNode class here.
        private TrieNode[] children = new TrieNode[62];
        private char c;
        private boolean isEnd = false;

        public TrieNode(char c) {
            this.c = c;
        }

        public boolean isEnd() {
            return this.isEnd;
        }

        public void setEnd() {
            this.isEnd = true;
        }

        public TrieNode findChild(char c) {
            int pos = getCharPos(c);
            if (pos == -1) return null;
            return this.children[pos];
        }

        public ArrayList<TrieNode> getChildren() {
            ArrayList<TrieNode> res = new ArrayList<>();
            for (TrieNode child : this.children) {
                if (child != null)
                    res.add(child);
            }
            return res;
        }

        public TrieNode insert(char c) {
            int pos = getCharPos(c);
            if (pos == -1) return null;
            if (this.children[pos] == null)
                this.children[pos] = new TrieNode(c);
            return this.children[pos];
        }

        private int getCharPos(char c) {
            if (c >= 'A' && c <= 'Z') return c - 65;
            else if (c >= 'a' && c <= 'z') return c - 97 + 26;
            else if (c >= '0' && c <= '9') return c - 48 + 52;
            else return -1;
        }
    }

    public Trie() {
        this.root = new TrieNode(Trie.START);
    }

    /**
     * Inserts string s into the Trie.
     *
     * @param s string to insert into the Trie
     */
    void insert(String s) {
        TrieNode curr = this.root;
        for (int i = 0; i < s.length(); i++) {
            char c = s.charAt(i);
            curr = curr.insert(c);
        }
        curr.setEnd();
    }

    /**
     * Checks whether string s exists inside the Trie or not.
     *
     * @param s string to check for
     * @return whether string s is inside the Trie
     */
    boolean contains(String s) {
        TrieNode curr = this.root;
        for (int i = 0; i < s.length() && curr != null; i++) {
            char c = s.charAt(i);
            curr = curr.findChild(c);
        }
        return curr != null && curr.isEnd();
    }

    /**
     * Searches for strings with prefix matching the specified pattern sorted by lexicographical order. This inserts the
     * results into the specified ArrayList. Only returns at most the first limit results.
     *
     * @param s       pattern to match prefixes with
     * @param results array to add the results into
     * @param limit   max number of strings to add into results
     */
    void prefixSearch(String s, ArrayList<String> results, int limit) {
        prefixSearchHelper(this.root, s, 0, new StringBuilder(), results, limit);
    }
    
    private void prefixSearchHelper(TrieNode curr, String s, int n, StringBuilder word, ArrayList<String> results, int limit) {
        if (results.size() >= limit) return;

        if (n >= s.length()) {
            if (curr.isEnd())
                results.add(word.toString());
            for (TrieNode child : curr.getChildren()) {
                word.append(child.c);
                prefixSearchHelper(child, s, n, word, results, limit);
                word.deleteCharAt(word.length() - 1);
            }
            return;
        }

        char c = s.charAt(n);
        if (c == Trie.WILDCARD) {
            for (TrieNode child : curr.getChildren()) {
                word.append(child.c);
                prefixSearchHelper(child, s, n + 1, word, results, limit);
                word.deleteCharAt(word.length() - 1);
            }
        } else {
            TrieNode child = curr.findChild(c);
            if (child != null) {
                word.append(c);
                prefixSearchHelper(child, s, n + 1, word, results, limit);
                word.deleteCharAt(word.length() - 1);
            }
        }
    }


    // Simplifies function call by initializing an empty array to store the results.
    // PLEASE DO NOT CHANGE the implementation for this function as it will be used
    // to run the test cases.
    String[] prefixSearch(String s, int limit) {
        ArrayList<String> results = new ArrayList<>();
        prefixSearch(s, results, limit);
        return results.toArray(new String[0]);
    }


    public static void main(String[] args) {
        Trie t = new Trie();
        t.insert("peter");
        t.insert("piper");
        t.insert("picked");
        t.insert("a");
        t.insert("peck");
        t.insert("of");
        t.insert("pickled");
        t.insert("peppers");
        t.insert("pepppito");
        t.insert("pepi");
        t.insert("pik");

        //String[] result1 = t.prefixSearch("pe", 10);
        //String[] result2 = t.prefixSearch("pe.", 10);
        // result1 should be:
        // ["peck", "pepi", "peppers", "pepppito", "peter"]
        // result2 should contain the same elements with result1 but may be ordered arbitrarily
    }
}
