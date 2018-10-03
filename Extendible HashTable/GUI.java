import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.ArrayList;

import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.ListSelectionModel;
import javax.swing.SwingUtilities;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.DefaultTableModel;

public class GUI {

	private JFrame frame;
	
	private JTable currentCoinTable;
	private JTable alreadyAddedCoinTable;
	
	private static final Object[] columnNames = new Object[]{"Key", "Name", "Value", "Currency"};
	private static final int horizontalSpacingBetween = 50;
	
	private ArrayList<CengCoin> currentCoins;
	private ArrayList<CengCoin> addedCoins;

	private JPanel bucketListPanel;
	private JPanel tablesPanel;
	
	private GUIConnectorContainer connectorContainerPanel;
	
	private GUIHashTable guiHashTable;
	private GUIBucketList guiBucketList;
	
	public GUI()
	{
		initialize();		
	}
	
	public void show()
	{
		frame.setVisible(true);
	}
	
	public void modelNeedsUpdate()
	{
		// System.out.println("Updating model...");
		
		updateBucketList();
		updateBuckets();
	}
	
	// Private methods
	
	private void updateBucketList()
	{
		frame.remove(bucketListPanel);
		addBucketListPanel();
		
		frame.getContentPane().revalidate();
		frame.getContentPane().repaint();
		
		frame.pack();
	}
	
	private void updateBuckets()
	{
		ArrayList<GUIHashRow> guiRows = guiHashTable.getHashRows();
		
		for(GUIHashRow guiHashRow : guiRows)
		{			
			CengBucket bucket = guiHashRow.bucketForRow();
			
			if (bucket != null)
			{
				GUIBucket guiBucket = guiBucketList.guiForBucket(bucket);
				
				if (guiBucket != null)
				{
					connectorContainerPanel.link(guiHashRow, guiBucket);
				}
			}
		}		
	}
	
	private void initialize()
	{
		currentCoins = new ArrayList<CengCoin>();
		addedCoins = new ArrayList<CengCoin>();
		
		constructFrame();
		
		addBucketListPanel();
		addTablesPanel();
		
		addCurrentTable();
		addAlreadyAddedTable();
		
		updateBuckets();
		
        frame.pack();
	}
	
	private void constructFrame()
	{
		frame = new JFrame();
		frame.setSize(512, 384);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.getContentPane().setLayout(new BoxLayout(frame.getContentPane(), BoxLayout.X_AXIS));	
	}
	
	private void addBucketListPanel()
	{
		bucketListPanel = new JPanel();
		bucketListPanel.setLayout(new BoxLayout(bucketListPanel, BoxLayout.X_AXIS));
		
		CengHashTable hashTable = CengCoinExchange.getHashTable();
		guiHashTable = new GUIHashTable(hashTable);
		
		bucketListPanel.add(guiHashTable);
		
		// Add spacing between table and the buckets
		
		bucketListPanel.add(Box.createRigidArea(new Dimension(horizontalSpacingBetween, 0)));

		CengBucketList bucketList = CengCoinExchange.getBucketList();
		guiBucketList = new GUIBucketList(bucketList);
		
		bucketListPanel.add(guiBucketList);
		
		connectorContainerPanel = new GUIConnectorContainer();
		connectorContainerPanel.setLayout(null);
				
		frame.setGlassPane(connectorContainerPanel);
		
		connectorContainerPanel.setVisible(true);
		
		frame.getContentPane().add(bucketListPanel, 0);
	}
	
	private void addTablesPanel()
	{
		tablesPanel = new JPanel();
		tablesPanel.setLayout(new BoxLayout(tablesPanel, BoxLayout.Y_AXIS));
		
		frame.getContentPane().add(tablesPanel);
	}
	
	private void addCurrentTable()
	{
		JPanel tablePanel = new JPanel();
		tablePanel.setLayout(new BoxLayout(tablePanel, BoxLayout.Y_AXIS));
				
		DefaultTableModel currentGiftTableModel = new DefaultTableModel(columnNames, 0);

		currentCoinTable = new JTable(currentGiftTableModel)
		{
			// Anon class
			private static final long serialVersionUID = 1L;

			public boolean isCellEditable(int row, int column) 
	        {
                return false;               	
	        };
		};
		
		modifyTable(currentCoinTable);
		
		ArrayList<CengCoin> inputCoins = CengCoinParser.parseCoinsFromFile(CengCoinExchange.getInputFileName());
		
		for(CengCoin coin : inputCoins)
		{
			addCoinToTable(coin, currentCoinTable);
		}
		
		if(currentCoins.size() == 0)
		{
			currentCoins.addAll(inputCoins);
		}
		
        final JButton addButton = new JButton("Add Selected Coin");
        
        addButton.addActionListener(new ActionListener() 
        {
			@Override
			public void actionPerformed(ActionEvent e) 
			{
				addSelectedCoin();
			}
        });
        
        addButton.setAlignmentX(Component.CENTER_ALIGNMENT);
        tablePanel.add(addButton);
        tablePanel.add(currentCoinTable.getTableHeader());
		tablePanel.add(currentCoinTable);
	
		JScrollPane scrollPaneCurrentGifts = new JScrollPane(tablePanel);
		
		// Change width manually
        // scrollPaneCurrentGifts.getViewport().setPreferredSize(new Dimension(400, scrollPaneCurrentGifts.getViewport().getPreferredSize().height));

		tablesPanel.add(scrollPaneCurrentGifts);
	}
	
