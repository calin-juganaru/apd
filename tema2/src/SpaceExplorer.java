import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Set;
import java.nio.charset.StandardCharsets;

public class SpaceExplorer extends Thread
{
	Integer hashCount;
	Set<Integer> discovered;
	CommunicationChannel channel;

	public SpaceExplorer(Integer hashCount, Set<Integer> discovered, CommunicationChannel channel)
	{
		this.hashCount = hashCount;
		this.discovered = discovered;
		this.channel = channel;
	}

	@Override
	public void run()
	{
		while (true)
		{
			Message message = this.channel.getMessageHeadQuarterChannel();
			String freqs = message.getData();

			if (freqs.contains("EXIT")) return;

			if (!discovered.contains(message.getCurrentSolarSystem()))
			{
				String decrypted = encryptMultipleTimes(freqs, hashCount);

				Message aux = new Message(message.getParentSolarSystem(),
						message.getCurrentSolarSystem(), decrypted);

				synchronized (discovered)
				{
					discovered.add(message.getCurrentSolarSystem());
				}
				this.channel.putMessageSpaceExplorerChannel(aux);
			}
		}
	}

	private String encryptMultipleTimes(String input, Integer count) {
		String hashed = input;
		for (int i = 0; i < count; ++i) {
			hashed = encryptThisString(hashed);
		}

		return hashed;
	}

	private static String encryptThisString(String input) {
		try {
			MessageDigest md = MessageDigest.getInstance("SHA-256");
			byte[] messageDigest = md.digest(input.getBytes(StandardCharsets.UTF_8));

			// convert to string
			StringBuffer hexString = new StringBuffer();
			for (int i = 0; i < messageDigest.length; i++) {
				String hex = Integer.toHexString(0xff & messageDigest[i]);
				if (hex.length() == 1)
					hexString.append('0');
				hexString.append(hex);
			}
			return hexString.toString();

		} catch (NoSuchAlgorithmException e) {
			throw new RuntimeException(e);
		}
	}
}
