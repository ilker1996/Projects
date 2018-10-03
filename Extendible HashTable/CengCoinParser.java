import java.io.IOException;
import java.util.ArrayList;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.logging.Level;
import java.util.logging.Logger;
import java.io.InputStreamReader;

public class CengCoinParser {

	public static ArrayList<CengCoin> parseCoinsFromFile(String filename)
	{
            try {
                ArrayList<CengCoin> coinList = new ArrayList<CengCoin>();
                
                // You need to parse the input file in order to use GUI tables.
                // TODO: Parse the input file, and convert them into CengCoins
                BufferedReader reader = null;
                reader = new BufferedReader(new FileReader(filename));
                String currentLine;
                while((currentLine = reader.readLine()) != null){/*LOOK AGAIN FOR THE BLANK LINE AT THE END OF THE FILE*/
                    String[] pair = currentLine.split("\\|");
                    int key = Integer.parseInt(pair[1]);
                    String name = pair[2];
                    String value = pair[3];
                    String currency =  pair[4];
                    coinList.add(new CengCoin(key,name,value,currency));
                }
                    reader.close();
                    return coinList;
                
            } catch (FileNotFoundException ex) {
                Logger.getLogger(CengCoinParser.class.getName()).log(Level.SEVERE, null, ex);
            } catch (IOException ex) {
                Logger.getLogger(CengCoinParser.class.getName()).log(Level.SEVERE, null, ex);
            }
            return null;
        }
	
	public static void startParsingCommandLine() throws IOException
	{
		// TODO: Start listening and parsing command line -System.in-.
		// There are 4 commands:
		// 1) quit : End the app, gracefully. Print nothing, call nothing.
		// 2) add : Parse and create the coin, and call CengCoinExchange.addCoin(newlyCreatedCoin).
		// 3) search : Parse the key, and call CengCoinExchange.searchCoin(parsedKey).
		// 4) print : Print the whole hash table with the corresponding buckets, call CengCoinExchange.printEverything().

		// Commands (quit, add, search, print) are case-insensitive.
            BufferedReader br = null;

            try {
               
                while (true) {
                    br = new BufferedReader(new InputStreamReader(System.in));
                    String line = br.readLine();
                    String[] input = line.split("\\|");
                    switch (input[0]) {
                        case "quit":
                            System.exit(0);
                            break;
                        case "add":
                            CengCoinExchange.addCoin(new CengCoin(Integer.parseInt(input[1]),input[2],input[3],input[4]));
                            break;
                        case "search":
                            CengCoinExchange.searchCoin(Integer.parseInt(input[1]));
                            break;
                        case "print":
                            CengCoinExchange.printEverything();
                            break;
                        default:
                            System.out.println("THIS IS NOT A GOOD COMMAND!");
                            System.exit(0);
                    }
         
                }

            }catch (IOException e) {
                e.printStackTrace();
            } finally {
                if (br != null) {
                    try {
                        br.close();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }
        }
}
