package entities;
import java.io.IOException;
import java.util.*;

/**
 * @author ljhay
 *
 */
public class ManagerTerminal {
	
	public void makeReport(Scanner sc, Storage s) throws IOException {
		Report r = new Report();
	
		while(true) {
			System.out.println("\n\n---------------------------------------\n\n");
			System.out.println("Which report do you wish to run? (Input correspondng number)");
			System.out.println("1. Summary Report");
			System.out.println("2. Provider Report");
			System.out.println("3. Member Report");
			System.out.println("4. Eft Data");
			System.out.println();
			System.out.println("5. Main Accounting Procedure");
			System.out.println();
			System.out.println("6. Sign Out");
		
			int input = sc.nextInt();
	
			if(input == 1) {
				r.generateSummaryReport(s);
			
			} else if(input == 2) {
				r.generateProviderReport(s);
		
			} else if(input == 3) {
				r.generateMemberReport(s);
		
			} else if(input == 4) {
				r.generateEFTData(s);
			
			} else if(input == 5) {
				r.generateSummaryReport(s);
				r.generateProviderReport(s);
				r.generateMemberReport(s);
				r.generateEFTData(s);
				
			} else if(input == 6) {
				return;
				
			} else {
				System.out.println("Invalid Input");
				System.out.println();
			}
		}
	}
	
}