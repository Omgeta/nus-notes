/**
 * @author gilbert
 * Interface: ILFShiftRegister
 * Description: A linear feedback shift register based on XOR with one tap.
 */
public interface ILFShiftRegister {
    // Sets the value of the shift register to the specified seed.
    void setSeed(int[] seed);

    // Shifts the register one time, returning the low-order bit.
    int shift();

    // Shifts the register k times, returning a k-bit integer.
    int generate(int k);
}
