import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.Semaphore;
import java.util.Comparator;

class ReaderThread extends Thread
{
    String fileName;

    public ReaderThread(String fileName)
    {
        this.fileName = fileName;
    }

    public void run()
    {
        try
        {
            BufferedReader br = new BufferedReader(new FileReader(this.fileName));
            String line;

            while ((line = br.readLine()) != null)
            {
                Main.list.add(Integer.valueOf(line));
            }
            br.close();
        }
        catch (IOException e) {}

        Main.semaphore.release();
    }
}

class SortThread extends Thread
{
    public void run()
    {
        try
        {
            Main.semaphore.acquire();
        }
        catch (InterruptedException e)
        {
            e.printStackTrace();
        }
        Collections.sort(Main.list);
    }
}

public class Main
{
    static Semaphore semaphore = new Semaphore(-2);
    static List<Integer> list = Collections.synchronizedList(new ArrayList<Integer>());

    public static void main(String[] args)
    {
        Thread threads[] = new Thread[4];

        threads[0] = new ReaderThread("elemente1.txt");
        threads[1] = new ReaderThread("elemente2.txt");
        threads[2] = new ReaderThread("elemente3.txt");
        threads[3] = new SortThread();

        for (int i = 0; i < 4; i++)
        {
            threads[i].start();
        }

        for (int i = 0; i < 4; i++)
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

        System.out.print(list);
    }
}
