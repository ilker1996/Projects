
import java.util.ArrayList;

public class CengBucket {

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
    ArrayList<CengCoin> coinList;
    boolean visited;
	int prefixlength;
    
    public CengBucket(){
    	visited = false;
    	prefixlength = 0;
    	coinList = new ArrayList<CengCoin>();
    }  
    public void addCoin(CengCoin coin)
    {
        coinList.add(coin);
    }
    public void IncrPrefixLength()
    {
        this.prefixlength++;
    }
	public int coinCount()
	{
		// TODO: Return the coin count in the bucket.
                return coinList.size();		
	}
	
	public CengCoin coinAtIndex(int index)
	{
		// TODO: Return the corresponding coin at the index.
                if(index < this.coinCount()){
                    return coinList.get(index);
                }
				else{
					System.out.println("CengBucket out of index");
					return null;
				}
	}
	
	public int getHashPrefix()
	{
		// TODO: Return hash prefix length.
		return this.prefixlength;
	}
	
	public Boolean isVisited()
	{
		// TODO: Return whether the bucket is found while searching.
		return this.visited;		
	}
        public Boolean isLoaded(){
            return (this.coinCount()) >= (CengCoinExchange.getBucketSize());
        }
	
	// Own Methods
}
