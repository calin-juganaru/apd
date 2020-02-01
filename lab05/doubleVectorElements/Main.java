package doubleVectorElements;

public class Main
{
	static int N = 26, P = 4;
	static int v[] = new int[N];

	public static void main(String[] args)
	{
		for(int i = 0; i < N; ++i)
			v[i] = i;

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

		for (int i = 0; i < N; i++)
		{
			if (v[i] != i * 2)
			{
				System.out.println("Wrong answer");
				System.out.println(v[i] + " != " + i * 2);
				System.exit(1);
			}
		}

		System.out.println("Correct");
	}

}

class MyThread extends Thread
{
    private int thread_id, begin, end;

	public MyThread(int thread_id)
    {
        this.thread_id = thread_id;

		begin = (Main.N / Main.P) * thread_id;
		end = begin + Main.N / Main.P;

		if (thread_id == Main.P - 1)
			end += Main.N % Main.P;
    }

	public void run()
    {
		for (int i = begin; i < end; ++i)
		{
			Main.v[i] = Main.v[i] * 2;
		}
    }
}