package executorSample;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main
{
	public static void main(String[] args)
	{
		ExecutorService tpe = Executors.newFixedThreadPool(4);
		tpe.submit(new MyRunnable(tpe, 0));
		tpe.submit(new MyRunnable(tpe, 1));
		tpe.submit(new MyRunnable(tpe, 2));
	}
}

/*
diagonala principala: i = 0, n-1 --- j = i
diagonala secundara:  i = 0, n-1 --- j = n - i
*/