package me.zarkopafilis.ceid.algo;

public class BinarySearch implements SearchAlgo{
    @Override
    public int search(Integer[] from, int x) {
        int l = 0;
        int r = from.length;
        while (l <= r) {
            int mid = (l + r) / 2;
            if (from[mid] < x) {
                l = mid + 1;
            } else if (from[mid] > x) {
                r = mid - 1;
            } else if (from[mid] == x) {
                return mid;
            }else{
                return NOT_FOUND;
            }
        }
        return NOT_FOUND;
    }
}
