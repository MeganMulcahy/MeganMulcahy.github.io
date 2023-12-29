
//author Megan Mulcahy: mgmulcahy

package entities;
import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

@SuppressWarnings("unused")
/*
 * Prints options to add delete update or sign out of the operator terminal. The terminal is looking for an integer input matching a corresponding option.
 */
public class OperatorTerminal {

	public void operatorMenu(Scanner sc, Storage s) { 

		while(true) {
			System.out.println("What is your position? (Input correspondng number)");
			System.out.println("1. Add Provider");
			System.out.println("2. Add Member");
			
			System.out.println("3. Delete Provider");
			System.out.println("4. Delete Member");
			
			System.out.println("5. Update Provider");
			System.out.println("6. Update Member");
			
			System.out.println("7. Sign Out");

			int input = sc.nextInt();
			
			if(input == 1) {
				//add provider
				addProvider(sc, s);
			}
			else if(input == 2) {
				//add member
				addMember(sc, s);
			}
			else if(input == 3) {
				//delete provider
				deleteProvider(sc, s);
			}
			else if(input == 4) {
				//delete member
				deleteMember(sc, s);
			}
			else if(input == 5) {
				//update provider
				updateProvider(sc, s);
			}
			else if(input == 6) {
				//update member
				updateMember(sc, s);
			}
			else if(input == 7) {
				//log out
				System.out.println("Logging out");
				return;
			}
			else {
				//invalid input
				System.out.println("Invalid selection. Please select a valid option.");
				System.out.println();
			}
		}
	}
	/*
	 * Validates provider number and zipcode length upon input, takes information from the input to be stored in the array of providers as prompted by the terminal.
	 */
	public void addProvider(Scanner sc, Storage s) {
		System.out.print("Enter provider's number: ");
        int number = sc.nextInt();
        String temp = Integer.toString(number);
        while(temp.length() != 9) {
        	System.out.println("Invalid provider number, must be 9 numbers long");
    		System.out.println("Enter provider's number: ");
        	number = sc.nextInt();
        	temp = Integer.toString(number);
        }
        System.out.print("Enter provider's name: ");
        sc.nextLine();
        String name = sc.nextLine();
        System.out.print("Enter provider's address: ");
        String address = sc.nextLine();
        System.out.print("Enter provider's city: ");
        String city = sc.nextLine();
        System.out.print("Enter provider's state: ");
        String state = sc.nextLine();
        System.out.print("Enter provider's email: ");
        String email = sc.nextLine();
        System.out.print("Enter provider's zip code: ");
        int zip = sc.nextInt();
        temp = Integer.toString(zip);
        while(temp.length() != 5) {
        	System.out.println("Invalid zip code, must be 5 numbers long");
            System.out.println("Enter provider's zip code: ");
            zip = sc.nextInt();
        	temp = Integer.toString(zip);
        }
        addProviderInfo(number, name, address, city, state, email, zip, s);
	}
	/*
	 * Validates member number length upon input, takes information from the input to be stored in the array of members as prompted by the terminal.
	 */
	public void addMember(Scanner sc, Storage s) {
		System.out.print("Enter member's number: ");
        int number = sc.nextInt();
        String temp = Integer.toString(number);
        while(temp.length() != 9) {
        	System.out.println("Invalid member number, must be 9 numbers long");
    		System.out.println("Enter member's number: ");
        	number = sc.nextInt();
        	temp = Integer.toString(number);
        }
        System.out.print("Enter member's name: ");
        sc.nextLine();
        String name = sc.nextLine();
        System.out.print("Enter member's address: ");
        String address = sc.nextLine();
        System.out.print("Enter member's city: ");
        String city = sc.nextLine();
        System.out.print("Enter member's state: ");
        String state = sc.nextLine();
        System.out.print("Enter member's email: ");
        String email = sc.nextLine();
        System.out.print("Enter member's zip code: ");
        int zip = sc.nextInt();
        temp = Integer.toString(zip);
        while(temp.length() != 5) {
        	System.out.println("Invalid zip code, must be 5 numbers long");
            System.out.println("Enter member's zip code: ");
            zip = sc.nextInt();
        	temp = Integer.toString(zip);
        }
        addMemberInfo(number, name, address, city, state, email, zip, 0, true, s);
        
	}
	/*
	 * Adds the infromation from "addProvider" to the array and prints a validation statement.	
	 */
	public void addProviderInfo(int number, String name, String address, String city, String state, String email, int zip, Storage s) {
		Provider newProvider = new Provider(number, name, address, city, state, zip, email);
	    s.providerList.add(newProvider);
	    System.out.println("Provider added");
	}
	/*
	 * Adds the infromation from "addMember" to the array and prints a validation statement.	
	 */
	public void addMemberInfo(int number, String name, String address, String city, String state, String email, int zip, int zero, boolean truth, Storage s) {
		Member newMember = new Member(number, name, address, city, state, email, zip, 0, true);
        s.memberList.add(newMember);
        System.out.println("Member added");
	}
	/*
	 * Validated the provider number is of length 9 (a valid number), and then calls the delete Provider info function.	
	 */
	public void deleteProvider(Scanner sc, Storage s) {
		System.out.println("Enter provider's number: ");
		int number = sc.nextInt();
		String temp = Integer.toString(number);
        while(temp.length() != 9) {
        	System.out.println("Invalid provider number, must be 9 numbers long");
    		System.out.println("Enter provider's number: ");
        	number = sc.nextInt();
        	temp = Integer.toString(number);
        }
        deleteProviderInfo(number, s);
	}
	/*
	 * Validated the members number is of length 9 (a valid number), and then calls the delete Member info function.	
	 */
	public void deleteMember(Scanner sc, Storage s) {
		System.out.println("Enter member's number: ");
		int number = sc.nextInt();
		String temp = Integer.toString(number);
        while(temp.length() != 9) {
        	System.out.println("Invalid member number, must be 9 numbers long");
    		System.out.println("Enter member's number: ");
        	number = sc.nextInt();
        	temp = Integer.toString(number);
        }
       deleteMemberInfo(number, s);
	}
	/*
	 * Validates that the number exists and is attached to a provider in the list of providers existing. If the provider does not
	 * exist the program returns an error statement and returns.
	 * If the provider is found it will be deleted from the array of providers. 	
	 */
	public void deleteProviderInfo(int number, Storage s) {
		int found = 0;
	       for(int i = 0; i < s.providerList.size();i++) {
	    	   Provider currentProv = s.providerList.get(i);
	    	   if(currentProv.getNumber() == number) {
	    		   found = 1;
	    		   System.out.println("Provider Found, deleting...");
	    		   //DELETE FROM ARRAYLIST?
	    		   s.providerList.remove(currentProv);
	    		   System.out.println("Provider, " + number + ", deleted");
	    	   }
	       }
	       if(found == 0) {
	    	   System.out.println("Provider number does not exist, enter a valid number");
	       }	
	}
	/*
	 * Validates that the number exists and is attached to a member in the list of providers existing. If the member does not
	 * exist the program returns an error statement and returns.
	 * If the member is found it will be deleted from the array of members. 	
	 */
	public void deleteMemberInfo(int number, Storage s) {
		int found = 0;
	       for(int i = 0; i < s.memberList.size();i++) {
	    	   Member currentMem = s.memberList.get(i);
	    	   if(currentMem.getNum() == number) {
	    		   found = 1;
	    		   System.out.println("Member Found, deleting...");
	    		   //DELETE FROM ARRAYLIST?
	    		   s.memberList.remove(currentMem);
	    		   System.out.println("Member, " + number + ", deleted");
	    	   }
	       }
	       if(found == 0) {
	    	   System.out.println("Member number does not exist, enter a valid number");
	       }	
	}
	/*
	 * Validates that the number exists and is attached to a provider in the list of providers existing. If the provider does not
	 * exist the program returns an error statement and returns.
	 * If the provider is found it will print the providers infromation and prompt you to chose which 
	 * infromation to update and will allow you to update it then print a validation of the updated infromation.	
	 */
	public void updateProvider(Scanner sc, Storage s) {
		System.out.println("Enter provider's number: ");
		int number = sc.nextInt();
		String temp = Integer.toString(number);
        while(temp.length() != 9) {
        	System.out.println("Invalid provider number, must be 9 numbers long");
    		System.out.println("Enter provider's number: ");
        	number = sc.nextInt();
        	temp = Integer.toString(number);
        }
       int found = 0;
       for(int i = 0; i < s.providerList.size();i++) {
    	   Provider currentProv = s.providerList.get(i);
    	   if(currentProv.getNumber() == number) {
    		   found = 1;
    		   System.out.println("Provider Found: " + currentProv.getName() + ", " + currentProv.getNumber() + ", " + currentProv.getAddress() + ", " + currentProv.getCity()+ ", " + currentProv.getState() + ", " + currentProv.getZipCode());
	    	   boolean exit = false;
    		   while(exit == false) {
	    			System.out.println("What do you want to update? (Input correspondng number)");
		   			System.out.println("1. Name");
		   			System.out.println("2. Number");
		   			System.out.println("3. Address");
		   			System.out.println("4. City");
		   			System.out.println("5. State");
		   			System.out.println("6. Zip");
		   			
		   			System.out.println("7. Exit");
		   			int input = sc.nextInt();
					
					if(input == 1) {
						System.out.println("Enter the new provider name: ");
			   	        sc.nextLine();
			   			String name = sc.nextLine();
			   			currentProv.setName(name);
			   			System.out.println("Provider " + currentProv.getNumber() + " new name: " + currentProv.getName());
					}
					else if(input == 2) {
						System.out.println("Enter the new provider number: ");
						int number2 = sc.nextInt();
						String temp2 = Integer.toString(number);
				        while(temp2.length() != 9) {
				        	System.out.println("Invalid provider number, must be 9 numbers long");
				    		System.out.println("Enter the new provider number: ");
				        	number2 = sc.nextInt();
				        	temp2 = Integer.toString(number);
				        }
			   			currentProv.setNumber(number2);
			   			System.out.println("Provider " + currentProv.getNumber() + " new number: " + currentProv.getNumber());

					}
					else if(input == 3) {
						System.out.println("Enter the new provider address: ");
			   	        sc.nextLine();
			   			String address = sc.nextLine();
			   			currentProv.setAddress(address);
			   			System.out.println("Provider " + currentProv.getNumber() + " new address: " + currentProv.getAddress());

					}
					else if(input == 4) {
						System.out.println("Enter the new provider city: ");
			   	        sc.nextLine();
			   	        String city = sc.nextLine();
			   			currentProv.setCity(city);
			   			System.out.println("Provider " + currentProv.getNumber() + " new city: " + currentProv.getCity());

					}
					else if(input == 5) {
						System.out.println("Enter the new provider state: ");
			   	        sc.nextLine();
			   	        String state = sc.nextLine();
			   			currentProv.setState(state);
			   			System.out.println("Provider " + currentProv.getNumber() + " new state: " + currentProv.getState());

					}
					else if(input == 6) {
						System.out.println("Enter the new provider zip: ");
			   	        sc.nextInt();
			   	        int zip = sc.nextInt();
			   			currentProv.setZipCode(zip);
			   			System.out.println("Provider " + currentProv.getNumber() + " new zip code: " + currentProv.getZipCode());

					}
					
					else if(input == 7) {
						System.out.println("Exiting 'Update Provider'");
						exit = true;
						return;
					}
					else {
						//invalid input
						System.out.println("Invalid selection. Please select a valid option.");
						System.out.println();
					}
	    		}
    		  
    	   }
       }
       if(found == 0) {
    	   System.out.println("Provider number does not exist, enter a valid number");
       }	
	}
	/*
	 * Validates that the number exists and is attached to a member in the list of members existing. If the member does not
	 * exist the program returns an error statement and returns.
	 * If the provider is found it will print the members infromation and prompt you to chose which 
	 * infromation to update and will allow you to update it then print a validation of the updated infromation.	
	 */
	public void updateMember(Scanner sc, Storage s) {
		System.out.println("Enter member's number: ");
		int number = sc.nextInt();
		String temp = Integer.toString(number);
        while(temp.length() != 9) {
        	System.out.println("Invalid member number, must be 9 numbers long");
    		System.out.println("Enter member's number: ");
        	number = sc.nextInt();
        	temp = Integer.toString(number);
        }
       int found = 0;
       for(int i = 0; i < s.memberList.size();i++) {
    	   Member currentMem = s.memberList.get(i);
    	   if(currentMem.getNum() == number) {
    		   found = 1;
    		   System.out.println("Member Found: " + currentMem.getName() + ", " + currentMem.getNum() + ", " + currentMem.getAddress() + ", " + currentMem.getCity()+ ", " + currentMem.getState() + ", " + currentMem.getZip());
	    	   boolean exit = false;
    		   while(exit == false) {
	    			System.out.println("What do you want to update? (Input correspondng number)");
		   			System.out.println("1. Name");
		   			System.out.println("2. Number");
		   			System.out.println("3. Address");
		   			System.out.println("4. City");
		   			System.out.println("5. State");
		   			System.out.println("6. Zip");
		   			
		   			System.out.println("7. Exit");
		   			int input = sc.nextInt();
					
					if(input == 1) {
						System.out.println("Enter the new member name: ");
			   	        sc.nextLine();
			   			String name = sc.nextLine();
			   			currentMem.setName(name);
			   			System.out.println("Member " + currentMem.getNum() + " new name: " + currentMem.getName());
					}
					else if(input == 2) {
						System.out.println("Enter the new member number: ");
						int number2 = sc.nextInt();
						String temp2 = Integer.toString(number);
				        while(temp2.length() != 9) {
				        	System.out.println("Invalid member number, must be 9 numbers long");
				    		System.out.println("Enter the new member number: ");
				        	number2 = sc.nextInt();
				        	temp2 = Integer.toString(number);
				        }
			   			currentMem.setNum(number2);
			   			System.out.println("Provider " + currentMem.getNum() + " new number: " + currentMem.getNum());

					}
					else if(input == 3) {
						System.out.println("Enter the new Member address: ");
			   	        sc.nextLine();
			   			String address = sc.nextLine();
			   			currentMem.setAddress(address);
			   			System.out.println("Member " + currentMem.getNum() + " new address: " + currentMem.getAddress());

					}
					else if(input == 4) {
						System.out.println("Enter the new Member city: ");
			   	        sc.nextLine();
			   	        String city = sc.nextLine();
			   			currentMem.setCity(city);
			   			System.out.println("Member " + currentMem.getNum() + " new city: " + currentMem.getCity());

					}
					else if(input == 5) {
						System.out.println("Enter the new Member state: ");
			   	        sc.nextLine();
			   	        String state = sc.nextLine();
			   			currentMem.setState(state);
			   			System.out.println("Member " + currentMem.getNum() + " new state: " + currentMem.getState());

					}
					else if(input == 6) {
						System.out.println("Enter the new Member zip: ");
			   	        sc.nextInt();
			   	        int zip = sc.nextInt();
			   			currentMem.setZip(zip);
			   			System.out.println("Member " + currentMem.getNum() + " new zip code: " + currentMem.getZip());

					}
					
					else if(input == 7) {
						System.out.println("Exiting 'Update Member'");
						exit = true;
						return;
					}
					else {
						//invalid input
						System.out.println("Invalid selection. Please select a valid option.");
						System.out.println();
					}
	    		}
    		  
    	   }
       }
       if(found == 0) {
    	   System.out.println("Member number does not exist, enter a valid number");
       }	
	}
}
