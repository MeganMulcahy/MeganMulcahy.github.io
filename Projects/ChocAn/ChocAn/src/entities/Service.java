/**
 * @Author Beboskus
 * 
 * Service Class: an entity class to hold information about services for provider directory.
 * 
 */

package entities;
import java.util.*;

@SuppressWarnings("unused")

public class Service {
	private int serviceCode; //6 digit service code
	private String serviceName; //name of service
	private int serviceFee; //cost for service 
	
	public Service() {
		serviceCode = 598470;
		serviceName = "dietitian";
		serviceFee = 123456;
		
	}
	
	/**
	 * for adding and removing a service
	 */
	public Service(int co, String na, int fe) {
		serviceCode = co;
		serviceName = na;
		serviceFee = fe;
	}
	
	/**
	 * made in order to use Service in storage and provider directory
	 */
	public void setServiceCode(int sc) {
		this.serviceCode = sc;
	}
	public void setServiceName(String sn) {
		this.serviceName = sn;
	}
	public void setServiceFee(int sf) {
		this.serviceFee = sf;
	}
	public int getServiceCode() {
		return serviceCode;
	}
	public String getServiceName() {
		return serviceName;
	}
	public int getServiceFee() {
		return serviceFee;
	}
	
}