	private void addAlreadyAddedTable()
	{
		JPanel tablePanel = new JPanel();
		tablePanel.setLayout(new BoxLayout(tablePanel, BoxLayout.Y_AXIS));
		
		DefaultTableModel addedGiftTableModel = new DefaultTableModel(columnNames, 0);
		
		alreadyAddedCoinTable = new JTable(addedGiftTableModel)
		{
			// Anon class
			private static final long serialVersionUID = 1L;

			public boolean isCellEditable(int row, int column) 
	        {
                return false;               	
	        };
		};
		
		modifyTable(alreadyAddedCoinTable);

		JPanel buttonPanel = new JPanel();
		buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.X_AXIS));

		final JButton searchButton = new JButton("Search Selected Coin");
        
        searchButton.addActionListener(new ActionListener() 
        {
			@Override
			public void actionPerformed(ActionEvent e) 
			{
				searchSelectedCoin();
			}
        });
        
        searchButton.setAlignmentX(Container.CENTER_ALIGNMENT);
        
        buttonPanel.add(searchButton);
        
        tablePanel.add(buttonPanel);
        
        tablePanel.add(alreadyAddedCoinTable.getTableHeader());
		tablePanel.add(alreadyAddedCoinTable);
		
		JScrollPane scrollPaneAddedGifts = new JScrollPane(tablePanel);
		
		frame.getContentPane().add(scrollPaneAddedGifts);
	}
	
	private void addSelectedCoin()
	{		
		if(currentCoinTable.getSelectedRow() == -1)
		{			
			return;
		}
		
		CengCoin selectedCoin = getSelectedCoinFromTable(currentCoinTable);
		
		System.out.println("add|" + selectedCoin.fullName()); // Only for visual purposes
		
		CengCoinExchange.addCoin(selectedCoin);
		
		currentCoins.remove(selectedCoin);
		
		addCoinToTable(selectedCoin, alreadyAddedCoinTable);
		
		addedCoins.add(selectedCoin);
		
		removeSelectionFromTable(currentCoinTable);
	}

	private void searchSelectedCoin()
	{
		if(alreadyAddedCoinTable.getSelectedRow() == -1)
		{
			return;
		}
		
		CengCoin selectedCoin = getSelectedCoinFromTable(alreadyAddedCoinTable);
		
		System.out.println("search|" + selectedCoin.key()); // Only for visual purposes

		CengCoinExchange.searchCoin(selectedCoin.key());
	}
	
	private CengCoin getSelectedCoinFromTable(JTable table)
	{
		Integer selectedRowIndex = table.getSelectedRow();
		
		if(table.equals(currentCoinTable))
		{
			return currentCoins.get(selectedRowIndex);
		}
		else
		{
			return addedCoins.get(selectedRowIndex);
		}
	}
	
	// Table Handlers
	
	private void addCoinToTable(CengCoin coin, JTable table)
	{
		DefaultTableModel tableModel = (DefaultTableModel) table.getModel();

		tableModel.addRow(new Object[]{coin.key(), coin.name(), coin.value(), coin.currency()});
	}
	
	private void removeSelectionFromTable(JTable table)
	{
		Integer selectedRowIndex = table.getSelectedRow();

		DefaultTableModel tableModel = (DefaultTableModel) table.getModel();
		tableModel.removeRow(selectedRowIndex);		
	}
	
	private void modifyTable(final JTable table)
	{
		table.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		
		final DefaultTableModel tableModel = (DefaultTableModel) table.getModel();

	    	if(tableModel.getRowCount() != 0)
	    	{
	    		// Initialize table with first row selected.
	        	table.addRowSelectionInterval(0, 0);
	    	}
    	
		tableModel.addTableModelListener(new TableModelListener() {      
		    @Override
		    public void tableChanged(TableModelEvent e) 
		    {
		        SwingUtilities.invokeLater(new Runnable() 
		        {
		            @Override
		            public void run()
		            {
			            	if(tableModel.getRowCount() != 0)
			            	{
				            	table.addRowSelectionInterval(0, 0);
			            	}
		            }
		        });
		    }
		});
	}
}
