import java.util.HashMap;
import java.util.concurrent.ArrayBlockingQueue;

public class CommunicationChannel
{
	ArrayBlockingQueue<Message> spaceExplorerMessages = new ArrayBlockingQueue<>(20000);
	ArrayBlockingQueue<Message> headQuarterMessages = new ArrayBlockingQueue<>(20000);

	HashMap<Integer, Integer> parrentSolarSystems = new HashMap<Integer, Integer>();
	HashMap<Integer, Integer> currentSolarSystems = new HashMap<Integer, Integer>();
	HashMap<Integer, Integer> round = new HashMap<Integer, Integer>();

	public CommunicationChannel() {}

	public void putMessageSpaceExplorerChannel(Message message)
	{
		try
		{
			spaceExplorerMessages.put(message);
		}
		catch (Exception e) {}
	}

	public Message getMessageSpaceExplorerChannel()
	{
		try
		{
			return spaceExplorerMessages.take();
		}
		catch (Exception e) {}
		return null;
	}

	public void putMessageHeadQuarterChannel(Message message)
	{
		try
		{
			//synchronized (this)
			{
				Integer thread_id = (int) Thread.currentThread().getId();
				round.putIfAbsent(thread_id, 0);

				if (message.getData().equals("END"))
				{
					parrentSolarSystems.remove(thread_id);
					currentSolarSystems.remove(thread_id);
					round.put(thread_id, 0);
					return;
				}

				if (message.getData().equals("EXIT"))
				{
					headQuarterMessages.put(new Message(-2, message.getData()));
					return;
				}

				if (round.get(thread_id) % 2 == 0)
				{
					parrentSolarSystems.put(thread_id, message.getCurrentSolarSystem());
				}
				else
				{
					currentSolarSystems.put(thread_id, message.getCurrentSolarSystem());
					headQuarterMessages.put(new Message(parrentSolarSystems.get(thread_id),
							currentSolarSystems.get(thread_id),
							message.getData()));

					parrentSolarSystems.remove(thread_id);
					currentSolarSystems.remove(thread_id);
				}

				round.put(thread_id, round.get(thread_id) + 1);
			}
		}
		catch (Exception e) {}
	}

	public Message getMessageHeadQuarterChannel()
	{
		try
		{
			return headQuarterMessages.take();
		}
		catch (Exception e) {}
		return null;
	}
}
