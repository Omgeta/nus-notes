import static org.junit.Assert.*;

public class SolutionTest {

    @org.junit.Test
    public void update() {
        Solution soln = new Solution(3);
        assertEquals(2, soln.update(2, 7));
        assertEquals(3, soln.update(3, 5));
        assertEquals(2, soln.update(1, 6));
        assertEquals(1, soln.update(1, 9));
    }
}