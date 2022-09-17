
import java.util.ArrayList;

public class CengHashTable {
        private CengHashRow[] Table;
        int prefixBitCount,size;
        public CengHashTable()
	{
		// TODO: Create a hash table with only 1 row.
                this.Table = new CengHashRow[1];
                Table[0] = new CengHashRow();
                this.prefixBitCount = 0;
                this.size = 1;
	}
        public void IncrPrefixBitCount()
        {
            this.prefixBitCount++;
        }
        public void expandTable(CengCoin coinToAdd,int index)
        {   
            ArrayList<CengCoin> coinsToAdd = new ArrayList<CengCoin>();
            CengHashRow[] NewTable = new CengHashRow[size*2];
            for(int i = 0; i < size ;i++)
            {
                if(i == index)
                {   
                    NewTable[i*2] = new CengHashRow();
                    NewTable[i*2+1] = new CengHashRow();
                    NewTable[i*2].hashPrefix = i*2;
                    NewTable[i*2+1].hashPrefix = i*2+1;
                    NewTable[i*2].bucket.prefixlength = Table[index].bucket.prefixlength + 1 ;
                    NewTable[i*2+1].bucket.prefixlength = Table[index].bucket.prefixlength + 1;
                    for(int j=0;j < Table[index].bucket.coinList.size() ;j++)
                    {
                        coinsToAdd.add(Table[index].bucket.coinList.get(j));
                    }
                    
                }
                else
                {   
                    NewTable[i*2] = new CengHashRow();
                    NewTable[i*2+1] = new CengHashRow();
                    NewTable[i*2].bucket = Table[i].bucket;
                    NewTable[i*2+1].bucket = Table[i].bucket;
                    NewTable[i*2].hashPrefix = i *2;
                    NewTable[i*2+1].hashPrefix = i*2 +1;
                    
                }
            }
            Table = new CengHashRow[size*2];
            this.Table = NewTable;
            this.size = (this.size) * 2;
            this.IncrPrefixBitCount();
            coinsToAdd.add(coinToAdd);
            for(int i = 0; i < coinsToAdd.size();i++)
            {
                this.addCoin(coinsToAdd.get(i));
            }
        }
        public void redistribute(CengCoin coinToAdd,int index,int prefixLength)
        {   
            ArrayList<CengCoin> coinsToAdd = new ArrayList<CengCoin>();
            int mod = (int) Math.pow(2,((this.prefixBitCount) - (prefixLength)));
            int startIndex = index - (index % mod );
            int endIndex = startIndex + mod;
            CengBucket tmp1 = new CengBucket();
            CengBucket tmp2 = new CengBucket();
            tmp1.prefixlength = prefixLength + 1;
            tmp2.prefixlength = prefixLength +1 ;
            for(int j = 0; j < Table[index].bucket.coinList.size();j++)
            {
                coinsToAdd.add(Table[index].bucket.coinAtIndex(j));
            }
            for(int i = 0; i < size ;i++)
            {
                if(i >= startIndex && i < (endIndex + startIndex)/2 )
                {   
                    Table[i].bucket = tmp1;
                }
                else if(i < endIndex && i >= (endIndex + startIndex)/2 )
                {   
                    Table[i].bucket = tmp2;
                }
                else{
                    continue;
                }
            }
            coinsToAdd.add(coinToAdd);
            for(int i = 0; i< coinsToAdd.size();i++)
            {
                this.addCoin(coinsToAdd.get(i));
            }
            
        }
	public void addCoin(CengCoin coin)
	{			
		// TODO: Empty Implementation
                int key = coin.key();
                int hashedValue = key % (CengCoinExchange.getHashMod());
                int shiftAmount = (int) ((Math.log(CengCoinExchange.getHashMod()))/ Math.log(2)) - (this.prefixBitCount);
                int index = ((hashedValue >> shiftAmount) % ((int)Math.pow(2,this.prefixBitCount)));
                System.out.println("ADDCOIN ---> " + index  + "  " + prefixBitCount);
                if(Table[index].bucket.isLoaded())
                {   
                    System.out.println("Table bit --> " + prefixBitCount + "       bucket prefix length --> " + Table[index].bucket.prefixlength);
                    if((this.prefixBitCount) == (Table[index].bucket.prefixlength))/*Expand table*/
                    {
                        expandTable(coin,index);
                    }
                    else/*Redistribute buckets*/
                    {
                        redistribute(coin,index,Table[index].bucket.prefixlength);
                    }
                      
                }
                else/*Insert at the bucket*/
                {   
                    Table[index].bucket.addCoin(coin);
                }
        }
	
	public void searchCoin(Integer key)
	{
		// TODO: Empty Implementation
                int hashedValue = key % (CengCoinExchange.getHashMod());
                int shiftAmount = (int) Math.sqrt(CengCoinExchange.getHashMod()) - (this.prefixBitCount);
                int index = ((hashedValue >> shiftAmount) % ((int) Math.pow(2,this.prefixBitCount)));
            
	}
	
	public void print()
	{
		// TODO: Empty Implementation
                for(int i = 0; i < size ;i ++){
                    Table[i].print();
                }
	}

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int prefixBitCount()
	{
		// TODO: Return table's hash prefix length.
		return prefixBitCount;		
	}
	
	public int rowCount()
	{
		// TODO: Return the count of HashRows in table.
		return size;		
	}
	
	public CengHashRow rowAtIndex(int index)
	{
		// TODO: Return corresponding hashRow at index.
		return Table[index];
	}
	
	// Own Methods
}
