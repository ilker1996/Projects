import java.awt.EventQueue;
import java.util.ArrayList;

public class CengCoinExchange {
	
	private static GUI window;
	private static Boolean guiEnabled;

	private static Integer hashMod = 8;
	private static Integer bucketSize = 2;
	private static String inputFileName;
	
	public static String getInputFileName() 
	{
		return inputFileName;
	}
	public static Integer getHashMod()
	{
		return hashMod;
	}
	public static Integer getBucketSize()
	{
		return bucketSize;
	}

	private static CengHashTable hashTable;
	private static CengBucketList bucketList;
	
	public static void main(String[] args) throws Exception 
	{		
		if(args.length != 4)
		{
			throw new Exception("Usage : java CengCoinExchange -hashMod- -bucketSize- -inputFileName- -guiEnabled (True or False)- \nExample usage: java CengCoinExchange 8 2 coins_20.txt True ");			
		}
		
		hashMod = Integer.parseInt(args[0]);
		bucketSize = Integer.parseInt(args[1]);
		inputFileName =args[2];
		guiEnabled = Boolean.parseBoolean(args[3]);
		
		if(guiEnabled) 
		{
			setGUI();
		}
				
		bucketList = new CengBucketList();
		hashTable = new CengHashTable();
		
		CengCoinParser.startParsingCommandLine();
                
	}

	public static CengHashTable getHashTable()
	{
		return CengCoinExchange.hashTable;
	}
	
	public static CengBucketList getBucketList()
	{
		return CengCoinExchange.bucketList;
	}
	
	public static void addCoin(CengCoin coin)
	{
		hashTable.addCoin(coin);		
		bucketList.addCoin(coin);
		
		updateWindow();
	}
	
	public static void searchCoin(Integer key)
	{		
		hashTable.searchCoin(key);
		bucketList.searchCoin(key);
		
		updateWindow();
	}
	
	public static void printEverything()
	{		
		hashTable.print();
		bucketList.print();
	}
	
	// GUI Methods

	private static void updateWindow()
	{
		if(guiEnabled)
		{
			if(window == null)
			{
				System.out.println("Err: Window is not initialized yet.");
				
				return;
			}
			
			window.modelNeedsUpdate();
		}
	}
		
	private static void setGUI() {
		EventQueue.invokeLater(new Runnable()
		{
			public void run() 
			{
				try 
				{
					window = new GUI();
					window.show();
				} 
				catch (Exception e) 
				{
					e.printStackTrace();
				}
			}
		});
	}
}
