class MyThread extends Thread
{
    public MyThread(String s)
    {
        super(s);
    }

    public void run()
    {
        System.out.println("Hello world! from thread " + getName());
    }

    public static void main(String[] args)
    {
        int N = 4;
        MyThread threads[] = new MyThread[N];

        for (Integer i = 0; i < N; ++i)
        {
            threads[i] = new MyThread(i.toString());
        }

        for (int i = 0; i < N; ++i)
        {
            threads[i].start();
        }

        System.out.println("Hello from main thread");

        for (int i = 0; i < N; ++i)
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