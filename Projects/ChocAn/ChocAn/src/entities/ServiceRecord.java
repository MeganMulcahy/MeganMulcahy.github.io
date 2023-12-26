/*
  	Author: Brooke Boskus
  
  	Service Record Class: an entity class for the providers services to a member. 
  	Each record includes: the current time and date, the date that the service was provided on,
  	the provider number, the member number and the any additional comments that the provider included
 */

package entities;
import java.io.*;
import java.util.*;

@SuppressWarnings("unused")

public class ServiceRecord {
	private String currentDateTime; //MM-DD-YYYY HH:MM:SS
	private String dateOfService; //MM-DD-YYYY
	private int providerNumber; // 9 digit
	private String providerName;
	private int memberNumber; // 9 digit
	private String memberName;
	private int serviceNumber; //6 digits
	private String serviceName;
	private String comments; //100 characters optional
	private int serviceCost;
	
	
	public ServiceRecord() {
		currentDateTime = "00-00-0000 00:00:00";
		dateOfService = "00-00-0000";
		providerNumber = 000000000;
		memberNumber = 000000000;
		comments = "some comments";
	}
	
	/*	
	  	lines 33-108 were made in order to use Service Record in storage
	*/
	public boolean setCurrentDateTime(String dt) {
		//makes sure Date and Time are correct length and format
		if (dt.length() != 19) {
			System.out.println("Invalid Format");
			return false;
		}
		//checks format
		for(int i = 0; i < dt.length(); i++) {
			char ch = dt.charAt(i);
			if(i == 2||i == 5) {
				if (ch != '-') {
					System.out.println("Invalid Format");
					return false;
				}
			}
		}
		for(int j = 0; j < dt.length(); j++) {
			char ch = dt.charAt(j);
			if(j == 13|| j == 16) {
				if (ch != ':') {
					System.out.println("Invalid Format");
					return false;
				}
			}
		}
		//sets currentDateTime if information is valid
		currentDateTime = dt;
		return true;
	}

	public boolean setDateOfService(String ds) {
		//makes sure that Date Of Service is correct length and format
		//checks length
		if (ds.length() != 10) {
			System.out.println("Invalid Format");
			return false;
		}
		//checks format
		for(int i = 0; i < ds.length(); i++) {
			char ch = ds.charAt(i);
			if(i == 2||i == 5) {
				if (ch != '-') {
					System.out.println("Invalid Format");
					return false;
				}
			}
		}
		//sets dateOfService if information is valid
		dateOfService = ds;
		return true;
	}
	public void setProviderNumber(int pn) {
		providerNumber = pn;
	}
	public void setMemberNumber(int mn) {
		memberNumber = mn;
	}
	public void setComments(String c) {
		comments = c;
	}
	public void setServiceNumber(int number) {
		serviceNumber = number;
	}
	public void setServiceName(String name) {
		serviceName = name;
	}
	public void setProviderName(String name) {
		providerName = name;
	}
	public void setCost(int cost) {
		serviceCost = cost;
	}
	public void setMemberName(String name) {
		memberName = name;
	}
	
	
	/*
	 	used to return values in order to write to report files
	*/
	public String getCurrentDateTime() {
		 return currentDateTime;
	}
	public String getDateOfService() {
		return dateOfService;
	}
	public int getProviderNumber() {
		return providerNumber;
	}
	public int getMemberNumber() {
		return memberNumber;
	}
	public String getComments() {
		return comments;
	}
	public int getServiceNumber() {
		return serviceNumber;
	}
	public String getServiceName() {
		return serviceName;
	}
	public String getProviderName() {
		return providerName;
	}
	public int getCost() {
		return serviceCost;
	}
	public String getMemberName() {
		return memberName;
	}
	

}