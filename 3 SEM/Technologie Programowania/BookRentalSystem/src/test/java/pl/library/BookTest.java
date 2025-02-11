package pl.library;

import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import static org.junit.jupiter.api.Assertions.*;

class BookTest {

    private Book book;

    @BeforeEach
    void setUp() {
        book = new Book("Java", "Jan Kowalski");
    }

    @Test
    void shouldAddCopy() {
        book.addCopy();
        assertEquals(1, book.getCopies().size(), "Lista kopii powinna zawierać jeden egzemplarz.");
    }

    @Test
    void shouldReturnAvailableCopy() {
        book.addCopy();
        assertNotNull(book.getAvailableCopy(), "Powinno zwrócić dostępny egzemplarz.");
    }

    @Test
    void shouldReturnNullIfNoAvailableCopies() {
        book.addCopy();
        book.getAvailableCopy().borrow();
        assertNull(book.getAvailableCopy(), "Powinno zwrócić null, jeśli brak dostępnych egzemplarzy.");
    }
}
