package entities;
import java.io.*;
import java.util.*;

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
	 * @author ljhay
	 *
	 */
	private void fillMeL() throws FileNotFoundException {
			  System.out.println("Beginning to fill Member List...");
		      File file = new File("Fall2022Team3/Storage/memberlist.txt");
		      Scanner myReader = new Scanner(file);
		      String[]holdingArray;
		      
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
		
		File providerInput = new File("Fall2022Team3/Storage/providerlist.txt");
		Scanner plScanner = new Scanner(providerInput);
		String[] holdingArray;
		//Provider holdingProvider = new Provider();
		
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
	 * @author ljhay
	 *
	 */
	private void fillMaL() throws FileNotFoundException {
		System.out.println("Beginning to fill Manager List...");
	      File file = new File("Fall2022Team3/Storage/managerList.txt");
	      Scanner myReader = new Scanner(file);
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
		File serviceListInput = new File("fall2022team3/src/entities/servicesList.txt");
		Scanner slScanner = new Scanner(serviceListInput);
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
	 	Author: Brooke Boskus
	 	fills array with information from given file "serviceRecords"
	*/
	private void fillSR() throws FileNotFoundException {
		System.out.println("Beginning to fill Service Record...");
		File serviceInput = new File("Fall2022Team3/Storage/serviceRecords.txt");
		Scanner srScanner = new Scanner(serviceInput);
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
	 * @author ljhay
	 *
	 */
	private void storeMeL() throws IOException{
		System.out.println("Beginning to store memberList...");
		File report = new File("Fall2022Team3/Storage/memberList.txt");
	    report.createNewFile();
	
	    FileWriter fw = new FileWriter("Fall2022Team3/Storage/memberList.txt");
			
		while(memberList.size() > 0) {
			Member newMember = memberList.get(0);
			int i = 0;
			if(newMember.getStatus()) {
				i = 1;
			}
			fw.write(newMember.getNum() + "," + newMember.getName() + "," + newMember.getAddress() + "," + newMember.getCity() + "," + newMember.getState() + "," + newMember.getEmail() + "," + newMember.getZip() + "," + newMember.getAmountOwed() + "," + i + "\n");
			memberList.remove(0);
		}
		fw.close();	
		System.out.println("memberList stored.\n");
	}
	
	/**
	 * @author ljhay
	 * @throws IOException 
	 *
	 */
	private void storePL() throws IOException{
		System.out.println("Beginning to store providerList...");
		File report = new File("Fall2022Team3/Storage/providerList.txt");
	    report.createNewFile();
	
	    FileWriter fw = new FileWriter("Fall2022Team3/Storage/providerList.txt");
			
		while(providerList.size() > 0) {
			Provider newProvider = providerList.get(0);
			fw.write(newProvider.getNumber() + "," + newProvider.getName() + "," + newProvider.getAddress() + "," + newProvider.getCity() + "," + newProvider.getState() + "," + newProvider.getZipCode() + "," + newProvider.getEmail() + "\n");
			providerList.remove(0);
		}
		fw.close();	
		System.out.println("providerList stored.\n");
	}
	
	/**
	 * @author ljhay 
	 *
	 */
	private void storeMaL() throws IOException{
		System.out.println("Beginning to store managerList...");
		File report = new File("Fall2022Team3/Storage/managerList.txt");
	    report.createNewFile();
	
	    FileWriter fw = new FileWriter("Fall2022Team3/Storage/managerList.txt");
			
		while(managerList.size() > 0) {
			Manager newManager = managerList.get(0);
			fw.write(newManager.getName() + "," + newManager.getNumber() + "\n");
			managerList.remove(0);
		}
		fw.close();	
		System.out.println("managerList stored.\n");
	}
	
	
	/*
	 	Author: Brooke Boskus
	 	stores service record onto a file
	*/
	private void storeSR() throws IOException{
		System.out.println("Beginning to store serviceRecord...");
		File sReport = new File("Fall2022Team3/Storage/serviceRecords.txt");
		sReport.createNewFile();
		
		FileWriter fw = new FileWriter("Fall2022Team3/Storage/serviceRecords.txt");
		
		while(serviceRecords.size() > 0) {
			ServiceRecord newService = serviceRecords.get(0);
			fw.write(newService.getCurrentDateTime() + "," + newService.getDateOfService() + "," + newService.getProviderNumber() + "," + newService.getProviderName() + "," + newService.getMemberNumber() + "," + newService.getMemberName() + "," + newService.getServiceNumber() + "," + newService.getServiceName() + "," + newService.getComments() + "," + newService.getCost() + "\n");
			serviceRecords.remove(0);
			
		}
		fw.close();
		System.out.println("serviceRecord stored.\n");		
	}
}