package getPathSequential;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

class MyRunnable implements Runnable
{
	int destination;
	ArrayList<Integer> partialPath;
	ExecutorService tpe;

	public MyRunnable(ExecutorService tpe, ArrayList<Integer> partialPath, int destination)
	{
		this.partialPath = partialPath;
		this.tpe = tpe;
		this.destination = destination;
	}

	@Override
	public void run()
	{
		if (partialPath.get(partialPath.size() - 1) == destination)
		{
			System.out.println(partialPath);
			tpe.shutdown();
			return;
		}

		int lastNodeInPath = partialPath.get(partialPath.size() - 1);
		for (int i = 0; i < Main.graph.length; i++)
		{
			if (Main.graph[i][0] == lastNodeInPath)
			{
				if (partialPath.contains(Main.graph[i][1]))  continue;
				ArrayList<Integer> newPartialPath = (ArrayList<Integer>) partialPath.clone();
				newPartialPath.add(Main.graph[i][1]);

				//System.out.println(partialPath);
				tpe.submit(new MyRunnable(tpe, newPartialPath, destination));
			}
		}
	}
}

public class Main
{
	static int[][] graph =
	{
			{ 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 },
			{ 2, 7 }, { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 },
			{ 5, 7 }, { 5, 8 }, { 6, 1 }, { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 },
			{ 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 }, { 9, 7 }
	};

	public static void main(String[] args)
	{
		ExecutorService tpe = Executors.newFixedThreadPool(4);
		ArrayList<Integer> partialPath = new ArrayList<Integer>();
		partialPath.add(0);
		tpe.submit(new MyRunnable(tpe, partialPath, 3));
		tpe.submit(new MyRunnable(tpe, partialPath, 3));
	}
}
