package entities;


/**
 * @author ljhay
 *
 */
public class Manager {
	private String managerName;
	private int managerNumber;
	
	public Manager() {
		managerName = "TestName";
		managerNumber = 123456789;
	}
	
	public Manager(String name,int number) {
		managerName = name;
		managerNumber = number;
	}
	
	public String getName() {
		return managerName;
	}
	
	public int getNumber() {
		return managerNumber;
	}
	
	public void setName(String name) {
		managerName = name;
	}
	
	public void setNumber(int number) {
		managerNumber = number;
	}
}

