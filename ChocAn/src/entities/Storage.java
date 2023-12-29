package entities;
import java.io.*;
import java.util.*;
import java.nio.charset.StandardCharsets;
import java.net.URL;

@SuppressWarnings("unused")

public class Storage {
	public ArrayList<Member> memberList; // A list of all Members.
	public ArrayList<Provider> providerList; // A list of all Providers.
	public ArrayList<Manager> managerList; // A list of all Managers.
	public ArrayList<ServiceRecord> serviceRecords; // The list of all services rendered this week
	public ArrayList<Service> serviceList;
	
	
	public void fileToArray() throws FileNotFoundException {
		memberList = new ArrayList<Member>();
		fillMeL();
		
		providerList = new ArrayList<Provider>();
		fillPL();
		
		managerList = new ArrayList<Manager>();
		fillMaL();
		
		serviceRecords = new ArrayList<ServiceRecord>();
		fillSR();
		
		serviceList = new ArrayList<Service>();
	}
	
	
	/**
	 * @author ljhay Edited for optimization and JAR File by Megan Mulcahy
	 *
	 */
	private void fillMeL() throws FileNotFoundException {
			System.out.println("Beginning to fill Member List...");
	        InputStream inputStream = Storage.class.getResourceAsStream("/entities/memberlist.txt");
	        Scanner myReader = new Scanner(inputStream);
	        String[] holdingArray;
	
	        while (myReader.hasNextLine()) {
	            holdingArray = myReader.nextLine().split(",");
		    	  
		    	  int number = Integer.parseInt(holdingArray[0]);
		    	  String name = holdingArray[1];
		    	  String address = holdingArray[2];
		    	  String city = holdingArray[3];
		    	  String state = holdingArray[4];
		    	  String email = holdingArray[5];
		    	  int zip = Integer.parseInt(holdingArray[6]);
		    	  int owed = Integer.parseInt(holdingArray[7]);
		    	  int StatusNumber = Integer.parseInt(holdingArray[8]);
		    	  boolean status;
		    	  if(StatusNumber == 0) {
		    		  status = false;
		    	  } else {
		    		  status = true;
		    	  }
		    	  
		    	  Member newMember = new Member(number, name, address, city, state, email, zip, owed, status);
		    	  
		    	  memberList.add(newMember);
		      }  
		      myReader.close();
		      System.out.println("memberList complete.\n");
		}
	
	private void fillPL() throws FileNotFoundException {
		System.out.println("Beginning to fill ProviderList...");
        InputStream inputStream = Storage.class.getResourceAsStream("/entities/providerlist.txt");
        Scanner plScanner = new Scanner(inputStream);
        String[] holdingArray;
		
		while (plScanner.hasNextLine()) {
			Provider holdingProvider = new Provider();

			holdingArray = plScanner.nextLine().split(",");
			
			holdingProvider.setNumber(Integer.parseInt(holdingArray[0]));
			holdingProvider.setName(holdingArray[1]);
			//System.out.println(holdingArray[1]);
			holdingProvider.setAddress(holdingArray[2]);
			holdingProvider.setCity(holdingArray[3]);
			holdingProvider.setState(holdingArray[4]);
			holdingProvider.setZipCode(Integer.parseInt(holdingArray[5]));
			holdingProvider.setEmail(holdingArray[6]);
			providerList.add(holdingProvider);
		}
		plScanner.close();
		System.out.println("ProviderList complete.\n");
//		for(Provider currentProv: providerList) {
//			   System.out.println(currentProv.getName());
//
//	       }
	}
	

	/**
	 * @author ljhay & Edited for optimization and JAR File by Megan Mulcahy
	 *
	 */
	private void fillMaL() throws FileNotFoundException {
		System.out.println("Beginning to fill Manager List...");
		  InputStream inputStream = Storage.class.getResourceAsStream("/entities/managerList.txt");
	      Scanner myReader = new Scanner(inputStream);
	      String[]holdingArray;
	      
	      while (myReader.hasNextLine()) {
	    	  holdingArray = myReader.nextLine().split(",");  
	    	  Manager newManager = new Manager(holdingArray[0], Integer.parseInt(holdingArray[1]));
	    	  managerList.add(newManager);
	      }  
	      myReader.close();
	      System.out.println("managerList Complete.\n");
	}


	private void fillSL() throws FileNotFoundException {
		System.out.println("Beginning to fill Service List (provider directory)...");
        InputStream inputStream = Storage.class.getResourceAsStream("/entities/serviceList.txt");
		Scanner slScanner = new Scanner(inputStream);
		String[] holdingArray;
		
		while (slScanner.hasNextLine()) {
			Service currentService = new Service();
			holdingArray = slScanner.nextLine().split(",");
			
			currentService.setServiceCode(Integer.parseInt(holdingArray[0]));
			currentService.setServiceName(holdingArray[1]);
			currentService.setServiceFee(Integer.parseInt(holdingArray[2]));
			
			serviceList.add(currentService);
		}
		
		slScanner.close();
		System.out.println("Service List is complete.");
	}
	
