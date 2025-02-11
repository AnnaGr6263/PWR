package pl.library;
import java.util.ArrayList;
import java.util.List;

/**
 * Klasa Library pełni rolę kontrolera, zarządzającego książkami i czytelnikami.
 * Zastosowanie GRASP: Controller - Library zarządza operacjami związanymi z książkami i czytelnikami.
 */
public final class Library {

    /** Lista książek dostępnych w bibliotece. */
    private final List<Book> books;

    /** Lista czytelników zarejestrowanych w systemie. */
    private final List<Reader> readers;

    /**
     * Tworzy nową instancję Library.
     */
    public Library() {
        this.books = new ArrayList<>();
        this.readers = new ArrayList<>();
    }

    /**
     * Dodaje nową książkę do biblioteki.
     *
     * @param bookTitle  tytuł książki
     * @param bookAuthor autor książki
     */
    public void addBook(final String bookTitle, final String bookAuthor) {
        Book existingBook = findBookByTitle(bookTitle);
        if (existingBook == null) {
            books.add(new Book(bookTitle, bookAuthor));
            System.out.println("Book added successfully.");
        } else {
            System.out.println("Book with this title already exists.");
        }
    }

    /**
     * Dodaje czytelnika do systemu, jeśli nie istnieje już czytelnik o takim samym imieniu.
     *
     * @param readerName imię czytelnika
     */
    public void addReader(final String readerName) {
        // Sprawdza, czy istnieje już czytelnik o podanym imieniu
        if (findReaderByName(readerName) != null) {
            System.out.println("Reader with this name already exists.");
        } else {
            readers.add(new Reader(readerName));
            System.out.println("Reader added successfully.");
        }
    }

    /**
     * Wyszukuje książkę po tytule.
     *
     * @param bookTitle tytuł książki
     * @return obiekt Book, jeśli znaleziono, lub null, jeśli brak wyników
     */
    public Book findBookByTitle(final String bookTitle) {
        for (Book book : books) {
            if (book.getTitle().equalsIgnoreCase(bookTitle)) {
                return book;
            }
        }
        return null;
    }

    /**
     * Wyszukuje czytelnika po imieniu.
     *
     * @param readerName imię czytelnika
     * @return obiekt Reader, jeśli znaleziono, lub null, jeśli brak wyników
     */
    public Reader findReaderByName(final String readerName) {
        for (Reader reader : readers) {
            if (reader.getName().equalsIgnoreCase(readerName)) {
                return reader;
            }
        }
        return null;
    }

    /**
     * Wypożycza książkę czytelnikowi.
     *
     * @param bookTitle  tytuł książki
     * @param readerName imię czytelnika
     */
    public void borrowBook(final String bookTitle, final String readerName) {
        Book book = findBookByTitle(bookTitle);
        Reader reader = findReaderByName(readerName);
        if (book != null && reader != null) {
            Copy copy = book.getAvailableCopy();
            if (copy != null) {
                reader.borrowBook(copy);
                System.out.println("Book borrowed successfully.");
            } else {
                System.out.println("No available copies.");
            }
        } else {
            System.out.println("Book or reader not found.");
        }
    }

    /**
     * Zwraca książkę do biblioteki.
     *
     * @param title      tytuł książki
     * @param readerName imię czytelnika
     */
    public void returnBook(final String title, final String readerName) {
        Book book = findBookByTitle(title);
        Reader reader = findReaderByName(readerName);
        if (book != null && reader != null) {
            for (Copy copy : reader.getBorrowedBooks()) {
                if (copy.getBookTitle().equals(title)) {
                    reader.returnBook(copy);
                    System.out.println("Book returned successfully.");
                    return;
                }
            }
            System.out.println("This reader did not borrow this book.");
        } else {
            System.out.println("Book or reader not found.");
        }
    }

    /**
     * Wyświetla wszystkie książki i ich status.
     */
    public void displayBooks() {
        for (Book book : books) {
            System.out.println("Title: " + book.getTitle() + " | Author: " + book.getAuthor());
            for (Copy copy : book.getCopies()) {
                System.out.println("  Copy - Available: " + copy.isAvailable());
            }
        }
    }

    /**
     * Wyświetla książki wypożyczone przez danego czytelnika.
     *
     * @param readerName imię czytelnika
     */
    public void displayBorrowedBooks(final String readerName) {
        Reader reader = findReaderByName(readerName);
        if (reader != null) {
            System.out.println("Books borrowed by " + reader.getName() + ":");
            for (Copy copy : reader.getBorrowedBooks()) {
                System.out.println("- " + copy.getBookTitle());
            }
        } else {
            System.out.println("Reader not found.");
        }
    }
}
