package entities;
import java.util.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;

/**
 * @Author lescales
 * 
 * ProviderDirectory class: provides an ordered list of all services, along with their names, fees, and fees.
 */
public class ProviderDirectory {
	// The Provider Directory is an ordered list of all Services - the code, name, and fee of each service.
	// We will need a way to persistently store all Providers as well - how should we do this?
	
	// This class has been superseced by Storage.
	
	public static ArrayList<String[]> readFile(String filePath) throws FileNotFoundException {
		ArrayList<String[]> serviceList = new ArrayList<String[]>();

		File file = new File(filePath);

		Scanner sc = new Scanner(file);

		String[] holdingArray;

		String currentLine = "";


		while(sc.hasNextLine()) { 
			currentLine = sc.nextLine();
			holdingArray = currentLine.split(",");
			serviceList.add(holdingArray);
		}

		for (int i = 0; i < serviceList.size() - 1; i++)
            for (int j = 0; j < serviceList.size() - i - 1; j++)
                if (serviceList.get(j)[0].compareTo(serviceList.get(j + 1)[0]) > 0) {
                    String temp = serviceList.get(j)[0];
                    serviceList.get(j)[0] = serviceList.get(j + 1)[0];
                    serviceList.get(j + 1)[0] = temp;
                }

		sc.close();
		
		return serviceList;
	}

	public static void writeToFile (String filePath, ArrayList<String[]> serviceList) throws FileNotFoundException {
		try {
			FileWriter file = new FileWriter(filePath);
			for (int i = 0; i < serviceList.size(); i++) {
				file.write(serviceList.get(i)[0] + "," + serviceList.get(i)[1] + "," + serviceList.get(i)[2] + "\n");
			}
			file.close();
		} catch (IOException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		  }

		
	}
}