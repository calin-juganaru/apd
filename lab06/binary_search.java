public class BinarySearch
{
    static int binarySearch(int arr[], int left, int right, int x)
    {
        if (right >= left)
        {
            int mid = (right + left) / 2;

            if (arr[mid] == x)
                return mid;

            if (arr[mid] > x)
                return binarySearch(arr, left, mid - 1, x);

            return binarySearch(arr, mid + 1, right, x);
        }
        return -1;
    }

    public static void main(String args[])
    {
        int arr[] = { 2, 3, 4, 10, 40 };
        int n = arr.length;
        int result = binarySearch(arr, 0, n - 1, 10);
        if (result == -1)
             System.out.println("Scuze boss");
        else System.out.println("Gasit la " + result);
    }
}
