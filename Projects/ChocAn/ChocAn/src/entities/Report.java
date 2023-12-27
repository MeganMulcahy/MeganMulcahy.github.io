package entities;
import java.util.*;
import java.io.*;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.net.URISyntaxException;
import java.nio.charset.StandardCharsets;


@SuppressWarnings("unused")

/**
 * @author ljhay
 *
 */
public class Report {
	
	/*
		A summary report is given to the manager for accounts payable. The report lists every
		provider to be paid that week, the number of consultations each had, and his or her total
		fee for that week. Finally, the total number of providers who provided services, the total
		number of consultations, and the overall fee total are printed. 
	*/
	public void generateSummaryReport(Storage s) throws IOException {
        try (BufferedWriter ri = new BufferedWriter(new FileWriter("SummaryReport"))) {

            int totalFee = 0;
            int totalServices = s.serviceList.size();
            int totalProviders = 0;

            for (int i = 0; i < s.providerList.size(); i++) {
                Provider currentProvider = s.providerList.get(i);
                for (int y = 0; y < s.serviceRecords.size(); y++) {
                    ServiceRecord check = s.serviceRecords.get(y);
                    if (check.getProviderNumber() == currentProvider.getNumber()) {
                        totalProviders++;
                        int providerServices = 0;
                        int providerCharge = 0;
                        for (int j = 0; j < s.serviceRecords.size(); j++) {
                            ServiceRecord current = s.serviceRecords.get(j);
                            if (current.getProviderNumber() == currentProvider.getNumber()) {
                                providerServices++;
                                totalServices++;
                                providerCharge += current.getCost();
                                totalFee += current.getCost();
                            }
                        }
                        ri.write("    " + currentProvider.getName() + " is owed $" + providerCharge +
                                " for providing " + providerServices + " consultations.\n");
                    }
                }
            }

            ri.write("\nTotal Active Providers: " + totalProviders);
            ri.write("\nTotal Consultations: " + totalServices);
            ri.write("\nOverall Fee: " + totalFee + "\n");

            System.out.println("Summary Report Finished.");
        }
    }


	
	
