package entities;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.*;
import entities.ProviderTerminal;
import entities.ManagerTerminal;
import entities.OperatorTerminal;

@SuppressWarnings("unused")

// TODO: Change providerMenu() and managerMenu() to take in a Storage and a 
// Provider/Manager (respectively) as parameters.
public class Menu {
	
	/**
	 * @author ljhay
	 *
	 */
	public void mainMenu() throws FileNotFoundException { 
		Storage s = new Storage();
		s.fileToArray();
		Scanner sc = new Scanner(System.in);
		while(true) {
			System.out.println("\n---------------------------------------\n");
			System.out.println("What is your position? (Input correspondng number)");
			System.out.println("1. Provider");
			System.out.println("2. Manager");
			System.out.println("3. Operator");
			System.out.println();
			System.out.println("4. Shut Down Terminal");
			
			int input = sc.nextInt();
			if(input == 1) {
				verifyProvider(sc, s);
					
					// Each terminal class will have its own menu to reduce coupling as much as possible.
					// Once the user is verified as a provider, create a new provider terminal object
					// and call its main menu.
			} else if (input == 2) {
				if(verifyManager(sc, s)) {
					ManagerTerminal mt = new ManagerTerminal();
					try {
						mt.makeReport(sc, s);
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			} else if (input == 3) {
					OperatorTerminal ot = new OperatorTerminal();
					ot.operatorMenu(sc, s);
			} else if(input == 4){
				sc.close();
				try {
					s.arrayToFile();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
				System.out.println("Shutting Down");
				return;
			} else {
				System.out.println("Invalid Input");
				System.out.println();
			}
		}
	}
	
	private boolean verifyProvider(Scanner sc, Storage s){
		System.out.print("Enter 9 digit Provider ID: ");
		int input = sc.nextInt();
	    String temp = Integer.toString(input);
	    
	    while(temp.length() != 9) {
        	System.out.println("Invalid provider number, must be 9 numbers long");
    		System.out.println("Enter provider's ID: ");
        	input = sc.nextInt();
        	temp = Integer.toString(input);
        }
		
			for (Provider currentProv : s.providerList) {
				if (currentProv.getNumber() == input) {
					ProviderTerminal provterm = new ProviderTerminal();
					provterm.providerMenu(sc, s, currentProv);
					return true;
				}
			}
        	System.out.println("Invalid provider number");
			return false;
	
	}

	/**
	 * @author ljhay
	 *
	 */
	private boolean verifyManager(Scanner sc, Storage s){
		System.out.print("Enter Manager ID: ");
		int input = sc.nextInt();
	    String temp = Integer.toString(input);
	    while(temp.length() != 9) {
	        System.out.println("Invalid provider number, must be 9 numbers long");
	    	System.out.println("Enter manager ID: ");
	    	input = sc.nextInt();
	       	temp = Integer.toString(input);
	    }
	    for(Manager currentManager : s.managerList) {
			if (currentManager.getNumber() == input) {
				return true;
			}
		}        	
	    System.out.println("Invalid provider number");
		return false;	
	}
}
