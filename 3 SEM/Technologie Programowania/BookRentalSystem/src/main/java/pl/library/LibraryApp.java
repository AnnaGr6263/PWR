package pl.library;

import java.util.Scanner;

/**
 * Główna klasa aplikacji do zarządzania wypożyczeniami książek.
 * @author:  Anna Grelewska
 */
public final class LibraryApp {

    /**
     * Prywatny konstruktor, aby zapobiec tworzeniu instancji.
     */
    private LibraryApp() {

    }

    /**
     * Uruchamia aplikację biblioteki.
     *
     * @param args argumenty linii poleceń
     */
    public static void main(final String[] args) {
        Library library = new Library();
        Scanner scanner = new Scanner(System.in);

        System.out.println("Welcome to the Book Rental System");

        while (true) {
            System.out.println("1. Add book");
            System.out.println("2. Add reader");
            System.out.println("3. Borrow book");
            System.out.println("4. Return book");
            System.out.println("5. Display borrowed books by reader");
            System.out.println("6. Display books");
            System.out.println("7. Exit");

            System.out.print("Choose an option: ");
            int choice = scanner.nextInt();
            scanner.nextLine();

            switch (choice) {
                case 1:
                    System.out.print("Enter book title: ");
                    String addBookTitle = scanner.nextLine();
                    System.out.print("Enter book author: ");
                    String author = scanner.nextLine();
                    library.addBook(addBookTitle, author);
                    System.out.print("How many copies? ");
                    int copies = scanner.nextInt();
                    for (int i = 0; i < copies; i++) {
                        library.findBookByTitle(addBookTitle).addCopy();
                    }
                    break;
                case 2:
                    System.out.print("Enter reader name: ");
                    String readerName = scanner.nextLine();
                    library.addReader(readerName);
                    break;
                case 3:
                    System.out.print("Enter book title to borrow: ");
                    String borrowBookTitle = scanner.nextLine();
                    System.out.print("Enter reader's name: ");
                    String borrowerName = scanner.nextLine();
                    library.borrowBook(borrowBookTitle, borrowerName);
                    break;
                case 4:
                    System.out.print("Enter book title to return: ");
                    String returnBookTitle = scanner.nextLine();
                    System.out.print("Enter reader's name: ");
                    String returnerName = scanner.nextLine();
                    library.returnBook(returnBookTitle, returnerName);
                    break;
                case 5:
                    System.out.print("Enter reader's name to view borrowed books: ");
                    String displayReaderName = scanner.nextLine();
                    library.displayBorrowedBooks(displayReaderName);
                    break;
                case 6:
                    library.displayBooks();
                    break;
                case 7:
                    System.out.println("Exiting the system. Goodbye!");
                    scanner.close();
                    return;
                default:
                    System.out.println("Invalid choice, please try again.");
            }
        }
    }
}
