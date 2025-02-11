package pl.library;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class ReaderTest {

    private Reader reader;
    private Book book;

    @BeforeEach
    void setUp() {
        reader = new Reader("Ewelina");
        book = new Book("Wzorce Projektowe", "Adam Zieliński");
    }

    @Test
    void testBorrowAndReturnBook() {
        Copy copy = new Copy(book);

        reader.borrowBook(copy);
        assertFalse(copy.isAvailable(), "Egzemplarz powinien być oznaczony jako wypożyczony.");
        assertEquals(1, reader.getBorrowedBooks().size(), "Czytelnik powinien mieć jedną wypożyczoną książkę.");

        reader.returnBook(copy);
        assertTrue(copy.isAvailable(), "Egzemplarz powinien być dostępny po zwrocie.");
        assertEquals(0, reader.getBorrowedBooks().size(), "Czytelnik nie powinien mieć wypożyczonych książek po zwrocie.");
    }
}
