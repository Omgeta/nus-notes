// Sources Used:
// https://junit.org/junit5/docs/current/user-guide/#writing-tests-exceptions-expected

import static org.junit.Assert.*;

import org.junit.Test;

/**
 * ShiftRegisterTest
 * @author dcsslg
 * Description: set of tests for a shift register implementation
 */
public class ShiftRegisterTest {
    /**
     * Returns a shift register to test.
     * @param size
     * @param tap
     * @return a new shift register
     */
    ILFShiftRegister getRegister(int size, int tap) {
        return new ShiftRegister(size, tap);
    }

    /**
     * Tests shift with simple example.
     */
    @Test
    public void testShift1() {
        ILFShiftRegister r = getRegister(9, 7);
        int[] seed = { 0, 1, 0, 1, 1, 1, 1, 0, 1 };
        r.setSeed(seed);
        int[] expected = { 1, 1, 0, 0, 0, 1, 1, 1, 1, 0 };
        for (int i = 0; i < 10; i++) {
            assertEquals(expected[i], r.shift());
        }
    }

    /**
     * Tests generate with simple example.
     */
    @Test
    public void testGenerate1() {
        ILFShiftRegister r = getRegister(9, 7);
        int[] seed = { 0, 1, 0, 1, 1, 1, 1, 0, 1 };
        r.setSeed(seed);
        int[] expected = { 6, 1, 7, 2, 2, 1, 6, 6, 2, 3 };
        for (int i = 0; i < 10; i++) {
            assertEquals("GenerateTest", expected[i], r.generate(3));
        }
    }

    /**
     * Tests register of length 1.
     */
    @Test
    public void testOneLength() {
        ILFShiftRegister r = getRegister(1, 0);
        int[] seed = { 1 };
        r.setSeed(seed);
        int[] expected = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
        for (int i = 0; i < 10; i++) {
            assertEquals(expected[i], r.generate(3));
        }
    }

    /**
     * Tests with erroneous seed.
     */
    @Test
    public void testError() {
        ILFShiftRegister r = getRegister(4, 1);
        int[] seed = { 1, 0, 0, 0, 1, 1, 0 };
        r.setSeed(seed);
        r.shift();
        r.generate(4);
    }
    // Ideally, we should throw an error when the size of the seed does not match
    // the specified register.
    // In this case we would expect a thrown exception.
    // Alternatively, we could truncate long seed and extend short seeds
    // with left padded 0s.
    // In this case, we should expect an appropriate result for the input.

    /**
     * Tests shift with edge case example: non-positive size.
     */
    @Test
    public void testSize() {
        assertThrows(RuntimeException.class, () -> {
            ILFShiftRegister r = getRegister(0, 0);
        });
    }

    /**
     * Tests shift with edge case example: tap out of range.
     */
    @Test
    public void testTapRange() {
        assertThrows(RuntimeException.class, () -> {
            ILFShiftRegister r = getRegister(1, 2);
        });
    }

    /**
     * Tests shift with edge case example: seed non-binary values.
     */
    @Test
    public void testSeedValid() {
        ILFShiftRegister r = getRegister(7, 2);
        int[] seed = { 1, 0, 0, 0, 1, 1, 2 };
        assertThrows(RuntimeException.class, () -> {
            r.setSeed(seed);
        });
    }
}
