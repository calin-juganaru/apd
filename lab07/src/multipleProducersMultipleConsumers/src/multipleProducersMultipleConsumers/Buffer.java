package multipleProducersMultipleConsumers;

import java.util.concurrent.ArrayBlockingQueue;

public class Buffer
{
	ArrayBlockingQueue<Integer> a = new ArrayBlockingQueue<>(5);

	void put(int value)
	{
		try
		{
			a.put(value);
		}
		catch (Exception e) {}
	}

	int get()
	{
		int b = -1;
		try
		{
			b = a.take();
		}
		catch (Exception e) {}

		return b;
	}
}
