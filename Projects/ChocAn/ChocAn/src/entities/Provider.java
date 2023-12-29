package entities;

public class Provider {
	private int providerNum; // 9-digit provider number
	private String providerName; // 20-character 
	private String providerStreetAddress; // 25-character street address
	private String providerCity; // 14-character city
	private String providerState; // 2-character state (use abbreviations, e.x. AL or AK)
	private int providerZip; // 5-digit zip code
	private String providerEmail; // email to send reports to for the provider

	public Provider() {
		providerNum = 123456789;
		providerName = "Walter White";
		providerStreetAddress = "308 Negra Arroyo Lane";
		providerCity = "Albuquerque";
		providerState = "NM";
		providerZip = 87104;
		providerEmail = "test@email.com";
	}
	
	public Provider(int nu, String na, String sa, String ci, String st, int zi, String em) {
		// This method will be used in OperatorTerminal for creating new Providers.
		// Order in the CSV will be number,name,address,city,state,zip,email
		providerNum = nu;
		providerName = na;
		providerStreetAddress = sa;
		providerCity = ci;
		providerState = st;
		providerZip = zi;
		providerEmail = em;
	}
	
	public void setName(String name) {
		this.providerName = name;
	}
	public void setNumber(int number) {
		this.providerNum = number;
	}
	public void setAddress(String address) {
		this.providerStreetAddress = address;
	}
	public void setCity(String city) {
		this.providerCity = city;
	}
	public void setState(String state) {
		this.providerState = state;
	}
	public void setZipCode(int zipCode) {
		this.providerZip = zipCode;
	}
	public void setEmail(String email) {
		this.providerEmail = email;
	}
	
	public String getName() {
		return providerName;
	}
	
	public int getNumber() {
		return providerNum;
	}
	
	public String getAddress() {
		return providerStreetAddress;
	}
	
	public String getCity() {
		return providerCity;
	}

	public String getState() {
		return providerState;
	}
	
	public String getEmail() {
		return providerEmail;
	}
	
	/**
	 * getZipCode() returns the zip code of the provider
	 * @return zipCode
	 */
	public int getZipCode() {
		return providerZip;
	}
}
