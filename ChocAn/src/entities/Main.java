package entities;
import java.util.*;
import entities.Menu;
import java.io.FileNotFoundException;

@SuppressWarnings("unused")

public class Main {
	public static void main(String[] args) throws FileNotFoundException{
		Menu menu = new Menu();
		
		menu.mainMenu();
		
		return;
	}
	
}
