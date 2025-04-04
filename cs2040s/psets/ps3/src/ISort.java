/**
 * Simple sorting interface for an object that contains a sorting routine.
 * 
 * @author gilbert
 */
public interface ISort {
    /**
     * Sort an array of KeyValuePair objects by key.
     * 
     * @param array KeyValuePair objects to sort
     * @return long representing cost of sorting
     */
    public long sort(KeyValuePair[] array);
}
