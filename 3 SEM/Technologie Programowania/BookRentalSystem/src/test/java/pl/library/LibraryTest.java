package pl.library;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class LibraryTest {

    private Library library;

    @BeforeEach
    void setUp() {
        library = new Library();
    }

    @Test
    void testAddBook() {
        library.addBook("Kodowanie", "Piotr Nowak");
        Book book = library.findBookByTitle("Kodowanie");
        assertNotNull(book, "Książka powinna być dodana i możliwa do znalezienia po tytule.");
    }

    @Test
    void testAddReader() {
        library.addReader("Agnieszka");
        Reader reader = library.findReaderByName("Agnieszka");
        assertNotNull(reader, "Czytelnik powinien być dodany i możliwy do znalezienia po imieniu.");
    }

    @Test
    void testBorrowBookSuccessfully() {
        library.addBook("Programowanie Pragmatyczne", "Andrzej Polak");
        library.addReader("Marek");
        Book book = library.findBookByTitle("Programowanie Pragmatyczne");
        book.addCopy();

        library.borrowBook("Programowanie Pragmatyczne", "Marek");
        assertNull(book.getAvailableCopy(), "Po wypożyczeniu książka powinna być niedostępna.");
    }

    @Test
    void testBorrowNonexistentBook() {
        library.addReader("Wojtek");
        library.borrowBook("Nieistniejąca Książka", "Wojtek");
        Reader reader = library.findReaderByName("Wojtek");
        assertEquals(0, reader.getBorrowedBooks().size(), "Czytelnik nie powinien mieć wypożyczonych książek.");
    }
}
