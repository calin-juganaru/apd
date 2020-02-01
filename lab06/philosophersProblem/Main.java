package philosophersProblem;


public class Main {
	public static final int N = 6;

	public static void main(String[] args) {
		Thread threads[] = new Thread[N];
		Object forks[] = new Object[N];
		System.out.println("Philosophers problem!");


		for (int i = 0; i < N; i++)
			forks[i] = new Object();
		for (int i = 0; i < N; i++)
			threads[i] = new Thread(new Philosopher(i, forks[i], forks[(i+1)%N]));
		for (int i = 0; i < N; i++)
			threads[i].start();

		for (int i = 0; i < N; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				e.printStackTrace();
			}
		}
	}

}

class Philosopher implements Runnable {
	Object leftFork, rightFork;
	int id;

	public Philosopher(int id, Object leftFork, Object rightFork) {
		this.leftFork = leftFork;
		this.rightFork = rightFork;
		this.id = id;
	}

	private void sleep() {
		try {
			Thread.sleep(100);
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void run() {
		synchronized (leftFork) {
			sleep(); // delay added to make sure the dead-lock is visible
			if (id != 0)
			synchronized (rightFork) {
				System.out.println("Philosopher " + id + " is eating");
			}
		}
	}
}