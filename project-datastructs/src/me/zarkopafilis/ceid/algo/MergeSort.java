package me.zarkopafilis.ceid.algo;

public class MergeSort {

    private final Integer[] array;
    private final int low;
    private final int high;

    public MergeSort(Integer[] array, int low, int high) {
        this.array = array;
        this.low = low;
        this.high = high;
    }

    public Integer[] getArray() {
        return array;
    }

    private void merge(int middle) {
        if (array[middle - 1] < array[middle]) {
            return; //sorted
        }
        int[] copy = new int[high - low];

        System.arraycopy(array, low, copy, 0, copy.length);

        int copyLow = 0;
        int copyHigh = high - low;
        int copyMiddle = middle - low;

        for (int i = low, p = copyLow, q = copyMiddle; i < high; i++) {
            if (q >= copyHigh || (p < copyMiddle && copy[p] < copy[q])) {
                array[i] = copy[p++];
            } else {
                array[i] = copy[q++];
            }
        }
    }

    public void start() {
        mergeSort(low, high);
    }

    private void mergeSort(int l, int h) {
        if (l < h) {
            int m = (l + h) / 2;
            /* Sort first and second halves */
            mergeSort(l, m);
            mergeSort(m + 1, h);
            merge(l, m, h);
        }
    }

    private void merge(int l, int m, int r) {
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;

        /* create temp arrays */
        int[] l_arr = new int[n1];
        int[] h_arr = new int[n2];

        /* Copy data to temp arrays*/
        for (i = 0; i < n1; i++)
            l_arr[i] = array[l + i];
        for (j = 0; j < n2; j++)
            h_arr[j] = array[m + 1 + j];

        /* Merge the temp arrays back into array[] */
        i = 0; // Initial index of first subarray
        j = 0; // Initial index of second subarray
        k = l; // Initial index of merged subarray
        while (i < n1 && j < n2) {
            if (l_arr[i] <= h_arr[j]) {
                array[k] = l_arr[i];
                i++;
            } else {
                array[k] = h_arr[j];
                j++;
            }
            k++;
        }

        while (i < n1) {
            array[k] = l_arr[i];
            i++;
            k++;
        }

        while (j < n2) {
            array[k] = h_arr[j];
            j++;
            k++;
        }
    }
}
