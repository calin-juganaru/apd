public class ParallelSearch
{
	static int v[] = { 1, 2, 3, 4, 7, 8, 10, 12, 19, 22, 30, 40 };
    static int N = v.length, P = 3;
    static int found = -1, elem = 12;

	public static void main(String[] args)
	{
        MyThread threads[] = new MyThread[P];

        for (int i = 0; i < P; ++i)
        {
            threads[i] = new MyThread(i);
        }

        for (int i = 0; i < P; ++i)
        {
            threads[i].start();
        }

		for (int i = 0; i < P; ++i)
        {
            try
			{
				threads[i].join();
			}
			catch(InterruptedException e)
			{
				e.printStackTrace();
			}

        }

        if (found == -1)
             System.out.println("Scuze boss");
        else System.out.println("Gasit la " + found);
	}

}

class MyThread extends Thread
{
    private int thread_id, begin, end;

	public MyThread(int thread_id)
    {
        this.thread_id = thread_id;

		begin = (ParallelSearch.N / ParallelSearch.P) * thread_id;
		end = begin + ParallelSearch.N / ParallelSearch.P;

		if (thread_id == ParallelSearch.P - 1)
			end += ParallelSearch.N % ParallelSearch.P;
    }

	public void run()
    {
        int left = ParallelSearch.v[begin];
        int right = ParallelSearch.v[end - 1];

        if (begin <= end && end < ParallelSearch.N)
        {
            int mid = (begin + end) / 2;

            if (ParallelSearch.v[mid] == ParallelSearch.elem)
            {
                ParallelSearch.found = mid;
                return;
            }

            if (ParallelSearch.v[mid] > ParallelSearch.elem)
            {
                end = (begin + end) / 2 - 1;
                run();
                //return;
            }

            begin = (begin + end) / 2 + 1;
            run();
            //return;
        }
    }
}