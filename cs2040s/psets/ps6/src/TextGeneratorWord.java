import java.io.FileInputStream;

/**
 * This class is used to generated text using a Markov Model
 */
public class TextGeneratorWord {

    // For testing, we will choose different seeds
    private static long seed;

    // Sets the random number generator seed
    public static void setSeed(long s) {
        seed = s;
    }

    /**
     * Reads in the file and builds the MarkovModel.
     *
     * @param fileName the name of the file to read
     * @param model the Markov Model to build
     * @return the first word of the file to be used as the seed text
     */
    public static String buildModel(String fileName, MarkovModelWord model) {
        // Get ready to parse the file.
        // StringBuffer is used instead of String as appending character to String is slow
        StringBuilder text = new StringBuilder();

        // Loop through the text
        try {
            FileInputStream inputStream = new FileInputStream(fileName);

            // Determine the size of the file, in bytes
            int fileSize = inputStream.available();

            // Read in the file, one character at a time.
            for (int i = 0; i < fileSize; i++) {
                // Read a character
                char c = (char) inputStream.read();
                text.append(c);
            }

            // Make sure that length of input text is longer than requested Markov order
            if (text.length() < 2) {
                System.out.println("Text is shorter than 2 words");
                return null;
            }
        } catch (Exception e) {
            System.out.println("Problem reading file " + fileName + ".");
            return null;
        }

        // Build Markov Model of order from text
        model.initializeText(text.toString());
        return text.toString().split("\\s+")[0];
    }

    /**
     * generateText outputs to stdout text of the specified length based on the specified seedText
     * using the given Markov Model.
     *
     * @param model the Markov Model to use
     * @param seedText the initial kgram used to generate text
     * @param length the length of the text to generate
     */
    public static void generateText(MarkovModelWord model, String seedText, int length) {
        // Use the first order characters of the text as the starting string
        StringBuffer output = new StringBuffer();
        output.append(seedText);

        // Generate length characters
        String wordToAppend;
        int wordCount = 1;
        while (wordCount < length) {
            // Get the next word based on the previous word
            wordToAppend = model.nextWord(output.substring(output.lastIndexOf(" ") + 1));

            // If there is no next word, restart generation with initial word which
            // Starts from 0th position.
            if (!wordToAppend.equals(MarkovModelWord.NOWORD)) {
                output.append(" ");
                output.append(wordToAppend);
                wordCount++;
            } else {
                // This prefix has never appeared in the text.
                // Give up?
                System.out.println(output);
                return;
            }
        }

        // Output the generated characters, not including the initial seed.
        System.out.println(output);
    }

    /**
     * The main routine.  Takes 3 arguments:
     * args[0]: the length of the text to generate
     * args[1]: the filename for the input text
     */
    public static void main(String[] args) {
        int length;
        String fileName;

        // Check that we have three parameters
        if (args.length != 2) {
            System.out.println("Number of input parameters are wrong.");
            length = 64;
            fileName = "aesop.txt";
        } else {
            // Get the input:
            length = Integer.parseInt(args[0]);
            fileName = args[1];
        }

        // Create the model
        MarkovModelWord markovModel = new MarkovModelWord(seed);
        String seedText = buildModel(fileName, markovModel);

        // Generate text
        generateText(markovModel, seedText, length);
    }
}