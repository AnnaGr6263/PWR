package pl.library;

import java.util.ArrayList;
import java.util.List;

/**
 * Klasa Reader reprezentuje czytelnika i jego wypożyczenia.
 * Zastosowanie GRASP: High Cohesion -
 * Klasa Reader jest odpowiedzialna wyłącznie za operacje związane z wypożyczeniami czytelnika.
 */
public final class Reader {

    /** Imię czytelnika. */
    private final String name;

    /** Lista wypożyczonych książek przez czytelnika. */
    private final List<Copy> borrowedBooks;

    /**
     * Tworzy nową instancję Reader.
     *
     * @param readerName imię czytelnika.
     */
    public Reader(final String readerName) {
        this.name = readerName;
        this.borrowedBooks = new ArrayList<>();
    }

    /**
     * Wypożycza egzemplarz książki.
     *
     * @param copy egzemplarz książki do wypożyczenia.
     */
    public void borrowBook(final Copy copy) {
        borrowedBooks.add(copy);
        copy.borrow();
    }

    /**
     * Zwraca egzemplarz książki.
     *
     * @param copy egzemplarz książki do zwrotu.
     */
    public void returnBook(final Copy copy) {
        if (borrowedBooks.remove(copy)) {
            copy.returnCopy();
        } else {
            System.out.println("This copy is not in the borrowed list.");
        }
    }

    /**
     * Zwraca imię czytelnika.
     *
     * @return imię czytelnika.
     */
    public String getName() {
        return name;
    }

    /**
     * Zwraca listę wypożyczonych książek.
     *
     * @return lista wypożyczonych książek.
     */
    public List<Copy> getBorrowedBooks() {
        return borrowedBooks;
    }
}