	/*
	 	Author: Brooke Boskus & Megan Mulcahy
	 	fills array with information from given file "serviceRecords"
	*/
	private void fillSR() throws FileNotFoundException {
		System.out.println("Beginning to fill Service Record...");
        InputStream inputStream = Storage.class.getResourceAsStream("/entities/serviceRecords.txt");
		Scanner srScanner = new Scanner(inputStream);
		String[] holdingArray;
				
		while(srScanner.hasNextLine()){
			ServiceRecord currentService = new ServiceRecord();
			holdingArray = srScanner.nextLine().split(",");
			
			currentService.setCurrentDateTime(holdingArray[0]);
			currentService.setDateOfService(holdingArray[1]);
			currentService.setProviderNumber(Integer.parseInt((holdingArray[2])));
			currentService.setProviderName((holdingArray[3]));
			currentService.setMemberNumber(Integer.parseInt((holdingArray[4])));
			currentService.setMemberName(holdingArray[5]);
			currentService.setServiceNumber(Integer.parseInt((holdingArray[6])));
			currentService.setServiceName((holdingArray[7]));
			currentService.setComments(holdingArray[8]);
			currentService.setCost(Integer.parseInt(holdingArray[9]));
			
			serviceRecords.add(currentService);
		}
		srScanner.close();
		System.out.println("ServiceRecord is complete.\n");
	}
	
	public void arrayToFile() throws IOException {
		
		System.out.println("\n---------------------------------------\n");
		
		storeMeL();
		
		storePL();
		
		storeMaL();

		storeSR();
	}
	
	/**
	 * @author mgmulcahy
	 *
	 */
	private void storeMeL() throws IOException {
	    System.out.println("Beginning to store memberList...");

	    try (InputStream inputStream = Storage.class.getResourceAsStream("/entities/memberlist.txt");
	         InputStreamReader reader = new InputStreamReader(inputStream);
	         BufferedReader bufferedReader = new BufferedReader(reader);
	         BufferedWriter fw = new BufferedWriter(new OutputStreamWriter(
	                 new FileOutputStream(new File("memberlist.txt")), StandardCharsets.UTF_8))) {

	        String line;
	        while ((line = bufferedReader.readLine()) != null) {
	            fw.write(line);
	            fw.newLine();
	        }

	        System.out.println("memberlist stored.\n");
	    }
	}


	
	/**
	 * @author mgmulcahy
	 * @throws IOException 
	 *
	 */
	private void storePL() throws IOException {
	    System.out.println("Beginning to store providerList...");

	    try (InputStream inputStream = Storage.class.getResourceAsStream("/entities/providerlist.txt");
		         InputStreamReader reader = new InputStreamReader(inputStream);
		         BufferedReader bufferedReader = new BufferedReader(reader);
		         BufferedWriter fw = new BufferedWriter(new OutputStreamWriter(
		                 new FileOutputStream(new File("providerlist.txt")), StandardCharsets.UTF_8))) {

	        String line;
	        while ((line = bufferedReader.readLine()) != null) {
	            fw.write(line);
	            fw.newLine();
	        }

	        System.out.println("providerlist stored.\n");
	    }
	}




	
	/**
	 * @author mgmulcahy 
	 * you should avoid using File and FileWriter directly, as they are designed for file operations on the local file system. Instead, you should use getResourceAsStream to get an input stream and then use BufferedWriter to write to a file within the JAR.
	 *
	 */
	private void storeMaL() throws IOException {
	    System.out.println("Beginning to store managerList...");

	    try (InputStream inputStream = Storage.class.getResourceAsStream("/entities/managerList.txt");
		         InputStreamReader reader = new InputStreamReader(inputStream);
		         BufferedReader bufferedReader = new BufferedReader(reader);
		         BufferedWriter fw = new BufferedWriter(new OutputStreamWriter(
		                 new FileOutputStream(new File("managerList.txt")), StandardCharsets.UTF_8))) {

		        String line;
		        while ((line = bufferedReader.readLine()) != null) {
		            fw.write(line);
		            fw.newLine();
		        }

	        System.out.println("managerList stored.\n");
	    }
	}

	
	
	/*
	 	Author: Megan Mulcahy & ChatGPT
	 	stores service record onto a file
	*/
	private void storeSR() throws IOException {
	    System.out.println("Beginning to store serviceRecord...");

	    try (InputStream inputStream = Storage.class.getResourceAsStream("/entities/serviceRecords.txt");
		         InputStreamReader reader = new InputStreamReader(inputStream);
		         BufferedReader bufferedReader = new BufferedReader(reader);
		         BufferedWriter fw = new BufferedWriter(new OutputStreamWriter(
		                 new FileOutputStream(new File("serviceRecords.txt")), StandardCharsets.UTF_8))) {

		        String line;
		        while ((line = bufferedReader.readLine()) != null) {
		            fw.write(line);
		            fw.newLine();
		        }

	        System.out.println("serviceRecord stored.\n");
	    }
	}

}