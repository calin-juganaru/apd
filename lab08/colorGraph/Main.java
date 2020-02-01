package colorGraph;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main
{
	static int N = 10;
	static int COLORS = 3;
	static int[][] graph =
	{
			{ 0, 1 }, { 0, 4 }, { 0, 5 }, { 1, 0 }, { 1, 2 }, { 1, 6 }, { 2, 1 }, { 2, 3 },
			{ 2, 7 }, { 3, 2 }, { 3, 4 }, { 3, 8 }, { 4, 0 }, { 4, 3 }, { 4, 9 }, { 5, 0 },
			{ 5, 7 }, { 5, 8 }, { 6, 1 }, { 6, 8 }, { 6, 9 }, { 7, 2 }, { 7, 5 }, { 7, 9 },
			{ 8, 3 }, { 8, 5 }, { 8, 6 }, { 9, 4 }, { 9, 6 }, { 9, 7 }
	};

	public static boolean verifyColors(int[] colors, int step)
	{
		for (int i = 0; i < step; i++)
		{
			if (colors[i] == colors[step] && isEdge(i, step))
				return false;
		}
		return true;
	}

	private static boolean isEdge(int a, int b)
	{
		for (int i = 0; i < graph.length; i++)
		{
			if (graph[i][0] == a && graph[i][1] == b)
				return true;
		}
		return false;
	}

	static void printColors(int[] colors)
	{
		String aux = "";
		for (int i = 0; i < colors.length; i++)
		{
			aux += colors[i] + " ";
		}
		System.out.println(aux);
	}

	public static void main(String[] args)
	{
		ExecutorService tpe = Executors.newFixedThreadPool(4);
		int[] colors = new int[N];
		tpe.submit(new MyRunnable(tpe, colors, 0));
	}
}

class MyRunnable implements Runnable
{
	int step;
	int[] colors;
	ExecutorService tpe;

	public MyRunnable(ExecutorService tpe, int[] colors, int step)
	{
		this.colors = colors;
		this.tpe = tpe;
		this.step = step;
	}

	@Override
	public void run()
	{
		if (step == Main.N)
		{
			Main.printColors(colors);
			tpe.shutdown();
			return;
		}

		for (int i = 0; i < Main.COLORS; i++)
		{
			int[] newColors = colors.clone();
			newColors[step] = i;
			if (Main.verifyColors(newColors, step))
				tpe.submit(new MyRunnable(tpe, newColors, step + 1));
		}
	}
}