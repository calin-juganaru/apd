package shortestPathsFloyd_Warshall;

public class Main
{
	static int N = 5, M = 9, P = 2;
	static int graph[][] = { { 0, 1, M, M, M },
				             { 1, 0, 1, M, M },
				             { M, 1, 0, 1, 1 },
				             { M, M, 1, 0, M },
				             { M, M, 1, M, 0 } };

	public static void main(String[] args)
	{
		MyThread threads[] = new MyThread[P];

		for (int k = 0; k < N; k++)
		{
			for (int I = 0; I < N; I++)
			{
				for (int i = 0; i < P; ++i)
            		threads[i] = new MyThread(i, I, k);

				for (int i = 0; i < P; ++i)
					threads[i].start();

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
			}
		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				System.out.print(graph[i][j] + " ");
			}
			System.out.println();
		}
	}
}

class MyThread extends Thread
{
    private int thread_id, begin, end, i, k;

	public MyThread(int thread_id, int i, int k)
    {
        this.thread_id = thread_id;
		this.i = i; this.k = k;

		begin = (Main.N / Main.P) * thread_id;
		end = begin + Main.N / Main.P;

		if (thread_id == Main.P - 1)
			end += Main.N % Main.P;
    }

	public void run()
    {
		for (int j = begin; j < end; j++)
		{
			Main.graph[i][j] = Math.min(Main.graph[i][k] + Main.graph[k][j], Main.graph[i][j]);
		}
    }
}