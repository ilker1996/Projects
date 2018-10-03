public class CengHashRow {

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	CengBucket bucket;
    int hashPrefix;
    boolean visited;
    public CengHashRow(){
    	bucket = new CengBucket();
    	hashPrefix = 0;
    	visited = false;
    }
    public void print(){
        System.out.println("<row>");
        for(int i = 0; i < bucket.coinList.size() ; i++)
        {
            System.out.println("\t"+bucket.coinAtIndex(i).fullName());
        }
        System.out.println("<row");
    }
	public String hashPrefix()
	{
		// TODO: Return row's hash prefix (such as 0, 01, 010, ...)
		return Integer.toBinaryString(this.hashPrefix);		
	}
	
	public CengBucket getBucket()
	{
		// TODO: Return the bucket that the row points at.
		return this.bucket;		
	}
	
	public boolean isVisited()
	{
		// TODO: Return whether the row is used while searching.
		return this.visited;		
	}
	
	// Own Methods
}
