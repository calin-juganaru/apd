package queens;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Main
{
    static int N = 4;
    static final Object shipi = new Object();

    public static boolean condition(int[] v, int end)
    {
        for (int i = 0; i < end; i++)
        {
            if (v[i] == v[end] ||
                v[i] - v[end] == (end - i) ||
                v[end] - v[i] == (end - i))
                    return false;
        }
        return true;
    }

    public static void print(int[] v)
    {
        Lock l = new ReentrantLock();
        l.lock();
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (v[i] == j)
                     System.out.print("Q ");
                else System.out.print("* ");
            }
            System.out.println();
        }
        System.out.println();
        l.unlock();
    }

    public static void main(String[] args)
    {
        ExecutorService tpe = Executors.newFixedThreadPool(5);
        int[] v1 = new int[N];
        int[] v2 = new int[N];
        int[] v3 = new int[N];
        int[] v4 = new int[N];

        v1[0] = 1; v2[0] = 2;
        v3[0] = 3; v4[0] = 0;
        tpe.submit(new MyRunnable(tpe, v1, 1));
        tpe.submit(new MyRunnable(tpe, v2, 1));
        tpe.submit(new MyRunnable(tpe, v3, 1));
        tpe.submit(new MyRunnable(tpe, v4, 1));
    }

}

class MyRunnable implements Runnable
{
    int k;
    int[] v;
    ExecutorService tpe;

    public MyRunnable(ExecutorService tpe, int[] v, int k)
    {
        this.tpe = tpe;
        this.v = v;
        this.k = k;
    }

    @Override
    public void run()
    {
        if (k == Main.N)
        {
            synchronized(Main.shipi)
            {
                Main.print(v);
            }
            tpe.shutdown();
            return;
        }
        else
        {
            for (int i = 0; i < Main.N; i++)
            {
                int aux = v[k];
                v[k] = i;
                if (Main.condition(v, k))
                {
                    int[] u = v.clone();
                    tpe.submit(new MyRunnable(tpe, u, k + 1));
                }
                v[k] = aux;
            }
        }
    }
}