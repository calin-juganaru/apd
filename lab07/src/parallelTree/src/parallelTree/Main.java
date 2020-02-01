package parallelTree;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;


public class Main
{
	static int N_ITERATIONS = 100;
	static CyclicBarrier bar = new CyclicBarrier(3);

	public static void main(String[] args)
	{
		Thread threads[] = new Thread[3];
		System.out.println("Parallel tree problem");
		
		for (int j = 0; j < N_ITERATIONS; j++)
		{
			TreeNode tree = new TreeNode(1);
			threads[0] = new Thread(new ReadTreePart(tree, "treePart1.txt"));
			threads[1] = new Thread(new ReadTreePart(tree, "treePart2.txt"));
			threads[2] = new Thread(new VerifyTree(tree));
			for (int i = 0; i < 3; i++)
				threads[i].start();

			for (int i = 0; i < 3; i++)
			{
				try
				{
					threads[i].join();
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
			}
		}
	}
}
