package parallelTree;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class ReadTreePart implements Runnable
{
	TreeNode tree;
	String fileName;

	public ReadTreePart(TreeNode tree, String fileName)
	{
		this.tree = tree;
		this.fileName = fileName;
	}

	@Override
	public void run()
	{
		try
		{
			BufferedReader br = new BufferedReader(new FileReader(this.fileName));
			String line;

			while ((line = br.readLine()) != null)
			{
				String tokens[] = line.split("\t");
				Integer to = Integer.valueOf(tokens[0]);
				Integer from = Integer.valueOf(tokens[1]);

				TreeNode parinte = tree.getNode(from);

				while (parinte == null)
				{
					parinte = tree.getNode(from);
				}

				synchronized (parinte)
				{
					parinte.addChild(new TreeNode(to));
				}
			}

			br.close();
		}
		catch (IOException e) {}

		try
		{
			Main.bar.await();
		}
		catch (Exception e)//(InterruptedException | BrokenBarrierException e)
		{
			e.printStackTrace();
		}
	}
}