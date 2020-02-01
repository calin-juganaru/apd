package oneProducerOneConsumer;
import java.util.concurrent.Semaphore;

public class Main {
	public static final int N = 1000000;

	public static void main(String[] args) {
		Buffer buffer = new Buffer();

		Thread threads[] = new Thread[2];
		threads[0] = new Thread(new Producer(buffer));
		threads[1] = new Thread(new Consumer(buffer));

		for (int i = 0; i < 2; i++)
			threads[i].start();
		for (int i = 0; i < 2; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}

class Buffer
{
	int a;
	Semaphore s1 = new Semaphore(1);
	Semaphore s2 = new Semaphore(0);

	void put(int value)
	{
		try
		{
			s1.acquire();
		}
		catch (Exception e) {}

		a = value;
		s2.release();
	}

	int get()
	{
		try
		{
			s2.acquire();
		}
		catch (Exception e) {}

		int b = a;
		s1.release();
		return b;
	}
}

class Producer implements Runnable {
	Buffer buffer;

	Producer(Buffer buffer) {
		this.buffer = buffer;
	}

	@Override
	public void run() {
		for (int i = 0; i < Main.N; i++) {
			buffer.put(i);
		}
	}

}

class Consumer implements Runnable {
	Buffer buffer;

	Consumer(Buffer buffer) {
		this.buffer = buffer;
	}

	@Override
	public void run() {
		for (int i = 0; i < Main.N; i++) {
			int value = buffer.get();
			if (value != i) {
				System.out.println("Wrong value. I was supposed to get " + i + " but I received " + value);
				System.exit(1);
			}
		}
		System.out.println("I finished Correctly");
	}
}