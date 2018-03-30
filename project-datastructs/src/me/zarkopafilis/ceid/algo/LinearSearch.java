package me.zarkopafilis.ceid.algo;

public class LinearSearch implements SearchAlgo {

    @Override
    public int search(Integer[] from, int x) {
        for(int i = 0; i < from.length; i++) {
            if (from[i] == x) {
                return i;
            }
        }
        return NOT_FOUND;
    }
}
