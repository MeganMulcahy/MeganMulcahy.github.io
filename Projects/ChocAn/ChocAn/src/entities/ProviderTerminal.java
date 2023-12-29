package entities;
import java.util.*;
import java.time.format.DateTimeFormatter;  
import java.time.LocalDateTime;    

@SuppressWarnings("unused")

public class ProviderTerminal {
	private Provider providerUser;
	
	public void providerMenu(Scanner sc, Storage s, Provider p) {
		providerUser = p;
		boolean logout = false;
		while (logout != true)
		{
			System.out.println("What would you like to do? (Input correspondng number)");
			System.out.println("1. Validate Member");
			System.out.println("2. Provide Service");
			System.out.println("3. Email Provider Directory");
			System.out.println("4. Log Out");
			int input = sc.nextInt();
			
			if (input == 1) {
				System.out.println("Please enter the 9-digit member number:");
				int memNum = sc.nextInt();
				int temp = verifyMember(memNum, s);
				if (temp == 0) {
					System.out.println(memNum + " is not a valid member number.");
				}
				else if (temp == 1) {
					for (Member currentMem : s.memberList) {
						if (currentMem.getNum() == memNum)
						{
							System.out.println(currentMem.getName() + " is a valid, non-suspended member.");
						}
					}
				}
				else {
					for (Member currentMem : s.memberList) {
						if (currentMem.getNum() == memNum)
						{
							System.out.println(currentMem.getName() + " is a valid, but SUSPENDED member who owes $" + currentMem.getAmountOwed() + " in fees.");
						}
					}
				}
			} else if (input == 2) {
				provideService(sc, s);

			} else if (input == 3) {
				requestDirectory(s);
				
			} else if (input == 4) {
				System.out.println("Logging out " + providerUser.getName() +".");
				logout = true;
			} else {
				System.out.println("Invalid selection. Please select a valid option.");
			}
		}
	}
	
	// Use this function for unit testing
	public int verifyMember(int memNum, Storage s) {
		// Returns 0 for INVALID, 1 for VALID, otherwise returns the amount owed (e.x. 1443).
		for (Member currentMember : s.memberList) {
			if (currentMember.getNum() == memNum) {
				if (currentMember.getStatus() == true) {
					return 1;
				}
				else {
					return currentMember.getAmountOwed();
				}
			}
		}
		return 0; // Couldn't find a member with that number!
	}
	
	public void provideService(Scanner sc, Storage s) {
		ServiceRecord holdingSR = new ServiceRecord();
		String holdingString = null;
		String holdingDate = null;
		DateTimeFormatter dtf = DateTimeFormatter.ofPattern("MM-dd-yyyy HH:mm:ss");  
		LocalDateTime now = LocalDateTime.now();
		System.out.println("Current time is " + dtf.format(now) + ".");
		holdingSR.setCurrentDateTime(dtf.format(now));
		System.out.println("Enter the date the service was performed.");
		boolean loopBreaker = true;
		while(loopBreaker) {
			System.out.println("What month (in 2-digit numerical form) was the service provided?:");
			holdingString = sc.next();
			if (holdingString.length() == 2 && Integer.parseInt(holdingString) >= 1 && Integer.parseInt(holdingString) <= 12) {
				holdingDate = holdingString + "-"; // set holdingDate equal to the month
				loopBreaker = false;
			}
			else {
				System.out.println("Error in month format. Please try again.");
			}
		}
		loopBreaker = true;
		while (loopBreaker) {
			System.out.println("What day (in 2-digit numerical form) was the service provided?");
			holdingString = sc.next();
			if (holdingString.length() == 2 && Integer.parseInt(holdingString) >= 1 && Integer.parseInt(holdingString) <= 31) {
				holdingDate = holdingDate + holdingString + "-"; // append the day
				loopBreaker = false;
			}
			else {
				System.out.println("Error in day format. Please try again.");
			}
		}
		loopBreaker = true;
		while(loopBreaker) {
			System.out.println("What year (in 4-digit numerical form) was the service provided?");
			holdingString = sc.next();
			if (holdingString.length() == 4 && Integer.parseInt(holdingString) >= 1900) {
				holdingDate = holdingDate + holdingString; //append the year
				loopBreaker = false;
			}
			else {
				System.out.println("Error in year format. Please try again.");
			}
		}
		holdingSR.setDateOfService(holdingDate);
		System.out.println("Found provider number :" + providerUser.getNumber());
		holdingSR.setProviderNumber(providerUser.getNumber());
		loopBreaker = true;
		while(loopBreaker) {
			System.out.println("Enter the member number (9 digits) of the patient recieving care:");
			holdingString = sc.next();
			if (holdingString.length() == 9 && Integer.parseInt(holdingString) >= 0 && Integer.parseInt(holdingString) <= 999999999) {
				for (Member currentMem : s.memberList) {
					if (currentMem.getNum() == Integer.parseInt(holdingString)) {
						holdingSR.setMemberNumber(Integer.parseInt(holdingString));
						loopBreaker = false;
					}
				}
			}
			else {
				System.out.println("Error in member number format or member not found. Please try again.");
			}
		}
		loopBreaker = true;
		while(loopBreaker) {
			System.out.println("Enter the service number (6 digits) of the service rendered:");
			holdingString = sc.next();
			if (holdingString.length() == 6 && Integer.parseInt(holdingString) >= 0 && Integer.parseInt(holdingString) <= 999999) {
				for (Service currentService : s.serviceList) {
					if (currentService.getServiceCode() == Integer.parseInt(holdingString)) {
						holdingSR.setServiceNumber(Integer.parseInt(holdingString));
						loopBreaker = false;
					}
				}
			}
			else {
				System.out.println("Error in service number format or service not found. Please try again.");
			}
		}
		System.out.println("If you have any comments, input them now, or press ENTER to skip:");
		holdingString = sc.next() + " ";
		holdingSR.setComments(holdingString);
		s.serviceRecords.add(holdingSR);
		System.out.println("Service Record added to ServiceRecord!");
	}
	
	public boolean addService() {
		
		return true;
	}
	
	public void requestDirectory(Storage s) {
		int i = 1;
		System.out.println("Format follows service code//service name//service fee.");
		for (Service currentServ : s.serviceList) {
			// System.out.println("TRIGGER REQUESTDIRECTORY");
			System.out.println("Service #" + i + ": " + currentServ.getServiceCode() + " // " + currentServ.getServiceName() + " // $" + currentServ.getServiceFee());
			i++;
		}
		System.out.println("Report emailed to " + providerUser.getEmail() + ".");
	}
	
	public void billChocAn() {
		
	}
}