import java.util.Random;
import java.util.HashMap;

/**
 * This is the main class for your Markov Model.
 *
 * Assume that the text will contain ASCII characters in the range [1,255].
 * ASCII character 0 (the NULL character) will be treated as a non-character.
 *
 * Any such NULL characters in the original text should be ignored.
 */
public class MarkovModel {

	// Use this to generate random numbers as needed
	private final Random generator = new Random();

	// This is a special symbol to indicate no character
	public static final char NOCHARACTER = (char) 0;

	// HashMap for storing counts for each kgram String
	private final HashMap<String, CountMap<Character>> model;

	// Order of kgrams
	private final int order;

	/**
	 * Constructor for MarkovModel class.
	 *
	 * @param order the number of characters to identify for the Markov Model sequence
	 * @param seed the seed used by the random number generator
	 */
	public MarkovModel(int order, long seed) {
		// Initialize your class here
		this.model = new HashMap<>();
		this.order = order;

		// Initialize the random number generator
		this.generator.setSeed(seed);
	}

	/**
	 * Builds the Markov Model based on the specified text string.
	 */
	public void initializeText(String text) {
		// Iterate through and find all order-length kgrams and
		// their next character, adding each next character to a count
		// for that specific kgram
		for (int i = 0; i < text.length() - this.order; i++) {
			String key = text.substring(i, i + this.order);
			Character next = text.charAt(i + this.order);

			this.model.computeIfAbsent(key, k -> new CountMap<>())
					.add(next);
		}
	}

	/**
	 * Returns the number of times the specified kgram appeared in the text.
	 */
	public int getFrequency(String kgram) {
		CountMap<Character> counts = model.get(kgram);
		return counts != null ? counts.getTotal() : 0;
	}

	/**
	 * Returns the number of times the character c appears immediately after the specified kgram.
	 */
	public int getFrequency(String kgram, char c) {
		CountMap<Character> counts = model.get(kgram);
		return counts != null ? counts.getCount(c) : 0;
	}

	/**
	 * Generates the next character from the Markov Model.
	 * Return NOCHARACTER if the kgram is not in the table, or if there is no
	 * valid character following the kgram.
	 */
	public char nextCharacter(String kgram) {
		CountMap<Character> counts = this.model.get(kgram);

		if (counts != null) {
			// n here is [0, total)
			int n = this.generator.nextInt(counts.getTotal());
			Character next = counts.getNth(n);
			if (next != null) return next;
		}

		return NOCHARACTER;
	}

	private static class CountMap<T extends Comparable<T>> {
		// Count of total occurrences
		private int total;
		// Count of each key
		private final HashMap<T, Integer> counts;

		/**
		 * Constructor for MarkovCell class.
		 */
		public CountMap() {
			this.total = 0;
			this.counts = new HashMap<>();
		}

		/**
		 * Adds a new key to the cell counts
		 */
		public void add(T c) {
			this.total++;
			this.counts.put(c, getCount(c) + 1);
		}

		/**
		 * Get total count
		 */
		public int getTotal() {
			return this.total;
		}

		/**
		 * Get char count
		 */
		public int getCount(T c) {
			return this.counts.getOrDefault(c, 0);
		}

		/**
		 * Get Nth key ordered by ASCII values
		 */
		public T getNth(int n) {
			// Run through sorted chars and return first char which pushes
			// the cumulative count past n
			int accum = 0;
			for (T c : this.counts.keySet().stream().sorted().toList()) {
				accum += this.counts.get(c);
				if (accum > n) return c;
			}
			return null;
		}
	}
}