	/*
	 	Provider name (25 characters).
 		Provider number (9 digits).
 		Provider street address (25 characters).
 		Provider city (14 characters).
 		Provider state (2 letters).
 		Provider ZIP code (5 digits).
 		
 		For each service provided, the following details are required:
 			Date of service (MM–DD–YYYY).
 			Date and time data were received by the computer (MM–DD–YYYY HH:MM:SS).
 			Member name (25 characters).
 			Member number (9 digits).
 			Service code (6 digits).
 			Fee to be paid (up to $999.99).
 		
 		Total number of consultations with members (3 digits).
 		Total fee for week (up to $99,999.99).
	*/
	public void generateProviderReport(Storage s) throws IOException {
	    for (int i = 0; i < s.providerList.size(); i++) {
	        Provider currentProvider = s.providerList.get(i);

	        String fileName = currentProvider.getName() + "_report.txt";

	        try (BufferedWriter ri = new BufferedWriter(new FileWriter(fileName))) {
	            ri.write("Provider name: ");
	            ri.write(currentProvider.getName());
	            ri.write("\nProvider Number: ");
	            ri.write(String.valueOf(currentProvider.getNumber()));
	            ri.write("\nProvider Street Address: ");
	            ri.write(currentProvider.getAddress());
	            ri.write("\nMember City: ");
	            ri.write(currentProvider.getCity());
	            ri.write("\nMember State: ");
	            ri.write(currentProvider.getState());
	            ri.write("\nMember Zip Code: ");
	            ri.write(String.valueOf(currentProvider.getZipCode()));

	            int totalCost = 0;
	            int totalServices = 0;
	            ri.write("\n\nServices:\n");
	            for (int j = 0; j < s.serviceRecords.size(); j++) {
	                ServiceRecord currentService = s.serviceRecords.get(j);
	                if (currentService.getProviderNumber() == currentProvider.getNumber()) {
	                    ri.write("    Date of Service: ");
	                    ri.write(currentService.getDateOfService());
	                    ri.write("\n    Date of Service Report: ");
	                    ri.write(currentService.getCurrentDateTime());
	                    ri.write("\n    Member Name: ");
	                    ri.write(currentService.getMemberName());
	                    ri.write("\n    Member Number: ");
	                    ri.write(String.valueOf(currentService.getMemberNumber()));
	                    ri.write("\n    Service Code: ");
	                    ri.write(String.valueOf(currentService.getServiceNumber()));
	                    ri.write("\n    Fee to be paid: ");
	                    ri.write(String.valueOf(currentService.getCost()) + "\n\n");

	                    totalCost += currentService.getCost();
	                    totalServices++;
	                }
	            }

	            ri.write("\nTotal number of consultations with members: " + String.valueOf(totalServices));
	            ri.write("\nTotal fee for the week: " + String.valueOf(totalCost));
	            System.out.println("Provider Reports Finished for " + currentProvider.getName());

	        }
            System.out.println("Provider Reports Finished for all providers");

	    }
	}

					
	
	
	/*
	    Member name (25 characters).
 		Member number (9 digits).
 		Member street address (25 characters).
 		Member city (14 characters).
 		Member state (2 letters).
 		Member ZIP code (5 digits).
 		
 		For each service provided, the following details are required:
 		Date of service (MM–DD–YYYY).
 		Provider name (25 characters).
 		Service name (20 characters).
	*/
	public void generateMemberReport(Storage s) throws IOException {
		for (int i = 0; i < s.memberList.size(); i++) {
	        Member currentMember = s.memberList.get(i);
	        String fileName = currentMember.getName() + "_report.txt";
	            try (BufferedWriter ri = new BufferedWriter(new FileWriter(fileName))) {
	            	
	                ri.write("Member name:");
	                ri.write(currentMember.getName());
	                ri.write("\nMember Number: ");
	                ri.write(String.valueOf(currentMember.getNum()));
	                ri.write("\nMember Street Address: ");
	                ri.write(currentMember.getAddress());
	                ri.write("\nMember City: ");
	                ri.write(currentMember.getCity());
	                ri.write("\nMember State: ");
	                ri.write(currentMember.getState());
	                ri.write("\nMember Zip Code: ");
	                ri.write(String.valueOf(currentMember.getZip()));

	                ri.write("\n\nServices:\n");

	                for (int j = 0; j < s.serviceRecords.size(); j++) {
	                    ServiceRecord currentService = s.serviceRecords.get(j);
	                    if (currentService.getMemberNumber() == currentMember.getNum()) {
	                        ri.write("    Date of Service: ");
	                        ri.write(currentService.getDateOfService());
	                        ri.write("\n    Provider Name: ");
	                        ri.write(currentService.getProviderName());
	                        ri.write("\n    Service Name: ");
	                        ri.write(currentService.getServiceName() + "\n\n");
	                    }
	                }

	                System.out.println("Member Reports Finished for " + currentMember.getName());
	            }
	        }
	        System.out.println("All Member Reports Finished.");
	    }

	
	
	/*
		A record consisting of electronic funds transfer (EFT) data is then written to a disk;
		banking computers will later ensure that each provider’s bank account is credited with the
		appropriate amount. 
		
		EFT data, all that is required is that a file be set up
		containing the provider name, provider number, and the amount to be transferred. 
	*/
	public void generateEFTData(Storage s) throws IOException {
	    try (BufferedWriter ri = new BufferedWriter(new FileWriter("EFTData.txt"))) {
	        // Add your logic for generating EFT data here
	        ri.write("Charges:\n");
	        for (int i = 0; i < s.providerList.size(); i++) {
	            Provider currentProvider = s.providerList.get(i);
	            for (int y = 0; y < s.serviceRecords.size(); y++) {
	                ServiceRecord check = s.serviceRecords.get(y);
	                if (check.getProviderNumber() == currentProvider.getNumber()) {
	                    int providerServices = 0;
	                    int providerCharge = 0;
	                    for (int j = 0; j < s.serviceRecords.size(); j++) {
	                        ServiceRecord current = s.serviceRecords.get(j);
	                        if (current.getProviderNumber() == currentProvider.getNumber()) {
	                            providerServices++;
	                            providerCharge += current.getCost();
	                        }
	                    }
	                    ri.write("    " + currentProvider.getName() + "(" + currentProvider.getNumber() + ") is owed $" + providerCharge + "\n");
	                }
	            }
	        }

	        System.out.println("EFTData Finished.");
	    } 
	}
}