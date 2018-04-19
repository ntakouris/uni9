package me.zarkopafilis.ceid.algo;

public class InterpolationSearch implements SearchAlgo{
    @Override
    public int search(Integer[] from, int x) {
        int l = 0;
        int r = from.length - 1;
        while (l <= r && x >= from[l] && x <= from[r])
        {
            int i = l + ((x - from[l]) / (from[r] - from[l]));
            int t = from[i];

            if(t < x){
                l = i + 1;
            }else if( t > x){
                r = i - 1;
            } else if (t == x) {
                return i;
            }else{
                return NOT_FOUND;
            }
        }

        return NOT_FOUND;
    }
}
