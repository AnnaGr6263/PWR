package pl.library;

import java.util.ArrayList;
import java.util.List;

/**
 * Klasa Book reprezentuje książkę i zarządza jej egzemplarzami.
 * Zastosowanie wzorca GRASP: Information Expert
 * - klasa Book jest ekspertem informacji,
 * ponieważ zarządza własnymi egzemplarzami (copies).
 */
public final class Book {

    /** Tytuł książki. */
    private final String title;

    /** Autor książki. */
    private final String author;

    /** Lista egzemplarzy książki. */
    private final List<Copy> copies;

    /**
     * Tworzy nowy obiekt Book.
     *
     * @param bookTitle  tytuł książki.
     * @param bookAuthor autor książki.
     */
    public Book(final String bookTitle, final String bookAuthor) {
        this.title = bookTitle;
        this.author = bookAuthor;
        this.copies = new ArrayList<>();
    }

    /**
     * Dodaje nowy egzemplarz do listy kopii.
     * Zastosowanie GRASP: Information Expert - Book zarządza swoimi egzemplarzami.
     */
    public void addCopy() {
        copies.add(new Copy(this));
    }

    /**
     * Zwraca pierwszy dostępny egzemplarz (jeśli taki istnieje).
     * Zastosowanie GRASP: Information Expert
     * - Book jest ekspertem w kwestii dostępności swoich egzemplarzy.
     *
     * @return pierwszy dostępny egzemplarz lub null, jeśli brak dostępnych egzemplarzy.
     */
    public Copy getAvailableCopy() {
        for (Copy copy : copies) {
            if (copy.isAvailable()) {
                return copy;
            }
        }
        return null;
    }

    /**
     * Zwraca tytuł książki.
     *
     * @return tytuł książki.
     */
    public String getTitle() {
        return title;
    }

    /**
     * Zwraca autora książki.
     *
     * @return autor książki.
     */
    public String getAuthor() {
        return author;
    }

    /**
     * Zwraca listę egzemplarzy książki.
     *
     * @return lista egzemplarzy książki.
     */
    public List<Copy> getCopies() {
        return copies;
    }
}
