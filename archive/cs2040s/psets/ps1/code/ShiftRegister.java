// Sources Used:
// https://docs.oracle.com/javase/8/docs/api/ (for String and Streams API)

///////////////////////////////////
// This is the main shift register class.
// Notice that it implements the ILFShiftRegister interface.
// You will need to fill in the functionality.
///////////////////////////////////

/**
 * class ShiftRegister
 * @author
 * Description: implements the ILFShiftRegister interface.
 */
public class ShiftRegister implements ILFShiftRegister {
    ///////////////////////////////////
    // Create your class variables here
    ///////////////////////////////////
    private int tap;
    public int[] data;

    ///////////////////////////////////
    // Create your constructor here:
    ///////////////////////////////////
    ShiftRegister(int size, int tap) {
      if (size <= 0) {
          throw new RuntimeException("Size must be positive");
      }

      if (tap < 0 || tap > size - 1) {
          throw new RuntimeException("Tap must be positive and less than size");
      }

      this.tap = tap;
      this.data = new int[size];
    }

    ///////////////////////////////////
    // Create your class methods here:
    ///////////////////////////////////
    /**
     * setSeed
     * @param seed
     * Description: Sets the shift register to the specified initial seed. The initial
     * seed is specified as an array of 0’s and 1’s (represented as integers). If the seed contains any
     * other value, that is an error. If the seed is too long, it is truncated to the size of the shift
     */
    @Override
    public void setSeed(int[] seed) {
      for (int i = 0; i < this.data.length; i++) {
          int seedValue = seed[this.data.length - i - 1];
          if (seedValue == 0 || seedValue == 1) {
              this.data[i] = seedValue;
          } else {
            throw new RuntimeException("Seed must only consist of 0's and 1's");
          }
      }
    }

    ///////////////////////////////////
    // Create your class methods here:
    ///////////////////////////////////
    /**
     * setSeed
     * @param seedStr
     * Description: Sets the shift register to the specified initial seed. The initial
     * seed is specified as a string which converts into a ASCII binary seed to be used.
     * For this implementation, I don't reverse the output bit array to keep MSB on the left
     * as per Q2. because I believe in the context of a seed, it doesn't matter.
     * Naturally, length n string requires size 7*n ShiftRegister
     * Example: setSeed("a") creates seed {1, 0, 0, 0, 0, 1, 1}
     */
    public void setSeed(String seedStr) {
        int[] seed = seedStr.chars()
                .mapToObj(i -> Integer.toString(i, 2))
                .flatMap(bStr -> bStr.chars().mapToObj(c -> c - '0'))
                .mapToInt(Integer::valueOf)
                .toArray();
        setSeed(seed);
    }

    /**
     * shift
     * @return
     * Description: Executes one shift step and returns the least significant bit of the resulting
     * register. 
     */
    @Override
    public int shift() {
      int tapBit = this.data[this.data.length - this.tap - 1];
      int msb = this.data[0];
      int newLsb = msb ^ tapBit;

      int i;
      for (i = 0; i < this.data.length - 1; i++) {
          this.data[i] = this.data[i+1];
      }
        this.data[i] = newLsb;

      return newLsb;
    }

    /**
     * generate
     * @param k
     * @return
     * Description: Extracts k bits from the shift register, where k ≤ 32. It executes the
     * shift operation k times, saving the k bits returned. It then converts these k bits from binary
     * into an integer.
     */
    @Override
    public int generate(int k) {
      int[] res = new int[k];
      for (int i = 0; i < k; i++) {
        res[i] = shift();
      }

      return ShiftRegister.toDecimal(res);
    }

    /**
     * Returns the integer representation for a binary int array.
     * @param array
     * @return
     */
    private static int toDecimal(int[] array) {
      int res = 0;
      for (int i = 0; i < array.length; i++) {
        int bit = array[array.length - i - 1];
        if (bit == 0 || bit == 1) {
            res += bit * (1 << i);
        } else {
            throw new RuntimeException("Array passed must have only binary int values");
        }
      }
      return res;
    }
}
